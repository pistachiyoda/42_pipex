NAME = pipex
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
CFILES = ./src/main.c ./src/exec.c ./src/error.c ./src/file.c ./src/command.c
OBJ = $(CFILES:.c=.o)
LIBS = -Llibft -lft
LIBFT = ./libft/libft.a

all: $(NAME)
$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(OBJ) -o $(NAME) $(LIBS) $(CFLAGS)
$(LIBFT):
	$(MAKE) -C ./libft/
clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all