#include "./judge/judge.h"


/*****************系统数据定义**********************/
ext_game_state_t       				        GameState;					      //0x0001
ext_game_result_t            		      GameResult;				    	  //0x0002              
ext_game_robot_HP_t                   RoboHP;                   //0x0003 机器人血量   *****
ext_dart_status_t                     DartState;                //0x0004
//ext_ICRA_buff_debuff_zone_status_t                            //0x0005 智能赛       xxxxx
ext_event_data_t        			        EventData;				      	//0x0101
ext_supply_projectile_action_t		    SupplyProjectileAction;	  //0x0102 补给站       *****
ext_referee_warning_t 	              RefWarning;               //0x0104
ext_dart_remaining_time_t             DartTime;                 //0x0105
ext_game_robot_state_t			  	      GameRobotStat;				    //0x0201 机器人状态   *****
ext_power_heat_data_t		  		        PowerHeatData;				    //0x0202 功率热量     *****
ext_game_robot_pos_t			            GameRobotPos;				      //0x0203
ext_buff_musk_t					              BuffMusk;				      	  //0x0204 增益数据     *****
aerial_robot_energy_t				          AerialRobotEnergy;	  		//0x0205
ext_robot_hurt_t				              RobotHurt;					      //0x0206 伤害类型     *****
ext_shoot_data_t					            ShootData;					      //0x0207 射击信息     *****
ext_bullet_remaining_t                ReBullet;                 //0x0208 剩余子弹数量 *****    
ext_rfid_status_t                     RFIDState;                //0x0209 RFID状态     *****
ext_dart_client_cmd_t                 DartClient;               //0x020A

xFrameHeader                          FrameHeader;	          	//发送帧头信息
ext_SendClientData_t                  ShowData;			            //客户端信息
ext_CommunatianData_t                 CommuData;	             	//队友通信信息
/***************************************** ***********/

bool Judge_Data_TF = FALSE;//裁判数据是否可用,辅助函数调用
uint8_t Judge_Self_ID;//当前机器人的ID
uint16_t Judge_SelfClient_ID;//发送者机器人对应的客户端ID



/**************裁判系统数据辅助****************/
uint16_t ShootNum;//统计发弹量,0x0003触发一次则认为发射了一颗
bool Hurt_Data_Update = FALSE;//装甲板伤害数据是否更新,每受一次伤害置TRUE,然后立即置FALSE,给底盘闪避用
#define BLUE  0
#define RED   1

uint8_t * test_add;

/**
  * @brief  读取裁判数据,中断中读取保证速度
  * @param  缓存数据
  * @retval 是否对正误判断做处理
  * @attention  在此判断帧头和CRC校验,无误再写入数据，不重复判断帧头
  */
