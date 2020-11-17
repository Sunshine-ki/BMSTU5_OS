// Написать программу, в которой предок и потомок
// обмениваются сообщением через программный канал.

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h> // bool.
#include <string.h>

#define OK 0
#define ERROR 1
#define ERROR_FORK -1
#define ERROR_PIPE -1
#define LEN 32
#define FIRST_TEXT "First child write\n"
#define SECOND_TEXT "Second child write\n"

_Bool flag = false;

void check_status(int status);

void catch_sig(int sig_numb)
{
	flag = true;
	printf("catch_sig: %d\n", sig_numb);
}

int main()
{
	int childpid_1, childpid_2;
	int fd[2];

	signal(SIGINT, catch_sig);
	printf("Parent: нажмите \"CTRL+C\", если хотите получить сообщение.\n\n");
	sleep(2);

	if (pipe(fd) == ERROR_PIPE)
	{
		perror("Can\'t pipe.\n");
		return ERROR;
	}

	if ((childpid_1 = fork()) == ERROR_FORK)
	{
		// Если при порождении процесса произошла ошибка.
		perror("Can\'t fork.\n");
		return ERROR;
	}
	else if (!childpid_1 && flag) // Это процесс потомок.
	{
		close(fd[0]);
		write(fd[1], FIRST_TEXT, strlen(FIRST_TEXT) + 1);
		exit(OK);
	}

	// Аналогично 2 процесс.
	if ((childpid_2 = fork()) == ERROR_FORK)
	{
		perror("Can\'t fork.\n");
		return ERROR;
	}
	else if (!childpid_2 && flag) // Это процесс потомок.
	{
		close(fd[0]);
		write(fd[1], SECOND_TEXT, strlen(SECOND_TEXT) + 1);
		exit(OK);
	}

	if (childpid_1 && childpid_2)
	{
		char text[LEN], text2[LEN];
		pid_t child_pid;
		int status;

		close(fd[1]);

		int a = read(fd[0], text, LEN);
		if (!a)
			return OK;
		read(fd[0], text2, LEN);

		printf("A: %d\n", a);

		printf("Text: %s\n", text);
		printf("Text: %s\n", text2);

		child_pid = wait(&status);
		check_status(status);

		child_pid = wait(&status);
		check_status(status);
	}

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