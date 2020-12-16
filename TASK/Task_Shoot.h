#ifndef TASK_SHOOT
#define TASK_SHOOT

#include "./remote_control/remote_control.h"
#include "./pid/pid.h"
#include "System.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "Robo_config.h"
#include "param.h"
extern RC_REV rc;


void Task_Shoot(void *argument);
void shoot_init(void);
void shoot_up(void);
void shoot_down(void);
#endif
