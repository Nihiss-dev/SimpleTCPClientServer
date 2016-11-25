NAME	= server

NAME_SELECT = server_select

NAME_CLIENT = client

SRC	= $(shell find sources_server -type f -name "*.c")

SRC_SELECT = $(shell find sources_select -type f -name "*.c")

SRC_CLIENT = $(shell find sources_client -type f -name "*.c")

CC	= gcc -Iincludes/

OBJ	= $(SRC:.c=.o)

OBJ_SELECT = $(SRC_SELECT:.c=.o)

OBJ_CLIENT = $(SRC_CLIENT:.c=.o)

all:	$(NAME) $(NAME_SELECT) $(NAME_CLIENT)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

$(NAME_SELECT): $(OBJ_SELECT)
	$(CC) -o $(NAME_SELECT) $(OBJ_SELECT)

$(NAME_CLIENT): $(OBJ_CLIENT)
	$(CC) -o $(NAME_CLIENT) $(OBJ_CLIENT)
clean:
	rm -f $(OBJ) $(OBJ_SELECT) $(OBJ_CLIENT)

fclean: clean
	rm -f $(NAME) $(NAME_SELECT) $(NAME_CLIENT)

re:	fclean all

.phony: all clean fclean re
