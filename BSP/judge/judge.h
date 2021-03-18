#ifndef _JUDGE_H
#define _JUDGE_H

#include "main.h"
#include "stdbool.h"
#include "string.h"
#include "can.h"
#include "./judge/CRC.h"
#include "./NR_Printf/NR_Printf.h"

#define    LEN_HEADER    5        //帧头长
#define    LEN_CMDID     2        //命令码长度
#define    LEN_TAIL      2	      //帧尾CRC16
#define    FALSE    0
#define    TRUE     1

//起始字节,协议固定为0xA5
#define    JUDGE_FRAME_HEADER         (0xA5)

//CAN
#define hcan1 hcan1

typedef enum 
{
	FRAME_HEADER         = 0,
	CMD_ID               = 5,
	DATA                 = 7,
	
}JudgeFrameOffset;

//5字节帧头,偏移位置
typedef enum
{
	SOF          = 0,//起始位
	DATA_LENGTH  = 1,//帧内数据长度,根据这个来获取数据长度
	SEQ          = 3,//包序号
	CRC8         = 4 //CRC8
	
}FrameHeaderOffset;

/***************命令码ID********************/

/* 

	ID: 0x0001  Byte:  3    比赛状态数据       			发送频率 1Hz      
	ID: 0x0002  Byte:  1    比赛结果数据         		比赛结束后发送      
	ID: 0x0003  Byte:  2    比赛机器人存活数据   		1Hz发送  
	ID: 0x0101  Byte:  4    场地事件数据   				事件改变后发送
	ID: 0x0102  Byte:  3    场地补给站动作标识数据    	动作改变后发送 
	ID: 0X0103  Byte:  2    场地补给站预约子弹数据      参赛队发送，10Hz 
	ID: 0X0201  Byte: 15    机器人状态数据        		10Hz
	ID: 0X0202  Byte: 14    实时功率热量数据   			50Hz       
	ID: 0x0203  Byte: 16    机器人位置数据           	10Hz
	ID: 0x0204  Byte:  1    机器人增益数据           	增益状态改变后发送
	ID: 0x0205  Byte:  3    空中机器人能量状态数据      10Hz
	ID: 0x0206  Byte:  1    伤害状态数据           		伤害发生后发送
	ID: 0x0207  Byte:  6    实时射击数据           		子弹发射后发送
	ID: 0x0301  Byte:  n    机器人间交互数据           	发送方触发发送,10Hz
	
*/



//命令码ID,用来判断接收的是什么数据
typedef enum
{ 
	ID_game_state       		             	= 0x001,//比赛状态数据
	ID_game_result 	   				            = 0x002,//比赛结果数据
	ID_game_robot_HP                    	= 0x003,//比赛机器人存活数据
	ID_dart_status                        = 0x004,//人工智能挑战赛加成与惩罚区状态
	ID_ICRA_buff_debuff_zone_status       = 0x005,//飞镖发射状态
	ID_event_data  				           	    = 0x101,//场地事件数据 
	ID_supply_projectile_action   	      = 0x102,//补给站动作标识
	ID_referee_warning 	                  = 0x104,//裁判警告信息
	ID_dart_remaining_time                = 0x105,//飞镖发射口倒计时
	ID_game_robot_state    			          = 0x201,//比赛机器人状态数据
	ID_power_heat_data    			          = 0x202,//实时功率热量数据
	ID_game_robot_pos        	            = 0x203,//机器人位置数据
	ID_buff_musk					                = 0x204,//机器人增益数据
	ID_aerial_robot_energy			          = 0x205,//空中机器人能量状态数据
	ID_robot_hurt					                = 0x206,//伤害状态数据
	ID_shoot_data					                = 0x207,//实时射击数据
	ID_bullet_remaining                   = 0x208,//子弹剩余发射数(空中机器人，哨兵机器人以及 ICRA 机器人主控发送)            
  ID_rfid_status                        = 0x209,//机器人 RFID 状态
	ID_dart_client_cmd                    = 0x20A, //飞镖机器人客户端指令数据
} CmdID;



