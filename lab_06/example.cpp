// Пример из методы.
// Но на самом деле он вот от сюда:
// https://eax.me/winapi-threads/

// Также полезно:
// https://www.firststeps.ru/mfc/winapi/r.php?119

#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define THREADS_NUMBER 3
#define ITERATIONS_NUMBER 10
#define PAUSE 10 /* ms */

TCHAR szMessage[256];

HANDLE hStdOut;

DWORD dwCounter = 0;

HANDLE hMutex;

int readersRand[THREADS_NUMBER * ITERATIONS_NUMBER];

int readersID[THREADS_NUMBER];

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
  DWORD dwTemp;
  int index = *(int *)lpParam;
  DWORD i;
  for (i = 0; i < ITERATIONS_NUMBER; i++)
  {
    WaitForSingleObject(hMutex, INFINITE);
    printf("index = %d Counter = %d Rand = %d\r\n", index, dwCounter, readersRand[i * index]);

    WriteConsole(hStdOut, szMessage, lstrlen(szMessage), &dwTemp, NULL);

    dwCounter++;
    ReleaseMutex(hMutex);
    // Sleep(readersRand[i * index]);
  }
  ExitThread(0);
}

VOID Error(CONST HANDLE hStdOut, CONST LPCWSTR szMessage)
{
  DWORD dwTemp;
  TCHAR szError[256];
  //		 WriteConsole(hStdOut, szMessage, lstrlen(szMessage), &dwTemp, NULL);
  wsprintf(szError, TEXT("LastError = %d\r\n"), GetLastError());
  WriteConsole(hStdOut, szError, lstrlen(szError), &dwTemp, NULL);
  ExitProcess(0);
}

INT main()
{
  srand(time(NULL));

  DWORD dwTemp, i;
  HANDLE hThreads[THREADS_NUMBER];
  hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  hMutex = CreateMutex(NULL, FALSE, NULL);
  //		 if(NULL == hMutex) {
  //				 Error(hStdOut, TEXT("Failed to create mutex.\r\n"));
  //		 }

  for (int i = 0; i < THREADS_NUMBER * ITERATIONS_NUMBER; i++)
    readersRand[i] = rand() % 100 + 50;

  for (i = 0; i < THREADS_NUMBER; i++)
  {
    readersID[i] = i;
    hThreads[i] = CreateThread(NULL, 0, &ThreadProc, readersID + i, 0, NULL);
    // if(NULL == hThreads[i]) {
    //		 Error(hStdOut, TEXT("Failed to create thread.\r\n"));
    // }
  }

  WaitForMultipleObjects(THREADS_NUMBER, hThreads, TRUE, INFINITE);
  wsprintf(szMessage, TEXT("Counter = %d\r\n"), dwCounter);
  WriteConsole(hStdOut, szMessage, lstrlen(szMessage), &dwTemp, NULL);

  for (i = 0; i < THREADS_NUMBER; i++)
  {
    CloseHandle(hThreads[i]);
  }
  CloseHandle(hMutex);
  ExitProcess(0);
}