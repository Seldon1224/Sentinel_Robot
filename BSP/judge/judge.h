#include "main.h"



//#define 	ID_supply_projectile_action_t   	       0x0102,//补给站动作标识        *****
//#define 	ID_game_robot_state_t    			           0x0201,//比赛机器人状态数据    *****
//#define 	ID_power_heat_data_t    			           0x0202,//实时功率热量数据      *****
//#define 	ID_robot_hurt_t					                 0x0206,//伤害状态数据          *****
//#define 	ID_shoot_data_t					                 0x0207,//实时射击数据          *****
//#define   ID_rfid_status_t                         0x0209,//机器人 RFID 状态      *****

typedef enum//为了和电机ID分开，首位变成1,分帧发的第二位参与编号。
{ 
	ID_game_state_t       		             	= 0x1001,//比赛状态数据
	ID_game_result_t 	   				            = 0x1002,//比赛结果数据                   
	ID_game_robot_HP_t                    	= 0x1003,//比赛机器人存活数据
	ID_dart_status_t                        = 0x1004,//人工智能挑战赛加成与惩罚区状态
	ID_ICRA_buff_debuff_zone_status_t       = 0x1005,//飞镖发射状态
	ID_event_data_t  				           	    = 0x1101,//场地事件数据 
	ID_supply_projectile_action_t   	      = 0x1102,//补给站动作标识                 *****
	ID_referee_warning_t 	                  = 0x1104,//裁判警告信息
	ID_dart_remaining_time_t                = 0x1105,//飞镖发射口倒计时
	
	ID_game_robot_state_one_t    			      = 0x1211,//比赛机器人状态数据 1           *****
	ID_game_robot_state_two_t    			      = 0x1221,//比赛机器人状态数据 2           *****
  ID_game_robot_state_three_t    			    = 0x1231,//比赛机器人状态数据 3           *****
	
	ID_power_heat_data_one_t    			      = 0x1212,//实时功率热量数据   1           *****
	ID_power_heat_data_two_t    			      = 0x1222,//实时功率热量数据   2           *****
	
	ID_game_robot_pos_t        	            = 0x1203,//机器人位置数据
	ID_buff_musk_t					                = 0x1204,//机器人增益数据
	ID_aerial_robot_energy_t			          = 0x1205,//空中机器人能量状态数据
	ID_robot_hurt_t					                = 0x1206,//伤害状态数据                   *****
	ID_shoot_data_t					                = 0x1207,//实时射击数据                   *****
	ID_bullet_remaining_t                   = 0x1208,//子弹剩余发射数(空中机器人，哨兵机器人以及 ICRA 机器人主控发送)            
  ID_rfid_status_t                        = 0x1209,//机器人 RFID 状态               *****
	ID_dart_client_cmd_t                    = 0x120A, //飞镖机器人客户端指令数据

} CmdID;

/* ID: 0x0001  Byte:  3    比赛状态数据 */
typedef __packed struct 
{ 
	uint8_t game_type : 4;
	uint8_t game_progress : 4;
	uint16_t stage_remain_time;
} ext_game_state_t; 


/* ID: 0x0002  Byte:  1    比赛结果数据 */
typedef __packed struct 
{ 
	uint8_t winner;
} ext_game_result_t; 


/* ID: 0x0003  Byte:  2    比赛机器人存活数据 */
typedef __packed struct
{
 uint16_t red_1_robot_HP;
 uint16_t red_2_robot_HP;
 uint16_t red_3_robot_HP;
 uint16_t red_4_robot_HP;
 uint16_t red_5_robot_HP;
 uint16_t red_7_robot_HP;
 uint16_t red_outpost_HP;
 uint16_t red_base_HP;
 uint16_t blue_1_robot_HP;
 uint16_t blue_2_robot_HP;
 uint16_t blue_3_robot_HP;
 uint16_t blue_4_robot_HP;
 uint16_t blue_5_robot_HP;
 uint16_t blue_7_robot_HP;
 uint16_t blue_outpost_HP;
 uint16_t blue_base_HP;
} ext_game_robot_HP_t;


