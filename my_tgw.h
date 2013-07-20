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
#include <fcntl.h>
#include <time.h>

typedef struct {
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;
    struct hostent *hostinfo;
    char buffer[256];
    int sockfd;
    int newsockfd;
    int portno;
    int clilen;
} t_socket;

#endif	/* _MY_TGW_H_ */

