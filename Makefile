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
		$(CC) -o $(NAME) $(OBJ)

all	:	$(NAME)

clean	:
		$(RM) $(OBJ)

fclean	:	clean
		$(RM) $(NAME)

re	:	fclean all