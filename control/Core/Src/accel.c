#include "accel.h"

#define TIMEOUT 1000

#define R_DEV_ADDR 0x3B00
#define W_DEV_ADDR 0x3A00

#define OFSX 0x1E00
#define OFSY 0x1F00
#define OFSZ 0x2000
#define BW_RATE 0x2C00
#define POWER_CTL 0x2D00
#define DATA_FORMAT 0x3100
#define DATAX0 0x3200
#define DATAX1 0x3300
#define DATAY0 0x3400
#define DATAY1 0x3500
#define DATAZ0 0x3600
#define DATAZ1 0x3700

#define RATE_3200HZ 0b00001111
#define RATE_1600HZ 0b00001110
#define RATE_800HZ 0b00001101
#define RATE_400HZ 0b00001100
#define RATE_200HZ 0b00001011
#define RATE_100HZ 0b00001010

static enum accel_state state;
static uint8_t tx_buff;
static uint8_t rx_buff[6];
static struct Accel accel_buff[2];
static int write_index;

const struct Accel * accel_data;

void accel_init()
{
  state = ACCEL_IDLE;
  write_index = 1;
  accel_data = &accel_buff[0];

  tx_buff = RATE_400HZ;
  HAL_I2C_Mem_Write(&hi2c2, W_DEV_ADDR, BW_RATE, 1, &tx_buff, 1, TIMEOUT);

  tx_buff = 0b00001011;
  HAL_I2C_Mem_Write(&hi2c2, W_DEV_ADDR, DATA_FORMAT, 1, &tx_buff, 1, TIMEOUT);

  tx_buff = 0b00001000;
  HAL_I2C_Mem_Write(&hi2c2, W_DEV_ADDR, POWER_CTL, 1, &tx_buff, 1, TIMEOUT);
}

int accel_read()
{
  if (state == ACCEL_BUSY)
  {
    return 1;
  }
  
  state = ACCEL_BUSY;
  HAL_I2C_Mem_Read_IT(&hi2c2, R_DEV_ADDR, DATAX0, 1, rx_buff, 6);
  return 0;
}

enum accel_state accel_get_state()
{
  return state;
}

void accel_stop()
{
  tx_buff = 0b00000000;
  HAL_I2C_Mem_Write(&hi2c2, W_DEV_ADDR, POWER_CTL, 1, &tx_buff, 1, TIMEOUT);
  HAL_I2C_DeInit(&hi2c2);
}

void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef* hi2c)
{
  struct Accel * write_buff = &accel_buff[write_index];

  write_buff -> x = (rx_buff[1] << 8) | rx_buff[0];
  write_buff -> y = (rx_buff[3] << 8) | rx_buff[2];
  write_buff -> z = (rx_buff[5] << 8) | rx_buff[4];

  //write buffer becomes read buffer
  accel_data = write_buff;

  //read buffer becomes write buffer
  write_index = !write_index;

  state = ACCEL_IDLE;
}