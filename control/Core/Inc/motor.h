#ifndef __MOTOR_H
#define __MOTOR_H

#include "stdint.h"
#include "dshot.h"

enum motor_type {
  MOTOR_LEFT, MOTOR_RIGHT
};

void motor_init();

// Throttle can be 0-1999
// Returns 1 when throttle is out of range, 0 otherwise
int motor_set_throttle(enum motor_type motor, uint16_t throttle);

void motor_stop();

#endif