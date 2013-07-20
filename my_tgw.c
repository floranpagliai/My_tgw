#include "my_tgw.h"

void
logger (char *txt, char *ip_local, int port_local, char *ip_dist, int port_dist)
{
  FILE *fd;
  char buff[256];
  time_t timeInfo;
  struct tm t;

  time(&timeInfo);
  t = *localtime (&timeInfo);

  if ((fd = fopen ("my_tgw.log", "a")) == NULL)
    my_error (1, "open()", 0);
  fprintf (fd, "%d-%02d-%02d %02d-%02d-%02d:%s:%d:%s:%d:%s\n",
           t.tm_year+1900, t.tm_mon+1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec,
           ip_local, port_local, ip_dist, port_dist, txt);
  fclose (fd);
}

void
initServer (t_socket *server, int port_local)
{
  if ((server->sockfd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    my_error (1, "socket()", -1);
  bzero ((char *) &server->serv_addr, sizeof (server->serv_addr));
  server->serv_addr.sin_family = AF_INET;
  server->serv_addr.sin_addr.s_addr = INADDR_ANY;
  server->serv_addr.sin_port = htons (port_local);
  if (bind (server->sockfd, (struct sockaddr *) &server->serv_addr,
            sizeof (server->serv_addr)) < 0)
    my_error (2, "bind()", server->sockfd);
  listen (server->sockfd, 5);
  server->clilen = sizeof (server->cli_addr);
  if ((server->newsockfd = accept (server->sockfd, (struct sockaddr *)
                                   &server->cli_addr, &server->clilen)) < 0)
    my_error (2, "accept()", server->newsockfd);
  connect (server->sockfd, (struct sockaddr *) &server->serv_addr, sizeof (server->serv_addr));
}

void
initClient (t_socket *client, char *ip_dist, int port_dist)
{
  if ((client->sockfd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    my_error (2, "socket()", -1);
  if ((client->hostinfo = gethostbyname (ip_dist)) == NULL)
    my_error (2, "L'hôte n'existe pas", client->sockfd);
  bzero ((char *) &client->serv_addr, sizeof (client->serv_addr));
  client->serv_addr.sin_family = AF_INET;
  client->serv_addr.sin_addr.s_addr = INADDR_ANY;
  client->serv_addr.sin_port = htons (port_dist);
  if ((client->newsockfd = connect (client->sockfd, (struct sockaddr *)
                                    &client->serv_addr, sizeof (client->serv_addr))) < 0)
    my_error (2, "connect()", client->newsockfd);
}

int
readSocket (t_socket *socket)
{
  int ret;

  bzero (socket->buffer, 256);
  ret = recv (socket->newsockfd, socket->buffer, 255, 0);
  if (ret <= 0)
    return -1;
  return 1;
}

int
sendSocket (t_socket *socket, char *buffer)
{
  if (strlen (buffer) > 0)
    {
      if (send (socket->sockfd, buffer, strlen (buffer), 0) == -1)
        my_error (0, "send()", -1);
    }
}

void
my_tgw (char *ip_local, char *ip_dist, int port_local, int port_dist)
{
  t_socket server;
  t_socket client;
  int ok;

  ok = 1;
  initServer (&server, port_local);
  initClient (&client, ip_dist, port_dist);
  logger ("BEGIN", ip_local, port_local, ip_dist, port_dist);
  while (ok == 1)
    {
      ok = readSocket (&server);
      sendSocket (&client, server.buffer);
      server.buffer[0] == '\0';
    }
  logger ("END", ip_local, port_local, ip_dist, port_dist);
  shutdown (server.sockfd, 2);
  close (server.sockfd);
  shutdown (client.sockfd, 2);
  close (client.sockfd);
}
