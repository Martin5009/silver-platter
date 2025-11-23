#ifndef __RGB_H
#define __RGB_H

#include "stdint.h"
#include "tim.h"

void rgb_init();

void rgb_set_color(uint16_t r, uint16_t g, uint16_t b);

void rgb_stop();

#endif