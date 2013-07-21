#include "dataFuncs.h"

char *get_ip(char *str)
{
    int i;
    int len;
    char *ip;

    i = 0;
    len = strlen(str);
    if ((ip = malloc(sizeof (char*) * len)) == NULL)
        my_error(1, "malloc()", 0);
    memset(ip, 0, len);
    while (str[i] != ':') {
        ip[i] = str[i];
        i++;
    }
    return (ip);
}

char *get_port(char *str)
{
    int i;
    int k;
    char *port;

    i = 0;
    k = 0;
    if ((port = malloc(sizeof (char*) * 5)) == NULL)
        my_error(1, "malloc()", 0);
    while (str[i] != ':')
        i++;
    i++;
    while (str[i] != '\0')
        port[k++] = str[i++];
    return (port);
}
