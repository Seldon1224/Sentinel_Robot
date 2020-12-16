#include "./ANO_DT/ANO_DT.h"
#include "usart.h"

/////////////////////////////////////////////////////////////////////////////////////
//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )
	

dt_flag_t f;            //需要发送数据的标志
u8 data_to_send[50];    //发送数据缓存


/////////////////////////////////////////////////////////////////////////////////////
void ANO_DT_Display(s16 user_1, s16 user_2, s16 user_3,s16 user_4,s16 user_5,s16 user_6,s16 user_7,s16 user_8,s16 user_9)
{
	ANO_DT_Send_Udata(user_1,user_2,user_3,user_4,user_5,user_6,user_7,user_8,user_9);
}
//Data_Exchange函数处理各种数据发送请求，比如想实现每5ms发送一次传感器数据至上位机，即在此函数内实现
//此函数应由用户每1ms调用一次
void ANO_DT_Data_Exchange(void)
{
    static u8 cnt = 0;
		static u8 udata_cnt			=	25;
//    static u8 status_cnt    = 15;
//    static u8 motopwm_cnt   = 20;
		
//    static u8 power_cnt     = 50;			//计量值用来确定数据传输频率
    

//    if((cnt % status_cnt) == (status_cnt-1))
//        f.send_status = 1;  
//    
//    if((cnt % motopwm_cnt) == (motopwm_cnt-1))
//        f.send_motopwm = 1; 
//    
		if((cnt % udata_cnt) == (udata_cnt-1))
				f.udata =1;
//    if((cnt % power_cnt) == (power_cnt-1))
//        f.send_power = 1;					//到达计数值，标志位置一
    
    cnt++;
/*
/////////////////////////////////////////////////////////////////////////////////////
    if(f.send_status)//传输UAV当前状态、姿态角、高度
    {
        f.send_status = 0;
        ANO_DT_Send_Status(attitude.roll, attitude.pitch, attitude.yaw, attitude.height,2,1);
    }   
/////////////////////////////////////////////////////////////////////////////////////
    else if(f.send_senser)//传输传感器数据6050和磁力计等数据
    {
        f.send_senser = 0;
        ANO_DT_Send_Senser(debug.temp_duty_Kd*100, debug.temp_duty_Kp*100, debug.angle,attitude.gyro_x,attitude.gyro_y,attitude.gyro_z,accel[0],accel[1],accel[2]);
    }   
  else if(f.udata)
	{
			f.udata=0;
		ANO_DT_Send_Udata(0,0,0,0,0,0,0,0,0);
	}
/////////////////////////////////////////////////////////////////////////////////////  

    else if(f.send_motopwm)//传输电机PWM信号
    {
        f.send_motopwm = 0;
        ANO_DT_Send_MotoPWM(motor_duty[0],motor_duty[1],motor_duty[2],motor_duty[3],motor_duty[0],motor_duty[1],motor_duty[2],motor_duty[3]);
    }   
/////////////////////////////////////////////////////////////////////////////////////
    else if(f.send_power)//传输电池电量
    {
        f.send_power = 0;
				Power_V=Get_Adc(1)*330*4.49/4096;	//测量此时的电池电量并进行数据传输
				if(Power_V<10.5)PID_yaw.Kp=1;
        ANO_DT_Send_Power(Power_V,20);
    } */
		
		
		
	if(f.udata)
	{
		f.udata=0;
		ANO_DT_Send_Udata(0,0,0,0,0,0,0,0,200);
	}
}
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
//Send_Data函数是协议中所有发送数据功能使用到的发送函数
//移植时，用户应根据自身应用的情况，根据使用的通信方式，实现此函数
void ANO_DT_Send_Data(u8 *dataToSend , u8 length)
{
	HAL_UART_Transmit(&MY_UART,dataToSend,length,0xFF);
}