bool Judge_Read_Data(uint8_t *ReadFromUsart)
{
	bool retval_tf = FALSE;//数据正确与否标志,每次调用读取裁判系统数据函数都先默认为错误
	
	uint16_t judge_length;//统计一帧数据长度 
	
	int CmdID = 0;//数据命令码解析
	
	/***------------------*****/
	//无数据包，则不作任何处理
	if (ReadFromUsart == NULL)
	{
		return -1;
	}
	
	//写入帧头数据,用于判断是否开始存储裁判数据
	memcpy(&FrameHeader, ReadFromUsart, LEN_HEADER);
	
	//判断帧头数据是否为0xA5
	if(ReadFromUsart[ SOF ] == JUDGE_FRAME_HEADER)
	{
		//帧头CRC8校验
		if (Verify_CRC8_Check_Sum( ReadFromUsart, LEN_HEADER ) == TRUE)
		{
			//统计一帧数据长度,用于CR16校验
			judge_length = ReadFromUsart[ DATA_LENGTH ] + LEN_HEADER + LEN_CMDID + LEN_TAIL;;

			//帧尾CRC16校验
			if(Verify_CRC16_Check_Sum(ReadFromUsart,judge_length) == TRUE)
			{
				retval_tf = TRUE;//都校验过了则说明数据可用
				
				CmdID = (ReadFromUsart[6] << 8 | ReadFromUsart[5]);
				//解析数据命令码,将数据拷贝到相应结构体中(注意拷贝数据的长度)
				switch(CmdID)
				{
					case ID_game_state:        			//0x0001
						memcpy(&GameState, (ReadFromUsart + DATA), LEN_game_state);
					break;
					
					case ID_game_result:          		//0x0002
						memcpy(&GameResult, (ReadFromUsart + DATA), LEN_game_result);
					break;
					case ID_game_robot_HP:       //0x0003
						memcpy(&RoboHP, (ReadFromUsart + DATA), LEN_game_robot_HP_t );
						set_robo_hp_one(&hcan1, ID_game_robot_HP_one_tx, RoboHP.red_1_robot_HP, RoboHP.red_2_robot_HP, RoboHP.red_3_robot_HP, RoboHP.red_4_robot_HP);
						set_robo_hp_two(&hcan1, ID_game_robot_HP_two_tx, RoboHP.red_5_robot_HP, RoboHP.red_7_robot_HP, RoboHP.red_outpost_HP, RoboHP.red_base_HP);
						set_robo_hp_three(&hcan1, ID_game_robot_HP_three_tx, RoboHP.blue_1_robot_HP, RoboHP.blue_2_robot_HP, RoboHP.blue_3_robot_HP, RoboHP.blue_4_robot_HP);
						set_robo_hp_four(&hcan1, ID_game_robot_HP_four_tx, RoboHP.blue_5_robot_HP, RoboHP. blue_7_robot_HP, RoboHP.blue_outpost_HP);
					
					break;
					
					case ID_dart_status:       //0x0004
						memcpy(&DartState, (ReadFromUsart + DATA), LEN_dart_status_t );
					break;

					
					case ID_event_data:    				     //0x0101
						memcpy(&EventData, (ReadFromUsart + DATA), LEN_event_data);
					break;
					
					case ID_supply_projectile_action:   //0x0102
						memcpy(&SupplyProjectileAction, (ReadFromUsart + DATA), LEN_supply_projectile_action_t );
				    set_supply_projectile_action(&hcan1,ID_supply_projectile_action_tx, SupplyProjectileAction.supply_projectile_id,SupplyProjectileAction.supply_robot_id,SupplyProjectileAction.supply_projectile_step,SupplyProjectileAction.supply_projectile_num);
					break;
					
					case ID_referee_warning:        //0x0104
						memcpy(&RefWarning, (ReadFromUsart + DATA), LEN_referee_warning_t);
					break;
					
					case ID_game_robot_state:      		//0x0201
						memcpy(&GameRobotStat, (ReadFromUsart + DATA), LEN_game_robot_state);
					  set_game_robot_state_one(&hcan1, ID_game_robot_state_one_tx,  GameRobotStat.robot_id, GameRobotStat.robot_level, GameRobotStat.remain_HP, GameRobotStat.max_HP, GameRobotStat.shooter_id1_17mm_cooling_rate);
				  	set_game_robot_state_two(&hcan1, ID_game_robot_state_two_tx,  GameRobotStat.shooter_id1_17mm_cooling_limit, GameRobotStat.shooter_id1_17mm_speed_limit, GameRobotStat.shooter_id2_17mm_cooling_rate, GameRobotStat.shooter_id2_17mm_cooling_limit);
				  	set_game_robot_state_three(&hcan1, ID_game_robot_state_three_tx,  GameRobotStat.shooter_id2_17mm_speed_limit, GameRobotStat.shooter_id1_42mm_cooling_rate, GameRobotStat.shooter_id1_42mm_cooling_limit, GameRobotStat.shooter_id1_42mm_speed_limit);
				  	set_game_robot_state_four(&hcan1, ID_game_robot_state_four_tx,  GameRobotStat.chassis_power_limit, GameRobotStat.mains_power_gimbal_output, GameRobotStat.mains_power_chassis_output, GameRobotStat.mains_power_shooter_output);
					break;
					
					case ID_power_heat_data:      		//0x0202
						memcpy(&PowerHeatData, (ReadFromUsart + DATA), LEN_power_heat_data);
				  	set_power_heat_data_one(&hcan1, ID_power_heat_data_one_tx, PowerHeatData.chassis_volt, PowerHeatData.chassis_current, PowerHeatData.chassis_power);
				  	set_power_heat_data_two(&hcan1, ID_power_heat_data_two_tx, PowerHeatData.chassis_power_buffer, PowerHeatData.shooter_id1_17mm_cooling_heat, PowerHeatData.shooter_id2_17mm_cooling_heat, PowerHeatData.shooter_id1_42mm_cooling_heat);
					
					break;
					
					case ID_game_robot_pos:      		//0x0203
						memcpy(&GameRobotPos, (ReadFromUsart + DATA), LEN_game_robot_pos);
					break;
					
					case ID_buff_musk:      			//0x0204
						memcpy(&BuffMusk, (ReadFromUsart + DATA), LEN_buff_musk);
					  set_buff_musk(&hcan1, ID_buff_musk_tx, BuffMusk.power_rune_buff);
					/**/
					break;
					
					case ID_aerial_robot_energy:      	//0x0205
						memcpy(&AerialRobotEnergy, (ReadFromUsart + DATA), LEN_aerial_robot_energy);
					break;
					
					case ID_robot_hurt:      			//0x0206
						memcpy(&RobotHurt, (ReadFromUsart + DATA), LEN_robot_hurt);
						if(RobotHurt.hurt_type == 0)//非装甲板离线造成伤害
						{	Hurt_Data_Update = TRUE;	}//装甲数据每更新一次则判定为受到一次伤害
						set_robot_hurt(&hcan1, ID_robot_hurt_tx, RobotHurt.armor_id, RobotHurt.hurt_type);
					break;
					
					case ID_shoot_data:      			//0x0207
						memcpy(&ShootData, (ReadFromUsart + DATA), LEN_shoot_data);
					  set_shoot_data(&hcan1, ID_shoot_data_tx, ShootData.bullet_type,ShootData.shooter_id,ShootData.bullet_freq ,ShootData.bullet_speed);
		     	//JUDGE_ShootNumCount();//发弹量统计
					break;
										
					case ID_bullet_remaining:      	//0x0208 
						memcpy(&ReBullet, (ReadFromUsart + DATA), ID_bullet_remaining);
					  set_bullet_remaining(&hcan1, ID_bullet_remaining_tx, ReBullet.bullet_remaining_num_17mm,ReBullet.bullet_remaining_num_42mm, ReBullet.coin_remaining_num);

					break;
					
					case ID_rfid_status:      			//0x0209
						memcpy(&RFIDState, (ReadFromUsart + DATA), LEN_rfid_status_t);
					 set_rfid_status(&hcan1, ID_rfid_status_tx, RFIDState.rfid_status);
					break;
					
					case ID_dart_client_cmd:      			//0x020A
						memcpy(&DartClient, (ReadFromUsart + DATA), ID_dart_client_cmd);
					break;
				}
			}
		}
			//首地址加帧长度,指向CRC16下一字节,用来判断是否为0xA5,用来判断一个数据包是否有多帧数据
		if(*(ReadFromUsart + sizeof(xFrameHeader) + LEN_CMDID + FrameHeader.DataLength + LEN_TAIL) == 0xA5)
		{
	    test_add = ReadFromUsart + sizeof(xFrameHeader) + LEN_CMDID + FrameHeader.DataLength + LEN_TAIL;
			//如果一个数据包出现了多帧数据,则再次读取
			Judge_Read_Data(test_add);
		}
	}
	if (retval_tf == TRUE)
	{
		Judge_Data_TF = TRUE;//辅助函数用
	}
	else		//只要CRC16校验不通过就为FALSE
	{
		Judge_Data_TF = FALSE;//辅助函数用
	}
	
	return retval_tf;//对数据正误做处理
}

