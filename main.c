#include "./pipex.h"

// 二次元配列をfreeする
void	free_2d_array(char **two_d_array)
{
	int i;

	i = 0;
	while(two_d_array[i] != NULL)
	{
		free(two_d_array[i]);
		i++;
	}
	free(two_d_array);
}

// エラー発生時にエラー出力とfreeとexitをする関数
void	error(char *message, char **free_string_array, char *free_string)
{
	perror(message);
	if (free_string_array != NULL)
		free_2d_array(free_string_array);
	if (free_string != NULL)
		free(free_string);
	exit(1);
}

// fileが存在しているか、読み取り権限があるかを確認する
int	check_readability(char *file)
{
	if (access(file, F_OK) < 0 && access(file, R_OK) < 0)
		return (-1);
	return (0);
}

// fileが存在する場合、書き込み権限があるかを確認する(fileが存在しない場合はopen_or_create_fileで新規作成する)
int	check_writability(char *file)
{
	if (access(file, F_OK) == 0)
		if (access(file, W_OK) < 0)
			return (-1);
	return (0);
}

// commandが実行可能化をチェックする
int		is_executable(char *command)
{
	if (access(command, X_OK) == 0)
		return (1);
	return (0);
}

// ファイルを読み込み、書き込みができる状態で開く
int		open_file(char *file)
{
	int	file_fd;

	file_fd = open(file, O_RDWR);
	if (file_fd < 0)
		return (-1);
	return (file_fd);
}

// ファイルを読み込み、書き込みができる状態で開く。ファイルが存在しない場合は読み込み、書き込み権限を与えた状態でファイルを新規作成する。
int		open_or_create_file(char *file)
{
	int	file_fd;

	file_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	return (file_fd);
}

// 環境変数envpの中からkey(PATH, SHELLなど)に対応する値を取得する
char	*get_env(char *key, char **envp)
{
	int i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0)
			return (ft_split(envp[i], '=')[1]);
		i++;
	}
	return (NULL);
}

char	**split_command(char *unformatted_command)
{
	char	**command;

	command = ft_split(unformatted_command, ' ');
	return (command);
}

// path_envの中からcommandとつなげて実行可能なものを探す
char	*resolve_path(char *command, char *path_env)
{
	char	**paths;
	char	*path;
	char	*full_path;
	int		i;

	paths = ft_split(path_env, ':');
	i = 0;
	while (paths[i] != NULL)
	{
		path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path, command);
		free(path);
		if (is_executable(full_path))
		{
			free_2d_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_2d_array(paths);
	return (NULL);
}

void	handle_command(char *raw_command, char **envp)
{
	char	**command;
	char	*command_full_path;

	command = split_command(raw_command);
	if (command == NULL)
		error("split_command()", NULL, NULL);
	command_full_path = resolve_path(command[0], get_env("PATH", envp));
	if (command_full_path == NULL)
		error("resolve_full_path()", command, NULL);
	execve(command_full_path, &command[0], envp);
	error("execve()", command, command_full_path);
}

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
			error("check_readability()", NULL, NULL);
		file_fd = open_file(argv[1]);
		if (file_fd == -1)
			error("open_file()", NULL, NULL);
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
			error("check_writability()", NULL, NULL);
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

int main(int argc, char **argv, char **envp)
{
	int		pipe_fds[2];
	int		first_pid;
	int		last_pid;

	if (argc != 5)
	{
		ft_putstr_fd("Number of argument is incorrect.", 1);
		exit(0);
	}

	if (pipe(pipe_fds) < 0)
	{
		perror("pipe()");
		return -1;
	}

	first_pid = first_exec(pipe_fds, argv, envp);
	if (first_pid == -1)
		error("fork()", NULL, NULL);
	last_pid = last_exec(pipe_fds, argv, envp);
	if (last_pid == -1)
		error("fork()", NULL, NULL);
	if (close(pipe_fds[0]) == -1)
		error("close()", NULL, NULL);
	if (close(pipe_fds[1]) == -1)
		error("close()", NULL, NULL);
	if (waitpid(first_pid, NULL, 0) < 0)
		error("waitpid()", NULL, NULL);
	if (waitpid(last_pid, NULL, 0) < 0)
		error("waitpid()", NULL, NULL);
    exit (0);
}