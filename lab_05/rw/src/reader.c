#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "constants.h"
#include "reader.h"

struct sembuf StartRead[3] = {
	{WW, S, SEM_FLG}, // Пропускает всех ожидающих запись писателей.
	{CR, S, SEM_FLG}, // Ждет, пока писатель допишет.
	{R, V, SEM_FLG}	  // Увеличивает кол-во активных читателей.
};

struct sembuf StopRead[1] = {
	{R, P, SEM_FLG} // Уменьшает кол-во активных читателей.
};

extern int *counter;

void Reader(const int semId, const int readerId)
{
	int rv = semop(semId, StartRead, 3); // rv = return value
	if (rv == ERROR_SEMOP)
	{
		perror("Читатель не может изменить значение семафора.\n");
		exit(ERROR);
	}

	printf("%sЧитатель %d прочитал: %d\n", GREEN, readerId, *counter);

	rv = semop(semId, StopRead, 1);
	if (rv == ERROR_SEMOP)
	{
		perror("Читатель не может изменить значение семафора.\n");
		exit(ERROR);
	}

	sleep(READER_SLEEP_TIME);
}

void CreateReader(const int semId, const int readerId)
{
	pid_t childpid;
	if ((childpid = fork()) == ERROR_FORK)
	{
		perror("Ошибка при порождении читателя.");
		exit(ERROR);
	}
	else if (!childpid)
	{
		// Это процесс потомок.
		while (TRUE)
			Reader(semId, readerId);
		exit(OK);
	}
}