/**
  * @brief  上传自定义数据(没写完）
  * @param  void
  * @retval void
  * @attention  数据打包,打包完成后通过串口发送到裁判系统
  */
#define send_max_len     200
unsigned char CliendTxBuffer[send_max_len];
void JUDGE_Show_Data(void)
{
	static uint8_t datalength,i;
	uint8_t judge_led = 0xff;//初始化led为全绿
	//static uint8_t auto_led_time = 0;
	//static uint8_t buff_led_time = 0;
	
	determine_ID();//判断发送者ID和其对应的客户端ID
	
	ShowData.txFrameHeader.SOF = 0xA5;
	ShowData.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t) + sizeof(client_custom_data_t);
	ShowData.txFrameHeader.Seq = 0;
	memcpy(CliendTxBuffer, &ShowData.txFrameHeader, sizeof(xFrameHeader));//写入帧头数据
	Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(xFrameHeader));//写入帧头CRC8校验码
	
	ShowData.CmdID = 0x0301;
	
	ShowData.dataFrameHeader.data_cmd_id = 0xD180;//发给客户端的cmd,官方固定
	//ID已经是自动读取的了
	ShowData.dataFrameHeader.sender_ID 	 = Judge_Self_ID;//发送者的ID
	ShowData.dataFrameHeader.receiver_ID = Judge_SelfClient_ID;//客户端的ID，只能为发送者机器人对应的客户端
	
	/*- 自定义内容 -*/
