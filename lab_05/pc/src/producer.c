#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

#include "producer.h"

// Производитель.
struct sembuf ProducerBegin[2] = {
	{SE, P, SEM_FLG}, // Ожидает освобождения хотя бы одной ячейки буфера.
	{SB, P, SEM_FLG}  // Ожидает, пока другой производитель или потребитель выйдет из критической зоны.
};
struct sembuf ProducerEnd[2] = {
	{SB, V, SEM_FLG}, // Освобождает критическую зону.
	{SF, V, SEM_FLG}  // Увеличивает кол-во заполненных ячеек.
};

void ProducerRunning(const int semId, const int producerId, const char value)
{
	int rv; // rv = return value

	// Создаем случайные задержки.

	// Получаем доступ к критической зоне.
	rv = semop(semId, ProducerBegin, 2);
	if (rv == ERROR_SEMOP)
	{
		perror("Произведитель не может изменить значение семафора.\n");
		exit(ERROR);
	}

	// TODO: Положить в буфер.
	// printf("Производитель в критической зоне. Положил в буфер. ");
	// printf("Производитель: %c\n", value);

	rv = semop(semId, ProducerEnd, 2);
	if (rv == ERROR_SEMOP)
	{
		perror("Произведитель не может изменить значение семафора.\n");
		exit(ERROR);
	}
}

void CreateProducer(const int producerId, const int semId, char value)
{
	pid_t childpid;
	if ((childpid = fork()) == ERROR_FORK)
	{
		// Если при порождении процесса произошла ошибка.
		perror("Ошибка при порождении процесса производителя.");
		exit(ERROR);
	}
	else if (!childpid) // childpid == 0
	{
		// printf("%sВремя задержки производителя: %ld\n", GREEN, time(NULL));

		// Это процесс потомок.
		// TODO: Тут цикл работы производителей.
		ProducerRunning(semId, producerId, value);

		exit(OK);
	}
}