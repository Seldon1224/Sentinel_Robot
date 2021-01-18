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
#define MAX_COUNT_MOVE  90 //从一端运动到另一端的最大圈数 156
#define CHASSIS_DIR_NO_MOVE 0
#define CHASSIS_DIR_LEFT    1
#define CHASSIS_DIR_RIGHT   2


typedef enum
{
    NOT_ADD_AccXYW,
    ADD_AccXYW
} ADD_AccXYW_ENABLE;

extern uint8_t max_count;

void Task_Chassis(void *argument);
void GET_Control_Mode(void);
void GET_Chassis_Enable_Flag(void);
void GET_Chassis_Dir_Spd(void);
void getCurrentChassisDir(void);
void ChangeChassisDir(void);

#endif
