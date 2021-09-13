/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multipipe_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:43:32 by fmai              #+#    #+#             */
/*   Updated: 2021/09/13 18:31:29 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pipex_bonus.h"
void	stdin_from_file(int pipe_a[2], char *filepath)
{
	int		file_fd;

	check_readability(filepath);
	file_fd = open_file(filepath);
	if (file_fd == -1)
		exit_with_perr("open()", NULL, NULL);
	handle_close(pipe_a[0]);
	handle_dup2(file_fd, 0);
	handle_close(file_fd);
}

int	exec_first_command_with_file(
	int pipe_a[2], char *raw_command, char **envp, char *filepath)
{
	int		pid;

	pid = fork();
	if (pid == -1)
		exit_with_perr("fork()", NULL, NULL);
	if (pid == 0)
	{
		stdin_from_file(pipe_a, filepath);
		handle_dup2(pipe_a[1], 1);
		handle_close(pipe_a[1]);
		handle_command(raw_command, envp);
	}
	return (pid);
}

int	exec_command(
	int pipe_a[2], int pipe_b[2], char **envp, char *raw_command)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		exit_with_perr("fork()", NULL, NULL);
	if (pid == 0)
	{
		handle_close(pipe_a[1]);
		handle_close(pipe_b[0]);
		handle_dup2(pipe_a[0], 0);
		handle_dup2(pipe_b[1], 1);
		handle_close(pipe_a[0]);
		handle_close(pipe_b[1]);
		handle_command(raw_command, envp);
	}
	return (pid);
}
