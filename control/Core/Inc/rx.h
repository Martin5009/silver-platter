#ifndef __RX_H
#define __RX_H

#include "stdint.h"
#include "usart.h"
#include "string.h"

#define RX_CHANNEL_NUM 14

struct Rx {
  uint16_t ch[RX_CHANNEL_NUM];
};

//Always contains valid data
extern const struct Rx * rx_data;

enum rx_state {
  RX_BUSY,
  RX_IDLE
};

void rx_init();

//Starts a read into rx_data only if RX_IDLE
//Returns 0 if start is successful
//Returns 1 if start is unsuccessful
int rx_read();

enum rx_state rx_get_state();

void rx_stop();

#endif