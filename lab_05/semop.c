#include <stdio.h>
#include <stdlib.h> // rand()
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>

struct sembuf sem_arr[1] = {{0, -1, 1}};
struct sembuf sem_arr2[1] = {{0, 1, 1}};

int main(void)
{
	int isemDescr;

	int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	// IPC_PRIVATE - ключ, который показывает, что
	// Набор семафоров могут использовать только процессы,
	// Порожденные процессом, создавшим семафор.
	// Создаем новый набор 3 семафоров.
	isemDescr = semget(IPC_PRIVATE, 2, IPC_CREAT | perms);

	if (isemDescr == -1)
	{
		perror("Ошибка при создании набора семафоров.");
		return -1;
	}

	pid_t childpid;
	if ((childpid = fork()) == -1)
	{
		// Если при порождении процесса произошла ошибка.
		perror("Ошибка при порождении процесса производителя.");
		exit(-1);
	}
	else if (!childpid) // childpid == 0
	{
		printf("Child begin\n");

		// Это процесс потомок.
		int rv = semop(isemDescr, sem_arr, 1);

		printf("Return value = %d\n", rv);

		if (rv == -1)
		{
			printf("Child in semop\n");

			perror("Error semop child");
			return 1;
		}
		printf("Child Ok\n");

		exit(0);
	}

	sleep(1);

	if (semop(isemDescr, sem_arr2, 1) == -1)
	{
		perror("Error semop");
		return 1;
	}

	printf("Ok\n");

	return 0;
}