// Написать программу, в которой процесс - потомок вызывает системный
// вызов exec(), а процесс - предок ждет завершения процесса - потомка.
// Следует создать не менее двух потомков.

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define OK 0
#define ERROR 1
#define ERROR_FORK -1
#define ERROR_EXEC -1

void check_status(int status);

// int execl(char *name, char *arg0, ... /*NULL*/);
// Суффикс l (список):
// Аргументы командной строки передаются в форме списка arg0, arg1.... argn, NULL.
// Эту форму используют, если количество аргументов известно.
// В случае успеха функция exec не возвращает значения. При неудаче возвращается значение —1
int main()
{
	int childpid_1, childpid_2;
	int status;
	pid_t child_pid;

	// Первый процесс.
	// Создается дочерний процесс
	// Тут содается 2 идентичных процесса.
	if ((childpid_1 = fork()) == ERROR_FORK)
	{
		// Если при порождении процесса произошла ошибка.
		perror("Can\'t fork.\n");
		return ERROR;
	}
	else if (!childpid_1) // Это процесс потомок (ребенок).
	{
		// p - определяет, что функция будет искать "дочернюю"
		// программу     в    директориях,    определяемых
		// переменной среды DOS PATH. Без суффикса p поиск
		// будет  производиться только в рабочем каталоге.
		printf("First child: id: %d ppid: %d  pgrp: %d\n", getpid(), getppid(), getpgrp());

		if (execlp("cat", "cat", "text1.txt", NULL) == ERROR_EXEC)
		{
			perror("First child can\'t exec");
			exit(ERROR);
		}
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
		printf("Second child: id: %d ppid: %d  pgrp: %d\n", getpid(), getppid(), getpgrp());
		if (execlp("cat", "cat", "text2.txt", NULL) == ERROR_EXEC)
		{
			perror("Second child can\'t exec");
			exit(ERROR);
		}
		exit(OK);
	}

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