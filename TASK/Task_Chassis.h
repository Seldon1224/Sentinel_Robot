#ifndef TASK_CHASSIS
#define TASK_CHASSIS

#include "System.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "mytype.h"
#include "math.h"
#include "Robo_config.h"
#include "./remote_control/remote_control.h"
#include "./pid/pid.h"
#include "param.h"
#include "Task_Gimbal.h"
//底盘
//键盘控制量
#define V_max_ch 4000   //最大车速  mm/s
#define V_min_ch 0      //最小车速	mm/s

//电机相关

typedef enum
{
    NOT_ADD_AccXYW,
    ADD_AccXYW
} ADD_AccXYW_ENABLE;



void Task_Chassis(void *argument);
void GET_Control_Mode(void);
void GET_Chassis_Enable_Flag(void);
void GET_Chassis_Dir_Spd(void);

#endif
