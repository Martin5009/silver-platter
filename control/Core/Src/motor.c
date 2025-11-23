#include "motor.h"

void motor_init()
{
    dshot_init();
}

int motor_set_throttle(enum motor_type motor, uint16_t throttle)
{
    return dshot_send(motor, throttle + 48);
}

void motor_stop()
{
    dshot_stop();
}