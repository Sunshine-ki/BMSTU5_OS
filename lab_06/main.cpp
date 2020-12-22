// Надо использовать неделимые операции:
// InterLockedIncrement, InterLockedDecrement.
// В программе должно быть 3 счетчика:
// ждущих писателей, ждущих читателей и активных читателей.
// Активный писатель м.б. только один и это логический тип.

#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define OK 0

#define READERS_NUMBER 3
#define WRITERS_NUMBER 3

#define ITERATIONS_NUMBER 10

HANDLE canRead; // event.

HANDLE reader_Threads[READERS_NUMBER];
HANDLE writer_Threads[WRITERS_NUMBER];

HANDLE hMutex;

int readersID[READERS_NUMBER];
int writersID[WRITERS_NUMBER];

// int readersRand[READERS_NUMBER * ITERATIONS_NUMBER];

// volatile LONG active_readers = 0, waiting_writers = 0;

int value = 0;

// void StartRead()
// {
// }

DWORD WINAPI Writer(CONST LPVOID param)
{
}

DWORD WINAPI Reader(CONST LPVOID param)
{
	int id = *(int *)param;
	for (int i = 0; i < ITERATIONS_NUMBER; i++)
	{
		WaitForSingleObject(canRead, INFINITE);

		printf("Thread with id = %d, i = %d value = %d\n", id, i, value);
		Sleep(20);
	}
}

void CreateThreads()
{
	DWORD id = 0;
	for (int i = 0; i < READERS_NUMBER; i++)
	{
		readersID[i] = i;
		// Параметры слева направо:
		// NULL - Атрибуты защиты определены по умолчанию;
		// 0 - размер стека устанавливается по умолчанию;
		// Reader - определяет адрес функции потока, с которой следует начать выполнение потока;
		// readersID + i - указатель на переменную, которая передается в поток;
		//  0 - исполнение потока начинается немедленно;
		// Последний - адрес переменной типа DWORD, в которую функция возвращает идентификатор потока.
		reader_Threads[i] = CreateThread(NULL, 0, &Reader, readersID + i, 0, &id);
		printf("Created reader with thread id = %d\n", id);
	}

	for (int i = 0; i < WRITERS_NUMBER; i++)
	{
		writersID[i] = i;
		writer_Threads[i] = CreateThread(NULL, 0, &Writer, writersID + i, 0, &id);
		printf("Created writer with thread id = %d\n", id);
	}
}

void Close()
{
	// Закрываем дескрипторы mutex, event и всех созданных потоков.
	for (int i = 0; i < READERS_NUMBER; i++)
		CloseHandle(reader_Threads[i]);

	for (int i = 0; i < WRITERS_NUMBER; i++)
		CloseHandle(writer_Threads[i]);

	CloseHandle(canRead);
	CloseHandle(hMutex);
}

// void CreateRand()
// {
// 	for (int i = 0; i < READERS_NUMBER * ITERATIONS_NUMBER; i++)
// 		readersRand[i] = rand() % 100 + 50;
// }

int main(void)
{
	setbuf(stdout, NULL);
	srand(time(NULL));

	printf("\n\n\n\n\n\n\n\n");
	// 2ой аргумент == false значит мьютекс свободный.
	hMutex = CreateMutex(NULL, FALSE, NULL); // CONST

	// 2ой аргумент == true значит сброс вручную
	// 3ий аргумент == true значит объект в сигнальном состоянии.
	canRead = CreateEvent(NULL, TRUE, TRUE, NULL);

	// CreateRand();

	CreateThreads();

	// READERS_NUMBER - кол-во инетерсующих нас объектов ядра.
	// reader_Threads - указатель на массив описателей объектов ядра.
	// TRUE - функция не даст потоку возобновить свою работу, пока не освободятся все объекты.
	// INFINITE - указывает, сколько времени поток готов ждать освобождения объекта.
	WaitForMultipleObjects(READERS_NUMBER, reader_Threads, TRUE, INFINITE);
	WaitForMultipleObjects(WRITERS_NUMBER, writer_Threads, TRUE, INFINITE);

	printf("\n\n\n\n\n\n\n\n");

	Close();

	printf("Ok!\n");
	return OK;
}