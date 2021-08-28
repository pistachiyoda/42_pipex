#include "./pipex.h"

// fileが存在しているか、読み取り権限があるかを確認する
void	check_readability(char *file)
{
	if (access(file, F_OK) < 0 && access(file, R_OK) < 0)
		perror("access()");
}

// fileが存在する場合、書き込み権限があるかを確認する(fileが存在しない場合はopen_or_create_fileで新規作成する)
void	check_writability(char *file)
{
	if (access(file, F_OK) == 0)
		if (access(file, W_OK) < 0)
		{
			perror("access()");
			exit(1);
		}
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
		perror("open()");
	return (file_fd);
}

// ファイルを読み込み、書き込みができる状態で開く。ファイルが存在しない場合は読み込み、書き込み権限を与えた状態でファイルを新規作成する。
int		open_or_create_file(char *file)
{
	int	file_fd;

	file_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (file_fd < 0)
		perror("open()");
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
	if (command == NULL)
		perror("ft_split()");  //todo:free処理
	return (command);
}

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
		if (is_executable(full_path))
		{
			// todo: pathsとfull_pathのfree
			return (full_path);
		}
		i++;
	}
	// todo: pathsのfree
	return (NULL);
}

int	first_exec(int pipe_fds[2], char **argv, char **envp)
{
	pid_t	pid;
	int		file_fd;
	char	**command;
	char	*command_full_path;

	if ((pid = fork()) < 0)
	{
		perror("fork()");
		return -1;
	}

	if (pid == 0)
	{
		check_readability(argv[1]);
		file_fd = open_file(argv[1]);
		// infileの内容を第三引数のコマンドに標準入力として渡すために、infileのfdを標準入力0に置き換えている
		dup2(file_fd, 0);
		command = split_command(argv[2]);
		command_full_path = resolve_path(command[0], get_env("PATH", envp));
		// 使用しないためcloseしておく
        close(pipe_fds[0]);
		// コマンドの実行結果を第四引数のコマンドに標準入力として渡すために、標準出力1をpipe_fds[1]（書き込み側）に置き換えている
		dup2(pipe_fds[1], 1);
		if (!command_full_path)
			perror("command_full_path()");
		execve(command_full_path, &command[0], envp);
		close(pipe_fds[1]);
		close(file_fd);
		exit (1);
	}
	return (pid);
}

int	last_exec(int pipe_fds[2], char **argv, char **envp)
{
	pid_t	pid;
	int		file_fd;
	char	**command;
	char	*command_full_path;

	if ((pid = fork()) < 0)
	{
		perror("fork()");
		return -1;
	}

	if (pid == 0)
	{
		// pipe_fds[1]（書き込み側）で書き込まれた内容を第三引数で受け取るため、pipe_fds[0]（読み込み側）を標準入力0に置き換えている
		dup2(pipe_fds[0], 0);
		check_writability(argv[4]);
		file_fd = open_or_create_file(argv[4]);
		command = split_command(argv[3]);
		command_full_path = resolve_path(command[0], get_env("PATH", envp));
		// 使用しないためcloseしておく
		close(pipe_fds[1]);
		close(pipe_fds[0]);
		// コマンドの実行結果を第五引数のコマンドに標準出力として渡すために、標準出力1をfile_fdに置き換えている
		dup2(file_fd, 1);
		if (!command_full_path)
			perror("command_full_path()");
		if (execve(command_full_path, &command[0], envp) < 0)
			perror("execve()");
		exit(1);
	}
	return (pid);
}

int main(int argc, char **argv, char **envp)
{
	int		pipe_fds[2];
	int		first_pid;
	int		last_pid;
	int		status;

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
	last_pid = last_exec(pipe_fds, argv, envp);

	close(pipe_fds[0]);
	close(pipe_fds[1]);
	if (waitpid(first_pid, NULL, 0) < 0)
		perror("waitpid()");
	if (waitpid(last_pid, NULL, 0) < 0)
		perror("waitpid()");
    exit (0);
}