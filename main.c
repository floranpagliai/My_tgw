#include "dataFuncs.h"

int     main(int ac, char** av)
{
  char *ip_local;
  char *ip_dist;
  int port_local;
  int port_dist;

  if (ac < 3)
    printf("[Usage] : ./my_tgw <host_local>:<port_local> <host_dst>:<port_dst>\n");
  else {
  ip_local = get_ip(av[1]);
  ip_dist = get_ip(av[2]);
  port_local = atoi(get_port(av[1]));
  port_dist = atoi(get_port(av[2]));
  printf("[%s:%d] <---> [thegate] <---> [%s:%d]\n", ip_local, port_local, ip_dist, port_dist);
  my_tgw(ip_local, ip_dist, port_local, port_dist);
    }
  return (0);
}