typedef enum
{ 
	ID_game_state_tx       		               	= 0x0F1,//比赛状态数据
	ID_game_result_tx 	   				            = 0x0F2,//比赛结果数据
	
	ID_game_robot_HP_one_tx                   = 0x013,//比赛机器人存活数据 1           *****
	ID_game_robot_HP_two_tx                   = 0x023,//比赛机器人存活数据 2           *****
	ID_game_robot_HP_three_tx                 = 0x033,//比赛机器人存活数据 3           *****
	ID_game_robot_HP_four_tx                  = 0x043,//比赛机器人存活数据 4           *****
	
	ID_dart_status_tx                         = 0x0F4,//人工智能挑战赛加成与惩罚区状态
	ID_ICRA_buff_debuff_zone_status_tx        = 0x0F5,//飞镖发射状态
	ID_event_data_tx  				           	    = 0x1F1,//场地事件数据 
	ID_supply_projectile_action_tx   	        = 0x1F2,//补给站动作标识                 *****
	ID_referee_warning_tx 	                  = 0x1F4,//裁判警告信息
	ID_dart_remaining_time_tx                 = 0x1F5,//飞镖发射口倒计时
	
	ID_game_robot_state_one_tx    			      = 0x211,//比赛机器人状态数据 1           *****
	ID_game_robot_state_two_tx   			        = 0x221,//比赛机器人状态数据 2           *****
  ID_game_robot_state_three_tx    			    = 0x231,//比赛机器人状态数据 3           *****
	ID_game_robot_state_four_tx               = 0x241,//比赛机器人状态数据 4           *****
	
	ID_power_heat_data_one_tx    			        = 0x212,//实时功率热量数据   1           *****
	ID_power_heat_data_two_tx    			        = 0x222,//实时功率热量数据   2           *****
	
	ID_game_robot_pos_tx        	            = 0x2F3,//机器人位置数据
	ID_buff_musk_tx					                  = 0x2F4,//机器人增益数据                 *****
	ID_aerial_robot_energy_tx			            = 0x2F5,//空中机器人能量状态数据
	ID_robot_hurt_tx					                = 0x2F6,//伤害状态数据                   *****
	ID_shoot_data_tx					                = 0x2F7,//实时射击数据                   *****
	ID_bullet_remaining_tx                    = 0x2F8,//子弹剩余发射数                 *****             
  ID_rfid_status_tx                         = 0x2F9,//机器人 RFID 状态               *****
	ID_dart_client_cmd_tx                     = 0x2FA, //飞镖机器人客户端指令数据

} TX_CmdID;




//命令码数据段长,根据官方协议来定义长度
typedef enum
{
	LEN_game_state       		             	= 11, //比赛状态数据
	LEN_game_result 	   				          = 1, //比赛结果数据
	LEN_game_robot_HP_t                 	= 28,//比赛机器人存活数据
	LEN_dart_status_t                     = 3, //人工智能挑战赛加成与惩罚区状态
	LEN_ICRA_buff_debuff_zone_status_t    = 11, //飞镖发射状态
	LEN_event_data  				           	  = 3, //场地事件数据 
	LEN_supply_projectile_action_t   	    = 2, //补给站动作标识
	LEN_referee_warning_t 	              = 2, //裁判警告信息
	LEN_dart_remaining_time_t             = 1, //飞镖发射口倒计时
	LEN_game_robot_state    			        = 15,//比赛机器人状态数据
	LEN_power_heat_data    			          = 14,//实时功率热量数据
	LEN_game_robot_pos        	          = 16,//机器人位置数据
	LEN_buff_musk					                = 1, //机器人增益数据
	LEN_aerial_robot_energy			          = 3, //空中机器人能量状态数据
	LEN_robot_hurt					              = 1, //伤害状态数据
	LEN_shoot_data					              = 6, //实时射击数据  6
	LEN_bullet_remaining_t                = 2, //子弹剩余发射数(空中机器人，哨兵机器人以及 ICRA 机器人主控发送)            
  LEN_rfid_status_t                     = 4, //机器人 RFID 状态
	LEN_dart_client_cmd_t                 = 12,//飞镖机器人客户端指令数据
	
} JudgeDataLength;

/* 自定义帧头 */
typedef __packed struct
{
	uint8_t  SOF;
	uint16_t DataLength;
	uint8_t  Seq;
	uint8_t  CRC8;
	
} xFrameHeader;

