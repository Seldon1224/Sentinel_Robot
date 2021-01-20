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
float set_revolve_spd[2];

//当前值
float cur_yaw[2];
float cur_pit[2];

//视觉角度系数
float visionK = 0.3;

//yaw轴电机巡逻系数
float auto_rate_yaw = AUTO_RATE_VALUE;
float auto_rate_pit = AUTO_RATE_VALUE;

//自瞄角度调整
float adjust_angle_pit = 0;
float adjust_angle_yaw = 0;

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
						 pid_pit[GIMBAL_Above].pos_out,	 //2
						 pid_yaw_spd[GIMBAL_Below].delta_out,	 //3 ---下yaw
//						 pid_yaw[GIMBAL_Below].pos_out,
//						 pid_pit[GIMBAL_Below].pos_out); //4 ---下pit
						 pid_pit_spd[GIMBAL_Below].delta_out); //4 ---下pit

	set_moto_current_all(&hcan1, 1,						 //0x2ff
						 0,								 //5 ---上yaw
						 0,								 //6 ---上pit
						 0,								 //7
						 0);							 //8
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
		set_revolve_spd[GIMBAL_Below] = 500;
		if (rc.CONTROLLER.sw2 == RC_SW_UP)
		{
//			set_pit[GIMBAL_Above] = -rc.CONTROLLER.ch2 * 30.0f / 660; //PITCH轴为绝对位置
//			set_yaw[GIMBAL_Above] = rc.CONTROLLER.ch1 * 60.0f / 660;
//			Gun_Motor_SHOOT();
		}
		else if (rc.CONTROLLER.sw2 == RC_SW_MID)
		{
			Gun_Motor_Stop();
		}
		else if (rc.CONTROLLER.sw2 == RC_SW_DOWN)
		{
//			Gun_Motor_SHOOT();
			set_pit[GIMBAL_Below] = rc.CONTROLLER.ch2 * 30.0f / 660; //PITCH轴为绝对位置
			set_yaw[GIMBAL_Below] = rc.CONTROLLER.ch1 * 30.0f / 660;
			
			set_revolve_spd[GIMBAL_Below] = rc.CONTROLLER.ch4 * 1500.0f / 660;
		}
		break;
	case AutoAim_mode:
		//自瞄
		//视觉测试（下云台）
		if (VisionRecvData.identify_target)
		{
			max_count = 30;
			if (Vision_If_Update() == true)
			{
				set_pit[GIMBAL_Below] = VisionRecvData.pitch_angle * visionK + cur_pit[GIMBAL_Below] + adjust_angle_pit;
				set_yaw[GIMBAL_Below] = VisionRecvData.yaw_angle * visionK + cur_yaw[GIMBAL_Below] + adjust_angle_yaw;
				//清楚视觉识别Flag
				Clear_Vision_Get_Flag();
			}
			if(VisionRecvData.shoot_cmd)
			{
//				Gun_Motor_SHOOT();
			}
			else 
			{
				Gun_Motor_Stop();
			}
		}
		else
		{
			max_count = MAX_COUNT_MOVE;
			set_yaw[GIMBAL_Below] += auto_rate_yaw * 0.5f;
			set_pit[GIMBAL_Below] += auto_rate_pit * 3.0f;  //4.0f
			changeAutoRate();
		}
		break;
	case Disable_mode:
		set_pit[GIMBAL_Above] = -150;
		set_yaw[GIMBAL_Above] = 0;
		set_pit[GIMBAL_Below] = 0;
		set_yaw[GIMBAL_Below] = 0;
		set_revolve_spd[GIMBAL_Above] = 0;
		set_revolve_spd[GIMBAL_Below] = 0;
		break;
	}

	//设定角度限幅
	set_pit[GIMBAL_Below] = fp32_constrain(set_pit[GIMBAL_Below], MIN_BELOW_PIT_ANGLE, MAX_BELOW_PIT_ANGLE);
	set_yaw[GIMBAL_Below] = fp32_constrain(set_yaw[GIMBAL_Below], -MAX_BELOW_YAW_ANGLE, MAX_BELOW_YAW_ANGLE);
}

