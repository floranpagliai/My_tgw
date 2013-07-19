#include "my_error.h"

void    my_error(int type, char *error, int socket)
{

    printf ("Error : %s.\n", error);
  if (type >= 1)
    {
      fprintf (stderr, "Program closed.\n");
      exit(EXIT_FAILURE);
    }
  if (type == 2)
    close(socket);
}
