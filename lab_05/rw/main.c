// Писателей 3 (Должно быть не меньше 3х)
// Читателей 5 (Должно быть не меньше 5ти)

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h>

#include "constants.h"
#include "writer.h"
#include "reader.h"

int *counter = NULL;

// struct sembuf InitValue[1] = {
// 	{CW, 0, SEM_FLG} // Изначально CW равно 1.
// };

int main(void)
{
	int semDescr;
	int status;
	int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

	// shmget - создает новый разделяемый сегмент.
	int shmid = shmget(IPC_PRIVATE, INT_SIZE, perms);
	if (shmid == ERROR_SHMGET)
	{
		perror("Не удалось создать разделяемый сегмент.\n");
		return ERROR;
	}

	// Функция shmat() возвращает указатель на сегмент
	counter = shmat(shmid, NULL, 0);
	if (*(char *)counter == -1)
	{
		perror("Не удалось получить указатель на сегмент.");
		return ERROR;
	}

	*(counter) = 0;

	// Создаем новый набор, состоящий из SEM_COUNT семафоров.
	semDescr = semget(IPC_PRIVATE, SEM_COUNT, IPC_CREAT | perms);

	if (semDescr == ERROR_SEMGET)
	{
		perror("Ошибка при создании набора семафоров.");
		return ERROR;
	}

	// // Задаем начальные значения семафоров.
	// if (semop(semDescr, InitValue, 1))
	// {
	// 	perror("Ошибка при попытке изменить семафор.");
	// 	return ERROR;
	// }

	for (int i = 0; i < NUMBER_READERS; i++)
		CreateReader(semDescr, i + 1);

	for (int i = 0; i < NUMBER_WRITERS; i++)
		CreateWriter(semDescr, i + 1);

	for (int i = 0; i < NUMBER_READERS + NUMBER_WRITERS; i++)
		wait(&status);

	if (shmdt(counter) == -1)
		perror("Ошибка при попытке отключить разделяемый сегмент от адресного пространства процесса.");

	return OK;
}
