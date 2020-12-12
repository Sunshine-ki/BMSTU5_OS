#ifndef CONSTANTS_H

#define CONSTANTS_H

#include <unistd.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/stat.h>

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

#define N 32

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

#endif