/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Prepare函数是协议预解析，根据协议的格式，将收到的数据进行一次格式性解析，格式正确的话再进行数据解析
//移植时，此函数应由用户根据自身使用的通信方式自行调用，比如串口每收到一字节数据，则调用此函数一次
//此函数解析出符合格式的数据帧后，会自行调用数据解析函数
//接收函数，对接收到的数据进行数据解析，确认是否符合规定的数据传输格式
void ANO_DT_Data_Receive_Prepare(u8 data)
{
    static u8 RxBuffer[50];
    static u8 _data_len = 0,_data_cnt = 0;
    static u8 state = 0;
    
    if(state==0&&data==0xAA)
    {
        state=1;
        RxBuffer[0]=data;
    }
    else if(state==1&&data==0xAF)
    {
        state=2;
        RxBuffer[1]=data;
    }else if (state==2&&data==0x05)
		{
			state=3;
			RxBuffer[2]=data;
		}
    else if(state==3&&data<0XF1)
    {
        state=4;
        RxBuffer[3]=data;
    }
    else if(state==4&&data<50)
    {
        state = 5;
        RxBuffer[4]=data;
        _data_len = data;
        _data_cnt = 0;
    }
    else if(state==5&&_data_len>0)
    {
        _data_len--;
        RxBuffer[5+_data_cnt++]=data;
        if(_data_len==0)
            state = 6;
    }
    else if(state==6)
    {
        state = 0;
        RxBuffer[5+_data_cnt]=data;
        ANO_DT_Data_Receive_Anl(RxBuffer,_data_cnt+6);//校验完成，数据格式符合协议要求
    }
    else
        state = 0;			//接收校验，根据上文定义的发送方式制定接收校验准则
}
/////////////////////////////////////////////////////////////////////////////////////
//Data_Receive_Anl函数是协议数据解析函数，函数参数是符合协议格式的一个数据帧，该函数会首先对协议数据进行校验
//校验通过后对数据进行解析，实现相应功能
//此函数可以不用用户自行调用，由函数Data_Receive_Prepare自动调用
void ANO_DT_Data_Receive_Anl(u8 *data_buf,u8 num)
{
    u8 sum = 0,i;
    for(i=0;i<(num-1);i++)
        sum += *(data_buf+i);
    if(!(sum==*(data_buf+num-1)))       return ;     //判断sum
    if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF && *(data_buf+2)==0x05))     return  ;     //判断帧头

		if(*(data_buf+3)==0XE0)//校准命令，各个参数控制不同模块进行校准
    {
			if (*(data_buf + 5) == 0x01)
			{
				switch (*(data_buf + 7))				
				{				
				case 0x01:  break;		//ACC校准
				case 0x02:  break;		//GYRO校准
				case 0x04:  break;		//MAG校准
				case 0x05:  break;		//BARO校准
				default: break;
				}
			}
			else if (*(data_buf + 5) == 0x10)
			{
				switch (*(data_buf+7))
				{
				case 0x01:	break;			//一键起飞
				case 0x02:	break;			//一键降落
				case 0x03:	break;			//上升
				case 0x04:	break;			//下降
				case 0x05:	break;			//前进
				case 0x06:	break;			//后退
				case 0x07:	break;			//向左
				case 0x08:	break;			//向右
				case 0x09:	break;			//左旋
				case 0x0A:	break;			//右旋
				case 0xA0:	break;			//紧急停机
				default: break;
				}
			}else if(*(data_buf+5)==0xE1)
			{
				ANO_DT_Check(*(data_buf+7));
			}
			ANO_DT_Send_Data(data_buf, num);
    }
   
    
		if(*(data_buf+3)==0XE1)		//读取参数请求模块
    {
			switch (*(data_buf + 6))
			{
				case 1 :PID[1].Kp = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10)); break;
				case 2 :PID[1].Ki = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10)); break;
				case 3 :PID[1].Kd = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10)); break;
				case 4 :PID[2].Kp = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 5 :PID[2].Ki = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 6 :PID[2].Kd = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 7 :PID[3].Kp = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 8 :PID[3].Ki = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 9 :PID[3].Kd = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 10:PID[4].Kp	= 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 11:PID[4].Ki = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 12:PID[4].Kd = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 13:PID[5].Kp = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 14:PID[5].Ki = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 15:PID[5].Kd = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 16:PID[6].Kp = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 17:PID[6].Ki = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 18:PID[6].Kd = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 19:PID[7].Kp = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 20:PID[7].Ki = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
				case 21:PID[7].Kd = 0.001 * ((vs16)(*(data_buf + 9) << 8) | *(data_buf + 10));break;
			default:  break;
			}
			*(data_buf+1)=0x05;
			*(data_buf+2)=0xAF;
			ANO_DT_Send_Data(data_buf,num);
    }
}

