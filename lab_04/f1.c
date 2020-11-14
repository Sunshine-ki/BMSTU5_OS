// Процессы-сироты.
// В программе создаются не менее двух потомков.
// В потомках вызывается sleep().
// Чтобы предок гарантированно завершился раньше своих помков.
// Продемонстрировать с помощью соответствующего вывода информацию
// об идентификаторах процессов и их группе.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define OK 0
#define ERROR 1
#define SLEEP_TIME 2
#define ERROR_FORK -1

int main()
{
	int childpid_1, childpid_2;

	// Первый процесс.
	// Создается дочерний процесс
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
		exit(ERROR);
	}
	else if (!childpid_2)
	{
		// Это процесс потомок.
		sleep(SLEEP_TIME);
		printf("Second child: id: %d ppid: %d  pgrp: %d\n", getpid(), getppid(), getpgrp());
		exit(OK);
	}

	if (childpid_1 && childpid_2)
	{
		// Это процесс предок.
		printf("Parent: id: %d pgrp: %d child1: %d child2: %d\n", getpid(), getpgrp(), childpid_1, childpid_2);
	}

	return OK;
}
