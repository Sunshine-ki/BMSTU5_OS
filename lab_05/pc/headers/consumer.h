#ifndef CONSUMER_H

#define CONSUMER_H

#include "constants.h"
#include "delay.h"

void ConsumerRunning(const int semId, const int consumerId, Delay *delays);

void CreateConsumer(const int consumerId, const int semId, Delay *delays);

#endif