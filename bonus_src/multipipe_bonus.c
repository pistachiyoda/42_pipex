/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multipipe_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:43:32 by fmai              #+#    #+#             */
/*   Updated: 2021/09/13 23:56:25 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pipex_bonus.h"
// ファイルのオープンとファイルからの入力を受け取るためのfdの操作
void	stdin_from_file(char *filepath)
{
	int		file_fd;

	check_readability(filepath);
	file_fd = open_file(filepath);
	handle_dup2(file_fd, 0);
	handle_close(file_fd);
}

// ファイルからの入力を受け取りと、次のコマンドに渡すためのfdの操作とコマンドの実行
int	exec_first_command_with_file(
	int pipe_a[2], char *raw_command, char **envp, char *filepath)
{
	int		pid;

	pid = handle_fork();
	if (pid == 0)
	{
		handle_close(pipe_a[0]);
		stdin_from_file(filepath);
		handle_dup2(pipe_a[1], 1);
		handle_close(pipe_a[1]);
		handle_command(raw_command, envp);
	}
	return (pid);
}

// コマンド~コマンド間の処理
int	exec_command(
	int pipe_a[2], int pipe_b[2], char **envp, char *raw_command)
{
	pid_t	pid;

	pid = handle_fork();
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
