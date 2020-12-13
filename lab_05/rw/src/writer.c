#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "constants.h"
#include "writer.h"

struct sembuf StartWrite[5] = {
	{WW, V, SEM_FLG}, // Увеличивает кол-во ожидающий писателей.
	{R, S, SEM_FLG},  // Ждет, пока все читатели дочитают.
	{CW, P, SEM_FLG}, // Ждет, пока писатель допишет.
	{CR, V, SEM_FLG}, // Запрещает читать.
	{WW, P, SEM_FLG}  // Уменьшает кол-во ожидающий писателей. Т.к. он уже не ждет, а пишет
};

struct sembuf StopWrite[2] = {
	{CR, P, SEM_FLG}, // Разрешает читать
	{CW, V, SEM_FLG}  // Разрешает писать.
};

extern int *counter;

void Writer(const int semId, const int writerId)
{
	int rv = semop(semId, StartWrite, 5); // rv = return value
	if (rv == ERROR_SEMOP)
	{
		perror("Писатель не может изменить значение семафора.\n");
		exit(ERROR);
	}

	(*counter)++;
	printf("%sПисатель %d записал: %d\n", YELLOW, writerId, *counter);

	rv = semop(semId, StopWrite, 2);
	if (rv == ERROR_SEMOP)
	{
		perror("Писатель не может изменить значение семафора.\n");
		exit(ERROR);
	}

	sleep(WRITER_SLEEP_TIME);
}

void CreateWriter(const int semId, const int writerId)
{
	pid_t childpid;
	if ((childpid = fork()) == ERROR_FORK)
	{
		perror("Ошибка при порождении писателя.");
		exit(ERROR);
	}
	else if (!childpid)
	{
		// Это процесс потомок.
		while (TRUE)
			Writer(semId, writerId);
		exit(OK);
	}
}