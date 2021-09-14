/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:43:32 by fmai              #+#    #+#             */
/*   Updated: 2021/09/14 19:13:35 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pipex_bonus.h"

// gnlで入力内容を取得
int	stdin_from_heredoc(int doc_pipe[2], char *limiter)
{
	int		pid;
	char	*line;
	int		ret;

	pid = handle_fork();
	if (pid == 0)
	{
		handle_close(doc_pipe[0]);
		while (true)
		{
			ret = get_next_line(0, &line);
			if (ft_strcmp(line, limiter))
			{
				free(line);
				break ;
			}
			write(doc_pipe[1], line, ft_strlen(line));
			write(doc_pipe[1], "\n", 1);
			free(line);
		}
		handle_close(doc_pipe[1]);
		exit(0);
	}
	return (pid);
}

// 標準入力をcmd1に渡す部分の処理
int	exec_first_command_with_heredoc(
	int pipe_a[2], char *raw_command, char **envp, char *limiter)
{
	int		pid;
	int		heredoc_pid;
	int		doc_pipe[2];

	if (pipe(doc_pipe) == -1)
		exit_with_perr("pipe()", NULL, NULL);
	heredoc_pid = stdin_from_heredoc(doc_pipe, limiter);
	waitpid(heredoc_pid, NULL, 0);
	pid = handle_fork();
	if (pid == 0)
	{
		handle_close(doc_pipe[1]);
		handle_dup2(doc_pipe[0], 0);
		handle_close(doc_pipe[0]);
		handle_close(pipe_a[0]);
		handle_dup2(pipe_a[1], 1);
		handle_close(pipe_a[1]);
		handle_command(raw_command, envp);
	}
	handle_close(doc_pipe[0]);
	handle_close(doc_pipe[1]);
	return (pid);
}
