#ifndef PRODUCER_H

#define PRODUCER_H

#include "constants.h"
#include "delay.h"

void ProducerRunning(const int semId, const int producerId, Delay *delays);

void CreateProducer(const int producerId, const int semId, Delay *delays);

#endif