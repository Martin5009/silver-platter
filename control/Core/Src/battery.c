#include "battery.h"

uint16_t battery_read()
{
    uint16_t val;
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 1);
    val = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);

    return val;
}