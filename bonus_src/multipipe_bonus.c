/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multipipe_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:43:32 by fmai              #+#    #+#             */
/*   Updated: 2021/09/12 16:38:54 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pipex_bonus.h"
void	stdin_from_file(int pipe_a[2], char *filepath)
{
	int		file_fd;

	is_readable(filepath);
	file_fd = open_file(filepath);
	if (file_fd == -1)
		exit_with_perr("open()", NULL, NULL);
	close(pipe_a[0]);
	if (dup2(file_fd, 0) == -1)
		exit_with_perr("dup2()", NULL, NULL);
	close(file_fd);
}

int		exec_first_command_with_file(int pipe_a[2], char *raw_command, char **envp, char *filepath)
{
	int		pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork()");
		return (-1);
	}
	if (pid == 0)
	{
		stdin_from_file(pipe_a, filepath);
		if (dup2(pipe_a[1], 1) == -1)
			exit_with_perr("dup2()", NULL, NULL);
		close(pipe_a[1]);
		handle_command(raw_command, envp);
	}
	return (pid);
}
