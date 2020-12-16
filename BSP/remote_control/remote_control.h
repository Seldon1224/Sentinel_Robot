#ifndef __REMOTE_CONTROL_H__
#define __REMOTE_CONTROL_H__

#include "usart.h"
#include "Task_Chassis.h"
#include "stdlib.h"
#include "math.h"
#include "Robo_config.h"
#include "FreeRTOS.h"
#include "task.h"

#define UART_RX_DMA_SIZE (1024)
#define DBUS_MAX_LEN (50)
#define DBUS_BUFLEN (18)
#define DBUS_HUART huart1 //串口1

//按键判断键值
#define KEY_V 0x4000
#define KEY_C 0x2000
#define KEY_X 0x1000
#define KEY_Z 0x0800
#define KEY_G 0x0400
#define KEY_F 0x0200
#define KEY_R 0x0100
#define KEY_E 0x0080
#define KEY_Q 0x0040
#define KEY_CTRL 0x0020
#define KEY_SHIFT 0x0010
#define KEY_D 0x0008
#define KEY_A 0x0004
#define KEY_S 0x0002
#define KEY_W 0x0001

#define RC_SW_UP 1
#define RC_SW_DOWN 2
#define RC_SW_MID 3

/*	控制模式
    sw1=2 上 遥控器模式
	sw1=1 下 自瞄模式
*/
typedef enum
{
    Disable_mode = 0,
    Remote_mode,
    AutoAim_mode,
} CONTROL_MODE;

//遥控器数据结构体
typedef enum
{
    PRESS_OFF = 0,
    PRESS_ON,
} KEYBOARD_STASUS;

//键盘状态记录结构体
typedef struct
{
    //遥控器通道值
    struct
    {
        int16_t ch1; //  		  		ch4| 					ch2|
        int16_t ch2; //                    |					   |
        int16_t ch3; //		      ch3 -----------	        ch1------------
        int16_t ch4; //        	    	   |				       |
                     //                    |				       |
        uint8_t sw1;
        uint8_t sw2;
    } CONTROLLER;
    /* 
				/	Romote_Control							/	SWING
		sw1 -	    DISABLE								sw2 -	ROLL
				\	AutoAim_Control 						\	LOCK	
*/
    //鼠标
    struct
    {
        int16_t x;
        int16_t y;
        int32_t total_y;
        int32_t total_x;
        uint8_t press_left;
        uint8_t press_right;
    } MOUSE;
    struct
    {
        uint16_t key_code;

        KEYBOARD_STASUS Q;
        KEYBOARD_STASUS W;
        KEYBOARD_STASUS E;
        KEYBOARD_STASUS R;
        KEYBOARD_STASUS A;
        KEYBOARD_STASUS S;
        KEYBOARD_STASUS D;
        KEYBOARD_STASUS F;
        KEYBOARD_STASUS G;
        KEYBOARD_STASUS Z;
        KEYBOARD_STASUS X;
        KEYBOARD_STASUS C;
        KEYBOARD_STASUS V;
        KEYBOARD_STASUS B;
        KEYBOARD_STASUS SHIFT;
        KEYBOARD_STASUS CTRL;

        //判断按键是否按下
        bool LAST_Q;
        bool LAST_W;
        bool LAST_E;
        bool LAST_R;
        bool LAST_A;
        bool LAST_S;
        bool LAST_D;
        bool LAST_F;
        bool LAST_G;
        bool LAST_Z;
        bool LAST_X;
        bool LAST_C;
        bool LAST_V;
        bool LAST_B;
        bool LAST_SHIFT;
        bool LAST_CTRL;

    } KEYBOARD;
} RC_REV;
/*******************************************************************************************
       * 键盘键值:15   14   13   12   11   10   9    8    7    6     5     4     3     2    1					Bit位数
       *          V    C    X	   Z    G    F    R    E    Q   CTRL SHIFT   D     A     S    W
********************************************************************************************/

typedef struct
{
    CONTROL_MODE control_mode; //控制模式
} ROBO_STATUS;

extern RC_REV rc;
extern ROBO_STATUS roboStatus;

void uart_receive_handler(UART_HandleTypeDef *huart);
void dbus_uart_init(void);
void GET_Control_Mode(void);
void keyboard_scan(void);

#endif
