//#include "Motor_task.h"

//void Motor_task()
//{
//	uint8_t CheatMode = 0;																//自瞄模式-------欢乐时光就要弿始辣?
//	uint16_t Shoot_Speed = 1095;
//	
//	uint8_t poseedg_z = 0;
//	uint8_t poseedg_c = 0;
//	
//	int32_t set_send;																			//拨盘送蛋
//	int32_t set_spd[4];																		//0---3  底盘代码   4--pitch   5--yaw 6--拨盘
//	int32_t set_yaw;
//	int32_t set_pit;
//	

//	//3508电机PID初始匿 
//	for(int i=0; i<4; i++)						//调参有问颿? 
//	{
//		PID_struct_init(&pid_spd[i], POSITION_PID, 20000, 20000,
//							1.5f,	0.4f,	0.0f	);  //4 motos angular rate closeloop.
//	}
//	PID_struct_init(&pid_pit, POSITION_PID, 20000, 1000,
//						40.0f,	1.0f,	0.0f	);  //竖直电机
//	PID_struct_init(&pid_yaw, POSITION_PID, 20000, 1000,		//按理来说应该电机[4]失能了？
//						20.0f,	2.0f,	20.0f	);  //yaw电机			40.0f,	2.0f,	40.0f	);		
//	PID_struct_init(&pid_send, POSITION_PID, 20000, 20000,
//						1.5f,	0.1f,	0.0f	);  //拨盘电机	
//	while(1)
//	{
//		//更新陿螺仪 
////		mpu_get_data();
////		imu_ahrs_update();
////		imu_attitude_update(); 
//		
//		if(keyboard.X)				//x键切换自瞄 
//		{
//			if(!CheatMode)
//				CheatMode=1;
//			else
//				CheatMode=0;
//		}

//		if(rc.mouse.press_left)
//					set_send = Send_Bullet_Speed;
//				else
//					set_send = 0;

//		if(rc.mouse.press_right)
//			GUN_MOTOR_SHOOT(Shoot_Speed);
//		else
//			GUN_MOTOR_SHOOT(1000);
//		if(keyboard.Z)
//			if(poseedg_z == 0)			//上升沿检测  
//			{
//				poseedg_z = 1;
//				Shoot_Speed-=20;
//			}
//		if(keyboard.C)
//			if(poseedg_c == 0)
//			{
//				poseedg_c = 1;
//				Shoot_Speed+=20;
//			}
//		if(Shoot_Speed<=1080)
//				Shoot_Speed=1080;
//			else if (Shoot_Speed>=1500)
//				Shoot_Speed=1500;
//		if(!keyboard.Z)
//			poseedg_z = 0;
//		if(!keyboard.C)
//			poseedg_c = 0;

//		GET_Conrtol_Model(&rc,&dbus_command);
///* 
//				/	Romote_Control							/	FOLLOW_PTZ 
//		sw1 	-	DISABLE							sw2 	-	ROLL 
//				\	KeyBoard_Control 						\	LOCK_UP	
//*/
//		//获取底盘电机期望速度
//		GET_Chassis_Dir_xyw(&rc,&dbus_command, 70, 70, 70,ADD_AccXYW);		//NOT_ADD_AccXYW不平滑
//		SpeedDistribute(&dbus_command);										//vx->Speed_Want
//		
//		if(dbus_command.Chssis_Enable_flag)			//原始输入
//		{
//			set_spd[0] = dbus_command.Chassis_Motor.Motor_A_Speed_Want;
//			set_spd[1] = dbus_command.Chassis_Motor.Motor_B_Speed_Want;
//			set_spd[2] = dbus_command.Chassis_Motor.Motor_C_Speed_Want;
//			set_spd[3] = dbus_command.Chassis_Motor.Motor_D_Speed_Want;
//			
//			set_yaw = rc.mouse.total_x * X_Sensitivity;					//rc输入原始数据*鼠标灵敏度
//			set_pit = rc.mouse.total_y * Y_Sensitivity;					//x,y方向上移动的距离
//		}
//		else
//			set_spd[0] = set_spd[1] = set_spd[2] = set_spd[3]= set_pit = set_yaw = 0;

