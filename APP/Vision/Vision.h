#ifndef _VISION_H_
#define _VISION_H_

#include "stdio.h"
#include "./remote_control/remote_control.h"
#include "./can_receive/can_receive.h"
#include "main.h"
#include "can.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"

#include "gpio.h"

#define VISION_RX_LENGTH 25
#define VISION_TX_LENGTH 25
#define VISION_SOF 0xA5
#define VISION_TOF 0xA6

#define VISION_TIME_DELTA 116110812848148

//STM32接收,直接将串口接收到的数据拷贝进结构体
typedef __packed struct
{
	/* 头 */
	uint8_t SOF;   //帧头起始位,0xA5
	uint8_t CmdID; //指令

	/* 数据 */
	float yaw_angle;		 	 //例：100.1 33 33 C8 42
	float pitch_angle;		 //注意：stm32 float为小端
	float distance;			 	 //距离
	uint8_t shoot_cmd;		 //是否开始射击  0  1开始
	uint8_t identify_target;//视野内是否有目标/是否识别到了目标   0否  1是
	//预留
	long long time; //视觉发来的时间戳
	/* 尾 */
	uint8_t TOF;
} extVisionRecvData_t;

//STM32发送,直接将打包好的数据一个字节一个字节地发送出去
typedef __packed struct
{
	/* 头 */
	uint8_t SOF;   //帧头起始位,0xA5
	uint8_t CmdID; //指令

	/* 数据 */
	float yaw_angle;
	float pitch_angle;
	float yaw_speed;		 
	float pitch_speed;
	uint8_t target_mode; //自瞄模式{ 0:普通自瞄 1:吊射 2:能量机关}
	
  //预留
	uint8_t blank_a;
	uint8_t blank_b;
	uint8_t blank_c;
	uint8_t blank_e;
	uint8_t blank_f;
	/* 尾 */
	uint8_t TOF;

} extVisionSendData_t;

extern extVisionRecvData_t VisionRecvData; //视觉接收结构体

extern extVisionSendData_t VisionSendData; //视觉发送结构体

extern uint8_t ReadTemp[VISION_RX_LENGTH];

extern uint8_t Vision_Get_New_Data;

uint8_t Vision_If_Update(void);
void Clear_Vision_Get_Flag(void);
void Vision_Read_Data(uint8_t *ReadFormUsart);
void Vision_Send_Data(uint8_t CmdID);

#endif
