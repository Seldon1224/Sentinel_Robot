#include "GUN_MOTOR.h"

void Gun_Motor_Init()
{
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_3);
	Gun_Motor_Stop();
}

//887-902 max 902
void Gun_Motor_SHOOT()
{
	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, 860); 
	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_4, 849); 
}
//stop 790
void Gun_Motor_Stop()
{
	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, 790);
	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_4, 790);
}