////云台
///* 
//				/	Romote_Control							/	FOLLOW_PTZ 
//		sw1 	-	DISABLE							sw2 	-	ROLL 
//				\	KeyBoard_Control 						\	LOCK_UP	
//*/
//		if(dbus_command.control_mode == Keyboard_mode)
//		{
//			switch(dbus_command.follow_mode)
//			{
//				case CHASSIS_FOLLOW_PTZ:
//					//跟随模式默认云台朝向不懂
//					//注意跟板载陀螺仪  YAW\ROL\PITCH  的关系
//					//迭代关系没有写
//					pid_calc(&pid_pit, imu.rol, set_pit + imu_temp[0]);
//					pid_calc(&pid_yaw, imu.yaw, set_yaw + imu_temp[1]);
//					break;

//				case ROLL:
//					
//				//这个小陀螺是假的
//					//pid_calc(&pid_spd[4], imu.rol+200, IMU_Config[0]+rc.mouse.total_y*Slow_Down_Cow_Boy);
//					//pid_calc(&pid_spd[5], imu.yaw+200, IMU_Config[1]+rc.mouse.total_x*Slow_Down_Cow_Boy);
//				
////					pid_calc(&pid_spd[4], moto_pit.speed_rpm, imu.wyd+rc.mouse.y*Slow_Down_Cow_Boy);
////					pid_calc(&pid_spd[5], moto_pit.speed_rpm, imu.wxd+rc.mouse.x*Slow_Down_Cow_Boy);
//					pid_calc(&pid_pit, moto_pit.angle-PIT_MID_POS, set_pit);
//					pid_calc(&pid_yaw, moto_yaw.speed_rpm, CheatPTZ+rc.mouse.x);
//					break;
//				case CHASSIS_NOT_FOLLOW_PTZ:
//					//底盘云台锁定
//					//pid_calc(&pid_yaw, moto_yaw.total_angle - 8192*abs(moto_yaw.round_cnt), 0);
//					//计算total角，并令角度处于0-8192内。
//				
//					//跟can.receive联动。我觉得这个模式好像没啥用。。。
//					pid_calc(&pid_pit, moto_pit.angle-PIT_MID_POS, set_pit);
//					pid_calc(&pid_yaw, moto_yaw.angle, YAW_MID_POS);
//					break;
//				default : break;
//			}
//		}
//		
//		pid_calc(&pid_send, moto_send.speed_rpm, set_send);	//拨盘

//		for(int i=0; i<4; i++)
//			pid_calc(&pid_spd[i], moto_chassis[i].speed_rpm, set_spd[i]);

//		set_moto_current_6020(&hcan1, 1,
//								pid_spd[0].pos_out,
//								pid_spd[1].pos_out,
//								pid_spd[2].pos_out,
//								pid_spd[3].pos_out);//lalalahhh

//		set_moto_current_6020(&hcan1, 0,
//								pid_pit.pos_out, 
//								pid_yaw.pos_out,
//								pid_send.pos_out,
//								0);
//	}
//}
//void Motor_task()
//{
//	uint8_t CheatMode = 0;			//自瞄模式-------欢乐时光就要开始辣?
//	uint8_t Active_Stability = 0;

//	uint16_t Shoot_Speed = 1095;
//	
//	uint8_t poseedg_z = 0;
//	uint8_t poseedg_c = 0;

//	for(int i=0; i<4; i++)						//调参有问
//	{
//		PID_struct_init(&pid_spd[i], POSITION_PID, 20000, 20000,
//							1.5f,	0.4f,	0.0f	);  //4 motos angular rate closeloop.
//	}
//	PID_struct_init(&pid_pit, POSITION_PID, 20000, 1000,
//						40.0f,	1.0f,	0.0f	);  //竖直电机
//	PID_struct_init(&pid_yaw, POSITION_PID, 20000, 1000,		//按理来说应该电机[4]失能了？
//						20.0f,	2.0f,	20.0f	);  //yaw电机			40.0f,	2.0f,	40.0f	);		
//	PID_struct_init(&pid_send, POSITION_PID, 20000, 20000,
//						1.5f,	0.1f,	0.0f	);  //拨盘电机	
//	while(1)
//	{
//		//更新陀螺仪  
//		
//		//HAL_Delay(5);				//这个Delay干嘛的？
//		
//		if(keyboard.X)				//x键切换自稳
//		{
//			if(!CheatMode)                                
//						CheatMode=1;                              
//			else                                          
//				CheatMode=0;
//			Active_Stability=0;
//		}
//		if(keyboard.Q)				//Q键切换自瞄
//		{
//			CheatMode=0;
//			if(!Active_Stability)
//				Active_Stability=1;
//			else
//				Active_Stability=0;
//		}

