#include "remote_control.h"


//#define abs(x) ((x > 0) ? (x) : (-x))

#define IF_KEY_PRESSED_W (keyboard.W == PRESS_ON) ? (1) : (0)
#define IF_KEY_PRESSED_S (keyboard.S == PRESS_ON) ? (1) : (0)
#define IF_KEY_PRESSED_A (keyboard.A == PRESS_ON) ? (1) : (0)
#define IF_KEY_PRESSED_D (keyboard.D == PRESS_ON) ? (1) : (0)

#define TIME_DEC_NORMAL 500 //键盘斜坡,越大减小速度越快(一般要比INC大一点,这样松开键盘能更快为0,太大则会造成底盘停下来的时候跳跃)

uint8_t dbus_buf[DBUS_BUFLEN];
ROBO_STATUS roboStatus;
RC_REV rc;

float chassis_speed_rate = 0.871621;

//将buff转化为rc各通道值
void rc_callback_handler(RC_REV *rc, uint8_t *buff)
{
    //上次键盘赋值 用于消除抖动
    //last_rc=*rc;

    //加上错误判断
    uint8_t error = (buff[5] >> 4) & 0x0003;
    if (error < 1 || error > 3)
    {
    }
    else
    {
        //遥控器赋值
        rc->CONTROLLER.ch1 = (buff[0] | buff[1] << 8) & 0x07FF;
        rc->CONTROLLER.ch1 -= 1024;
        rc->CONTROLLER.ch2 = (buff[1] >> 3 | buff[2] << 5) & 0x07FF;
        rc->CONTROLLER.ch2 -= 1024;
        rc->CONTROLLER.ch3 = (buff[2] >> 6 | buff[3] << 2 | buff[4] << 10) & 0x07FF;
        rc->CONTROLLER.ch3 -= 1024;
        rc->CONTROLLER.ch4 = (buff[4] >> 1 | buff[5] << 7) & 0x07FF;
        rc->CONTROLLER.ch4 -= 1024;
        rc->CONTROLLER.sw1 = ((buff[5] >> 4) & 0x000C) >> 2;
        rc->CONTROLLER.sw2 = (buff[5] >> 4) & 0x0003;

        if ((abs(rc->CONTROLLER.ch1) > 660) || //不明白干吗用的
				   	(abs(rc->CONTROLLER.ch2) > 660) || 
            (abs(rc->CONTROLLER.ch3) > 660) ||
            (abs(rc->CONTROLLER.ch4) > 660))
        {
            memset(rc, 0, sizeof(RC_REV));
        }
        //消除摇杆漂移
        if ((abs(rc->CONTROLLER.ch1) < 5))
        {
            rc->CONTROLLER.ch1 = 0;
        }
				if((abs(rc->CONTROLLER.ch2) < 5)){
				    rc->CONTROLLER.ch2 = 0;
				}
        //鼠标赋值
        rc->MOUSE.x = -(buff[6] | (buff[7] << 8));
        rc->MOUSE.y = buff[8] | (buff[9] << 8);
        rc->MOUSE.total_x += rc->MOUSE.x/100;
        rc->MOUSE.total_y += rc->MOUSE.y/100;

        //云台限位
//        if (-(rc->MOUSE.total_y) >= PIT_HIGH_DIS)
//            rc->MOUSE.total_y = -PIT_HIGH_DIS;
//        if (rc->MOUSE.total_y >= PIT_LOW_DIS)
//            rc->MOUSE.total_y = PIT_LOW_DIS;
        //鼠标赋值
        rc->MOUSE.press_left = buff[12];
        rc->MOUSE.press_right = buff[13];
        rc->KEYBOARD.key_code = buff[14] | buff[15] << 8;
        
    }
}

//旋转时降低xy方向速度
//返回值是降低比率




//对遥控得到的数据，转化为车身xyw三个方向上
//float accx,float accy, float accw为三个方向上的加速度，越小起步越慢
//ADD_AccXYW_ENABLE参数，1为平滑处理 0是不进行平滑处理

//当前底盘使能状态

//读取获取控制模式
void GET_Control_Mode()
{
    //控制模式
    if (rc.CONTROLLER.sw1 == 1)
        roboStatus.control_mode = Remote_mode;
    else if (rc.CONTROLLER.sw1 == 2)
        roboStatus.control_mode = AutoAim_mode;
    else
        roboStatus.control_mode = Disable_mode; //电机停止
}

//键盘扫描

//返回dma数据剩余大小
uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream)
{
    /* Return the number of remaining data units for DMAy Streamx */
    return ((uint16_t)(dma_stream->NDTR));
}