void ANO_DT_Check(u8 num)
{
	vs16 _temp;
	u8 sum = 0;
	data_to_send[0]=0xAA;
	data_to_send[1]=0x05;
	data_to_send[2]=0xAF;
	data_to_send[3]=0xE1;
	data_to_send[4]=0x06;
	data_to_send[5]=0x00;
	data_to_send[6]=num;
	data_to_send[7]=0x00;
	data_to_send[8]=0x00;
//	data_to_send[9]=0x00;
//	data_to_send[10]=0x15;
	switch(num){
		case 1:
			_temp = 1000 *PID[1].Kp;break;
		case 2:
			_temp = 1000 *PID[1].Ki;break;
		case 3:
			_temp = 1000 *PID[1].Kd;break;
		case 4:
			_temp = 1000 *PID[2].Kp;break;
		case 5:
			_temp = 1000 *PID[2].Ki;break;
		case 6:
			_temp = 1000 *PID[2].Kd;break;
		case 7:
			_temp = 1000 *PID[3].Kp;break;
		case 8:
			_temp = 1000 *PID[3].Ki;break;
		case 9:
			_temp = 1000 *PID[3].Kd;break;
		case 10:
			_temp = 1000 *PID[4].Kp;break;
		case 11:
			_temp = 1000 *PID[4].Ki;break;
		case 12:
			_temp = 1000 *PID[4].Kd;break;
		case 13:
			_temp = 1000 *PID[5].Kp;break;
		case 14:
			_temp = 1000 *PID[5].Ki;break;
		case 15:
			_temp = 1000 *PID[5].Kd;break;
		case 16:
			_temp = 1000 *PID[6].Kp;break;
		case 17:
			_temp = 1000 *PID[6].Ki;break;
		case 18:
			_temp = 1000 *PID[6].Kd;break;
		case 19:
			_temp = 1000 *PID[7].Kp;break;
		case 20:
			_temp = 1000 *PID[7].Ki;break;
		case 21:
			_temp = 1000 *PID[7].Kd;break;
		
	}
	data_to_send[9] = BYTE1(_temp);
	data_to_send[10] = BYTE0(_temp);
	for(u8 i=0;i<11;i++)
        sum += data_to_send[i];
   data_to_send[11]=sum;
	ANO_DT_Send_Data(data_to_send,12);
}
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed)
{
//    u8 _cnt=0;
//		u8 sum = 0,i;
//    vs16 _temp;
//    vs32 _temp2 = alt;
//    
//    data_to_send[_cnt++]=0xAA;
//    data_to_send[_cnt++]=0x05;
//		data_to_send[_cnt++]=0xAF;
//    data_to_send[_cnt++]=0x01;
//    data_to_send[_cnt++]=0;
//    
//    _temp = (int)(angle_rol*100);
//    data_to_send[_cnt++]=BYTE1(_temp);
//    data_to_send[_cnt++]=BYTE0(_temp);
//    _temp = (int)(angle_pit*100);
//    data_to_send[_cnt++]=BYTE1(_temp);
//    data_to_send[_cnt++]=BYTE0(_temp);
//    _temp = (int)(angle_yaw*100);
//    data_to_send[_cnt++]=BYTE1(_temp);
//    data_to_send[_cnt++]=BYTE0(_temp);		//传输角度对应值
//    
//    data_to_send[_cnt++]=BYTE3(_temp2);
//    data_to_send[_cnt++]=BYTE2(_temp2);
//    data_to_send[_cnt++]=BYTE1(_temp2);
//    data_to_send[_cnt++]=BYTE0(_temp2);		//传输ALT高度信息
//    
//    data_to_send[_cnt++] = fly_model;		//传输此刻飞行模式
//											//无	00
//											//姿态	01
//											//定高	02
//											//定点	03
//											//航线	11
//											//降落	20
//											//返航	21

//    data_to_send[_cnt++] = armed;			//传输armed  0:加锁，1：解锁
//    
//    data_to_send[4] = _cnt-5;
//    

//    for(i=0;i<_cnt;i++)
//        sum += data_to_send[i];
//    data_to_send[_cnt++]=sum;
//    
//    ANO_DT_Send_Data(data_to_send, _cnt);
}

