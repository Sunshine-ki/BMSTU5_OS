// В программу с программным каналом включить собственный обработчик сигнала.
// Использовать сигнал для изменения хода выполнения программы.

// TODO: Добавляем свой обработчик сигнала.
// Меняем ход выполнения программы.
// Это копипаст f4.c (Они продолжают обмениваться через pipe,
// Но мы как-то меняем выполнение программы.)

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h> // bool.
#include <string.h>

// Colors.
#define green() printf("\33[32m")
#define yellow() printf("\33[33m")
#define blue() printf("\33[34m")

#define OK 0
#define ERROR 1
#define ERROR_FORK -1
#define ERROR_PIPE -1
#define LEN 32
#define FIRST_TEXT "Hi! I am first child!\n"
#define SECOND_TEXT "Hello! I am second child!\n"

_Bool flag = false;

void check_status(int status);

void catch_sig(int sig_numb)
{
	flag = true;
	printf("catch_sig: %d\n", sig_numb);
}

int main()
{
	signal(SIGINT, catch_sig);

	int childpid_1, childpid_2;
	int fd[2];

	if (pipe(fd) == ERROR_PIPE)
	{
		perror("Can\'t pipe.\n");
		return ERROR;
	}

	if ((childpid_1 = fork()) == ERROR_FORK)
	{
		// Если при порождении процесса произошла ошибка.
		perror("Can\'t fork.\n");
		return ERROR;
	}
	else if (!childpid_1) // Это процесс потомок.
	{
		close(fd[0]);
		write(fd[1], FIRST_TEXT, strlen(FIRST_TEXT) + 1);
		exit(OK);
	}

	// Аналогично 2 процесс.
	if ((childpid_2 = fork()) == ERROR_FORK)
	{
		perror("Can\'t fork.\n");
		return ERROR;
	}
	else if (!childpid_2) // Это процесс потомок.
	{
		close(fd[0]);
		write(fd[1], SECOND_TEXT, strlen(SECOND_TEXT) + 1);
		exit(OK);
	}

	if (childpid_1 && childpid_2)
	{
		char text[LEN], text2[LEN];
		pid_t child_pid;
		int status;

		close(fd[1]);

		read(fd[0], text, LEN);
		read(fd[0], text2, LEN);

		yellow();
		printf("Parent: нажмите \"CTRL+C\", если хотите увидеть сообщение от 2 потомка.\n\
Иначе уведите сообщение от 1 потомка.\n\n");
		sleep(2);

		if (flag)
		{
			green();
			printf("Text: %s\n", text2);
		}
		else
		{
			blue();
			printf("Text: %s\n", text);
		}
	}

	return OK;
}