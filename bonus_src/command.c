/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:43:19 by fmai              #+#    #+#             */
/*   Updated: 2021/09/09 15:58:52 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"
// commandが実行可能化をチェックする
bool	is_executable(char *command)
{
	if (access(command, X_OK) == 0)
		return (true);
	return (false);
}

// 環境変数envpの中からkey(PATH, SHELLなど)に対応する値を取得する
char	**get_env_str(char *key, char **envp)
{
	int		i;
	char	**value;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], key, ft_strlen(key)) == 0)
		{
			value = ft_split(envp[i], '=');
			return (value);
		}
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
	char	**path_env;

	command = split_command(raw_command);
	if (command == NULL)
		exit_with_perr("split_command()", NULL, NULL);
	if (is_executable(command[0]))
	{
		execve(command[0], command, envp);
		exit_with_perr("execve()", command, NULL);
	}
	path_env = get_env_str("PATH", envp);
	command_full_path = resolve_path(command[0], path_env[1]);
	free_2d_array(path_env);
	if (command_full_path == NULL)
		exit_with_strerr("command not found", command[0], command, NULL);
	execve(command_full_path, command, envp);
	exit_with_perr("execve()", command, command_full_path);
}
