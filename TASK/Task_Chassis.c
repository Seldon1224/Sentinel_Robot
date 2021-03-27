#include "Task_Chassis.h"
#include "user_lib.h"

int32_t set_chassis; //滑轨速度目标值
uint8_t flag_auto_aim = 0;
uint8_t max_count = MAX_COUNT_MOVE;
//
ramp_function_source_t ramp_chassis_spd1, ramp_chassis_spd2;

int switch_state[2] = {0};

void Task_Chassis(void *argument)
{
  /* USER CODE BEGIN Header_StartMotorTask */
  /* USER CODE BEGIN StartMotorTask */

  PID_struct_init(&pid_chassis, POSITION_PID, 30000, 1000,
                  PID[1].Kp, PID[1].Ki, PID[1].Kd); //4 motos angular rate closeloop.

  ramp_init(&ramp_chassis_spd1,  1.0, V_max_ch, -V_max_ch);
  ramp_init(&ramp_chassis_spd2, -1.0, V_max_ch, -V_max_ch);
	
  /* Infinite loop */
  for (;;)
  {
    
		//获取底盘电机期望速度
    GET_Chassis_Dir_Spd();
    pid_calc(&pid_chassis, moto_chassis.speed_rpm, set_chassis);
    set_moto_current_all(&hcan1, 2,
                         pid_chassis.pos_out,                 //1
                         pid_revolve[GIMBAL_Below].delta_out, //2  下拨盘
                         0,                                   //3
                         0);                                  //4
    osDelay(3);
  }
}

void GET_Chassis_Dir_Spd()
{
  switch (roboStatus.control_mode)
  {
  case Remote_mode:
    set_chassis = CHASSIS_SPEED_AUTO_MAX * rc.CONTROLLER.ch3 / 660.0f;
		flag_auto_aim = 0;
    break;
  case AutoAim_mode:
		if(flag_auto_aim == 0)
		{
			flag_auto_aim = 1;
			set_chassis = CHASSIS_SPEED_AUTO_MAX;
		}
		//通过限位开关来修改set_chassis的值（在stm32f4xx_it.c中）
    break;
  case Disable_mode:
    set_chassis = 0;
		flag_auto_aim = 0;
    return;
  }
  if (set_chassis > V_max_ch)
    set_chassis = V_max_ch;
  else if (set_chassis < -V_max_ch)
    set_chassis = -V_max_ch;
}




