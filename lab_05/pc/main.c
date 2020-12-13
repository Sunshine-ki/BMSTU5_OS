#include <stdio.h>
#include <stdlib.h>	 // rand()
#include <pthread.h> // time()
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/stat.h>

#include "constants.h"

#include "delay.h"
#include "consumer.h"
#include "producer.h"

struct sembuf InitValue[2] = {
	{SB, 1, SEM_FLG}, // SB изначально установлен в 1.
	{SE, N, SEM_FLG}  // SE изначально равно N.
};

int *memory_addr;
int *consumer_pos;
int *producer_pos;
char *buffer;

int main(void)
{
	// Чтобы при повторном запуске новые рандомные сила были.
	srand(time(NULL));

	int semDescr;

	int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	// IPC_PRIVATE - ключ, который показывает, что
	// Набор семафоров могут использовать только процессы,
	// Порожденные процессом, создавшим семафор.
	// Создаем новый набор, состоящий из 3 семафоров.
	semDescr = semget(IPC_PRIVATE, SEM_COUNT, IPC_CREAT | perms);

	if (semDescr == ERROR_SEMGET)
	{
		perror("Ошибка при создании набора семафоров.");
		return ERROR;
	}

	// Задаем начальные значения семафоров.
	if (semop(semDescr, InitValue, 2))
	{
		perror("Ошибка при попытке изменить семафор.");
		return ERROR;
	}

	char c = 'G';
	CreateProducer(0, semDescr, c);
	CreateConsumer(0, semDescr);

	CreateProducer(0, semDescr, c);
	CreateConsumer(0, semDescr);

	CreateProducer(0, semDescr, c);
	CreateConsumer(0, semDescr);

	int status;
	wait(&status);

	printf("Ok\n");

	return OK;
}

// Delay *delayProducer = CreateRandomDelays(10);
// for (int i = 0; i < 10; i++)
// 	printf("%d ", getDelay(delayProducer));

// printf("\n");

// for (int i = 0; i < 15; i++)
// 	printf("%d ", getDelay(delayProducer));

// DestroyDelay(delayProducer);