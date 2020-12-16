#include "Task_Chassis.h"
#include "user_lib.h"

int32_t set_chassis; //滑轨速度目标值


void Task_Chassis(void* argument)
{
    /* USER CODE BEGIN Header_StartMotorTask */
      /* USER CODE BEGIN StartMotorTask */
		
		PID_struct_init(&pid_chassis, DELTA_PID, 30000, 1000,
				PID[1].Kp, PID[1].Ki, PID[1].Kd); //4 motos angular rate closeloop.
    /* Infinite loop */
    for (;;)
    {
        //获取底盘电机期望速度
				GET_Chassis_Dir_Spd();
				pid_calc(&pid_chassis, moto_chassis.speed_rpm, set_chassis);				
//        set_moto_current_all(&hcan1, 2,
//            pid_spd[0].delta_out,  //1
//            pid_spd[1].delta_out,  //2
//            pid_spd[2].delta_out,  //3
//            pid_spd[3].delta_out); //4
        osDelay(3);
    }
}


void GET_Chassis_Dir_Spd()
{
    
    switch (roboStatus.control_mode)
    {
    case Remote_mode:
        set_chassis=rc.CONTROLLER.ch1;
        break;
    case AutoAim_mode:
       set_chassis=0;
        break;
    case Disable_mode:
				set_chassis=0;
			return;
    }
		if(set_chassis > V_max_ch)
			set_chassis = V_max_ch;
		else if(set_chassis < -V_max_ch)
			set_chassis = -V_max_ch;
}



