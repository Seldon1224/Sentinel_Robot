#include "judge.h"

/*****************系统数据定义**********************/
ext_supply_projectile_action_t		    SupplyProjectileAction;	  //0x0102 补给站       *****
ext_game_robot_state_t			  	      GameRobotStat;				    //0x0201 机器人状态   *****
ext_power_heat_data_t		  		        PowerHeatData;				    //0x0202 功率热量     *****
ext_robot_hurt_t				              RobotHurt;					      //0x0206 伤害类型     *****
ext_shoot_data_t					            ShootData;					      //0x0207 射击信息     *****     
ext_rfid_status_t                     RFIDState;                //0x0209 RFID状态     *****
/****************************************************/


/*****************检测射速上限改变**********************/
void speed_limit_change(uint8_t speed_limit)
{
	switch(speed_limit){
//		case 15:	 Shoot_Speed = 950;  break;//没测试
//		case 18:   Shoot_Speed = 1000; break;//没测试
//		case 22:   Shoot_Speed = 1020; break;//没测试
//		case 30:   Shoot_Speed = 1040; break;//没测试
	}
}

/*****************检测热量上限改变**********************/
void cooling_limit_change(uint16_t cooling_limit)
{
	switch(cooling_limit){

	
	}
}

/*****************检测底盘功率上限改变**********************/
void max_chassis_power_change(uint16_t max_chassis_power)
{
	switch(max_chassis_power){
		case 50:	  chassis_speed_rate = 0.871621;  break;//没测试
		case 60:    chassis_speed_rate = 1.0;       break;//没测试
		case 70:    chassis_speed_rate = 1.2;       break;//没测试
		case 100:   chassis_speed_rate = 1.4;       break;//没测试
	}
}

/*****************补给站动作标识**********************/
void get_supply_projectile_action(ext_supply_projectile_action_t *ptr, uint8_t *Data)
{
		ptr->supply_projectile_id   = Data[0];
	 	ptr->supply_robot_id        = Data[1];
		ptr->supply_projectile_step = Data[2];
  	ptr->supply_projectile_num  = Data[3];
}

/*****************比赛机器人状态数据：分为1，2，3部分**********************/
void get_game_robot_state_one(ext_game_robot_state_t *ptr, uint8_t *Data)
{
		ptr->robot_id    = Data[0];
	 	ptr->robot_level = Data[1];
		ptr->remain_HP   = (uint16_t)(Data[2]<<8 | Data[3]);
  	ptr->max_HP      = (uint16_t)(Data[4]<<8 | Data[5]);
	  ptr->shooter_heat0_cooling_rate   = (uint16_t)(Data[6]<<8 | Data[7]);
}

void get_game_robot_state_two(ext_game_robot_state_t *ptr, uint8_t *Data)
{
		if(ptr->shooter_heat0_cooling_limit!=(uint16_t)(Data[0]<<8 | Data[1])){//检测射速上限
		cooling_limit_change((uint16_t)(Data[0]<<8 | Data[1]));
		}
		ptr->shooter_heat0_cooling_limit  = (uint16_t)(Data[0]<<8 | Data[1]);
  	ptr->shooter_heat1_cooling_rate   = (uint16_t)(Data[2]<<8 | Data[3]);
		ptr->shooter_heat1_cooling_limit  = (uint16_t)(Data[4]<<8 | Data[5]);
	  if(ptr->shooter_heat0_speed_limit!=Data[6]){//检测射速上限
		speed_limit_change(Data[6]);
		}
  	ptr->shooter_heat0_speed_limit    = Data[6];
	  ptr->shooter_heat1_speed_limit    = Data[7];
}

void get_game_robot_state_three(ext_game_robot_state_t *ptr, uint8_t *Data)
{
		if(ptr->max_chassis_power!=Data[0]){//检测射速上限
		max_chassis_power_change(Data[0]);
		}
		ptr->max_chassis_power           = Data[0];
	 	ptr->mains_power_gimbal_output   = Data[1];
		ptr->mains_power_chassis_output  = Data[2];
  	ptr->mains_power_shooter_output  = Data[3];
}

/*****************实时功率热量数据：分为1，2部分**********************/
void get_power_heat_data_one(ext_power_heat_data_t *ptr, uint8_t *Data)
{
		ptr->chassis_volt           = (uint16_t)(Data[0]<<8 | Data[1]);
	 	ptr->chassis_current        = (uint16_t)(Data[2]<<8 | Data[3]);
		ptr->chassis_power          = (Data[4]<< 24) + (Data[5]<< 16) + (Data[6] << 8) + (Data[7]);  //没测试，四字节转float
}

void get_power_heat_data_two(ext_power_heat_data_t *ptr, uint8_t *Data)
{
		ptr->chassis_power_buffer   = (uint16_t)(Data[0]<<8 | Data[1]);
	 	ptr->shooter_heat0          = (uint16_t)(Data[2]<<8 | Data[3]);
		ptr->shooter_heat1          = (uint16_t)(Data[4]<<8 | Data[5]);
	 	ptr->mobile_shooter_heat2   = (uint16_t)(Data[6]<<8 | Data[7]);
}

/*****************伤害状态数据**********************/
void get_robot_hurt(ext_robot_hurt_t *ptr, uint8_t *Data)
{
		ptr->armor_id  = Data[0];
	 	ptr->hurt_type = Data[1];
}

/*****************实时射击数据 **********************/
void get_shoot_data(ext_shoot_data_t *ptr, uint8_t *Data)
{
		ptr->bullet_type = Data[0];
	 	ptr->bullet_freq = Data[1];
	  ptr->bullet_speed =(Data[2]<< 24) + (Data[3]<< 16) + (Data[4] << 8) + (Data[5]);  //没测试，四字节转float
}

/*****************机器人 RFID 状态**********************/
void get_rfid_status(ext_rfid_status_t *ptr, uint8_t *Data)
{
		ptr->rfid_status = (Data[0]<< 24) + (Data[1]<< 16) + (Data[2] << 8) + (Data[3]);  //没测试，四字节转float
}
