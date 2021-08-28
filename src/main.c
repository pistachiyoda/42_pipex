#include "../pipex.h"
int main(int argc, char **argv, char **envp)
{
	int		pipe_fds[2];
	int		first_pid;
	int		last_pid;
	int		status;

	if (argc != 5)
	{
		ft_putstr_fd("Number of argument is incorrect.", 1);
		exit(0);
	}

	if (pipe(pipe_fds) < 0)
	{
		perror("pipe()");
		return -1;
	}

	first_pid = first_exec(pipe_fds, argv, envp);
	if (first_pid == -1)
		error("fork()", NULL, NULL);
	last_pid = last_exec(pipe_fds, argv, envp);
	if (last_pid == -1)
		error("fork()", NULL, NULL);
	if (close(pipe_fds[0]) == -1)
		error("close()", NULL, NULL);
	if (close(pipe_fds[1]) == -1)
		error("close()", NULL, NULL);
	if (waitpid(first_pid, &status, 0) < 0)
		error("waitpid()", NULL, NULL);
	if (waitpid(last_pid, &status, 0) < 0)
		error("waitpid()", NULL, NULL);
	exit(WEXITSTATUS(status));
}