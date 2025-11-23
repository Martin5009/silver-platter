#ifndef __BATTERY_H
#define __BATTERY_H

#include "stdint.h"
#include "adc.h"

#define BATTERY_EMPTY 2857
#define BATTERY_LOW 3334
#define BATTERY_FULL 3967

uint16_t battery_read();

#endif