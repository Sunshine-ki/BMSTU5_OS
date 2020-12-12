#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

#include "consumer.h"

// Потребитель.
struct sembuf ConsumerBegin[2] = {
	{SF, P, SEM_FLG}, // Ожидает, что будет заполнена хотя бы одна ячейка буфера.
	{SB, P, SEM_FLG}  // Ожидает, пока другой производитель или потребитель выйдет из критической зоны.
};

struct sembuf ConsumerEnd[2] = {
	{SB, V, SEM_FLG}, // Освобождает критическую зону.
	{SE, V, SEM_FLG}  // Увеличивает кол-во пустых ячеек.
};

void ConsumerRunning(const int semId, const int consumerId)
{
	int rv; // rv = return value

	// Создаем случайные задержки.
	int delay = rand() % DELAY_TIME;
	// sleep(delay);

	// Получаем доступ к критической зоне.
	rv = semop(semId, ConsumerBegin, 2);
	if (rv == ERROR_SEMOP)
	{
		perror("Потребитель не может изменить значение семафора.\n");
		exit(ERROR);
	}

	// TODO: Получить из буфер.
	// printf("Потребитель в критической зоне. Получил из буфера.\n");

	// printf("Consumer: %c\n", value);

	rv = semop(semId, ConsumerEnd, 2);
	if (rv == ERROR_SEMOP)
	{
		perror("Потребитель не может изменить значение семафора.\n");
		exit(ERROR);
	}
}

void CreateConsumer(const int consumerId, const int semId)
{
	pid_t childpid;
	if ((childpid = fork()) == ERROR_FORK)
	{
		// Если при порождении процесса произошла ошибка.
		perror("Ошибка при порождении процесса потребителя.");
		exit(ERROR);
	}
	else if (!childpid) // childpid == 0
	{
		// printf("%sВремя задержки потребителя: %ld\n", YELLOW, time(NULL));

		// Это процесс потомок.
		// TODO: Тут цикл работы потребителей.
		ConsumerRunning(semId, consumerId);

		exit(OK);
	}
}
