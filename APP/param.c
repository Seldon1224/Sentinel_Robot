#include "param.h"
PID_parameter PID[8];
//p,i,d
float chassis_pid[3] = {20.0f, 0.0f, 10.0f};
//ÔÆÌ¨pid


float gimbal_pitch_pid[3] = {4500.0f, 100.0f, 6000.0f};
float gimbal_yaw_pid[3] = {4000.0f, 2.0f, 20000.0f};

float gimbal_yaw_lock_pid[3] = {200.0f, 0.0f, 100.0f};
float shoot_forward_pid[3] = {1.5f, 0.1f, 0.0f};
float shoot_reverse_pid[3] = {1.5f, 0.1f, 0.0f};

void PID_Assign(PID_parameter *PID, float pid[3])
{
	PID->Kp = pid[0];
	PID->Ki = pid[1];
	PID->Kd = pid[2];
}

void PID_Param_Init()
{
	PID_Assign(&PID[1], chassis_pid);
	PID_Assign(&PID[2], gimbal_pitch_pid);
	PID_Assign(&PID[3], gimbal_yaw_pid);
	PID_Assign(&PID[4], shoot_forward_pid);
	PID_Assign(&PID[5], shoot_reverse_pid);
	//	PID_Assign(&PID[6], chassis_pid);
	//	PID_Assign(&PID[7], chassis_pid);
}
