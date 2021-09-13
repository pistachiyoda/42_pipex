/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:43:32 by fmai              #+#    #+#             */
/*   Updated: 2021/09/13 13:17:51 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pipex_bonus.h"

int	exec_last_command(
	int pipe_a[2], char *raw_command, char **envp, char *filepath)
{
	pid_t	pid;
	int		file_fd;

	pid = fork();
	if (pid == -1)
		exit_with_perr("fork()", NULL, NULL);
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

void	exec(int pipe_a[2], char **argv, char **envp, int argc)
{
	int		pids[10000];
	int		i;
	int		pipe_b[2];
	int		cmd_cnt;

	if (ft_strcmp(argv[1], "here_doc"))
	{
		pids[0] = exec_first_command_with_heredoc(
				pipe_a, argv[3], envp, argv[2]);
		cmd_cnt = 2;
	}
	else
	{
		pids[0] = exec_first_command_with_file(pipe_a, argv[2], envp, argv[1]);
		cmd_cnt = argc - 3;
	}
	i = 1;
	while (i < cmd_cnt)
	{
		pipe(pipe_b);
		pids[i] = exec_command(
				pipe_a, pipe_b, argv[argc - (cmd_cnt - i) - 1], envp);
		if (pids[i] == -1)
			exit_with_perr("fork()", NULL, NULL);
		close(pipe_a[0]);
		close(pipe_a[1]);
		pipe_a[0] = pipe_b[0];
		pipe_a[1] = pipe_b[1];
		i++;
	}
	if (ft_strcmp(argv[1], "here_doc"))
		pids[i] = exec_last_command(
				pipe_a, argv[argc - 2], envp, argv[argc - 1]);
	else
		pids[i] = exec_last_command(
				pipe_a, argv[argc - 2], envp, argv[argc - 1]);
	if (pids[i] == -1)
		exit_with_perr("fork()", NULL, NULL);
	close(pipe_a[0]);
	close(pipe_a[1]);
	wait_pids(pids, cmd_cnt);
}