/* ID: 0x0001  Byte:  3    比赛状态数据 */
typedef __packed struct 
{ 
	uint8_t game_type : 4;
	uint8_t game_progress : 4;
	uint16_t stage_remain_time;
	uint64_t  SyncTimeStamp;
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
	
 uint16_t red1_bullet_left;
 uint16_t red2_bullet_left;
 uint16_t blue1_bullet_left;
 uint16_t blue2_bullet_left;	
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
	uint8_t robot_id;                       //机器人ID，可用来校验发送
	uint8_t robot_level;                    //1一级，2二级，3三级
	uint16_t remain_HP;                     //机器人剩余血量
	uint16_t max_HP;                        //机器人满血量
  uint16_t shooter_id1_17mm_cooling_rate;
	
  uint16_t shooter_id1_17mm_cooling_limit;
  uint16_t shooter_id1_17mm_speed_limit;
  uint16_t shooter_id2_17mm_cooling_rate;
  uint16_t shooter_id2_17mm_cooling_limit;
	
  uint16_t shooter_id2_17mm_speed_limit;
  uint16_t shooter_id1_42mm_cooling_rate;
  uint16_t shooter_id1_42mm_cooling_limit;
  uint16_t shooter_id1_42mm_speed_limit;
	
  uint16_t chassis_power_limit;
  uint8_t mains_power_gimbal_output : 1;
  uint8_t mains_power_chassis_output : 1;
  uint8_t mains_power_shooter_output : 1;
} ext_game_robot_state_t; 


/* ID: 0X0202  Byte: 16    实时功率热量数据 */
typedef __packed struct 
{ 
 uint16_t chassis_volt;
 uint16_t chassis_current;
 float chassis_power;
 uint16_t chassis_power_buffer;
 uint16_t shooter_id1_17mm_cooling_heat;
 uint16_t shooter_id2_17mm_cooling_heat;
 uint16_t shooter_id1_42mm_cooling_heat;
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
	uint8_t power_rune_buff;  // bit 0：机器人血量补血状态 bit 1：枪口热量冷却加速 bit 2：机器人防御加成 bit 3：机器人攻击加成
} ext_buff_musk_t; 


/* ID: 0x0205  Byte:  3    空中机器人能量状态数据 */
typedef __packed struct 
{ 
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
 uint8_t shooter_id;
 uint8_t bullet_freq;
 float bullet_speed; 
} ext_shoot_data_t; 

/* ID: 0x0208  Byte:  2    子弹剩余发射数 */
typedef __packed struct
{
 uint16_t bullet_remaining_num_17mm;
 uint16_t bullet_remaining_num_42mm;
 uint16_t coin_remaining_num;
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



/* 
	
	交互数据，包括一个统一的数据段头结构，
	包含了内容 ID，发送者以及接受者的 ID 和内容数据段，
	整个交互数据的包总共长最大为 128 个字节，
	减去 frame_header,cmd_id,frame_tail 以及数据段头结构的 6 个字节，
	故而发送的内容数据段最大为 113。
	整个交互数据 0x0301 的包上行频率为 10Hz。

	机器人 ID：
	1，英雄(红)；
	2，工程(红)；
	3/4/5，步兵(红)；
	6，空中(红)；
	7，哨兵(红)；
	11，英雄(蓝)；
	12，工程(蓝)；
	13/14/15，步兵(蓝)；
	16，空中(蓝)；
	17，哨兵(蓝)。 
	客户端 ID： 
	0x0101 为英雄操作手客户端( 红) ；
	0x0102 ，工程操作手客户端 ((红 )；
	0x0103/0x0104/0x0105，步兵操作手客户端(红)；
	0x0106，空中操作手客户端((红)； 
	0x0111，英雄操作手客户端(蓝)；
	0x0112，工程操作手客户端(蓝)；
	0x0113/0x0114/0x0115，操作手客户端步兵(蓝)；
	0x0116，空中操作手客户端(蓝)。 
*/
/* 交互数据接收信息：0x0301  */
typedef __packed struct 
{ 
 uint16_t data_cmd_id;
 uint16_t sender_ID;
 uint16_t receiver_ID; 
} ext_student_interactive_header_data_t; 


/* 
	客户端 客户端自定义数据：cmd_id:0x0301。内容 ID:0xD180
	发送频率：上限 10Hz


	1.	客户端 客户端自定义数据：cmd_id:0x0301。内容 ID:0xD180。发送频率：上限 10Hz 
	字节偏移量 	大小 	说明 				备注 
	0 			2 		数据的内容 ID 		0xD180 
	2 			2 		送者的 ID 			需要校验发送者机器人的 ID 正确性 
	4 			2 		客户端的 ID 		只能为发送者机器人对应的客户端 
	6 			4 		自定义浮点数据 1 	 
	10 			4 		自定义浮点数据 2 	 
	14 			4 		自定义浮点数据 3 	 
	18 			1 		自定义 8 位数据 4 	 

*/
typedef __packed struct 
{ 
	float data1; 
	float data2; 
	float data3; 
	uint8_t masks; 
} client_custom_data_t;


/* 
	学生机器人间通信 cmd_id 0x0301，内容 ID:0x0200~0x02FF
	交互数据 机器人间通信：0x0301。
	发送频率：上限 10Hz  

	字节偏移量 	大小 	说明 			备注 
	0 			2 		数据的内容 ID 	0x0200~0x02FF 
										可以在以上 ID 段选取，具体 ID 含义由参赛队自定义 
	
	2 			2 		发送者的 ID 	需要校验发送者的 ID 正确性， 
	
	4 			2 		接收者的 ID 	需要校验接收者的 ID 正确性，
										例如不能发送到敌对机器人的ID 
	
	6 			n 		数据段 			n 需要小于 113 

*/
typedef __packed struct 
{ 
	uint8_t data[10]; //数据段,n需要小于113
} robot_interactive_data_t;

//帧头  命令码   数据段头结构  数据段   帧尾
//上传客户端
typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//帧头
	uint16_t		 						CmdID;//命令码
	ext_student_interactive_header_data_t   dataFrameHeader;//数据段头结构
	client_custom_data_t  					clientData;//数据段
	uint16_t		 						FrameTail;//帧尾
}ext_SendClientData_t;