//	ShowData.clientData.data1 = 
	
	/*--------------*/
	ShowData.clientData.masks = judge_led;//0~5位0红灯,1绿灯
	
	//打包写入数据段
	memcpy(	
			CliendTxBuffer + 5, 
			(uint8_t*)&ShowData.CmdID, 
			(sizeof(ShowData.CmdID)+ sizeof(ShowData.dataFrameHeader)+ sizeof(ShowData.clientData))
		  );			
			
	Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(ShowData));//写入数据段CRC16校验码	

	datalength = sizeof(ShowData); 
	for(i = 0;i < datalength;i++)
	{
//		HAL_UART_Transmit_DMA
//		while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET);
	}	 
}


/**
  * @brief  判断自己红蓝方
  * @param  void
  * @retval RED   BLUE
  * @attention  数据打包,打包完成后通过串口发送到裁判系统
  */
bool Color;
bool is_red_or_blue(void)
{
	Judge_Self_ID = GameRobotStat.robot_id;//读取当前机器人ID
	
	if(GameRobotStat.robot_id > 10)
	{
		return BLUE;
	}
	else 
	{
		return RED;
	}
}

/**
  * @brief  判断自身ID，选择客户端ID
  * @param  void
  * @retval RED   BLUE
  * @attention  数据打包,打包完成后通过串口发送到裁判系统
  */
void determine_ID(void)
{
	Color = is_red_or_blue();
	if(Color == BLUE)
	{
		Judge_SelfClient_ID = 0x0110 + (Judge_Self_ID-10);//计算客户端ID
	}
	else if(Color == RED)
	{
		Judge_SelfClient_ID = 0x0100 + Judge_Self_ID;//计算客户端ID
	}
}

/********************裁判数据辅助判断函数***************************/

/**
  * @brief  数据是否可用
  * @param  void
  * @retval  TRUE可用   FALSE不可用
  * @attention  在裁判读取函数中实时改变返回值
  */
bool JUDGE_sGetDataState(void)
{
	return Judge_Data_TF;
}

/**
  * @brief  读取瞬时功率
  * @param  void
  * @retval 实时功率值
  * @attention  
  */
float JUDGE_fGetChassisPower(void)
{
	return (PowerHeatData.chassis_power);
}

/**
  * @brief  读取剩余焦耳能量
  * @param  void
  * @retval 剩余缓冲焦耳能量(最大60)
  * @attention  
  */
uint16_t JUDGE_fGetRemainEnergy(void)
{
	return (PowerHeatData.chassis_power_buffer);
}

/**
  * @brief  读取当前等级
  * @param  void
  * @retval 当前等级
  * @attention  
  */
uint8_t JUDGE_ucGetRobotLevel(void)
{
    return	GameRobotStat.robot_level;
}

/**
  * @brief  读取枪口热量
  * @param  void
  * @retval 17mm
  * @attention  实时热量
  */
uint16_t JUDGE_usGetRemoteHeat17(void)
{
	return PowerHeatData.shooter_id1_17mm_cooling_heat;
}

/**
  * @brief  读取射速
  * @param  void
  * @retval 17mm
  * @attention  实时热量
  */
float JUDGE_usGetSpeedHeat17(void)
{
	return ShootData.bullet_speed;
}

///**
//  * @brief  统计发弹量
//  * @param  void
//  * @retval void
//  * @attention  
//  */
//portTickType shoot_time;//发射延时测试
//portTickType shoot_ping;//计算出的最终发弹延迟
//float Shoot_Speed_Now = 0;
//float Shoot_Speed_Last = 0;
//void JUDGE_ShootNumCount(void)
//{
//	Shoot_Speed_Now = ShootData.bullet_speed;
//	if(Shoot_Speed_Last != Shoot_Speed_Now)//因为是float型，几乎不可能完全相等,所以速度不等时说明发射了一颗弹
//	{
//		ShootNum++;
//		Shoot_Speed_Last = Shoot_Speed_Now;
//	}
//	shoot_time = xTaskGetTickCount();//获取弹丸发射时的系统时间
//	shoot_ping = shoot_time - REVOL_uiGetRevolTime();//计算延迟
//}

