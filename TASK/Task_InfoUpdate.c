#include "Task_InfoUpdate.h"

extern uint8_t DebugRevFlag;
extern uint8_t originData[5];
uint8_t readyFlag = 0;
uint16_t pid[3]; //   P/I/D

void Task_InfoUpdate(void *argument)
{
	for (;;)
	{
		Debug();			   //debug串口
		imu_update(&mpu_data); //陀螺仪

		//2020.11.11新增，从can_receive修改到此
		keyboard_scan();
		GET_Control_Mode();

		osDelay(3);
	}
}

void imu_update(IMU_UART *mpu_data)
{
	//精度取0.5
	mpu_data->yaw = (int)mpu_data->real_yaw + ((mpu_data->real_yaw - (int)mpu_data->real_yaw) >= 0.5f ? 0.5f : 0);
	mpu_data->pitch = (int)mpu_data->real_pitch + ((mpu_data->real_pitch - (int)mpu_data->real_pitch) >= 0.5f ? 0.5f : 0);
	mpu_data->roll = (int)mpu_data->real_roll + ((mpu_data->real_roll - (int)mpu_data->real_roll) >= 0.5f ? 0.5f : 0);

	//初始化，确定初始零位
	if (!mpu_data->imu_start_flag)
	{
		if (mpu_data->yaw && mpu_data->pitch)
		{
			mpu_data->offset_imu_yaw = mpu_data->yaw;
			mpu_data->offset_imu_pitch = mpu_data->pitch;
			mpu_data->offset_imu_roll = mpu_data->roll;
			mpu_data->imu_start_flag = 1;
		}
	}
	else
	{
		//yaw轴转动圈数
		if (mpu_data->yaw - mpu_data->last_imu_yaw > 180)
		{
			mpu_data->round_cnt--;
		}
		else if (mpu_data->yaw - mpu_data->last_imu_yaw < -180)
		{
			mpu_data->round_cnt++;
		}
		//偏移角
		mpu_data->total_imu_yaw = (mpu_data->yaw - mpu_data->offset_imu_yaw) + mpu_data->round_cnt * 360;
		mpu_data->total_imu_pitch = (mpu_data->pitch - mpu_data->offset_imu_pitch) + mpu_data->round_cnt * 360;
		mpu_data->total_imu_roll = (mpu_data->roll - mpu_data->offset_imu_roll) + mpu_data->round_cnt * 360;

		//last_imu计算
		mpu_data->last_imu_yaw = mpu_data->yaw;
		mpu_data->last_imu_pitch = mpu_data->pitch;
		mpu_data->last_imu_roll = mpu_data->roll;
	}
}

uint16_t ucharToSet() //数据格式：char[5] -> (0~65535)
{
	uint16_t valueToSet = 0;

	for (uint8_t i = 0; i < 5; i++)
	{
		if (originData[i] >= 48 && originData[i] <= 57) //合法性检测, 保证全部为数字
		{
			originData[i] -= 48;
			valueToSet += originData[i] * pow(10, i);
		}
		else
			return 0;
	}
	return valueToSet;
}

void Debug()
{
	if (readyFlag)
	{
		switch (DebugRevFlag) //Debug函数使用方法：1. 调整DebugRevFlag阈值，比如PID三个参数就在case3执行完成后置零。
		{
		case 1:
			pid[0] = ucharToSet();
			break;
		case 2:
			pid[1] = ucharToSet();
			break;
		case 3:
			pid[2] = ucharToSet();
			DebugRevFlag = 0;
			//	执行函数写这里，例如PID_SET

			//
			break;
		default:
			break;
		}
		readyFlag = 0;
	}
}
