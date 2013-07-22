##
## Makefile for Makefile in /home/paglia_f//my_tgw
## 
## Made by floran pagliai
## Login   <paglia_f@epitech.net>
## 
## Started on  Mon Jul 22 09:58:29 2013 floran pagliai
## Last update Mon Jul 22 09:58:32 2013 floran pagliai
##

NAME	=	my_tgw

SRC	=	main.c\
		my_tgw.c\
		my_error.c\
		dataFuncs.c


OBJ	=	$(SRC:.c=.o)

CC	=	gcc

RM	=	rm -f

FLAG	=	-W -Wall -Werror -Wextra

$(NAME)	:	$(SRC)
		$(CC) -c $(SRC)
		$(CC) -o $(NAME) $(OBJ) -lpthread

all	:	$(NAME)

clean	:
		$(RM) $(OBJ)

fclean	:	clean
		$(RM) $(NAME)

re	:	fclean all