/* ID: 0x0004  Byte:  3    飞镖发射状态 */
typedef __packed struct
{
 uint8_t dart_belong;
 uint16_t stage_remaining_time;
} ext_dart_status_t;


/* ID: 0x0005  Byte:  3    . 人工智能挑战赛加成与惩罚区状态(用不到) */
typedef __packed struct
{
 uint8_t F1_zone_status:1;
 uint8_t F1_zone_buff_debuff_status:3;
 uint8_t F2_zone_status:1;
 uint8_t F2_zone_buff_debuff_status:3;
 uint8_t F3_zone_status:1;
 uint8_t F3_zone_buff_debuff_status:3;
 uint8_t F4_zone_status:1;
 uint8_t F4_zone_buff_debuff_status:3;
 uint8_t F5_zone_status:1;
 uint8_t F5_zone_buff_debuff_status:3;
 uint8_t F6_zone_status:1;
 uint8_t F6_zone_buff_debuff_status:3;
}ext_ICRA_buff_debuff_zone_status_t;


/* ID: 0x0101  Byte:  4    场地事件数据 */
typedef __packed struct 
{ 
	uint32_t event_type;
} ext_event_data_t; 


/* ID: 0x0102  Byte:  4    场地补给站动作标识数据 */
typedef __packed struct
{
 uint8_t supply_projectile_id;
 uint8_t supply_robot_id;
 uint8_t supply_projectile_step;
 uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;


/* ID: 0x0104  Byte:  2    裁判警告信息 */
typedef __packed struct
{
 uint8_t level;
 uint8_t foul_robot_id;
} ext_referee_warning_t;


/* ID: 0x0105  Byte:  1    飞镖发射口倒计时 */
typedef __packed struct
{
 uint8_t dart_remaining_time;
} ext_dart_remaining_time_t;


/* ID: 0X0201  Byte: 18    机器人状态数据 */
typedef __packed struct 
{ 
	uint8_t robot_id;   //机器人ID，可用来校验发送
	uint8_t robot_level;  //1一级，2二级，3三级
	uint16_t remain_HP;  //机器人剩余血量
	uint16_t max_HP; //机器人满血量
	uint16_t shooter_heat0_cooling_rate;  //机器人 17mm 子弹热量冷却速度 单位 /s
	uint16_t shooter_heat0_cooling_limit;   // 机器人 17mm 子弹热量上限
	uint16_t shooter_heat1_cooling_rate;   
	uint16_t shooter_heat1_cooling_limit; 
  uint8_t shooter_heat0_speed_limit;
  uint8_t shooter_heat1_speed_limit;
  uint8_t max_chassis_power;  
	uint8_t mains_power_gimbal_output : 1;  
	uint8_t mains_power_chassis_output : 1;  
	uint8_t mains_power_shooter_output : 1; 
} ext_game_robot_state_t; 


/* ID: 0X0202  Byte: 16    实时功率热量数据 */
typedef __packed struct 
{ 
	uint16_t chassis_volt;   
	uint16_t chassis_current;    
	float chassis_power;   //瞬时功率 
	uint16_t chassis_power_buffer;//60焦耳缓冲能量
	uint16_t shooter_heat0;//17mm
	uint16_t shooter_heat1;  
	uint16_t mobile_shooter_heat2;//机动17mm热量
} ext_power_heat_data_t; 


/* ID: 0x0203  Byte: 16    机器人位置数据 */
typedef __packed struct 
{   
	float x;   
	float y;   
	float z;   
	float yaw; 
} ext_game_robot_pos_t; 


/* ID: 0x0204  Byte:  1    机器人增益数据 */
typedef __packed struct 
{ 
	uint8_t power_rune_buff; 
} ext_buff_musk_t; 


/* ID: 0x0205  Byte:  3    空中机器人能量状态数据 */
typedef __packed struct 
{ 
	uint8_t energy_point;
	uint8_t attack_time; 
} aerial_robot_energy_t; 


/* ID: 0x0206  Byte:  1    伤害状态数据 */
typedef __packed struct 
{ 
	uint8_t armor_id : 4; 
	uint8_t hurt_type : 4; 
} ext_robot_hurt_t; 


/* ID: 0x0207  Byte:  6    实时射击数据 */
typedef __packed struct 
{ 
	uint8_t bullet_type;   
	uint8_t bullet_freq;   
	float bullet_speed;  
} ext_shoot_data_t; 

/* ID: 0x0208  Byte:  2    子弹剩余发射数（仅空中和哨兵） */
typedef __packed struct
{
 uint16_t bullet_remaining_num;
} ext_bullet_remaining_t;


/* ID: 0x0209  Byte:  4    机器人 RFID 状态 */
typedef __packed struct
{
 uint32_t rfid_status;
} ext_rfid_status_t;

//ID：0x020A  Byte:  12    飞镖机器人客户端指令数据
typedef __packed struct
{
 uint8_t dart_launch_opening_status;
 uint8_t dart_attack_target;
 uint16_t target_change_time;
 uint8_t first_dart_speed;
 uint8_t second_dart_speed;
 uint8_t third_dart_speed;
 uint8_t fourth_dart_speed;
 uint16_t last_dart_launch_time;
 uint16_t operate_launch_cmd_time;
}ext_dart_client_cmd_t;


/*****************系统数据定义**********************/
extern ext_supply_projectile_action_t		    SupplyProjectileAction;	  //0x0102 补给站       *****
extern ext_game_robot_state_t			  	      GameRobotStat;				    //0x0201 机器人状态   *****
extern ext_power_heat_data_t		  		      PowerHeatData;				    //0x0202 功率热量     *****
extern ext_robot_hurt_t				              RobotHurt;					      //0x0206 伤害类型     *****
extern ext_shoot_data_t					            ShootData;					      //0x0207 射击信息     *****     
extern ext_rfid_status_t                    RFIDState;                //0x0209 RFID状态     *****
extern uint16_t                             Shoot_Speed ;             //当前射速      
extern int16_t                             Send_Bullet_Speed_left;   //左键射频
extern int16_t                             Send_Bullet_Speed_right ; //右键射频
extern float                                chassis_speed_rate;       //底盘运动速度比率
/****************************************************/

/*****************补给站动作标识**********************/
void get_supply_projectile_action(ext_supply_projectile_action_t *ptr, uint8_t *Data);
/*****************比赛机器人状态数据：分为1，2，3部分**********************/
void get_game_robot_state_one(ext_game_robot_state_t *ptr, uint8_t *Data);
void get_game_robot_state_two(ext_game_robot_state_t *ptr, uint8_t *Data);
void get_game_robot_state_three(ext_game_robot_state_t *ptr, uint8_t *Data);
/*****************实时功率热量数据：分为1，2部分**********************/
void get_power_heat_data_one(ext_power_heat_data_t *ptr, uint8_t *Data);
void get_power_heat_data_two(ext_power_heat_data_t *ptr, uint8_t *Data);
/*****************伤害状态数据**********************/
void get_robot_hurt(ext_robot_hurt_t *ptr, uint8_t *Data);
/*****************实时射击数据 **********************/
void get_shoot_data(ext_shoot_data_t *ptr, uint8_t *Data);
/*****************机器人 RFID 状态**********************/
void get_rfid_status(ext_rfid_status_t *ptr, uint8_t *Data);
/*****************检测射速上限改变**********************/
void speed_limit_change(uint8_t speed_limit);
/*****************检测热量上限改变**********************/
void cooling_limit_change(uint16_t cooling_limit);
/*****************检测底盘功率上限改变**********************/
void max_chassis_power_change(uint16_t max_chassis_power);


