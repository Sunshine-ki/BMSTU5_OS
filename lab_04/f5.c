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

#define OK 0
#define ERROR 1
#define ERROR_FORK -1
#define ERROR_PIPE -1
#define LEN 32

// TODO: Изменить ход программы.
// Глобальная переменная и тд.

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
	// 0 - выход для чтения.
	// 1 - выход для записи.

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
		write(fd[1], "First child write\n", LEN);
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
		write(fd[1], "Second child write\n", LEN);
		exit(OK);
	}

	char text[LEN], text2[LEN];
	pid_t child_pid;
	int status;

	printf("Parent: press CTRL+C (within 3 seconds)\n");
	sleep(3);

	close(fd[1]);

	read(fd[0], text, LEN);
	read(fd[0], text2, LEN);

	printf("Text: %s\n", text);
	printf("Text: %s\n", text2);

	child_pid = wait(&status);
	check_status(status);

	child_pid = wait(&status);
	check_status(status);

	if (flag)
		printf("Вы хотели завершить программу...\n");
	else
		printf("Завершение программы.\n");

	return OK;
}

void check_status(int status)
{
	if (WIFEXITED(status))
	{
		printf("Дочерний процесс завершен нормально.\n\n");
		return;
	}

	if (WEXITSTATUS(status))
	{
		printf("Код завершения дочернего процесса %d.\n", WIFEXITED(status));
		return;
	}

	if (WIFSIGNALED(status))
	{
		printf("Дочерний процесс завершается неперехватываемым сигналом.\n");
		printf("Номер сигнала %d.\n", WTERMSIG(status));
		return;
	}

	if (WIFSTOPPED(status))
	{
		printf("Дочерний процесс остановился.\n");
		printf("Номер сигнала %d.", WSTOPSIG(status));
	}
}