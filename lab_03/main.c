#include <stdio.h>
#include <unistd.h>

int main()
{
	int childpid;

	// Создается дочерний процесс
	// Тут содается 2 идентичных процесса.
	if ((childpid = fork()) == -1)
	{
		// Если при порождении процесса произошла ошибка.
		perror("Can\'t fork.\n");
		return 1;
	}
	else if (childpid == 0)
	{
		// Это процесс потом (ребенок).
		while (1)
			printf("Baby: %d\n", getpid());
		return 0;
	}
	else
	{
		// Это процесс предок.
		while (1)
			printf("Daddy: %d\n", getpid());
		return 0;
	}
}