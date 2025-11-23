#include "rx.h"

static volatile enum rx_state state;
static struct Rx rx_buff[2];
static uint8_t msg_buff[32];
static uint8_t uart_buff;
static int msg_index;
static int write_index;

const struct Rx * rx_data;

//Helper functions

//Returns 1 if CRC fails, 0 otherwise
int rx_ibus_checksum(uint8_t msg[32])
{
  uint16_t sum;
  for (int i=0; i<30; i++)
  {
    sum += msg[i];
  }

  return sum != msg[30] + msg[31];
}



//Library functions

void rx_init()
{
  state = RX_IDLE;
  msg_index = 0;
  write_index = 1;
  rx_data = &rx_buff[0];

  MX_USART2_UART_Init();
}

int rx_read()
{
  if (state == RX_BUSY)
  {
    return 1;
  }
  
  state = RX_BUSY;
  HAL_UART_Receive_IT(&huart2, &uart_buff, 1);
  return 0;
}

enum rx_state rx_get_state()
{
  return state;
}

void rx_stop()
{
  HAL_UART_DeInit(&huart2);
}



//UART Interrupt RX Callback

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (uart_buff == 0x20 && msg_index == 0)
  {
      msg_buff[msg_index] = uart_buff;
      msg_index++;
  }
  else if (uart_buff == 0x40 && msg_index == 1)
  {
      msg_buff[msg_index] = uart_buff;
      msg_index++;
  }
  else if (msg_index >= 2 && msg_index <= 31)
  {
      msg_buff[msg_index] = uart_buff;
      msg_index++;
  }
  else if (msg_index == 32)
  {
      msg_index = 0;

      //update rx_data and stop receiving if CRC succeeds
      if (rx_ibus_checksum(msg_buff)) 
      {
        struct Rx * write_buff = &rx_buff[write_index];

        for (int i=0; i<RX_CHANNEL_NUM; i++)
        {
          write_buff -> ch[i] = ((msg_buff[2*i+3] << 8) | msg_buff[2*i+2]) - 1000;
        }

        //write buffer becomes read buffer
        rx_data = write_buff;

        //read buffer becomes write buffer
        write_index = !write_index;

        state = RX_IDLE;

        return;
      }
  }
  else
  {
      msg_index = 0;
  }

  //continue receiving if no valid message
  HAL_UART_Receive_IT(&huart2, &uart_buff, 1);
}