#include "../pipex.h"

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

// エラー発生時にエラー出力とfreeとexitをする関数
void	error_str(char *message, char* cmd_or_file, char **free_string_array, char *free_string)
{
	ft_putstr_fd(message, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(cmd_or_file, 2);
	if (free_string_array != NULL)
		free_2d_array(free_string_array);
	if (free_string != NULL)
		free(free_string);
	exit(1);
}