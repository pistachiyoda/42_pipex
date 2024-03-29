/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:43:32 by fmai              #+#    #+#             */
/*   Updated: 2021/09/14 19:10:20 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pipex_bonus.h"

void	wait_pids(int *pids, int commands)
{
	int		i;
	int		status;

	i = 0;
	while (i < commands)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			exit_with_perr("waitpid()", NULL, NULL);
		i++;
	}
	exit(WEXITSTATUS(status));
}

// 最初の入力部分の処理。ヒアドキュメントなら標準入力をcmd1に渡す部分、マルチパイプならinfileをcmd1に渡す部分を処理。
int	exec_first(int pipe_a[2], t_cmdline_args *cmdline_args, int pids[10000])
{
	if (ft_strcmp(cmdline_args->argv[1], "here_doc"))
	{
		pids[0] = exec_first_command_with_heredoc(
				pipe_a, cmdline_args->argv[3],
				cmdline_args->envp, cmdline_args->argv[2]);
		return (2);
	}
	else
	{
		pids[0] = exec_first_command_with_file(
				pipe_a, cmdline_args->argv[2], cmdline_args->envp,
				cmdline_args->argv[1]);
		return (cmdline_args->argc - 3);
	}
}

// 出力先をファイルにするためにfdを操作し、最後のコマンドを実行
int	exec_last(
	int pipe_a[2], char *raw_command,
	t_cmdline_args *cmdline_args, char *filepath)
{
	pid_t	pid;
	int		file_fd;

	pid = handle_fork();
	if (pid == 0)
	{
		handle_close(pipe_a[1]);
		check_writability(filepath);
		if (ft_strcmp(cmdline_args->argv[1], "here_doc"))
			file_fd = open_or_create_file(
					filepath, O_APPEND | O_WRONLY | O_CREAT);
		else
			file_fd = open_or_create_file(
					filepath, O_TRUNC | O_WRONLY | O_CREAT);
		handle_dup2(pipe_a[0], 0);
		handle_dup2(file_fd, 1);
		handle_close(pipe_a[0]);
		handle_close(file_fd);
		handle_command(raw_command, cmdline_args->envp);
	}
	return (pid);
}

// メイン処理
void	exec(int pipe_a[2], t_cmdline_args *cmdline_args)
{
	int		pids[10000];
	int		i;
	int		pipe_b[2];
	int		cmd_cnt;

	cmd_cnt = exec_first(pipe_a, cmdline_args, pids);
	i = 1;
	while (i < cmd_cnt - 1)
	{
		if (pipe(pipe_b) == -1)
			exit_with_perr("pipe()", NULL, NULL);
		pids[i] = exec_command(
				pipe_a, pipe_b, cmdline_args->envp,
				cmdline_args->argv[cmdline_args->argc - (cmd_cnt - i) - 1]);
		handle_close(pipe_a[0]);
		handle_close(pipe_a[1]);
		pipe_a[0] = pipe_b[0];
		pipe_a[1] = pipe_b[1];
		i++;
	}
	pids[i] = exec_last(pipe_a, cmdline_args->argv[cmdline_args->argc - 2],
			cmdline_args, cmdline_args->argv[cmdline_args->argc - 1]);
	handle_close(pipe_a[0]);
	handle_close(pipe_a[1]);
	wait_pids(pids, cmd_cnt);
}
