#ifndef _ANO_DT_H
#define	_ANO_DT_H
#include "main.h"
#include "pid.h"
#include "param.h"
#define u8 uint8_t
#define u16 uint16_t
#define s16 int16_t
#define s32 int32_t

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

#define MY_UART huart7


typedef struct 
{
		u8 send_version;
		u8 send_status;
		u8 send_senser;
		u8 udata;
		u8 send_pid1;
		u8 send_pid2;
		u8 send_pid3;
		u8 send_pid4;
		u8 send_pid5;
		u8 send_pid6;
		u8 send_pid7;
		u8 send_offset;
		u8 send_motopwm;
		u8 send_power;
}dt_flag_t;

extern dt_flag_t f;		//定义标志位结构体，存储各个参数的标志位
//外部调用函数
void ANO_DT_Display(s16 user_1,s16 user_2,s16 user_3,s16 user_4,s16 user_5,s16 user_6,s16 user_7,s16 user_8,s16 user_9);
void ANO_DT_Data_Exchange(void);	//核心数据交换函数，每1ms调用一次（最快），外部只需要调用该函数即可
void ANO_DT_Data_Receive_Prepare(u8 data);

//内部调用函数
void ANO_DT_Data_Receive_Anl(u8 *data_buf,u8 num);
void ANO_DT_Send_Version(u8 hardware_type, u16 hardware_ver,u16 software_ver,u16 protocol_ver,u16 bootloader_ver);
void ANO_DT_Send_Status(float angle_rol, float angle_pit, float angle_yaw, s32 alt, u8 fly_model, u8 armed);
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z);
void ANO_DT_Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6);
void ANO_DT_Send_Power(u16 votage, u16 current);
void ANO_DT_Send_MotoPWM(u16 m_1,u16 m_2,u16 m_3,u16 m_4,u16 m_5,u16 m_6,u16 m_7,u16 m_8);
void ANO_DT_Send_Udata(s16 a_x,s16 a_y,s16 a_z,s16 g_x,s16 g_y,s16 g_z,s16 m_x,s16 m_y,s16 m_z);
void ANO_DT_Check(u8 num);
void ANO_DT_Send_Data(u8 *dataToSend , u8 length);
void ANO_DT_Send_PID(uint8_t num,float p);

#endif

