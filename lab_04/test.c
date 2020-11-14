#include <stdio.h>
#include <unistd.h>

#define N 2
#define SECONDS 2

int main()
{
	printf("PARENT: PID=%d, GRP=%d\n", getpid(), getpgrp());
	int childpid;
	for (size_t i = 0; i < N; i++)
	{
		if ((childpid = fork()) == -1)
		{
			perror("Can’t fork.\n");
			return 1;
		}
		else if (childpid == 0)
		{
			sleep(SECONDS);
			printf("CHILD : PID=%d, GRP=%d, PPID=%d\n", getpid(), getppid(), getpgrp());
			return 0;
		}
	}
	printf("PARENT IS GOING TO DIE!\n");
	return 0;
}