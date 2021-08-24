NAME = pipex
CC = gcc
CFLAFS = -Wall -Wextra -Werror
CFILES = ./main.c
OBJ = $(CFILES:.c=.o)
LIBS = -Llibft -lft
LIBFT = ./libft/libft.a

all: $(NAME)
$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(OBJ) -o $(NAME) $(LIBS)
$(LIBFT):
	$(MAKE) -C ./libft/
clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(NAME)
re: fclean all