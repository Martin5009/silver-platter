#ifndef __DSHOT_H
#define __DSHOT_H

#include "stdint.h"
#include "tim.h"
#include "stdbool.h"

void dshot_init();

//Returns 1 if throttle is not within 48-2047, and 0 otherwise
int dshot_send(int ch, int throttle);

void dshot_stop();

#endif