/*
** my_error.c for my_tgw in /home/paglia_f//my_tgw
** 
** Made by floran pagliai
** Login   <paglia_f@epitech.net>
** 
** Started on  Mon Jul 22 09:55:03 2013 floran pagliai
** Last update Mon Jul 22 09:55:26 2013 floran pagliai
*/

#include "my_error.h"

void	my_error(int type, char *error, int socket)
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
