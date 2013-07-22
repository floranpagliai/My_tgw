/*
** my_tgw.h for my_tgw in /home/paglia_f//my_tgw
**
** Made by floran pagliai
** Login   <paglia_f@epitech.net>
**
** Started on  Mon Jul 22 09:58:59 2013 floran pagliai
** Last update Mon Jul 22 10:01:41 2013 floran pagliai
*/

#ifndef _MY_TGW_H_
#define	_MY_TGW_H_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>
#include <netdb.h>
#include <pthread.h>

#include "my_error.h"

typedef struct {
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    struct hostent *hostinfo;
    int sockfd;
    //int portno;
    int clilen;
}       t_socket;

typedef struct {
    int sockclient;
    int sockserver;
    pthread_mutex_t socketmutex;
}       t_socketData;

void logger(char *);
void initServer(t_socket *);
void initClient(t_socket *);
int routeData(int, int);
void my_tgw(char *, char *, int, int);

#endif	/* _MY_TGW_H_ */

