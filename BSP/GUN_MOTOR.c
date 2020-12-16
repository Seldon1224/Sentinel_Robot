#include "GUN_MOTOR.h"

void GUN_MOTOR_Init()
{
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
}
void GUN_MOTOR_SHOOT(uint16_t temp)
{
	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, ((temp/100.0f * 1.8f) + 0.4) / 2.38f * 1999);
	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_4, ((temp/100.0f * 1.8f) + 0.4) / 2.38f * 1999);
}

