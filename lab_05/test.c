#include <stdio.h>
// #include <pthread.h>
// #include <semaphore.h>
// #include <unistd.h>

// #define OK 0

// int main(void)
// {
// 	int N = 24; /*размер буфера*/
// 	sem_t buffer_full, buffer_empty, bin_sem;

// 	sem_init(&buffer_empty, 1, N);

// 	return OK;
// }

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>

/* системный вызов semop требует массив sembuf структур */
// struct sembuf
// {
// 	ushort sem_num; /* индекс семафора в массиве */
// 	short sem_op;	/* операция над семафором */
// 	short sem_flg;	/* флаги */
// };

struct sembuf sem_arr[2] = {{0, 1, SEM_UNDO | IPC_NOWAIT}, {0, -1, SEM_UNDO | IPC_NOWAIT}};

int main(void)
{
	int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	printf("aaa");
	int isem_descr = semget(100, 2, IPC_CREAT | perms);

	if (isem_descr == -1)
	{
		perror("semget");
		return 1;
	}
	if (semop(isem_descr, sem_arr, 2) == -1)
	{
		perror("Nas");
		return 1;
	}
	printf("Ok\n");
	return 0;
}