//机器人交互信息
typedef __packed struct
{
	xFrameHeader   							txFrameHeader;//帧头
	uint16_t								CmdID;//命令码
	ext_student_interactive_header_data_t   dataFrameHeader;//数据段头结构
	robot_interactive_data_t  	 			interactData;//数据段
	uint16_t		 						FrameTail;//帧尾
}ext_CommunatianData_t;




extern ext_game_state_t       				        GameState;					      //0x0001
extern ext_game_result_t            		      GameResult;				    	  //0x0002
extern ext_game_robot_HP_t                    RoboHP;                   //0x0003
extern ext_dart_status_t                      DartState;                //0x0004
//ext_ICRA_buff_debuff_zone_status_t                                    //0x0005智能赛       xxxxxx
extern ext_event_data_t        			          EventData;				      	//0x0101
extern ext_supply_projectile_action_t		      SupplyProjectileAction;	  //0x0102 补给站       *****
extern ext_referee_warning_t 	                RefWarning;               //0x0104
extern ext_dart_remaining_time_t              DartTime;                 //0x0105
extern ext_game_robot_state_t			  	        GameRobotStat;				    //0x0201 机器人状态   *****
extern ext_power_heat_data_t		  		        PowerHeatData;				    //0x0202 功率热量     *****
extern ext_game_robot_pos_t			              GameRobotPos;				      //0x0203
extern ext_buff_musk_t					              BuffMusk;				      	  //0x0204
extern aerial_robot_energy_t				          AerialRobotEnergy;	  		//0x0205
extern ext_robot_hurt_t				                RobotHurt;					      //0x0206 伤害类型     *****
extern ext_shoot_data_t					              ShootData;					      //0x0207 射击信息     *****
extern ext_bullet_remaining_t                 ReBullet;                 //0x0208      
extern ext_rfid_status_t                      RFIDState;                //0x0209 RFID状态     *****
extern ext_dart_client_cmd_t                  DartClient;               //0x020A

extern xFrameHeader                           FrameHeader;	          	//发送帧头信息
extern ext_SendClientData_t                   ShowData;			            //客户端信息
extern ext_CommunatianData_t                  CommuData;	             	//队友通信信息



bool Judge_Read_Data(uint8_t *ReadFromUsart);
void JUDGE_Show_Data(void);
void Send_to_Teammate(void);
bool is_red_or_blue(void);
void determine_ID(void);

