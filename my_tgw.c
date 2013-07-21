#include "my_tgw.h"

static int PORT_LOCAL;
static int PORT_DIST;
static char *IP_DIST;

void logger(char *txt, char *ip_local, int port_local, char *ip_dist, int port_dist)
{
    FILE *fd;
    time_t timeInfo;
    struct tm t;

    time(&timeInfo);
    t = *localtime(&timeInfo);

    if ((fd = fopen("my_tgw.log", "a")) == NULL)
        my_error(1, "open()", 0);
    fprintf(fd, "%d-%02d-%02d %02d-%02d-%02d:%s:%d:%s:%d:%s\n",
            t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec,
            ip_local, port_local, ip_dist, port_dist, txt);
    fclose(fd);
}

void initServer(t_socket *server)
{
    int socklisten;

    if ((socklisten = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        my_error(1, "socket()", -1);
    bzero((char *) &server->serv_addr, sizeof (server->serv_addr));
    server->serv_addr.sin_family = AF_INET;
    server->serv_addr.sin_addr.s_addr = INADDR_ANY;
    server->serv_addr.sin_port = htons(PORT_LOCAL);
    if (bind(socklisten, (struct sockaddr *) &server->serv_addr,
             sizeof (server->serv_addr)) < 0)
        my_error(2, "bind()", server->sockfd);
    listen(socklisten, 5);
    server->clilen = sizeof (server->cli_addr);
    if ((server->sockfd = accept(socklisten, (struct sockaddr *)
                                 &server->cli_addr, (socklen_t *) & server->clilen)) < 0)
        my_error(2, "accept()", server->sockfd);
}

void initClient(t_socket *client)
{
    if ((client->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        my_error(2, "socket()", -1);
    if ((client->hostinfo = gethostbyname(IP_DIST)) == NULL)
        my_error(2, "L'hôte n'existe pas", client->sockfd);
    bzero((char *) &client->serv_addr, sizeof (client->serv_addr));
    client->serv_addr.sin_family = AF_INET;
    client->serv_addr.sin_addr.s_addr = inet_addr(IP_DIST);
    client->serv_addr.sin_port = htons(PORT_DIST);
    if ((connect(client->sockfd, (struct sockaddr *)
                 &client->serv_addr, sizeof (client->serv_addr))) < 0)
        my_error(2, "connect()", client->sockfd);
}

int routeData(int socketSource, int socketDest, char *side)
{
    char buffer[200];
    int ret;

    ret = recv(socketSource, buffer, 200, 0);
    if (ret <= 0)
        return ret;
    ret = send(socketDest, buffer, ret, 0);
    return ret;
}

static void *tserver(void *tserver)
{
    t_socket *server = (t_socket*) tserver;
    t_connectionData connectionData;
    pthread_t tclt;
    int ret;

    initServer(server);
    connectionData.sockserver = server->sockfd;
    connectionData.sockclient = -1;
    connectionData.stop = 0;
    pthread_mutex_init(&connectionData.socketmutex, NULL);
    pthread_create(&tclt, NULL, tclient, (void*) &connectionData);
    sleep(3);
    while (1) {
        ret = routeData(connectionData.sockserver, connectionData.sockclient, "server");
        if (ret <= 0)
            break;
        if (connectionData.stop == 1)
            break;
    }
    connectionData.stop = 1;
    pthread_mutex_lock(&connectionData.socketmutex);
    if (connectionData.sockserver != -1) {
        close(connectionData.sockserver);
        connectionData.sockserver = -1;
    }
    if (connectionData.sockclient != -1) {
        close(connectionData.sockclient);
        connectionData.sockclient = -1;
    }
    pthread_mutex_unlock(&connectionData.socketmutex);
    pthread_join(tclt, NULL);
}

static void *tclient(void *tconnectionData)
{
    t_socket client;
    t_connectionData *connectionData = (t_connectionData*) tconnectionData;
    int ret;

    initClient(&client);
    connectionData->sockclient = client.sockfd;
    while (1) {
        ret = routeData(connectionData->sockclient, connectionData->sockserver, "client");
        if (ret <= 0)
            break;
        if (connectionData->stop == 1)
            break;
    }
    connectionData->stop = 1;
    pthread_mutex_lock(&connectionData->socketmutex);
    if (connectionData->sockserver != -1) {
        close(connectionData->sockserver);
        connectionData->sockserver = -1;
    }
    pthread_mutex_unlock(&connectionData->socketmutex);
}

void my_tgw(char *ip_local, char *ip_dist, int port_local, int port_dist)
{
    t_socket server;
    pthread_t tsrv;
    int ok;

    ok = 1;
    IP_DIST = ip_dist;
    PORT_DIST = port_dist;
    PORT_LOCAL = port_local;
    pthread_create(&tsrv, NULL, tserver, (void*) &server);
    pthread_join(tsrv, NULL);

    /*
        initServer(&server, port_local);
        initClient(&client, ip_dist, port_dist);
        logger("BEGIN", ip_local, port_local, ip_dist, port_dist);
        while (ok == 1) {
            ok = readSocket(&server);
            sendSocket(&client, server.buffer);
        }
        logger("END", ip_local, port_local, ip_dist, port_dist);
        shutdown(server.sockfd, 2);
        close(server.sockfd);
        shutdown(client.sockfd, 2);
        close(client.sockfd);
     */
}
