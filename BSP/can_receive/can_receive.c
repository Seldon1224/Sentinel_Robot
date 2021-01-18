#include "can_receive.h"
#include "Task_Gimbal.h"
moto_measure_t moto_base[DJnumber]; //电机总数
moto_measure_t moto_chassis;		//chassis moto
moto_measure_t moto_pit[2];
moto_measure_t moto_yaw[2];
moto_measure_t moto_revolve[2];
moto_measure_t moto_info;

/*******************************************************************************************
  * @Func		  my_can_filter_init
  * @Brief    CAN1和CAN2滤波器配置
*/
void my_can_filter_init_recv_all(CAN_HandleTypeDef *_hcan)
{
	//can1 &can2 use same filter config
	CAN_FilterTypeDef CAN_FilterConfigStructure;

	if (_hcan == &hcan1)
	{
		CAN_FilterConfigStructure.FilterBank = 0;
	}
	else if (_hcan == &hcan2)
	{
		CAN_FilterConfigStructure.FilterBank = 14;
	}

	CAN_FilterConfigStructure.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_FilterConfigStructure.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_FilterConfigStructure.FilterIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterConfigStructure.SlaveStartFilterBank = 14;
	CAN_FilterConfigStructure.FilterActivation = ENABLE;

	if (HAL_CAN_ConfigFilter(_hcan, &CAN_FilterConfigStructure) != HAL_OK)
	{
		Error_Handler();
	}

	//filter config for can2
	//can1(0-13)和can2(14-27)分别得到一半的filter
	if (HAL_CAN_ConfigFilter(_hcan, &CAN_FilterConfigStructure) != HAL_OK)
	{
		/* Filter configuration Error */
		Error_Handler();
	}

	/* Start the CAN peripheral */
	if (HAL_CAN_Start(_hcan) != HAL_OK)
	{
		/* Start Error */
		Error_Handler();
	}

	/* Activate CAN RX notification */
	if (HAL_CAN_ActivateNotification(_hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		/* Notification Error */
		Error_Handler();
	}

	/* Activate CAN TX notification */
	if (HAL_CAN_ActivateNotification(_hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
	{
		/* Notification Error */
		Error_Handler();
	}
}

//void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* _hcan)
//这是一个回调函数
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint8_t Data[8];
	HAL_StatusTypeDef HAL_RetVal;
	if (hcan == &hcan1)
	{
		HAL_RetVal = HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, Data);
		if (HAL_OK == HAL_RetVal)
		{
			switch (RxHeader.StdId)
			{
			case CAN_3508Moto1_ID: //0
			case CAN_3508Moto2_ID: //1
			case CAN_3508Moto3_ID: //2
			case CAN_3508Moto4_ID: //3
			case CAN_6020Moto1_ID: //4
			case CAN_6020Moto2_ID: //5
			case CAN_6020Moto3_ID: //6
			case CAN_6020Moto4_ID: //7
			case CAN_6020Moto5_ID: //8
			case CAN_6020Moto6_ID: //9
			case CAN_6020Moto7_ID: //10
			{
				static u8 i;
				i = RxHeader.StdId - CAN_3508Moto1_ID;
				moto_base[i].msg_cnt++ <= 1 ? set_moto_offset() : get_moto_measure(&moto_base[i], Data);
				get_moto_measure(&moto_info, Data);
			}
			break;
			case ID_supply_projectile_action_t:
			{
				get_supply_projectile_action(&SupplyProjectileAction, Data);
			}
			break;
			case ID_game_robot_state_one_t:
			{
				get_game_robot_state_one(&GameRobotStat, Data);
			}
			break;
			case ID_game_robot_state_two_t:
			{
				get_game_robot_state_two(&GameRobotStat, Data);
			}
			break;
			case ID_game_robot_state_three_t:
			{
				get_game_robot_state_three(&GameRobotStat, Data);
			}
			break;
			case ID_power_heat_data_one_t:
			{
				get_power_heat_data_one(&PowerHeatData, Data);
			}
			break;
			case ID_power_heat_data_two_t:
			{
				get_power_heat_data_two(&PowerHeatData, Data);
			}
			break;
			case ID_robot_hurt_t:
			{
				get_robot_hurt(&RobotHurt, Data);
			}
			break;
			case ID_shoot_data_t:
			{
				get_shoot_data(&ShootData, Data);
			}
			break;
			case ID_rfid_status_t:
			{
				get_rfid_status(&RFIDState, Data);
			}
			break;
			}
			//电机对应关系
			moto_chassis = moto_base[0];			   //底盘
			moto_pit[GIMBAL_Below] = moto_base[7];	   //云台竖
			moto_yaw[GIMBAL_Below] = moto_base[6];	   //云台横xd
			moto_pit[GIMBAL_Above] = moto_base[5];	   //云台竖xd  //暂时改 之前为9
			moto_yaw[GIMBAL_Above] = moto_base[8];	   //云台横xd
			moto_revolve[GIMBAL_Below] = moto_base[1]; //下拨盘xd
			moto_revolve[GIMBAL_Above] = moto_base[6]; //上拨盘
			/*#### add enable can it again to solve can receive only one ID problem!!!####**/
			__HAL_CAN_ENABLE_IT(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
		}
	}
}

//void get_moto_measure(moto_measure_t *ptr, CAN_HandleTypeDef* hcan)
// 接收电机通过CAN发过来的信息
void get_moto_measure(moto_measure_t *ptr, uint8_t *Data)
{
	ptr->last_angle = ptr->angle;
	ptr->angle = (uint16_t)(Data[0] << 8 | Data[1]);
	ptr->real_current = (int16_t)(Data[2] << 8 | Data[3]);
	ptr->speed_rpm = ptr->real_current; //这里是因为两种电调对应位不一样的信息
	ptr->given_current = (int16_t)(Data[4] << 8 | Data[5]) / -5;
	ptr->hall = Data[6];
	if (ptr->angle - ptr->last_angle > 4096)
		ptr->round_cnt--;
	else if (ptr->angle - ptr->last_angle < -4096)
		ptr->round_cnt++;
	// if (ptr == moto_base + 4 || ptr == moto_base + 5)
	// 	ptr->total_angle = ptr->angle - ptr->offset_angle;
	// else if (ptr == moto_base + 0 || ptr == moto_base + 1 || ptr == moto_base + 2 || ptr == moto_base + 3)
	ptr->total_angle = ptr->angle + ptr->round_cnt * 8191;
	//上下云台电机的angle减去offset值
	for (u8 i = 0; i < 4; i++)
	{
		if (ptr == moto_base + 6 + i)
			ptr->total_angle = ptr->total_angle - ptr->offset_angle;
	}
}

/*this function should be called after system+can init */

void set_moto_offset()
{
	moto_base[6].offset_angle = YAW_BELOW_MID_POS;
	moto_base[7].offset_angle = PIT_BELOW_MID_POS;
	moto_base[8].offset_angle = YAW_ABOVE_MID_POS;
	moto_base[9].offset_angle = PIT_ABOVE_MID_POS;
}
void set_moto_current_all(CAN_HandleTypeDef *hcan, int16_t mark, int16_t iq1, int16_t iq2, int16_t iq3, int16_t iq4)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData[8];
	if (mark == 0)
	{
		TxHeader.StdId = 0x1FF; //620，610：5-8;   6020：1-4
	}
	else if (mark == 1)
	{
		TxHeader.StdId = 0x2FF; //6020：5-8
	}
	else if (mark == 2)
	{
		TxHeader.StdId = 0x200; //620，610：1-4
	}
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

	if (HAL_CAN_AddTxMessage(hcan, &TxHeader, TxData, (uint32_t *)CAN_TX_MAILBOX1) != HAL_OK)
	{
		printf("WRONG\r\n");
		Error_Handler();
	}
}
