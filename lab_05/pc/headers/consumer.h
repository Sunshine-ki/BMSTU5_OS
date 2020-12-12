#ifndef CONSUMER_H

#define CONSUMER_H

#include "constants.h"

void ConsumerRunning(const int semId, const int consumerId);

void CreateConsumer(const int consumerId, const int semId);

#endif