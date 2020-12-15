#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

#include "producer.h"
#include "buffer.h"

extern int *producer_pos;
extern char *buffer;

// Производитель.
struct sembuf ProducerBegin[2] = {
	{SE, P, SEM_FLG}, // Ожидает освобождения хотя бы одной ячейки буфера.
	{SB, P, SEM_FLG}  // Ожидает, пока другой производитель или потребитель выйдет из критической зоны.
};
struct sembuf ProducerEnd[2] = {
	{SB, V, SEM_FLG}, // Освобождает критическую зону.
	{SF, V, SEM_FLG}  // Увеличивает кол-во заполненных ячеек.
};

void ProducerRunning(const int semId, const int producerId, Delay *delays)
{
	// Создаем случайные задержки.
	sleep(getDelay(delays));
	// printf("%s Задержка потребителя: %d\n", RED, getDelay(delays));

	// Получаем доступ к критической зоне.
	int rv = semop(semId, ProducerBegin, 2); // rv = return value
	if (rv == ERROR_SEMOP)
	{
		perror("Произведитель не может изменить значение семафора.\n");
		exit(ERROR);
	}

	// Положить в буфер.
	printf("%sПроизводитель %d в критической зоне. Положил в буфер: %c\n", YELLOW, producerId, ALPHABET[*producer_pos]);

	buffer[(*producer_pos)++] = ALPHABET[*producer_pos];
	// BufferState();

	rv = semop(semId, ProducerEnd, 2);
	if (rv == ERROR_SEMOP)
	{
		perror("Произведитель не может изменить значение семафора.\n");
		exit(ERROR);
	}
	puts("");
}

void CreateProducer(const int producerId, const int semId, Delay *delays)
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
		// Это процесс потомок.

		// Каждый производитель производит
		// NUMBER_OF_WORKS товаров.
		for (int i = 0; i < NUMBER_OF_WORKS; i++)
			ProducerRunning(semId, producerId, delays);

		exit(OK);
	}
}