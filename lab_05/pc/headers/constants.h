#ifndef CONSTANTS_H

#define CONSTANTS_H

#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/stat.h>

#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define INIT_VALUE '0'

// Colors.
#define GREEN "\33[32m"
#define YELLOW "\33[32m"
#define BLUE "\33[34m"
#define RED "\33[31m"

// Errors
#define ERROR 1
#define ERROR_FORK -1
#define ERROR_PIPE -1
#define ERROR_SEMOP -1
#define ERROR_SEMGET -1
#define ERROR_SHMGET -1

// Каждый производитель производит 5 товаров.
// Каждый потребитель потребляет 5 товаров
#define NUMBER_OF_WORKS 5
// В программе создается 3 производителя =>
// 3 * 5 = 15 ячеек памяти потребуется.
#define N 15

#define OK 0

#define SEM_COUNT 3

// Для задержек.
#define CONSUMER_DELAY_TIME 2 // Потребитель.
#define PRODUCER_DELAY_TIME 2 // Производитель.

#define COUNT 3
#define COUNT_PRODUCER 3
#define COUNT_CONSUMER 3

// semaphore:
#define SF 0 // buffer full;
#define SE 1 // buffer empty;
#define SB 2 // binary.

// Операции над семафорами:
#define P -1 // Пропустить;
#define V 1	 // Освободить.

#define SEM_FLG 0

#endif