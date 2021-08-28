#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv, char **envp)
{
	int pid_a;
	int pid_b;

	pid_a = fork();

	printf("pid_a: %d\n", pid_a);

	if (pid_a == 0)
	{
		char *argv[] = { "ls", "-l", NULL };
		execve("/bin/ls", argv, envp);
	}

	pid_b = fork();

	if (pid_b == 0)
	{
		char *argv[] = { "cat", "infile", NULL };
		execve("/bin/cat", argv, envp);
	}

	printf("start\n");
	waitpid(pid_a, NULL, 0);
	waitpid(pid_b, NULL, 0);
	printf("end\n");
	return 0;
}