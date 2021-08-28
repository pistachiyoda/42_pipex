#include "../pipex.h"

int	first_exec(int pipe_fds[2], char **argv, char **envp)
{
	pid_t	pid;
	int		file_fd;

	if ((pid = fork()) < 0)
	{
		perror("fork()");
		return -1;
	}

	if (pid == 0)
	{
		// 使用しないためcloseしておく
        if (close(pipe_fds[0]) == -1)
			error("close()", NULL, NULL);
		if (check_readability(argv[1]) == -1)
			error_str("no such file or directory", argv[1], NULL, NULL);
		file_fd = open_file(argv[1]);
		if (file_fd == -1)
			error_str("permission denied", argv[1], NULL, NULL);
		// infileの内容を第三引数のコマンドに標準入力として渡すために、infileのfdを標準入力0に置き換えている
		if (dup2(file_fd, 0) == -1)
			error("dup2()", NULL, NULL);
		// コマンドの実行結果を第四引数のコマンドに標準入力として渡すために、標準出力1をpipe_fds[1]（書き込み側）に置き換えている
		if (dup2(pipe_fds[1], 1) == -1)
			error("dup2()", NULL, NULL);
		handle_command(argv[2], envp);
	}
	return (pid);
}

int	last_exec(int pipe_fds[2], char **argv, char **envp)
{
	pid_t	pid;
	int		file_fd;

	if ((pid = fork()) < 0)
	{
		perror("fork()");
		return (-1);
	}

	if (pid == 0)
	{
		// 使用しないためcloseしておく
		if (close(pipe_fds[1]) == -1)
			error("close()", NULL, NULL);
		if (check_writability(argv[4]) == -1)
			error_str("permission denied", argv[4], NULL, NULL);
		file_fd = open_or_create_file(argv[4]);
		if (file_fd == -1)
			error("open_or_create_file()", NULL, NULL);
		// pipe_fds[1]（書き込み側）で書き込まれた内容を第三引数で受け取るため、pipe_fds[0]（読み込み側）を標準入力0に置き換えている
		if (dup2(pipe_fds[0], 0) == -1)
			error("dup2()", NULL, NULL);
		// コマンドの実行結果を第五引数のコマンドに標準出力として渡すために、標準出力1をfile_fdに置き換えている
		if (dup2(file_fd, 1) == -1)
			error("dup2()", NULL, NULL);
		handle_command(argv[3], envp);
	}
	return (pid);
}