void Gimbal_PID_calculate()
{
	pid_calc(&pid_yaw[0], cur_yaw[0], set_yaw[0]);
	pid_calc(&pid_pit[0], cur_pit[0], set_pit[0]);
	pid_calc(&pid_pit[1], cur_pit[1], set_pit[1]);

	//串级pid 下pit
	pid_calc(&pid_pit[GIMBAL_Below], cur_pit[GIMBAL_Below], set_pit[GIMBAL_Below]);											 //角度环
	pid_calc(&pid_pit_spd[GIMBAL_Below], moto_pit[GIMBAL_Below].speed_rpm, pid_pit[GIMBAL_Below].pos_out); //速度环
	
	//串级pid 下yaw
	pid_calc(&pid_yaw[GIMBAL_Below], cur_yaw[GIMBAL_Below], set_yaw[GIMBAL_Below]);											 //角度环
	pid_calc(&pid_yaw_spd[GIMBAL_Below], moto_yaw[GIMBAL_Below].speed_rpm, pid_yaw[GIMBAL_Below].pos_out); //速度环

	//下拨盘
	pid_calc(&pid_revolve[GIMBAL_Below], moto_revolve[GIMBAL_Below].speed_rpm, set_revolve_spd[GIMBAL_Below]);
}

void Gimbal_PID_struct_init(void)
{
	//上云台
	PID_struct_init(&pid_pit[GIMBAL_Above], POSITION_PID, 20000, 1000,
					Gimbal_pitch_above_pid_KP, Gimbal_pitch_above_pid_KI, Gimbal_pitch_above_pid_KD); //pitch电机
	PID_struct_init(&pid_yaw[GIMBAL_Above], POSITION_PID, 20000, 1000,
					Gimbal_yaw_above_pid_KP, Gimbal_yaw_above_pid_KI, Gimbal_yaw_above_pid_KD); //yaw电机

	//下云台
	PID_struct_init(&pid_pit[GIMBAL_Below], POSITION_PID, 5000, 1000,
					Gimbal_pitch_below_pid_KP, Gimbal_pitch_below_pid_KI, Gimbal_pitch_below_pid_KD); //pitch电机
	PID_struct_init(&pid_yaw[GIMBAL_Below], POSITION_PID, 8000, 5000,
					Gimbal_yaw_below_pid_KP, Gimbal_yaw_below_pid_KI, Gimbal_yaw_below_pid_KD); //yaw电机

	//下拨盘电机
	PID_struct_init(&pid_revolve[GIMBAL_Below], DELTA_PID, 5000, 1000,
					Revolve_below_pid_KP, Revolve_below_pid_KI, Revolve_below_pid_KD);

	//内环（速度）下yaw轴
	PID_struct_init(&pid_yaw_spd[GIMBAL_Below],
					DELTA_PID, 20000, 1000, 1, 0, 0.1); //yaw电机
	//内环（速度）下pit轴
	PID_struct_init(&pid_pit_spd[GIMBAL_Below],
					DELTA_PID, 20000, 1000, 1, 0, 0); //pit电机
}

void changeAutoRate()
{
	if (set_yaw[GIMBAL_Below] >= MAX_BELOW_YAW_ANGLE)
		auto_rate_yaw = -auto_rate_yaw;
	else if (set_yaw[GIMBAL_Below] <= -MAX_BELOW_YAW_ANGLE)
		auto_rate_yaw = -auto_rate_yaw;

	if (set_pit[GIMBAL_Below] >= MAX_BELOW_PIT_ANGLE)
		auto_rate_pit = -auto_rate_pit;
	else if (set_pit[GIMBAL_Below] <= -MAX_BELOW_PIT_ANGLE)
		auto_rate_pit = -auto_rate_pit;
}
