/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/13 16:52:12 by fmai              #+#    #+#             */
/*   Updated: 2021/09/13 18:12:16 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pipex_bonus.h"

void	handle_close(int fd)
{
	if (close(fd) == -1)
		exit_with_perr("close()", NULL, NULL);
}

void	handle_dup2(int old, int new)
{
	if (dup2(old, new) == -1)
		exit_with_perr("dup2", NULL, NULL);
}