//		
//		if(rc.mouse.press_left)
//					set_send = Send_Bullet_Speed;
//				else
//					set_send = 0;

//		if(rc.mouse.press_right)
//			GUN_MOTOR_SHOOT(Shoot_Speed);
////		GUN_MOTOR_SHOOT(1000*PK);
//		else
//			GUN_MOTOR_SHOOT(1000*PK);
////		GUN_MOTOR_SHOOT(2000*PK);
//		if(keyboard.Z)
//			if(keyboard.Z && poseedg_z == 0)			//上升沿检?
//			{
//				poseedg_z = 1;
//				Shoot_Speed-=20;
//			}
//		if(keyboard.C)
//			if(keyboard.C && poseedg_c == 0)
//			{
//				poseedg_c = 1;
//				Shoot_Speed+=20;
//			}
//		if(Shoot_Speed<=1080)
//				Shoot_Speed=1080;
//			else if (Shoot_Speed>=1500)
//				Shoot_Speed=1500;
//		if(!keyboard.Z)
//			poseedg_z = 0;
//		if(!keyboard.C)
//			poseedg_c = 0;

//		GET_Conrtol_Model(&rc,&dbus_command);
///* 
//				/	Romote_Control							/	FOLLOW_PTZ
//		sw1 -	DISABLE									sw2 -	ROLL
//				\	KeyBoard_Control 						\	LOCK_UP	
//*/
//		//获取底盘电机期望速度
//		GET_Chassis_Dir_xyw(&rc,&dbus_command,1,1,1,NOT_ADD_AccXYW);		//NOT_ADD_AccXYW不平稳  
//		SpeedDistribute(&dbus_command);																	//vx->Speed_Want
//		
//		if(dbus_command.Chssis_Enable_flag)			//原始输入
//		{
//			set_spd[0] = dbus_command.Chassis_Motor.Motor_A_Speed_Want;
//			set_spd[1] = dbus_command.Chassis_Motor.Motor_B_Speed_Want;
//			set_spd[2] = dbus_command.Chassis_Motor.Motor_C_Speed_Want;
//			set_spd[3] = dbus_command.Chassis_Motor.Motor_D_Speed_Want;

//			set_pit = rc.mouse.total_y;					//PITCH轴为绝对位置?
//			
//			set_yaw = rc.mouse.x * Mouse_yaw_coefficient_CH;			//调整权重调整鼠标速度
//			
//				
//		}
//		else
//			set_spd[0] = set_spd[1] = set_spd[2] = set_spd[3]= set_pit = set_yaw = 0;

////云台计算
//		if(dbus_command.control_mode == Keyboard_mode)
//		{
//		pid_calc(&pid_pit, moto_pit.angle-PIT_MID_POS, set_pit);

//			switch(dbus_command.follow_mode)
//			{
//				case CHASSIS_FOLLOW_PTZ:

////					if(CheatMode)
////					{
////						pid_calc(&pid_pit, moto_pit.total_angle, cheat_data.pit);
////						pid_calc(&pid_yaw, moto_yaw.total_angle, cheat_data.yaw);
////					}
////					
////					else if(Active_Stability)																//朝向不动
////					{

//// 
////					}
////					else
//					{			//一定要通过total_angle进行位置闭环
//						pid_calc(&pid_yaw, moto_yaw.speed_rpm, set_yaw);
//					}
//					break;
//				case CHASSIS_NOT_FOLLOW_PTZ:
//					pid_calc(&pid_yaw, moto_yaw.total_angle, 0);
//					break;
//				case ROLL:
//					pid_calc(&pid_yaw, moto_yaw.speed_rpm, CheatPTZ+rc.mouse.x);
//					break;
//				default : break;
//			}
//			
//		}
//		pid_calc(&pid_send, moto_send.speed_rpm, set_send);		
//		for(int i=0; i<4; i++)
//		{
//			pid_calc(&pid_spd[i], moto_chassis[i].speed_rpm, set_spd[i]);
//		}

//		set_moto_current_all(&hcan1, 2,
//								pid_spd[0].pos_out,    //1
//								pid_spd[1].pos_out,    //2
//             		pid_spd[2].pos_out,    //3
//								pid_spd[3].pos_out);   //4

//		set_moto_current_all(&hcan1, 0,
//								pid_pit.pos_out,
//						  	pid_yaw.pos_out,       //5   //6
//								pid_send.pos_out,      //7
//								0);	
//		
//		HAL_Delay(3);
//	}
//}



