#include "System.h"
#include "Robo_config.h"
#include "./remote_control/remote_control.h"
#include "./pid/pid.h"
#include "./can_receive/can_receive.h"
#include "Vision.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "GUN_MOTOR.h"
#include "param.h"

extern osThreadId_t TASK_ShootHandle;
extern osThreadId_t TASK_ChassisHandle;
extern osThreadId_t TASK_GimbalHandle;
extern void system_disable(void);
extern void system_ensable(void);

void system_disable()
{
	Gun_Motor_Stop();
	set_moto_current_all(&hcan1, 0, 0, 0, 0, 0);
	set_moto_current_all(&hcan1, 1, 0, 0, 0, 0);
	set_moto_current_all(&hcan1, 2, 0, 0, 0, 0);
	vTaskSuspend(TASK_ChassisHandle); //将任务挂起
	vTaskSuspend(TASK_GimbalHandle);
	vTaskSuspend(TASK_ShootHandle);
}
void system_enable()
{
	vTaskResume(TASK_ChassisHandle);
	vTaskResume(TASK_GimbalHandle);
	vTaskResume(TASK_ShootHandle);
}

void system_Init()
{
	dbus_uart_init(); //初始化dbus
	Gun_Motor_Init(); //摩擦轮初始化
	//can1
	my_can_filter_init_recv_all(&hcan1);
	__HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
	//pid参数初始化
	PID_Param_Init();
}

//限幅
int constrain(int amt, int low, int high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}

float constrain_float(float amt, float low, float high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}

int16_t constrain_int16_t(int16_t amt, int16_t low, int16_t high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}

int32_t constrain_int32_t(int32_t amt, int32_t low, int32_t high)
{
	if (amt < low)
		return low;
	else if (amt > high)
		return high;
	else
		return amt;
}
