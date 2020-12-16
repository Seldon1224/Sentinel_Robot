#ifndef TASK_GIMBAL
#define TASK_GIMBAL
#include "./remote_control/remote_control.h"
#include "./pid/pid.h"
#include "./can_receive/can_receive.h"
#include "./Vision/Vision.h"
#include "System.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "param.h"


#define GIMBAL_Above 0
#define GIMBAL_Below 1

#define FORMAT_Angle(a) (a*360.0f/8192)
#define YAW_BELOW_MID_POS 2700
#define PIT_BELOW_MID_POS 3000
#define YAW_ABOVE_MID_POS 2735
#define PIT_ABOVE_MID_POS 3313
typedef struct
{
	//实际传回数据
	float real_yaw;
	float real_pitch;
	float real_roll;
	float a[3],w[3],T;
	
	//以下为新增
	float yaw;//精度为0.5
	float pitch;
	float roll;
	
	float last_imu_yaw;//上一次角度
	float last_imu_pitch;
	float last_imu_roll;
	
	float offset_imu_yaw;//零位
	float offset_imu_pitch;
	float offset_imu_roll;
	
	float total_imu_yaw;//偏移角
  float total_imu_pitch;
	float total_imu_roll;
	
	float round_cnt;//yaw轴转动圈数
	bool imu_start_flag;//启动标志，mpu_updata运行一次后为1
} IMU_UART;

//期望值
extern float set_yaw[2];
extern float set_pit[2];

//当前值
extern float cur_yaw[2];
extern float cur_pit[2];

extern IMU_UART mpu_data;

void Task_Gimbal(void *argument);
#endif
