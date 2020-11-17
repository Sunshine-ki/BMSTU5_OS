// Написать программу по схеме первого задания,
// но в процессе - предке выполнить системный вызов wait().
// Убедиться, что в этом случае идентификатор процесса
// потомка на 1 больше идентификатора процесса - предка.

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

#define OK 0
#define ERROR 1
#define ERROR_FORK -1
#define SLEEP_TIME 2

void check_status(int status);

int main()
{
	int childpid_1, childpid_2;

	if ((childpid_1 = fork()) == ERROR_FORK)
	{
		// Если при порождении процесса произошла ошибка.
		perror("Can\'t fork.\n");
		return ERROR;
	}
	else if (!childpid_1)
	{
		// Это процесс потомок.
		sleep(SLEEP_TIME);
		printf("First child: id: %d ppid: %d  pgrp: %d\n", getpid(), getppid(), getpgrp());
		exit(OK);
	}

	// Аналогично 2 процесс.
	if ((childpid_2 = fork()) == ERROR_FORK)
	{
		perror("Can\'t fork.\n");
		return ERROR;
	}
	else if (!childpid_2)
	{
		// Это процесс потомок.
		sleep(SLEEP_TIME);
		printf("Second child: id: %d ppid: %d  pgrp: %d\n", getpid(), getppid(), getpgrp());
		exit(OK);
	}

	int status;
	pid_t child_pid;

	child_pid = wait(&status);
	printf("status: %d, child_pid: %d\n", status, child_pid);
	check_status(status);

	child_pid = wait(&status);
	printf("status: %d, child_pid: %d\n", status, child_pid);
	check_status(status);

	printf("Parent: id: %d pgrp: %d child1: %d child2: %d\n", getpid(), getpgrp(), childpid_1, childpid_2);

	return OK;
}

void check_status(int status)
{
	if (WIFEXITED(status))
	{
		printf("Дочерний процесс завершен нормально.\n\n");
		return;
	}

	if (WEXITSTATUS(status))
	{
		printf("Код завершения дочернего процесса %d.\n", WIFEXITED(status));
		return;
	}

	if (WIFSIGNALED(status))
	{
		printf("Дочерний процесс завершается неперехватываемым сигналом.\n");
		printf("Номер сигнала %d.\n", WTERMSIG(status));
		return;
	}

	if (WIFSTOPPED(status))
	{
		printf("Дочерний процесс остановился.\n");
		printf("Номер сигнала %d.", WSTOPSIG(status));
	}
}