/**
  * @brief  读取发弹量
  * @param  void
  * @retval 发弹量
  * @attention 不适用于双枪管
  */
uint16_t JUDGE_usGetShootNum(void)
{
	return ShootNum;
}

/**
  * @brief  发弹量清零
  * @param  void
  * @retval void
  * @attention 
  */
void JUDGE_ShootNum_Clear(void)
{
	ShootNum = 0;
}

/**
  * @brief  读取枪口热量
  * @param  void
  * @retval 当前等级17mm热量上限
  * @attention  
  */
uint16_t JUDGE_usGetHeatLimit(void)
{
	return GameRobotStat.shooter_id1_17mm_cooling_limit;
}

/**
  * @brief  当前等级对应的枪口每秒冷却值
  * @param  void
  * @retval 当前等级17mm冷却速度
  * @attention  
  */
uint16_t JUDGE_usGetShootCold(void)
{
	return GameRobotStat.shooter_id1_17mm_cooling_rate;
}

///****************底盘自动闪避判断用*******************/
///**
//  * @brief  装甲板伤害数据是否更新
//  * @param  void
//  * @retval TRUE已更新   FALSE没更新
//  * @attention  
//  */
//bool JUDGE_IfArmorHurt(void)
//{
//	static portTickType ulCurrent = 0;
//	static uint32_t ulDelay = 0;
//	static bool IfHurt = FALSE;//默认装甲板处于离线状态

//	
//	ulCurrent = xTaskGetTickCount();

//	if (Hurt_Data_Update == TRUE)//装甲板数据更新
//	{
//		Hurt_Data_Update = FALSE;//保证能判断到下次装甲板伤害更新
//		ulDelay = ulCurrent + 200;//
//		IfHurt = TRUE;
//	}
//	else if (ulCurrent > ulDelay)//
//	{
//		IfHurt = FALSE;
//	}
//	
//	return IfHurt;
//}

