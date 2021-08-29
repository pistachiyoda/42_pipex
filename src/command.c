#include "../pipex.h"
// commandが実行可能化をチェックする
int	is_executable(char *command)
{
	if (access(command, X_OK) == 0)
		return (1);
	return (0);
}

// 環境変数envpの中からkey(PATH, SHELLなど)に対応する値を取得する
char	*get_env(char *key, char **envp)
{
	int	i;

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

// コマンドを実行できる形にトリムして実行する
void	handle_command(char *raw_command, char **envp)
{
	char	**command;
	char	*command_full_path;

	command = split_command(raw_command);
	if (command == NULL)
		error("split_command()", NULL, NULL);
	command_full_path = resolve_path(command[0], get_env("PATH", envp));
	if (command_full_path == NULL)
		error_str("command not found", command[0], command, NULL);
	execve(command_full_path, &command[0], envp);
	error("execve()", command, command_full_path);
}
