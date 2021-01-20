#include "Task_Chassis.h"
#include "user_lib.h"

int32_t set_chassis; //滑轨速度目标值
TickType_t current_time;
uint32_t timeFlag = 0;
int16_t count_offset;
int16_t current_count;
uint8_t chassis_dir; //0-静止 	1-向左 		2-向右
uint8_t flag_auto_aim = 0;
uint8_t dir_change_flag = 0;
uint8_t max_count = MAX_COUNT_MOVE;

ramp_function_source_t ramp_chassis_spd1, ramp_chassis_spd2;
void Task_Chassis(void *argument)
{
  /* USER CODE BEGIN Header_StartMotorTask */
  /* USER CODE BEGIN StartMotorTask */

  PID_struct_init(&pid_chassis, POSITION_PID, 30000, 1000,
                  PID[1].Kp, PID[1].Ki, PID[1].Kd); //4 motos angular rate closeloop.

  ramp_init(&ramp_chassis_spd1, 1.0, 2000, -2000);
  ramp_init(&ramp_chassis_spd2, -1.0, 2000, -2000);
	
  /* Infinite loop */
  for (;;)
  {
    //获取底盘电机期望速度
    GET_Chassis_Dir_Spd();
		getCurrentChassisDir();
    pid_calc(&pid_chassis, moto_chassis.speed_rpm, set_chassis);
    set_moto_current_all(&hcan1, 2,
                         pid_chassis.pos_out,               //1
                         pid_revolve[GIMBAL_Below].delta_out, //2  下拨盘
                         0,                                 //3
                         0);                                //4
    osDelay(3);
  }
}

void GET_Chassis_Dir_Spd()
{
  switch (roboStatus.control_mode)
  {
  case Remote_mode:
    set_chassis = 2000 * rc.CONTROLLER.ch3 / 660.0f;
		flag_auto_aim = 0;
    break;
  case AutoAim_mode:
		if(flag_auto_aim == 0)
		{
			flag_auto_aim = 1;
			set_chassis = 2000;
			count_offset = moto_chassis.round_cnt;
		}
		current_count = moto_chassis.round_cnt - count_offset;

		//达到最大圈数切换方向
		if(abs(current_count) >= max_count)
		{
				if(set_chassis == 2000)
					set_chassis = -2000;
				else if(set_chassis == -2000)
					set_chassis = 2000;
				count_offset = moto_chassis.round_cnt;
				current_count = moto_chassis.round_cnt - count_offset;
		}			
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

void getCurrentChassisDir()
{
	if(current_count > 0) chassis_dir = CHASSIS_DIR_LEFT;
	else if(current_count < 0) chassis_dir = CHASSIS_DIR_RIGHT;
	else chassis_dir = CHASSIS_DIR_NO_MOVE;
}

void ChangeChassisDir()
{
	if(chassis_dir == CHASSIS_DIR_LEFT)
		chassis_dir = CHASSIS_DIR_RIGHT;
	else if(chassis_dir == CHASSIS_DIR_RIGHT)
		chassis_dir = CHASSIS_DIR_LEFT;
}
