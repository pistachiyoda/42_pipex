#include "../pipex.h"
// 引数が４つ以外の場合エラーにする
void	arg_error(int argc)
{
	if (argc != 5)
	{
		ft_putstr_fd("Number of argument is incorrect.", 2);
		exit(1);
	}
}

// 二次元配列をfreeする
void	free_2d_array(char **two_d_array)
{
	int	i;

	i = 0;
	while (two_d_array[i] != NULL)
	{
		free(two_d_array[i]);
		i++;
	}
	free(two_d_array);
}

// エラー発生時にエラー出力(perror)とfreeとexitをする関数
void	exit_with_perr(
		char *message, char **free_string_array, char *free_string)
{
	perror(message);
	if (free_string_array != NULL)
		free_2d_array(free_string_array);
	if (free_string != NULL)
		free(free_string);
	exit(1);
}

int	ft_strcmp(char *a, char *b)
{
	while (1)
	{
		if (*a == '\0' && *b == '\0')
			return (1);
		if (*a != *b)
			return (0);
		a++;
		b++;
	}
}

// エラー発生時にエラー出力(putstr)とfreeとexitをする関数
void	exit_with_strerr(char *message, char *cmd_or_file,
					char **free_string_array, char *free_string)
{
	ft_putstr_fd(message, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(cmd_or_file, 2);
	ft_putchar_fd('\n', 2);
	if (free_string_array != NULL)
		free_2d_array(free_string_array);
	if (free_string != NULL)
		free(free_string);
	if (ft_strcmp(message, "command not found"))
		exit(127);
	exit(1);
}
