#ifndef TASK_INFO_UPDATE
#define TASK_INFO_UPDATE

#include "./remote_control/remote_control.h"
#include "./pid/pid.h"

#include "System.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "Task_Gimbal.h"
#include "./ANO_DT/ANO_DT.h"
#include "math.h"

extern RC_REV rc;

void Task_InfoUpdate(void *argument);
uint16_t ucharToSet(void);
void Debug(void);
void imu_update(IMU_UART *mpu_data);
#endif

