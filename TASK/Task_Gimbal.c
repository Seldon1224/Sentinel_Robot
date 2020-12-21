#include "Task_Gimbal.h"

void GET_Gimbal_Dir_xyw(void);
void Gimbal_PID_struct_init(void);
void Gimbal_PID_calculate(void);
void Gimbal_PID_rest(void);
void Gimbal_Send_Current(void);
int Angle_In_Range(float target, float min, float max);
//期望值
float set_yaw[2];
float set_pit[2];

//当前值
float cur_yaw[2];
float cur_pit[2];


uint8_t revFlag = 0;
float visionK = 0.3;
float valueStart = 0.1;
uint16_t delayTime = 100;



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
						 pid_yaw[GIMBAL_Below].pos_out, //3 ---下yaw
						 pid_pit[GIMBAL_Below].pos_out); //4 ---下pit
//	set_moto_current_all(&hcan1, 1, //0x2ff
//						 pid_yaw[GIMBAL_Above].pos_out,          //5 ---上yaw
//						 pid_pit[GIMBAL_Above].pos_out,          //6 ---上pit
//						 0,          //7
//						 0);				 //8
}

void GET_Gimbal_Dir_xyw(void)
{
	//角度解算
	cur_yaw[GIMBAL_Above] = FORMAT_Angle(moto_yaw[GIMBAL_Above].total_angle);
	cur_pit[GIMBAL_Above] = FORMAT_Angle(moto_pit[GIMBAL_Above].total_angle);
	cur_yaw[GIMBAL_Below] = FORMAT_Angle(moto_yaw[GIMBAL_Below].total_angle);
	cur_pit[GIMBAL_Below] = FORMAT_Angle(moto_pit[GIMBAL_Below].total_angle);
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
		//视觉测试（下云台）
		if (VisionRecvData.identify_target)
		{
			// i've changed here.   visionK
			
			if(Vision_If_Update()==true)
			{
				set_pit[GIMBAL_Below] = VisionRecvData.pitch_angle*visionK + cur_pit[GIMBAL_Below];
				set_yaw[GIMBAL_Below] = VisionRecvData.yaw_angle*visionK + cur_yaw[GIMBAL_Below];
				//清楚视觉识别Flag
				Clear_Vision_Get_Flag();
			}
			//设定角度限幅
			set_pit[GIMBAL_Below] = fp32_constrain(set_pit[GIMBAL_Below], -70, 40);
			set_yaw[GIMBAL_Below] = fp32_constrain(set_yaw[GIMBAL_Below], -90, 90);
		}
		else
		{
//			set_pit[GIMBAL_Below] = 0;
//			set_yaw[GIMBAL_Below] = 0;
		}

		break;
	case Disable_mode:
		set_pit[0] = 0;
		set_yaw[0] = 0;
		set_pit[1] = 0;
		set_yaw[1] = 0;
		break;
	}
	
}

void Gimbal_PID_calculate()
{
	pid_calc(&pid_yaw[0], cur_yaw[0], set_yaw[0]);
	pid_calc(&pid_pit[0], cur_pit[0], set_pit[0]);
	pid_calc(&pid_pit[1], cur_pit[1], set_pit[1]);

	//串级pid test:(
	pid_calc(&pid_yaw[1], cur_yaw[1], set_yaw[1]);								  //角度环
	pid_calc(&pid_yaw_spd, moto_yaw[GIMBAL_Below].speed_rpm, pid_yaw[1].pos_out); //速度环
}

void Gimbal_PID_struct_init(void)
{
	//上云台
	PID_struct_init(&pid_pit[GIMBAL_Above],POSITION_PID, 20000, 1000, 
					Gimbal_pitch_above_pid_KP, Gimbal_pitch_above_pid_KI, Gimbal_pitch_above_pid_KD); //pitch电机
	PID_struct_init(&pid_yaw[GIMBAL_Above],POSITION_PID, 20000, 1000, 
					Gimbal_yaw_above_pid_KP, Gimbal_yaw_above_pid_KI, Gimbal_yaw_above_pid_KD); //yaw电机
	
	//下云台
	PID_struct_init(&pid_pit[GIMBAL_Below],POSITION_PID,  5000, 1000, 
					Gimbal_pitch_below_pid_KP, Gimbal_pitch_below_pid_KI, Gimbal_pitch_below_pid_KD); //pitch电机
	PID_struct_init(&pid_yaw[GIMBAL_Below],POSITION_PID, 8000, 5000, 
					Gimbal_yaw_below_pid_KP, Gimbal_yaw_below_pid_KI, Gimbal_yaw_below_pid_KD); //yaw电机

	//内环 spd 下yaw
	PID_struct_init(&pid_yaw_spd,
					POSITION_PID, 20000, 1000, 20, 0, 0); //yaw电机
}

void Gimbal_PID_rest()
{
}

//控制视觉角度在设定的范围内
int Angle_In_Range(float target, float min, float max)
{
	if (target >= min && target <= max)
	{
		return 1;
	}
	return 0;
}
