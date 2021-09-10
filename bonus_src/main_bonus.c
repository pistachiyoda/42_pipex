/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:43:46 by fmai              #+#    #+#             */
/*   Updated: 2021/09/09 23:53:14 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pipex_bonus.h"

int	main(int argc, char **argv, char **envp)
{
	int		pipe_fds[2];

	arg_error(argc);
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe()");
		exit(1);
	}
	exec(pipe_fds, argv, envp);
}