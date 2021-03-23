#ifndef __CAN_RECEIVE
#define __CAN_RECEIVE

#ifdef STM32F4
#include "stm32f4xx_hal.h"
#elif defined STM32F1
#include "stm32f1xx_hal.h"
#endif

#include "can.h"
#include "mytype.h"
#include "stdio.h"
#include "Robo_config.h"
#include "judge.h"
/*CAN发送或是接收的ID*/
typedef enum
{
	//底盘电机ID
	CAN_3508Moto_ALL_ID = 0x200,
	CAN_3508Moto1_ID = 0x201,
	CAN_3508Moto2_ID = 0x202,
	CAN_3508Moto3_ID = 0x203,
	CAN_3508Moto4_ID = 0x204,
	CAN_6020Moto1_ID = 0x205,
	CAN_6020Moto2_ID = 0x206,
	CAN_6020Moto3_ID = 0x207,
	CAN_6020Moto4_ID = 0x208,
	CAN_6020Moto5_ID = 0x209,
	CAN_6020Moto6_ID = 0x20A,
	CAN_6020Moto7_ID = 0x20B,
	CAN_DriverPower_ID = 0x80,
	/* can id
				1------2
				   |
					 |
					 |
				3------4
	*/
}CAN_Message_ID;

#define FILTER_BUF_LEN		5
/*接收到的电机的参数结构体*/
typedef struct{
	int16_t	 	speed_rpm;
	int16_t  	real_current;
	int16_t  	given_current;
	uint8_t  	hall;
	uint16_t 	angle;				//abs angle range:[0,8191]
	uint16_t 	last_angle;			//abs angle range:[0,8191]
	uint16_t	offset_angle;
	int32_t		round_cnt;
	int32_t		total_angle;
	u8			  buf_idx;
	u16			  angle_buf[FILTER_BUF_LEN];
	u16			  fited_angle;
	u32			  msg_cnt;
}moto_measure_t;


/* Extern  ------------------------------------------------------------------*/
extern moto_measure_t  moto_chassis,moto_base[DJnumber];
extern moto_measure_t  moto_yaw[2],moto_pit[2],moto_revolve[2],moto_info;
extern float real_current_from_judgesys; //unit :mA
extern float dynamic_limit_current;	//unit :mA,;	//from judge_sys
extern float ZGyroModuleAngle,yaw_zgyro_angle;
extern CAN_TxHeaderTypeDef   TxHeader;
extern CAN_RxHeaderTypeDef   RxHeader;


void my_can_filter_init_recv_all(CAN_HandleTypeDef* _hcan);
void get_moto_measure(moto_measure_t *ptr, uint8_t *Data);
void set_moto_offset(void);

void set_moto_current_all(CAN_HandleTypeDef* hcan,s16 mark, s16 iq1, s16 iq2, s16 iq3, s16 iq4);
#endif
