#include "delay.h"

#include <stdlib.h> // rand()

#include "constants.h"

Delay *CreateRandomDelays(int const count)
{
	Delay *delay = malloc(sizeof(Delay));

	delay->delays = malloc(sizeof(int) * count);
	delay->count = count - 1;

	for (int i = 0; i < delay->count; i++)
		delay->delays[i] = rand() % DELAY_TIME;

	return delay;
}

int getDelay(Delay *delay)
{
	if (delay->index > delay->count)
		delay->index = 0;

	return delay->delays[delay->index++];
}

void DestroyDelay(Delay *delay)
{
	if (delay->delays)
		free(delay->delays);
	if (delay)
		free(delay);
}