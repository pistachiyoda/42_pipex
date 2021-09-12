NAME = pipex
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
CFILES = ./mandatory_src/main.c ./mandatory_src/exec.c ./mandatory_src/error.c ./mandatory_src/file.c ./mandatory_src/command.c
OBJ = $(CFILES:.c=.o)
BONUS_FILES = ./bonus_src/main_bonus.c ./bonus_src/exec_bonus.c ./bonus_src/error_bonus.c ./bonus_src/file_bonus.c ./bonus_src/command_bonus.c ./bonus_src/heredoc_bonus.c ./bonus_src/multipipe_bonus.c ./bonus_src/gnl/get_next_line.c ./bonus_src/gnl/get_next_line_utils.c
BONUS_OBJ = $(BONUS_FILES:.c=.o)
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
	$(MAKE) -C ./libft fclean
re: fclean all
bonus: $(BONUS_OBJ) $(LIBFT)
	$(CC) $(BONUS_OBJ) -o $(NAME) $(LIBS) $(CFLAGS)
clean_bonus:
	rm -f $(BONUS_OBJ)
fclean_bonus: clean_bonus
	rm -f $(NAME)
	$(MAKE) -C ./libft fclean
re_bonus: fclean_bonus bonus