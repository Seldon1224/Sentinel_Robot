#ifndef __PARAM__H_
#define __PARAM__H_

typedef struct PID_parameter{
	float Kp;
	float Ki;
	float Kd;
}PID_parameter;

#define Gimbal_pitch_below_pid_KP 10.0f
#define Gimbal_pitch_below_pid_KI 0.0f
#define Gimbal_pitch_below_pid_KD 0.0f

#define Gimbal_yaw_below_pid_KP 15.0f
#define Gimbal_yaw_below_pid_KI 0.0f
#define Gimbal_yaw_below_pid_KD 0.0f

#define Gimbal_pitch_above_pid_KP 200.0f
#define Gimbal_pitch_above_pid_KI 0.0f
#define Gimbal_pitch_above_pid_KD 0.0f

#define Gimbal_yaw_above_pid_KP 200.0f
#define Gimbal_yaw_above_pid_KI 0.0f
#define Gimbal_yaw_above_pid_KD 0.0f


#define Revolve_below_pid_KP 10.5f
#define Revolve_below_pid_KI 0.3f
#define Revolve_below_pid_KD 0.7f


extern PID_parameter PID[8];

void PID_Param_Init(void );

#endif

