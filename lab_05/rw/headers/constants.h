#ifndef CONSTANTS_H

#define CONSTANTS_H

#define ALPHABET "abcdefghijklmnopqrstuvwxyz"
#define INIT_VALUE '0'

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
#define ERROR_SHMGET -1

#define OK 0

#define TRUE 1
#define FALSE 0

#define INT_SIZE sizeof(int)

#define NUMBER_READERS 5
#define NUMBER_WRITERS 3

#define READER_SLEEP_TIME 1
#define WRITER_SLEEP_TIME 2

// semaphore:
#define R 0	 // READER Кол-во активных читателей;
#define CR 1 // CAN_WRITE Читатель может читать? - 1(нет)/0(да);
#define CW 2 // CAN_WRITE - писатель может записать? - 1(да)/0(нет);
#define WW 3 // WAIT_WRITERS - Кол-во ожидающий писателей, которые хотят записать.

#define SEM_COUNT 4

// Операции над семафорами:
#define P -1 // Пропустить;
#define V 1	 // Освободить.
#define S 0	 // sleep.

#define SEM_FLG 0

#endif