bool Judge_If_Death(void)
{
	if(GameRobotStat.remain_HP == 0 && JUDGE_sGetDataState() == TRUE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


/**********************发送各车血量***********************/
void set_robo_hp_one(CAN_HandleTypeDef* hcan, uint32_t mark, uint16_t red_1_robot_HP,uint16_t red_2_robot_HP,uint16_t red_3_robot_HP,uint16_t red_4_robot_HP){
 CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;
	
	TxData[0] = red_1_robot_HP >> 8;
	TxData[1] = red_1_robot_HP;
	TxData[2] = red_2_robot_HP >> 8;
	TxData[3] = red_2_robot_HP;
	TxData[4] = red_3_robot_HP >> 8;
	TxData[5] = red_3_robot_HP;
	TxData[6] = red_4_robot_HP >> 8;
	TxData[7] = red_4_robot_HP;

   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}
void set_robo_hp_two(CAN_HandleTypeDef* hcan, uint32_t mark, uint16_t red_5_robot_HP,uint16_t red_7_robot_HP,uint16_t red_outpost_HP,uint16_t red_base_HP){
  CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;
	
	TxData[0] = red_5_robot_HP >> 8;
	TxData[1] = red_5_robot_HP;
	TxData[2] = red_7_robot_HP >> 8;
	TxData[3] = red_7_robot_HP;
	TxData[4] = red_outpost_HP >> 8;
	TxData[5] = red_outpost_HP;
	TxData[6] = red_base_HP >> 8;
	TxData[7] = red_base_HP;

   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}
void set_robo_hp_three(CAN_HandleTypeDef* hcan, uint32_t mark, uint16_t blue_1_robot_HP,uint16_t blue_2_robot_HP,uint16_t blue_3_robot_HP,uint16_t blue_4_robot_HP){
  CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;
	
	TxData[0] = blue_1_robot_HP >> 8;
	TxData[1] = blue_1_robot_HP;
	TxData[2] = blue_2_robot_HP >> 8;
	TxData[3] = blue_2_robot_HP;
	TxData[4] = blue_3_robot_HP >> 8;
	TxData[5] = blue_3_robot_HP;
	TxData[6] = blue_4_robot_HP >> 8;
	TxData[7] = blue_4_robot_HP;

   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}
void set_robo_hp_four(CAN_HandleTypeDef* hcan, uint32_t mark, uint16_t blue_5_robot_HP,uint16_t blue_7_robot_HP,uint16_t blue_outpost_HP){
  CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;
	
	TxData[0] = blue_5_robot_HP >> 8;
	TxData[1] = blue_5_robot_HP;
	TxData[2] = blue_7_robot_HP >> 8;
	TxData[3] = blue_7_robot_HP;
	TxData[4] = blue_outpost_HP >> 8;
	TxData[5] = blue_outpost_HP;

   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}



/*****************发送补给站动作标识**********************/
void set_supply_projectile_action(CAN_HandleTypeDef* hcan, uint32_t mark, uint8_t supply_projectile_id, uint8_t supply_robot_id, uint8_t supply_projectile_step, uint8_t supply_projectile_num)
{
	CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox = CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;
	
	TxData[0] = supply_projectile_id;
	TxData[1] = supply_robot_id;
	TxData[2] = supply_projectile_step;
	TxData[3] = supply_projectile_num;

   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}

/*****************比赛机器人状态数据：分为1，2，3,4部分**********************/
void set_game_robot_state_one(CAN_HandleTypeDef* hcan, uint32_t mark,  uint8_t robot_id, uint8_t robot_level, uint16_t remain_HP, uint16_t max_HP, uint16_t shooter_id1_17mm_cooling_rate)
{
	CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;
	
	TxData[0] = robot_id;
	TxData[1] = robot_level;
	TxData[2] = remain_HP >> 8;
	TxData[3] = remain_HP;
	TxData[4] = max_HP >> 8;
	TxData[5] = max_HP;
	TxData[6] = shooter_id1_17mm_cooling_rate >> 8;
	TxData[7] = shooter_id1_17mm_cooling_rate;

   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}

void set_game_robot_state_two(CAN_HandleTypeDef* hcan, uint32_t mark,  uint16_t shooter_id1_17mm_cooling_limit, uint16_t shooter_id1_17mm_speed_limit, uint16_t shooter_id2_17mm_cooling_rate, uint16_t shooter_id2_17mm_cooling_limit)
{
	CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;
	
	TxData[0] = shooter_id1_17mm_cooling_limit >> 8;
	TxData[1] = shooter_id1_17mm_cooling_limit;
	TxData[2] = shooter_id1_17mm_speed_limit >> 8;
	TxData[3] = shooter_id1_17mm_speed_limit;
	TxData[4] = shooter_id2_17mm_cooling_rate >> 8;
	TxData[5] = shooter_id2_17mm_cooling_rate;
	TxData[6] = shooter_id2_17mm_cooling_limit >> 8;
	TxData[7] = shooter_id2_17mm_cooling_limit;

   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}

void set_game_robot_state_three(CAN_HandleTypeDef* hcan, uint32_t mark,  uint16_t shooter_id2_17mm_speed_limit, uint16_t shooter_id1_42mm_cooling_rate, uint16_t shooter_id1_42mm_cooling_limit, uint16_t shooter_id1_42mm_speed_limit)
{
	CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;
	
	TxData[0] = shooter_id2_17mm_speed_limit >> 8;
	TxData[1] = shooter_id2_17mm_speed_limit;
	TxData[2] = shooter_id1_42mm_cooling_rate >> 8;
	TxData[3] = shooter_id1_42mm_cooling_rate;
	TxData[4] = shooter_id1_42mm_cooling_limit >> 8;
	TxData[5] = shooter_id1_42mm_cooling_limit;
	TxData[6] = shooter_id1_42mm_speed_limit >> 8;
	TxData[7] = shooter_id1_42mm_speed_limit;

   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}


void set_game_robot_state_four(CAN_HandleTypeDef* hcan, uint32_t mark,  uint16_t chassis_power_limit, uint8_t mains_power_gimbal_output, uint8_t mains_power_chassis_output, uint8_t mains_power_shooter_output)
{
	CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;
	
	TxData[0] = chassis_power_limit >> 8;
	TxData[1] = chassis_power_limit;
	TxData[2] = mains_power_gimbal_output;
	TxData[3] = mains_power_chassis_output;
	TxData[4] = mains_power_shooter_output;
  
   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}

/*****************实时功率热量数据：分为1，2部分**********************/
void set_power_heat_data_one(CAN_HandleTypeDef* hcan, uint32_t mark, uint16_t chassis_volt, uint16_t chassis_current, float chassis_power)
{
	CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;
	

	TxData[0] = chassis_volt >> 8;
	TxData[1] = chassis_volt;
	TxData[2] = chassis_current >> 8;
	TxData[3] = chassis_current;
	TxData[4] = ((uint8_t*)(&chassis_power))[0];
  TxData[5] = ((uint8_t*)(&chassis_power))[1];
	TxData[6] = ((uint8_t*)(&chassis_power))[2];
  TxData[7] = ((uint8_t*)(&chassis_power))[3];


   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}

void set_power_heat_data_two(CAN_HandleTypeDef* hcan, uint32_t mark, uint16_t chassis_power_buffer, uint16_t shooter_id1_17mm_cooling_heat, uint16_t shooter_id2_17mm_cooling_heat, uint16_t shooter_id1_42mm_cooling_heat)
{
	CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;
	
	TxData[0] = chassis_power_buffer >> 8;
	TxData[1] = chassis_power_buffer;
	TxData[2] = shooter_id1_17mm_cooling_heat >> 8;
	TxData[3] = shooter_id1_17mm_cooling_heat;
	TxData[4] = shooter_id2_17mm_cooling_heat >> 8;
	TxData[5] = shooter_id2_17mm_cooling_heat;
	TxData[6] = shooter_id1_42mm_cooling_heat >> 8;
	TxData[7] = shooter_id1_42mm_cooling_heat;

   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}

/********************************************************/

void set_buff_musk(CAN_HandleTypeDef* hcan, uint32_t mark, uint8_t power_rune_buff)
{
	CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;
	
	TxData[0] = power_rune_buff;

   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}

void set_robot_hurt(CAN_HandleTypeDef* hcan, uint32_t mark, uint8_t armor_id, uint8_t hurt_type)
{
	CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;
	
	TxData[0] = armor_id ;
	TxData[1] = hurt_type;

   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}


void set_shoot_data(CAN_HandleTypeDef* hcan, uint32_t mark, uint8_t bullet_type,  uint8_t shooter_id,uint8_t bullet_freq, float bullet_speed)
{
	CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;

	TxData[0] = bullet_type; 
	TxData[1] = bullet_freq;
	TxData[2] = bullet_freq;
	TxData[3] = ((uint8_t*)(&bullet_speed))[0];
  TxData[4] = ((uint8_t*)(&bullet_speed))[1];
	TxData[5] = ((uint8_t*)(&bullet_speed))[2];
  TxData[6] = ((uint8_t*)(&bullet_speed))[3];

   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}


void set_bullet_remaining(CAN_HandleTypeDef* hcan, uint32_t mark,  uint16_t bullet_remaining_num_17mm, uint16_t bullet_remaining_num_42mm, uint16_t coin_remaining_num){
	CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;
	
	TxData[0] = bullet_remaining_num_17mm >> 8;
	TxData[1] = bullet_remaining_num_17mm;
	TxData[2] = bullet_remaining_num_42mm >> 8;
	TxData[3] = bullet_remaining_num_42mm;
	TxData[4] = coin_remaining_num >> 8;
	TxData[5] = coin_remaining_num;


   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}


void set_rfid_status(CAN_HandleTypeDef* hcan, uint32_t mark, uint32_t rfid_status)
{
	CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId=mark;
  TxHeader.DLC=0x08;
  TxHeader.IDE=CAN_ID_STD;
  TxHeader.RTR=CAN_RTR_DATA;
	
	TxData[0] = ((uint8_t*)(&rfid_status))[0];
  TxData[1] = ((uint8_t*)(&rfid_status))[1];
	TxData[2] = ((uint8_t*)(&rfid_status))[2];
  TxData[3] = ((uint8_t*)(&rfid_status))[3];

   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}

void transmit_judge_info(CAN_HandleTypeDef* hcan, int16_t mark, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4)
{
	CAN_TxHeaderTypeDef   TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox= CAN_TX_MAILBOX1;
	TxHeader.StdId = 0x301;
  TxHeader.DLC = 0x08;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.RTR = CAN_RTR_DATA;
	
	TxData[0] = (iq1 >> 8);
	TxData[1] = iq1;
	TxData[2] = (iq2 >> 8);
	TxData[3] = iq2;
	TxData[4] = iq3 >> 8;
	TxData[5] = iq3;
	TxData[6] = iq4 >> 8;
	TxData[7] = iq4;

   if(HAL_CAN_AddTxMessage(hcan,&TxHeader,TxData,&TxMailBox)!=HAL_OK)
  {
			Error_Handler();
  }
}




