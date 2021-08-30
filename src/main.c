#include "../pipex.h"
void end(void)__attribute__((destructor));
void end(void)
{
 int ret = system("leaks pipex &> leaks.txt");
 if (ret) printf("\e[31m!leak detected!\e[0m\n");
}

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
