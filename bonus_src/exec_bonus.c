/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:43:32 by fmai              #+#    #+#             */
/*   Updated: 2021/09/12 16:30:53 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pipex_bonus.h"

int		cnt_commands(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		i++;
	}
	return (i - 3);
}

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

int		exec_command(int pipe_a[2], int pipe_b[2], char *raw_command, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork()");
		return (-1);
	}
	if (pid == 0)
	{
		close(pipe_a[1]);
		close(pipe_b[0]);
		if (dup2(pipe_a[0], 0) == -1)
			exit_with_perr("dup2()", NULL, NULL);
		if (dup2(pipe_b[1], 1) == -1)
			exit_with_perr("dup2()", NULL, NULL);
		close(pipe_a[0]);
		close(pipe_b[1]);
		handle_command(raw_command, envp);
	}
	return (pid);
}

int		exec_last_command(int pipe_a[2], char *raw_command, char **envp, char *filepath)
{
	pid_t	pid;
	int		file_fd;

	pid = fork();
	if (pid == -1)
	{
		perror("fork()");
		return (-1);
	}
	if (pid == 0)
	{
		close(pipe_a[1]);
		if (!(is_writable(filepath)))
			exit_with_strerr("permission denied", filepath, NULL, NULL);
		file_fd = open_or_create_appendfile(filepath);
		if (file_fd == -1)
			exit_with_perr("open_or_create_appendfile()", NULL, NULL);
		close(0);
		if (dup2(pipe_a[0], 0) == -1)
			exit_with_perr("dup2()", NULL, NULL);
		close(1);
		if (dup2(file_fd, 1) == -1)
			exit_with_perr("dup2()", NULL, NULL);
		close(pipe_a[0]);
		close(file_fd);
		handle_command(raw_command, envp);
	}
	return (pid);
}

void	wait_pids(int *pids, int commands)
{
	int		i;
	int		status;

	i = 0;
	while (i < commands)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
	exit(WEXITSTATUS(status));
}

void	exec(int pipe_a[2], char **argv, char **envp)
{
	int		pids[10000];
	int		commands;
	int		i;
	int		pipe_b[2];

	commands = cnt_commands(argv);
	if (ft_strcmp(argv[1], "here_doc"))
	{
		pids[0] = exec_first_command_with_heredoc(pipe_a, argv[3], envp, argv[2]);
		i = 2;
	}
	else
	{
		pids[0] = exec_first_command_with_file(pipe_a, argv[2], envp, argv[1]);
		i = 1;
	}
	if (pids[0] == -1)
		exit_with_perr("fork()", NULL, NULL);
	while (i < (commands - 1))
	{
		pipe(pipe_b);
		pids[i] = exec_command(pipe_a, pipe_b, argv[i + 2], envp);
		if (pids[i] == -1)
			exit_with_perr("fork()", NULL, NULL);
		close(pipe_a[0]);
		close(pipe_a[1]);
		pipe_a[0] = pipe_b[0];
		pipe_a[1] = pipe_b[1];
		i++;
	}
	if (ft_strcmp(argv[1], "here_doc"))
		pids[i] = exec_last_command(pipe_a, argv[i + 2], envp, argv[commands + 2]);
	else
		pids[i] = exec_last_command(pipe_a, argv[i + 2], envp, argv[commands + 2]);
	if (pids[i] == -1)
		exit_with_perr("fork()", NULL, NULL);
	close(pipe_a[0]);
	close(pipe_a[1]);
	wait_pids(pids, commands);
}
