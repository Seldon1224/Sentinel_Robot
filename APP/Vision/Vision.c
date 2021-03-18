#include "Vision.h"
#include "Robo_config.h"

extVisionRecvData_t VisionRecvData; //视觉接收结构体

extVisionSendData_t VisionSendData; //视觉发送结构体

uint8_t ReadTemp[VISION_RX_LENGTH];
uint8_t TransmitTemp[VISION_TX_LENGTH];

uint8_t Vision_Get_New_Data = false; //视觉数据是否更新

void Vision_Read_Data(uint8_t *ReadFormUsart)
{

	//判断帧头数据是否为0xA5
	if (ReadFormUsart[0] == VISION_SOF && ReadFormUsart[19] == VISION_TOF)
	{
		//接收数据拷贝
		memcpy(&VisionRecvData, ReadFormUsart, VISION_RX_LENGTH);
		Vision_Get_New_Data = true;//标记视觉数据更新了
		//
		//	//帧计算
		//	Vision_Time_Test[NOW] = xTaskGetTickCount();
		//	Vision_Ping = Vision_Time_Test[NOW] - Vision_Time_Test[LAST];//计算时间间隔
		//	Vision_Time_Test[LAST] = Vision_Time_Test[NOW];
	}
}
uint8_t Vision_If_Update()
{
	return Vision_Get_New_Data;
}

void Clear_Vision_Get_Flag(){
	Vision_Get_New_Data = false;
}
void Vision_Send_Data(uint8_t CmdID)
{
}