bool JUDGE_sGetDataState(void);
float JUDGE_fGetChassisPower(void);
uint16_t JUDGE_fGetRemainEnergy(void);
uint8_t JUDGE_ucGetRobotLevel(void);
uint16_t JUDGE_usGetRemoteHeat17(void);
float JUDGE_usGetSpeedHeat17(void);
//void JUDGE_ShootNumCount(void);
uint16_t JUDGE_usGetShootNum(void);
void JUDGE_ShootNum_Clear(void);
uint16_t JUDGE_usGetHeatLimit(void);
uint16_t JUDGE_usGetShootCold(void);
bool JUDGE_IfArmorHurt(void);
bool Judge_If_Death(void);

void set_robo_hp_one(CAN_HandleTypeDef* hcan, uint32_t mark, uint16_t red_1_robot_HP,uint16_t red_2_robot_HP,uint16_t red_3_robot_HP,uint16_t red_4_robot_HP);
void set_robo_hp_two(CAN_HandleTypeDef* hcan, uint32_t mark, uint16_t red_5_robot_HP,uint16_t red_7_robot_HP,uint16_t red_outpost_HP,uint16_t red_base_HP);
void set_robo_hp_three(CAN_HandleTypeDef* hcan, uint32_t mark, uint16_t blue_1_robot_HP,uint16_t blue_2_robot_HP,uint16_t blue_3_robot_HP,uint16_t blue_4_robot_HP);
void set_robo_hp_four(CAN_HandleTypeDef* hcan, uint32_t mark, uint16_t blue_5_robot_HP,uint16_t blue_7_robot_HP,uint16_t blue_outpost_HP);

void set_supply_projectile_action(CAN_HandleTypeDef* hcan, uint32_t mark, uint8_t supply_projectile_id, uint8_t supply_robot_id, uint8_t supply_projectile_step, uint8_t supply_projectile_num);

void set_game_robot_state_one(CAN_HandleTypeDef* hcan, uint32_t mark,  uint8_t robot_id, uint8_t robot_level, uint16_t remain_HP, uint16_t max_HP, uint16_t shooter_id1_17mm_cooling_rate);
void set_game_robot_state_two(CAN_HandleTypeDef* hcan, uint32_t mark,  uint16_t shooter_id1_17mm_cooling_limit, uint16_t shooter_id1_17mm_speed_limit, uint16_t shooter_id2_17mm_cooling_rate, uint16_t shooter_id2_17mm_cooling_limit);
void set_game_robot_state_three(CAN_HandleTypeDef* hcan, uint32_t mark,  uint16_t shooter_id2_17mm_speed_limit, uint16_t shooter_id1_42mm_cooling_rate, uint16_t shooter_id1_42mm_cooling_limit, uint16_t shooter_id1_42mm_speed_limit);
void set_game_robot_state_four(CAN_HandleTypeDef* hcan, uint32_t mark,  uint16_t chassis_power_limit, uint8_t mains_power_gimbal_output, uint8_t mains_power_chassis_output, uint8_t mains_power_shooter_output);

void set_power_heat_data_one(CAN_HandleTypeDef* hcan, uint32_t mark, uint16_t chassis_volt, uint16_t chassis_current, float chassis_power);
void set_power_heat_data_two(CAN_HandleTypeDef* hcan, uint32_t mark, uint16_t chassis_power_buffer, uint16_t shooter_id1_17mm_cooling_heat, uint16_t shooter_id2_17mm_cooling_heat, uint16_t shooter_id1_42mm_cooling_heat);


void set_buff_musk(CAN_HandleTypeDef* hcan,uint32_t mark, uint8_t power_rune_buff);
void set_robot_hurt(CAN_HandleTypeDef* hcan, uint32_t mark, uint8_t armor_id, uint8_t hurt_type);
void set_shoot_data(CAN_HandleTypeDef* hcan, uint32_t mark, uint8_t bullet_type, uint8_t shooter_id, uint8_t bullet_freq, float bullet_speed);
void set_bullet_remaining(CAN_HandleTypeDef* hcan, uint32_t mark,  uint16_t bullet_remaining_num_17mm, uint16_t bullet_remaining_num_42mm, uint16_t coin_remaining_num);
void set_rfid_status(CAN_HandleTypeDef* hcan, uint32_t mark, uint32_t rfid_status);

void transmit_judge_info(CAN_HandleTypeDef* hcan, int16_t mark, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4);

#endif

