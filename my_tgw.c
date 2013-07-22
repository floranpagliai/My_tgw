/*
** my_tgw.c for my_tgw in /home/paglia_f//my_tgw
**
** Made by floran pagliai
** Login   <paglia_f@epitech.net>
**
** Started on  Mon Jul 22 09:55:43 2013 floran pagliai
** Last update Mon Jul 22 10:06:05 2013 floran pagliai
*/

#include "my_tgw.h"

static int s_port_local;
static int s_port_dist;
static char *s_ip_local;
static char *s_ip_dist;

static void *tserver(void *);
static void *tclient(void *);

void		logger(char *txt)
{
    FILE	*fd;
    time_t	timeInfo;
    struct tm	t;

    time(&timeInfo);
    t = *localtime(&timeInfo);
    if ((fd = fopen("my_tgw.log", "a")) == NULL)
      my_error(1, "open()", 0);
    fprintf(fd, "%d-%02d-%02d %02d-%02d-%02d:%s:%d:%s:%d:%s\n",
            t.tm_year + 1900, t.tm_mon + 1, t.tm_mday,
            t.tm_hour, t.tm_min, t.tm_sec,
            s_ip_local, s_port_local, s_ip_dist, s_port_dist, txt);
    fclose(fd);
}

void		initServer(t_socket *server)
{
  int		socklisten;

  if ((socklisten = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    my_error(1, "socket()", -1);
  bzero((char *) &server->serv_addr, sizeof (server->serv_addr));
  server->serv_addr.sin_family = AF_INET;
  server->serv_addr.sin_addr.s_addr = INADDR_ANY;
  server->serv_addr.sin_port = htons(s_port_local);
  if (bind(socklisten, (struct sockaddr *) &server->serv_addr,
	   sizeof (server->serv_addr)) < 0)
    my_error(2, "bind()", server->sockfd);
  listen(socklisten, 5);
  server->clilen = sizeof (server->cli_addr);
  server->sockfd = accept(socklisten, (struct sockaddr *)
&server->cli_addr, (socklen_t *) & server->clilen);
  if (server->sockfd < 0)
    my_error(2, "accept()", server->sockfd);
}

void		initClient(t_socket *client)
{
  if ((client->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    my_error(2, "socket()", -1);
  if ((client->hostinfo = gethostbyname(s_ip_dist)) == NULL)
        my_error(2, "L'hôte n'existe pas", client->sockfd);
    bzero((char *) &client->serv_addr, sizeof (client->serv_addr));
    client->serv_addr.sin_family = AF_INET;
    client->serv_addr.sin_addr.s_addr = inet_addr(s_ip_dist);
    client->serv_addr.sin_port = htons(s_port_dist);
    if ((connect(client->sockfd, (struct sockaddr *)
                 &client->serv_addr, sizeof (client->serv_addr))) < 0)
        my_error(2, "connect()", client->sockfd);
}

int		routeData(int socketSource, int socketDest)
{
    char	buffer[200];
    int		ret;

    ret = recv(socketSource, buffer, 200, 0);
    if (ret <= 0)
        return ret;
    ret = send(socketDest, buffer, ret, 0);
    return ret;
}

static void	*tserver(void *tserver)
{
    t_socket	*server;
    t_socketData socketData;
    pthread_t	tclt;
    int		ret;

    server = (t_socket*) tserver;
    ret = 1;
    initServer(server);
    socketData.sockserver = server->sockfd;
    socketData.sockclient = -1;
    pthread_mutex_init(&socketData.socketmutex, NULL);
    pthread_create(&tclt, NULL, tclient, (void*) &socketData);
    while (socketData.sockclient == -1)
    {}
    while (ret > 0)
        ret = routeData(socketData.sockserver, socketData.sockclient);
    pthread_mutex_lock(&socketData.socketmutex);
    if (socketData.sockserver != -1)
        close(socketData.sockserver);
    if (socketData.sockclient != -1)
        close(socketData.sockclient);
    pthread_mutex_unlock(&socketData.socketmutex);
    pthread_join(tclt, NULL);
    return NULL;
}

static void	*tclient(void *tsocketData)
{
    t_socket	client;
    t_socketData *socketData;
    int ret;

    socketData = (t_socketData*) tsocketData;
    ret = 1;
    initClient(&client);
    logger("BEGIN");
    socketData->sockclient = client.sockfd;
    while (socketData->sockserver == -1)
    {}
    while (ret > 0)
        ret = routeData(socketData->sockclient, socketData->sockserver);
    pthread_mutex_lock(&socketData->socketmutex);
    if (socketData->sockserver != -1)
        close(socketData->sockserver);
    pthread_mutex_unlock(&socketData->socketmutex);
    return NULL;
}

void		my_tgw(char *ip_local, char *ip_dist, int port_local, int port_dist)
{
    t_socket	server;
    pthread_t	tsrv;

    s_ip_dist = ip_dist;
    s_ip_local = ip_local;
    s_port_dist = port_dist;
    s_port_local = port_local;
    pthread_create(&tsrv, NULL, tserver, (void*) &server);
    pthread_join(tsrv, NULL);
    logger("END");
}
