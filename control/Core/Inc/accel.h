#ifndef __ACCEL_H
#define __ACCEL_H

#include "stdint.h"
#include "stdbool.h"
#include "i2c.h"

struct Accel {
    int x;
    int y;
    int z;
};

extern const struct Accel * accel_data;

enum accel_state {
    ACCEL_BUSY,
    ACCEL_IDLE
};

void accel_init();

int accel_read();

enum accel_state accel_get_state();

void accel_stop();

#endif