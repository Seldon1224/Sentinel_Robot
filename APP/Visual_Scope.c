#include "Visual_Scope.h"


void VisualScope_Output(float data1 ,float data2 ,float data3 ,float data4)
{
  int temp[4] = {0};
  unsigned int temp1[4] = {0};
  uint8_t databuf[10] = {0};
  int i;
  unsigned short CRC16 = 0;

  temp[0] = (int)data1;
  temp[1] = (int)data2;
  temp[2] = (int)data3;
  temp[3] = (int)data4;

  temp1[0] = (unsigned int)temp[0];
  temp1[1] = (unsigned int)temp[1];
  temp1[2] = (unsigned int)temp[2];
  temp1[3] = (unsigned int)temp[3];
  
  for(i=0;i<4;i++) 
  {
    databuf[i*2]   = (unsigned char)(temp1[i]%256);
    databuf[i*2+1] = (unsigned char)(temp1[i]/256);
  }
  
	
  CRC16 = CRC_CHECK(databuf, 8);
  databuf[8] = CRC16%256;
  databuf[9] = CRC16/256; 
  for(i=0; i<10; i++)
  {
		//while((USART2->SR&0X40)==0);
		while((huart7.Instance->SR & 0x40)==0);
		while(HAL_UART_Transmit(&huart7, &databuf[i], 1, 0xffff) != HAL_OK);
		//USART_SendData(USART2, databuf[i]);
  }
}

void VisualDisplay(float ch1, float ch2, float ch3, float ch4)
{
	float OutData[4];
	OutData[0] = ch1;
	OutData[1] = ch2;
	OutData[2] = ch3;
	OutData[3] =ch4;
	VisualScope_Output(OutData[0],OutData[1] ,OutData[2] ,OutData[3]);
	//delay_ms(1000);
}


//-------------------------------------------------------------------------------------------
//The following is the function of CRC16,please refer
//-------------------------------------------------------------------------------------------
unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){      
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}
//-------------------------------------------------------------------------------------------
//The above is the function of CRC16,please refer
//-------------------------------------------------------------------------------------------
