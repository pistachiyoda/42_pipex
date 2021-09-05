#include "../pipex.h"

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
