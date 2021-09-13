/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 18:50:31 by fmai              #+#    #+#             */
/*   Updated: 2021/09/13 20:34:03 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H
# include <sys/types.h>
# include <unistd.h>
# include <stdio.h>
# include <errno.h>
# include <string.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdbool.h>
# include "../libft/libft.h"
# include "./gnl/get_next_line.h"
typedef struct s_cmdline_args
{
	int		argc;
	char	**argv;
	char	**envp;
}				t_cmdline_args;
int		first_exec(int pipe_fds[2], char **argv, char **envp);
int		last_exec(int pipe_fds[2], char **argv, char **envp);
void	free_2d_array(char **two_d_array);
void	exit_with_perr(
			char *message, char **free_string_array, char *free_string);
void	check_readability(char *file);
void	check_writability(char *file);
int		open_file(char *file);
int		open_or_create_file(char *file, int open_flag);
bool	is_executable(char *command);
char	**get_env_str(char *key, char **envp);
char	**split_command(char *unformatted_command);
char	*resolve_path(char *command, char *path_env);
void	handle_command(char *raw_command, char **envp);
void	exit_with_strerr(char *message, char *cmd_or_file,
			char **free_string_array, char *free_string);
void	arg_error(int argc, char **argv);
void	exec(int pipe_fds[2], t_cmdline_args *cmdline_args);
int		ft_strcmp(char *a, char *b);
int		exec_first_command_with_heredoc(
			int pipe_a[2], char *raw_command, char **envp, char *limiter);
void	stdin_from_file(int pipe_a[2], char *filepath);
int		exec_first_command_with_file(
			int pipe_a[2], char *raw_command, char **envp, char *filepath);
int		exec_command(
			int pipe_a[2], int pipe_b[2], char **envp, char *raw_command);
void	handle_close(int fd);
void	handle_dup2(int old, int new);
int		handle_fork(void);
#endif
