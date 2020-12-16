#ifndef __PARAM__H_
#define __PARAM__H_

typedef struct PID_parameter{
	float Kp;
	float Ki;
	float Kd;
}PID_parameter;

extern PID_parameter PID[8];

void PID_Param_Init(void );

#endif

