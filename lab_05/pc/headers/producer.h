#ifndef PRODUCER_H

#define PRODUCER_H

#include "constants.h"

void ProducerRunning(const int semId, const int producerId, const char value);

void CreateProducer(const int producerId, const int semId, char value);

#endif