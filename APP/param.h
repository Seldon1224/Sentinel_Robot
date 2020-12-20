#ifndef __PARAM__H_
#define __PARAM__H_

typedef struct PID_parameter{
	float Kp;
	float Ki;
	float Kd;
}PID_parameter;

#define Gimbal_pitch_below_pid_KP 4500.0f
#define Gimbal_pitch_below_pid_KI 100.0f
#define Gimbal_pitch_below_pid_KD 6000.0f

#define Gimbal_yaw_below_pid_KP 4000.0f
#define Gimbal_yaw_below_pid_KI 2.0f
#define Gimbal_yaw_below_pid_KD 20000.0f

#define Gimbal_pitch_above_pid_KP 200.0f
#define Gimbal_pitch_above_pid_KI 0.0f
#define Gimbal_pitch_above_pid_KD 0.0f

#define Gimbal_yaw_above_pid_KP 200.0f
#define Gimbal_yaw_above_pid_KI 0.0f
#define Gimbal_yaw_above_pid_KD 0.0f

extern PID_parameter PID[8];

void PID_Param_Init(void );

#endif

