#include "../pipex.h"
void end(void)__attribute__((destructor));
void end(void)
{
	pid_t current_pid;
	char cmd_str[50];

	current_pid = getpid(); 
    sprintf(cmd_str, "%s %d %s\n", "leaks -q", current_pid, ">> leaks.txt 2>&1");
	// ft_putstr_fd(cmd_str, 2);
	int ret = system(cmd_str);
	if (ret) ft_putstr_fd("\e[31m!leak detected!\e[0m\n", 2);
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
