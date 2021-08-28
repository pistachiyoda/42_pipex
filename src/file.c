#include "../pipex.h"

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