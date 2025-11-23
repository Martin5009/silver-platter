#include "rgb.h"

void rgb_init()
{
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

void rgb_set_color(uint16_t r, uint16_t g, uint16_t b)
{
	TIM_OC_InitTypeDef config;

	config.OCMode = TIM_OCMODE_PWM1;
	config.OCPolarity = TIM_OCPOLARITY_HIGH;
	config.OCFastMode = TIM_OCFAST_DISABLE;

	config.Pulse = r;
	HAL_TIM_PWM_ConfigChannel(&htim4, &config, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);

	config.Pulse = g;
	HAL_TIM_PWM_ConfigChannel(&htim4, &config, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);

	config.Pulse = b;
	HAL_TIM_PWM_ConfigChannel(&htim4, &config, TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
}

void rgb_stop()
{
	HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_4);
}