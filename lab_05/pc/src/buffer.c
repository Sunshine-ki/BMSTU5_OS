#include <stdio.h>

#include "constants.h"

extern char *buffer;

void BufferState()
{
	printf("Состояние буфера: %s", GREEN);
	for (int i = 0; i < N; i++)
		printf("%c ", buffer[i]);
	puts("");
}

void InitBuffer()
{
	for (int i = 0; i < N; i++)
		buffer[i] = INIT_VALUE;
}