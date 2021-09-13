/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmai <fmai@student.42tokyo.jp>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 17:43:46 by fmai              #+#    #+#             */
/*   Updated: 2021/09/13 14:34:18 by fmai             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./pipex_bonus.h"

int	main(int argc, char **argv, char **envp)
{
	int				pipe_fds[2];
	t_cmdline_args	*cmdline_args;

	arg_error(argc, argv);
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe()");
		exit(1);
	}
	cmdline_args = (t_cmdline_args *)malloc(sizeof(t_cmdline_args));
	cmdline_args->argc = argc;
	cmdline_args->argv = argv;
	cmdline_args->envp = envp;
	exec(pipe_fds, cmdline_args);
}
