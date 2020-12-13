#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

#include "consumer.h"
#include "buffer.h"

extern int *consumer_pos;
extern char *buffer;

// Потребитель.
struct sembuf ConsumerBegin[2] = {
	{SF, P, SEM_FLG}, // Ожидает, что будет заполнена хотя бы одна ячейка буфера.
	{SB, P, SEM_FLG}  // Ожидает, пока другой производитель или потребитель выйдет из критической зоны.
};

struct sembuf ConsumerEnd[2] = {
	{SB, V, SEM_FLG}, // Освобождает критическую зону.
	{SE, V, SEM_FLG}  // Увеличивает кол-во пустых ячеек.
};

void ConsumerRunning(const int semId, const int consumerId, Delay *delays)
{
	// Создаем случайные задержки.
	sleep(getDelay(delays));
	// printf("%s Задержка потребителя: %d\n", RED, getDelay(delays));

	// Получаем доступ к критической зоне.
	int rv = semop(semId, ConsumerBegin, 2); // rv = return value
	if (rv == ERROR_SEMOP)
	{
		perror("Потребитель не может изменить значение семафора.\n");
		exit(ERROR);
	}

	// Получить из буфера.
	printf("%sПотребитель %d в критической зоне. Получил из буфера: %s%c\n", BLUE, consumerId, GREEN, buffer[*consumer_pos]);
	(*consumer_pos)++;

	rv = semop(semId, ConsumerEnd, 2);
	if (rv == ERROR_SEMOP)
	{
		perror("Потребитель не может изменить значение семафора.\n");
		exit(ERROR);
	}

	puts("");
}

void CreateConsumer(const int consumerId, const int semId, Delay *delays)
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
		// Это процесс потомок.

		// Каждый потребитель потребляет
		// NUMBER_OF_WORKS товаров.
		for (int i = 0; i < NUMBER_OF_WORKS; i++)
			ConsumerRunning(semId, consumerId, delays);

		exit(OK);
	}
}