void keyboard_scan()
{
    /*W*/
    if (rc.KEYBOARD.key_code & 0x0001)
        rc.KEYBOARD.W = PRESS_ON;
    else
        rc.KEYBOARD.W = PRESS_OFF;

    if (rc.KEYBOARD.key_code & 0x0002)
        rc.KEYBOARD.S = PRESS_ON;
    else
        rc.KEYBOARD.S = PRESS_OFF;

    if (rc.KEYBOARD.key_code & 0x0004)
        rc.KEYBOARD.A = PRESS_ON;
    else
        rc.KEYBOARD.A = PRESS_OFF;

    if (rc.KEYBOARD.key_code & 0x0008)
        rc.KEYBOARD.D = PRESS_ON;
    else
        rc.KEYBOARD.D = PRESS_OFF;

    if (rc.KEYBOARD.key_code & 0x0010)
        rc.KEYBOARD.SHIFT = PRESS_ON;
    else
        rc.KEYBOARD.SHIFT = PRESS_OFF;

    if (rc.KEYBOARD.key_code & 0x0020)
        rc.KEYBOARD.CTRL = PRESS_ON;
    else
        rc.KEYBOARD.CTRL = PRESS_OFF;

    if (rc.KEYBOARD.key_code & 0x0040)
        rc.KEYBOARD.Q = PRESS_ON;
    else
        rc.KEYBOARD.Q = PRESS_OFF;

    if (rc.KEYBOARD.key_code & 0x0080)
        rc.KEYBOARD.E = PRESS_ON;
    else
        rc.KEYBOARD.E = PRESS_OFF;

    if (rc.KEYBOARD.key_code & 0x0100)
        rc.KEYBOARD.R = PRESS_ON;
    else
        rc.KEYBOARD.R = PRESS_OFF;

    if (rc.KEYBOARD.key_code & 0x0200)
        rc.KEYBOARD.F = PRESS_ON;
    else
        rc.KEYBOARD.F = PRESS_OFF;

    if (rc.KEYBOARD.key_code & 0x0400)
        rc.KEYBOARD.G = PRESS_ON;
    else
        rc.KEYBOARD.G = PRESS_OFF;

    if (rc.KEYBOARD.key_code & 0x0800)
        rc.KEYBOARD.Z = PRESS_ON;
    else
        rc.KEYBOARD.Z = PRESS_OFF;

    if (rc.KEYBOARD.key_code & 0x1000)
        rc.KEYBOARD.X = PRESS_ON;
    else
        rc.KEYBOARD.X = PRESS_OFF;

    if (rc.KEYBOARD.key_code & 0x2000)
        rc.KEYBOARD.C = PRESS_ON;
    else
        rc.KEYBOARD.C = PRESS_OFF;

    if (rc.KEYBOARD.key_code & 0x4000)
        rc.KEYBOARD.V = PRESS_ON;
    else
        rc.KEYBOARD.V = PRESS_OFF;

    if (rc.KEYBOARD.key_code & 0x8000)
        rc.KEYBOARD.B = PRESS_ON;
    else
        rc.KEYBOARD.B = PRESS_OFF;
}

//串口接收回调函数
static void uart_rx_idle_callback(UART_HandleTypeDef *huart)
{
    //taskENTER_CRITICAL();
    /* clear idle it flag avoid idle interrupt all the time */
    __HAL_UART_CLEAR_IDLEFLAG(huart);

    /* handle received data in idle interrupt */
    if (huart == &DBUS_HUART)
    {
        /* clear DMA transfer complete flag */
        __HAL_DMA_DISABLE(huart->hdmarx);

        /* handle dbus data dbus_buf from DMA */
        if ((DBUS_MAX_LEN - dma_current_data_counter(huart->hdmarx->Instance)) == DBUS_BUFLEN)
        {
            rc_callback_handler(&rc, dbus_buf);
        }
        /* restart dma transmission */
        __HAL_DMA_SET_COUNTER(huart->hdmarx, DBUS_MAX_LEN);
        __HAL_DMA_ENABLE(huart->hdmarx);
    }
    //taskEXIT_CRITICAL();
}

/**
  * @brief      callback this function when uart interrupt 
  * @param[in]  huart: uart IRQHandler id
  * @retval  
 */
void uart_receive_handler(UART_HandleTypeDef *huart)
{
    if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) &&
        __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
    {
        uart_rx_idle_callback(huart);
    }
}

//初始化dbus
void dbus_uart_init(void)
{
    /* open uart idle it */
    __HAL_UART_CLEAR_IDLEFLAG(&DBUS_HUART);
    __HAL_UART_ENABLE_IT(&DBUS_HUART, UART_IT_IDLE);
    uint32_t tmp1 = 0;
    //获取当前状态
    tmp1 = DBUS_HUART.RxState;
    if (tmp1 == HAL_UART_STATE_READY)
    {
        DBUS_HUART.pRxBuffPtr = dbus_buf;
        DBUS_HUART.RxXferSize = DBUS_MAX_LEN;
        DBUS_HUART.ErrorCode = HAL_UART_ERROR_NONE;
        /* Enable the DMA Stream */
        HAL_DMA_Start(DBUS_HUART.hdmarx, (uint32_t)&DBUS_HUART.Instance->DR, (uint32_t)dbus_buf, DBUS_MAX_LEN);
        /* 
		 * Enable the DMA transfer for the receiver request by setting the DMAR bit
		 * in the UART CR3 register 
		 */
        SET_BIT(DBUS_HUART.Instance->CR3, USART_CR3_DMAR);
    }
}
