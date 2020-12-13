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
#include <sys/shm.h>

struct sembuf sem_arr[1] = {{0, -1, 1}};
struct sembuf sem_arr2[1] = {{0, 1, 1}};

int main(void)
{
	int semDescr;

	// int b = 5;
	// int *a = &b;

	// Таким образом код процесса - предка не копируется полностью,
	// а создаются только копии страниц, которые редактируются.

	int shmid;
	if ((shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | S_IRWXU | S_IRWXG | S_IRWXO)) == -1)
	{
		perror("Unable to create shared area");
		exit(1);
	}

	// Функция shmat() возвращает указатель на сегмент
	// shmaddr (второй аргумент) равно NULL,
	// то система выбирает подходящий (неиспользуемый)
	// адрес для подключения сегмента.
	int *a = shmat(shmid, NULL, 0);
	*a = 5;

	printf("Отец начало: %d адрес: %p \n", *a, a);

	pid_t childpid;
	if ((childpid = fork()) == -1)
	{
		// Если при порождении процесса произошла ошибка.
		perror("Ошибка при порождении процесса производителя.\n");
		exit(-1);
	}
	else if (!childpid) // childpid == 0
	{
		printf("Child begin\n");

		*a = 33;

		printf("Ребенок конец: %d адрес: %p \n", *a, a);

		printf("Child end\n");

		exit(0);
	}

	sleep(2);

	printf("Отец конец: %d адрес: %p \n", *a, a);

	printf("Ok\n");

	return 0;
}