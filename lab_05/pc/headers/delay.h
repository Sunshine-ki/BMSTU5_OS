#ifndef DELAY_H

#define DELAY_H

typedef struct
{
	int index;
	int count;
	int *delays;
} Delay;

Delay *CreateRandomDelays(int const count, const int delay_time);

void UpdateDelays(Delay *delay, const int delay_time);

int getDelay(Delay *delay);

void DestroyDelay(Delay *delay);

#endif