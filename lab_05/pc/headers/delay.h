#ifndef DELAY_H

#define DELAY_H

typedef struct
{
	int index;
	int count;
	int *delays;
} Delay;

Delay *CreateRandomDelays(int const count);

void UpdateDelays(Delay *delay);

int getDelay(Delay *delay);

void DestroyDelay(Delay *delay);

#endif