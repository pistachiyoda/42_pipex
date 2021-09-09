/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 18:50:31 by fmai              #+#    #+#             */
/*   Updated: 2021/09/09 15:02:39 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
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
# include "./libft/libft.h"
int		first_exec(int pipe_fds[2], char **argv, char **envp);
int		last_exec(int pipe_fds[2], char **argv, char **envp);
void	free_2d_array(char **two_d_array);
void	exit_with_perr(
			char *message, char **free_string_array, char *free_string);
void	is_readable(char *file);
bool	is_writable(char *file);
int		open_file(char *file);
int		open_or_create_file(char *file);
bool	is_executable(char *command);
char	**get_env_str(char *key, char **envp);
char	**split_command(char *unformatted_command);
char	*resolve_path(char *command, char *path_env);
void	handle_command(char *raw_command, char **envp);
void	exit_with_strerr(char *message, char *cmd_or_file,
			char **free_string_array, char *free_string);
void	arg_error(int argc);
void	exec(int pipe_fds[2], char **argv, char **envp);
#endif
