/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:43:32 by fmai              #+#    #+#             */
/*   Updated: 2021/09/09 23:53:00 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pipex_bonus.h"
// パイプ前半後半でコマンドを分けて実行する。
void	exec(int pipe_fds[2], char **argv, char **envp)
{
	int		first_pid;
	int		last_pid;
	int		status;

	first_pid = first_exec(pipe_fds, argv, envp);
	if (first_pid == -1)
		exit_with_perr("fork()", NULL, NULL);
	last_pid = last_exec(pipe_fds, argv, envp);
	if (last_pid == -1)
		exit_with_perr("fork()", NULL, NULL);
	if (close(pipe_fds[0]) == -1)
		exit_with_perr("close()", NULL, NULL);
	if (close(pipe_fds[1]) == -1)
		exit_with_perr("close()", NULL, NULL);
	if (waitpid(first_pid, &status, 0) < 0)
		exit_with_perr("waitpid()", NULL, NULL);
	if (waitpid(last_pid, &status, 0) < 0)
		exit_with_perr("waitpid()", NULL, NULL);
	exit(WEXITSTATUS(status));
}

// 前半部の実行
int	first_exec(int pipe_fds[2], char **argv, char **envp)
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
		if (close(pipe_fds[0]) == -1)
			exit_with_perr("close()", NULL, NULL);
		is_readable(argv[1]);
		file_fd = open_file(argv[1]);
		if (file_fd == -1)
			exit_with_perr("open()", NULL, NULL);
		if (dup2(file_fd, 0) == -1)
			exit_with_perr("dup2()", NULL, NULL);
		if (dup2(pipe_fds[1], 1) == -1)
			exit_with_perr("dup2()", NULL, NULL);
		handle_command(argv[2], envp);
	}
	return (pid);
}

// 後半部の実行
int	last_exec(int pipe_fds[2], char **argv, char **envp)
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
		if (close(pipe_fds[1]) == -1)
			exit_with_perr("close()", NULL, NULL);
		if (!(is_writable(argv[4])))
			exit_with_strerr("permission denied", argv[4], NULL, NULL);
		file_fd = open_or_create_file(argv[4]);
		if (file_fd == -1)
			exit_with_perr("open_or_create_file()", NULL, NULL);
		if (dup2(pipe_fds[0], 0) == -1)
			exit_with_perr("dup2()", NULL, NULL);
		if (dup2(file_fd, 1) == -1)
			exit_with_perr("dup2()", NULL, NULL);
		handle_command(argv[3], envp);
	}
	return (pid);
}
