#ifndef __ROBOT__CONFIG_H
#define __ROBOT__CONFIG_H


//操作手个人设置
#define X_Sensitivity 0.25 //输入灵敏度
#define Y_Sensitivity 0.25

#define Mouse_yaw_coefficient_CH 0.1   //鼠标扩大倍数  跟随板
#define Mouse_pitch_coefficient_CH 1 //鼠标扩大倍数  非陀螺仪版

#define Vision_yaw_coefficient_CH 0.1   //鼠标扩大倍数  跟随板
#define Vision_pitch_coefficient_CH 1 //鼠标扩大倍数  非陀螺仪版

#define Mouse_yaw_IMU_coefficient_CH  1  //鼠标扩大倍数
#define Mouse_pitch_IMU_coefficient_CH 1 //鼠标扩大倍数 陀螺仪版
#define AUTO_AIM_LOCK_Chassis 3.0

//未知 需要补充	--NATO
#define YAW_FEEDFORWARD 16.0         //锁死模式转向系数
#define Direction_W_Offset_Config (5.0f) //跟随模式速度系数
#define Slow_Down_Cow_Boy 1           //竖直方向灵敏度
#define DJnumber  10                    //1底盘 4云台（上下） 2拨盘（上下）
//视觉接收设置
#define USART_LEN 14 //串口接收长度

//电机offset设置
#define PIT_HIGH_POS 3300
#define PIT_MID_POS 4000
#define PIT_LOW_POS 4300

#define PIT_HIGH_DIS 1125 //PIT_HIGH_POS - PIT_MID_POS
#define PIT_LOW_DIS 200

#define YAW_MID_POS 2777

#define IMU_PIT 200
#define IMU_YAW 219

#define CheatPTZ -62
#define CheatcChassis 3160

//此后为20200711新加

#define Stuck_Revol_PIDTerm 4000 //PID输出大于这个数则认为有可能卡弹
#define Send_Revol_Speed 2000

//电机Ecd->angle_rad
#define Motor_Ecd_to_Rad 0.000766990394f //      2*  PI  /8192
#define AngleToRad(a) (a*2*3.1415926f/360.0)

#endif
