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

// #include <stdbool.h> // bool.

// Colors.
#define GREEN "\33[32m"
#define YELLOW "\33[33m"
#define BLUE "\33[34m"
#define RED "\33[31m"

// Errors
#define ERROR 1
#define ERROR_FORK -1
#define ERROR_PIPE -1
#define ERROR_SEMOP -1
#define ERROR_SEMGET -1

#define N 10

#define OK 0

#define SEM_COUNT 3

#define DELAY_TIME 3 // Для задержек.

// semaphore:
#define SF 0 // buffer full;
#define SE 1 // buffer empty;
#define SB 2 // binary.

// Операции над семафорами:
#define P -1 // Пропустить;
#define V 1	 // Освободить.

#define SEM_FLG 0 // TODO: OR (SEM_UNDO | IPC_NOWAIT) ?

struct sembuf InitValue[2] = {
	{SB, 1, SEM_FLG},  // SB изначально установлен в 1.
	{SF, N, SEM_FLG}}; // SF изначально равно N.

// Производитель.
struct sembuf ProducerBegin[2] = {
	{SE, P, SEM_FLG}, // Ожидает освобождения хотя бы одной ячейки буфера.
	{SB, P, SEM_FLG}  // Ожидает, пока другой производитель или потребитель выйдет из критической зоны.
};
struct sembuf ProducerEnd[2] = {
	{SB, V, SEM_FLG}, // Освобождает критическую зону.
	{SF, V, SEM_FLG}  // Увеличивает кол-во заполненных ячеек.
};

// Потребитель.
struct sembuf ConsumerBegin[2] = {
	{SF, P, SEM_FLG}, // Ожидает, что будет заполнена хотя бы одна ячейка буфера.
	{SB, P, SEM_FLG}  // Ожидает, пока другой производитель или потребитель выйдет из критической зоны.
};

struct sembuf ConsumerEnd[2] = {
	{SB, V, SEM_FLG}, // Освобождает критическую зону.
	{SE, V, SEM_FLG}  // Увеличивает кол-во пустых ячеек.
};

void Producer(const int semId, const int value, const int producerId)
{
	int rv; // rv = return value

	// Создаем случайные задержки.
	int delay = rand() % DELAY_TIME;
	// sleep(delay);

	// Получаем доступ к критической зоне.
	rv = semop(semId, ConsumerBegin, 2);
	if (rv == ERROR_SEMOP)
	{
		perror("Произведитель не может изменить значение семафора.\n");
		exit(ERROR);
	}

	// TODO: Положить в буфер.

	rv = semop(semId, ConsumerEnd, 2);
	if (rv == ERROR_SEMOP)
	{
		perror("Произведитель не может изменить значение семафора.\n");
		exit(ERROR);
	}
}

void CreateProducer(const int id, const int semId, int value)
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
		// TODO: Тут цикл работы производителей.
		Producer(semId, value, id);

		exit(OK);
	}
}

struct sembuf sem_arr[2] = {{0, 1, SEM_UNDO | IPC_NOWAIT}, {0, -1, SEM_UNDO | IPC_NOWAIT}};

int main(void)
{
	// Чтобы при повторном запуске новые рандомные сила были.
	srand(time(NULL));

	int semDescr;

	int perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	// IPC_PRIVATE - ключ, который показывает, что
	// Набор семафоров могут использовать только процессы,
	// Порожденные процессом, создавшим семафор.
	// Создаем новый набор 3 семафоров.
	semDescr = semget(IPC_PRIVATE, SEM_COUNT, IPC_CREAT | perms);

	if (semDescr == ERROR_SEMGET)
	{
		perror("Ошибка при создании набора семафоров.");
		return ERROR;
	}

	// Задаем начальные значения.
	semop(semDescr, InitValue, 2);

	if (semop(isem_descr, sem_arr, 2) == -1)
	{
		perror("Nas");
		return 1;
	}
	printf("Ok\n");

	return OK;
}