void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z)
{
//    u8 _cnt=0;
//		u8 sum = 0,i;
//    vs16 _temp;
//    
//    data_to_send[_cnt++]=0xAA;
//    data_to_send[_cnt++]=0x05;
//		data_to_send[_cnt++]=0xAF;
//    data_to_send[_cnt++]=0x02;
//    data_to_send[_cnt++]=0;
//    
//    _temp = a_x;
//    data_to_send[_cnt++]=BYTE1(_temp);
//    data_to_send[_cnt++]=BYTE0(_temp);
//    _temp = a_y;
//    data_to_send[_cnt++]=BYTE1(_temp);
//    data_to_send[_cnt++]=BYTE0(_temp);
//    _temp = a_z;    
//    data_to_send[_cnt++]=BYTE1(_temp);
//    data_to_send[_cnt++]=BYTE0(_temp);
//    
//    _temp = g_x;    
//    data_to_send[_cnt++]=BYTE1(_temp);
//    data_to_send[_cnt++]=BYTE0(_temp);
//    _temp = g_y;    
//    data_to_send[_cnt++]=BYTE1(_temp);
//    data_to_send[_cnt++]=BYTE0(_temp);
//    _temp = g_z;    
//    data_to_send[_cnt++]=BYTE1(_temp);
//    data_to_send[_cnt++]=BYTE0(_temp);
//    
//    _temp = m_x;    
//    data_to_send[_cnt++]=BYTE1(_temp);
//    data_to_send[_cnt++]=BYTE0(_temp);
//    _temp = m_y;    
//    data_to_send[_cnt++]=BYTE1(_temp);
//    data_to_send[_cnt++]=BYTE0(_temp);
//    _temp = m_z;    
//    data_to_send[_cnt++]=BYTE1(_temp);
//    data_to_send[_cnt++]=BYTE0(_temp);
//    
//    data_to_send[4] = _cnt-5;
//    

//    for(i=0;i<_cnt;i++)
//        sum += data_to_send[i];
//    data_to_send[_cnt++] = sum;
//    
//    ANO_DT_Send_Data(data_to_send, _cnt);
//}

//void ANO_DT_Send_Power(u16 votage, u16 current)
//{
//    u8 _cnt=0;
//    u16 temp;
//    u8 sum = 0,i;    
//    data_to_send[_cnt++]=0xAA;
//    data_to_send[_cnt++]=0x05;
//	data_to_send[_cnt++]=0xAF;
//    data_to_send[_cnt++]=0x05;
//    data_to_send[_cnt++]=0;
//    
//    temp = votage;
//    data_to_send[_cnt++]=BYTE1(temp);
//    data_to_send[_cnt++]=BYTE0(temp);
//    temp = current;
//    data_to_send[_cnt++]=BYTE1(temp);
//    data_to_send[_cnt++]=BYTE0(temp);
//    data_to_send[4] = _cnt-5;
//    

//    for(i=0;i<_cnt;i++)
//        sum += data_to_send[i];
//    
//    data_to_send[_cnt++]=sum;
//    
//    ANO_DT_Send_Data(data_to_send, _cnt);
}
void ANO_DT_Send_MotoPWM(u16 m_1,u16 m_2,u16 m_3,u16 m_4,u16 m_5,u16 m_6,u16 m_7,u16 m_8)
{
//    u8 _cnt=0;
//    u8 sum = 0,i;    
//    data_to_send[_cnt++]=0xAA;
//    data_to_send[_cnt++]=0x05;
//		data_to_send[_cnt++]=0xAF;
//    data_to_send[_cnt++]=0x06;
//    data_to_send[_cnt++]=0;
//    
//    data_to_send[_cnt++]=BYTE1(m_1);
//    data_to_send[_cnt++]=BYTE0(m_1);
//    data_to_send[_cnt++]=BYTE1(m_2);
//    data_to_send[_cnt++]=BYTE0(m_2);
//    data_to_send[_cnt++]=BYTE1(m_3);
//    data_to_send[_cnt++]=BYTE0(m_3);
//    data_to_send[_cnt++]=BYTE1(m_4);
//    data_to_send[_cnt++]=BYTE0(m_4);
//		data_to_send[_cnt++]=BYTE1(m_5);
//    data_to_send[_cnt++]=BYTE0(m_5);
//    data_to_send[_cnt++]=BYTE1(m_6);
//    data_to_send[_cnt++]=BYTE0(m_6);
//    data_to_send[_cnt++]=BYTE1(m_7);
//    data_to_send[_cnt++]=BYTE0(m_7);
//    data_to_send[_cnt++]=BYTE1(m_8);
//    data_to_send[_cnt++]=BYTE0(m_8);
//    
//    data_to_send[4] = _cnt-5;
//    

//    for(i=0;i<_cnt;i++)
//        sum += data_to_send[i];
//    
//    data_to_send[_cnt++]=sum;
//    
//    ANO_DT_Send_Data(data_to_send, _cnt);
}



//发送用户数据使用
void ANO_DT_Send_Udata(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z)
{
    u8 _cnt=0;
	u8 sum = 0,i;
    s16 _temp;
    
    data_to_send[_cnt++]=0xAA;
    data_to_send[_cnt++]=0x05;
	data_to_send[_cnt++]=0xAF;
    data_to_send[_cnt++]=0xF1;
    data_to_send[_cnt++]=0;
    
    _temp = a_x;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = a_y;
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = a_z;    
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    
    _temp = g_x;    
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = g_y;    
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = g_z;    
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    
    _temp = m_x;    
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = m_y;    
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    _temp = m_z;    
    data_to_send[_cnt++]=BYTE1(_temp);
    data_to_send[_cnt++]=BYTE0(_temp);
    
    data_to_send[4] = _cnt-5;
    

    for(i=0;i<_cnt;i++)
        sum += data_to_send[i];
    data_to_send[_cnt++] = sum;
    
    ANO_DT_Send_Data(data_to_send, _cnt);
}

