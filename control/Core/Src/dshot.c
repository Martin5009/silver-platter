#include "dshot.h"

#define ARM_TIME 500
#define DSHOT_FREQ 600000
#define APB2_TIM_CLK 120000000
#define DSHOT_1_DUTY 75
#define DSHOT_0_DUTY 37

int tim1_arr;
int dshot_0;
int dshot_1;
volatile bool dma_ready_flag;

uint16_t dshot_buff_1[16];
uint16_t dshot_buff_2[16];

//Helper Functions

void dshot_create_frame(int throttle, uint16_t buff[16])
{
    int frame;

    //throttle bits
    frame = throttle << 1;

    //crc bits
    uint8_t crc = (frame ^ (frame >> 4) ^ (frame >> 8)) & 0x0f;
    frame = frame << 4 | crc;

    //fill duty cycle buffer
    int bit;
    for (int i = 0; i < 16; i++)
    {
        bit = (frame >> (15 - i)) & 1;

        if (bit) buff[i] = dshot_1;
        else buff[i] = dshot_0;
    }
}


//Library Functions

void dshot_init()
{
    tim1_arr = (APB2_TIM_CLK/DSHOT_FREQ) - 1;
    dshot_0 = (DSHOT_0_DUTY * (tim1_arr - 1)) / 100;
    dshot_1 = (DSHOT_1_DUTY * (tim1_arr - 1)) / 100;

    //start disarmed
    dshot_create_frame(0, dshot_buff_1);
    dshot_create_frame(0, dshot_buff_2);

    //start dshot signal generation
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)dshot_buff_1, 16);
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_4, (uint32_t*)dshot_buff_2, 16);

    //wait until disarmed message sent
    HAL_Delay(ARM_TIME);
}

int dshot_send(int ch, int throttle)
{
    //error handling
    if (!dma_ready_flag)
    {
        return 1;
    }
    else if (throttle > 2047 || throttle < 48)
    {
        return 2;
    }
    else if (ch < 0 || ch > 1)
    {
        return 3;
    }

    //channel select
    switch (ch)
    {
        case 0:
            dma_ready_flag = false;
            dshot_create_frame(throttle, dshot_buff_1);
            HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)dshot_buff_1, 16);
            break;
        case 1:
            dma_ready_flag = false;
            dshot_create_frame(throttle, dshot_buff_2);
            HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_4, (uint32_t*)dshot_buff_2, 16);
            break;
        default:
            break;
    }

    return 0;
}

void dshot_stop()
{
    //allow existing dma transfer to complete
    while(!dma_ready_flag);

    dshot_create_frame(0, dshot_buff_1);
    dshot_create_frame(0, dshot_buff_2);

    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t*)dshot_buff_1, 16);
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_4, (uint32_t*)dshot_buff_2, 16);

    HAL_Delay(ARM_TIME);
}

void HAL_TIM_PWM_PulseFinishedCallback (TIM_HandleTypeDef * htim)
{
    dma_ready_flag = true;
}