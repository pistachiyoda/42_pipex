#include "./pipex.h"

// fileが存在しているか、読み取り権限があるかを確認する
void	access_file(char *file)
{
	if (access(file, F_OK) < 0 && access(file, R_OK) < 0)
		perror("access()");
}

// commandが実行可能化をチェックする
int		is_executable(char *command)
{
	if (access(command, X_OK) == 0)
		return (1);
	return (0);
}

int		open_file(char *file)
{
	int	file_fd;

	file_fd = open(file, O_RDWR);
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

int main(int argc, char **argv, char **envp)
{
	int		pipe_fds[2];
	pid_t	pid;
	int		status;
	char	buf[32];
	int		file_fd;
	char	**command;
	char	*command_full_path;

	memset(buf, 0, sizeof(buf));

	access_file(argv[1]);
	file_fd = open_file(argv[1]);
	command = split_command(argv[2]);
	command_full_path = resolve_path(command[0], get_env("PATH", envp));
	if (pipe(pipe_fds) < 0)
	{
		perror("pipe()");
		return -1;
	}

	if ((pid = fork()) < 0)
	{
		perror("fork()");
		return -1;
	}
	dup2(file_fd, 0);
	if (pid == 0)
	{
		if (!command_full_path)
			perror("command_full_path()");
		if (execve(command_full_path, &command[0], envp) < 0)
			perror("execve()");
		close(pipe_fds[0]);
		close(pipe_fds[1]);
	}
	else
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);

		waitpid(pid, &status, 0);
    }
    return 0;
}