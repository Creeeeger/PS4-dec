#pragma once

#ifndef BARRIER_H
#define BARRIER_H

#include "types.h"

int mutexInit(const char *name, unsigned int attributes);
int mutexDestroy(int mutex);

#endif
