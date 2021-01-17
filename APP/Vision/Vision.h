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

#define VISION_LENGTH 20
#define VISION_SOF 0xA5
#define VISION_TOF 0xA6

//STM32接收,直接将串口接收到的数据拷贝进结构体
typedef __packed struct
{
	/* 头 */
	uint8_t SOF;   //帧头起始位,暂定0xA5
	uint8_t CmdID; //指令
	//uint8_t   CRC8;			//帧头CRC校验,保证发送的指令是正确的

	/* 数据 */
	float yaw_angle;		 //例：100.1 33 33 C8 42
	float pitch_angle;		 //注意：stm32 float为小端
	float distance;			 //距离
	uint8_t shoot_lock;		 //是否瞄准到了中间  0没有  1瞄准到了
	uint8_t identify_target; //视野内是否有目标/是否识别到了目标   0否  1是
	uint8_t blank_a;

	uint8_t blank_b; //预留
	uint8_t blank_c;

	/* 尾 */
	uint8_t TOF;

} extVisionRecvData_t;

//STM32发送,直接将打包好的数据一个字节一个字节地发送出去
typedef struct
{
	/* 头 */
	uint8_t SOF;   //帧头起始位,暂定0xA5
	uint8_t CmdID; //指令
	uint8_t CRC8;  //帧头CRC校验,保证发送的指令是正确的

	/* 数据 */
	float yaw_angle;
	float pitch_angle;
	float distance;		 //距离
	uint8_t lock_sentry; //是否在抬头识别哨兵
	uint8_t base;		 //吊射

	uint8_t blank_a; //预留
	uint8_t blank_b;
	uint8_t blank_c;
	/* 尾 */
	uint16_t CRC16;

} extVisionSendData_t;

extern extVisionRecvData_t VisionRecvData; //视觉接收结构体

extern extVisionSendData_t VisionSendData; //视觉发送结构体

extern uint8_t ReadTemp[20];

extern uint8_t Vision_Get_New_Data;

uint8_t Vision_If_Update(void);
void Clear_Vision_Get_Flag(void);
void Vision_Read_Data(uint8_t *ReadFormUsart);
void Vision_Send_Data(uint8_t CmdID);

#endif
