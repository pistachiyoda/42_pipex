#include "../pipex.h"
int	main(int argc, char **argv, char **envp)
{
	int		pipe_fds[2];

	arg_error(argc);
	if (pipe(pipe_fds) < 0)
	{
		perror("pipe()");
		return (-1);
	}
	exec(pipe_fds, argv, envp);
}
