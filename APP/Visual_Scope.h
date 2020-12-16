#ifndef __VISUAL_SCOPE__
#define __VISUAL_SCOPE__
		 
#include "usart.h"

extern UART_HandleTypeDef huart6;
void VisualScope_Output(float data1 ,float data2 ,float data3 ,float data4);
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT);
void VisualDisplay(float ch1, float ch2, float ch3, float ch4);


#endif

