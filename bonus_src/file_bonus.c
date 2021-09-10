/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:43:37 by fmai              #+#    #+#             */
/*   Updated: 2021/09/09 23:53:09 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pipex_bonus.h"

// fileが存在しているか、読み取り権限があるかを確認する
void	is_readable(char *file)
{
	if (access(file, F_OK) == -1)
		exit_with_strerr("no such file or directory", file, NULL, NULL);
	if (access(file, R_OK) == -1)
		exit_with_strerr("permission denied", file, NULL, NULL);
}

// fileが存在する場合、書き込み権限があるかを確認する(fileが存在しない場合はopen_or_create_fileで新規作成する)
bool	is_writable(char *file)
{
	if (access(file, F_OK) == 0)
		if (access(file, W_OK) == -1)
			return (false);
	return (true);
}

// ファイルを読み込み、書き込みができる状態で開く
int	open_file(char *file)
{
	int	file_fd;

	file_fd = open(file, O_RDWR);
	if (file_fd == -1)
		return (-1);
	return (file_fd);
}

// ファイルを読み込み、書き込みができる状態で開く。ファイルが存在しない場合は読み込み、書き込み権限を与えた状態でファイルを新規作成する。
int	open_or_create_file(char *file)
{
	int	file_fd;

	file_fd = open(file, O_WRONLY | O_CREAT | O_TRUNC,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (file_fd == -1)
		return (-1);
	return (file_fd);
}
