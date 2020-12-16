# RM_FreeRTOS
RM_FreeRTOS
## 硬件资源分配
* can1
  > 4个3508  
  > 2个6020  
  > 1个2006  
  > 1个B板(裁判系统数据更新)

* 串口
  > usart1		DBUS  
  > usart3		视觉串口  （注意RX PD9 TX PD8）  
  > usart6		IMU(串口)  
  > uart7     匿名上位机  
  > uart8     Debug串口

## 目前任务：
1. 视觉  
2. Debug串口测试
3. 完成早期版本

## 使用说明
* 上位机使用说明：

  直接在freertos.c中的StartSendData任务中调用VisualDisplay(ch1,ch2,ch3,ch4)，一次显示4个通道的值

* 键位设置：

   Z-扭腰，X-锁死，C-自旋，Q-自瞄/手瞄切换

* 当前CubeMX版本为6.0.1，HAL库版本为1.25.2


## 更新日志
* 10.6 新增两个卡尔曼滤波.c
* 10.29 InfoUpdate任务新增imu_update，imu结构体新增offset，total，last，start_flag，陀螺仪回传数据改为real_imu，云台计算时陀螺仪数据精度取0.5，上位机串口发送处疑似卡死，已注释待检查。
* 11.11 InfoUpdate任务新增keyboard_scan和GET_Control_Mode（原位于rc_callback_handler，shoot任务框架修改，新增按键切换遥控及自瞄模式。
* 11.12 修改底盘和云台代码，无整车，待测试。
* 11.14 测试好上位机pid调节功能，新加param.c文件，主要存放pid参数（后续可以放更多参数进去）
* 11.21 修改imu_update的BUG，rc结构体新增按键状态变量判断（如LAST_Q）,云台底盘模式优化。（扭腰没下地，建议试一下，可能翻车） 
* 12.2  整体架构测试完成
* 12.6 修改自瞄模式，改为右键自瞄，解决切换运动模式云台抖动问题，扭腰模式需优化