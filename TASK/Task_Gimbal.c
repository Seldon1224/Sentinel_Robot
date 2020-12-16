#include "Task_Gimbal.h"
void GET_Gimbal_Dir_xyw(void);
void Gimbal_PID_struct_init(void);
void Gimbal_PID_calculate(void);
void Gimbal_PID_rest(void);
void Gimbal_Send_Current(void);
//期望值
float set_yaw[2];
float set_pit[2];

//当前值
float cur_yaw[2];
float cur_pit[2];
//偏差值
int32_t offset_lock_yaw_angle;
int32_t offset_lock_pit_angle;
int32_t offset_auto_aim_yaw_angle;
int32_t offset_auto_aim_pit_angle;

IMU_UART mpu_data;
bool lock_flag;
bool auto_aim_flag;

void Task_Gimbal(void *argument)
{
	Gimbal_PID_struct_init();
	for (;;)
	{
		GET_Gimbal_Dir_xyw(); //获取云台电机期望角度
		Gimbal_PID_calculate();
		Gimbal_Send_Current(); //向云台发送电流值
		osDelay(3);
	}
}

void Gimbal_Send_Current()
{
	set_moto_current_all(&hcan1, 0,						 //0x1ff
						 0,								 //1
						 0,								 //2
						 pid_yaw[GIMBAL_Below].pos_out,	 //3 ---下yaw
						 pid_pit[GIMBAL_Below].pos_out); //4 ---下pit
														 //		set_moto_current_all(&hcan1, 1, //0x2ff
														 //										 pid_yaw[GIMBAL_Above].pos_out,          //5 ---上yaw
														 //										 pid_pit[GIMBAL_Above].pos_out,          //6 ---上pit
														 //										 0,          //7
														 //										 0);				 //8
}

void GET_Gimbal_Dir_xyw(void)
{
	switch (roboStatus.control_mode)
	{
	case Remote_mode:
		if (rc.CONTROLLER.sw2 == RC_SW_UP)
		{
			set_pit[GIMBAL_Above] = -rc.CONTROLLER.ch2 * 30.0f / 660; //PITCH轴为绝对位置
			set_yaw[GIMBAL_Above] = rc.CONTROLLER.ch1 * 60.0f / 660;
		}
		else if (rc.CONTROLLER.sw2 == RC_SW_DOWN)
		{
			set_pit[GIMBAL_Below] = rc.CONTROLLER.ch2 * 30.0f / 660; //PITCH轴为绝对位置
			set_yaw[GIMBAL_Below] = rc.CONTROLLER.ch1 * 30.0f / 660;
		}
		break;
	case AutoAim_mode:
		//自瞄
		set_pit[0] = 0;
		set_yaw[0] = 0;
		set_pit[1] = 0;
		set_yaw[1] = 0;
		break;
	case Disable_mode:
		set_pit[0] = 0;
		set_yaw[0] = 0;
		set_pit[1] = 0;
		set_yaw[1] = 0;
		break;
	}
	//角度解算
	cur_yaw[GIMBAL_Above] = FORMAT_Angle(moto_yaw[GIMBAL_Above].total_angle);
	cur_pit[GIMBAL_Above] = FORMAT_Angle(moto_pit[GIMBAL_Above].total_angle);
	cur_yaw[GIMBAL_Below] = FORMAT_Angle(moto_yaw[GIMBAL_Below].total_angle);
	cur_pit[GIMBAL_Below] = FORMAT_Angle(moto_pit[GIMBAL_Below].total_angle);
}

void Gimbal_PID_calculate()
{
	pid_calc(&pid_yaw[0], cur_yaw[0], set_yaw[0]);
	pid_calc(&pid_pit[0], cur_pit[0], set_pit[0]);
	pid_calc(&pid_yaw[1], cur_yaw[1], set_yaw[1]);
	pid_calc(&pid_pit[1], cur_pit[1], set_pit[1]);
}

void Gimbal_PID_struct_init(void)
{
	//上云台
	PID_struct_init(&pid_pit[GIMBAL_Above],
					POSITION_PID, 20000, 1000, PID[2].Kp, PID[2].Ki, PID[2].Kd); //pitch电机
	PID_struct_init(&pid_yaw[GIMBAL_Above],
					POSITION_PID, 20000, 1000, PID[3].Kp, PID[3].Ki, PID[3].Kd); //yaw电机
	//下云台
	PID_struct_init(&pid_pit[GIMBAL_Below],
					POSITION_PID, 20000, 1000, PID[2].Kp, PID[2].Ki, PID[2].Kd); //pitch电机
	PID_struct_init(&pid_yaw[GIMBAL_Below],
					POSITION_PID, 20000, 1000, PID[3].Kp, PID[3].Ki, PID[3].Kd); //yaw电机
}

void Gimbal_PID_rest()
{
}
