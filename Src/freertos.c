/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "./ANO_DT/ANO_DT.h"
#include "Task_Chassis.h"
#include "Task_Gimbal.h"
#include "Task_Shoot.h"
#include "Task_InfoUpdate.h"
#include "System.h"
#include "./remote_control/remote_control.h"
#include "Robo_config.h"
#include "Visual_Scope.h"
#include "Vision.h"
#include "can_receive.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern ROBO_STATUS roboStatus;
/* USER CODE END Variables */
/* Definitions for TASK_10ms */
osThreadId_t TASK_10msHandle;
const osThreadAttr_t TASK_10ms_attributes = {
  .name = "TASK_10ms",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for sendData */
osThreadId_t sendDataHandle;
const osThreadAttr_t sendData_attributes = {
  .name = "sendData",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for TASK_500ms */
osThreadId_t TASK_500msHandle;
const osThreadAttr_t TASK_500ms_attributes = {
  .name = "TASK_500ms",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for TASK_Chassis */
osThreadId_t TASK_ChassisHandle;
const osThreadAttr_t TASK_Chassis_attributes = {
  .name = "TASK_Chassis",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for TASK_Gimbal */
osThreadId_t TASK_GimbalHandle;
const osThreadAttr_t TASK_Gimbal_attributes = {
  .name = "TASK_Gimbal",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for TASK_Shoot */
osThreadId_t TASK_ShootHandle;
const osThreadAttr_t TASK_Shoot_attributes = {
  .name = "TASK_Shoot",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
/* Definitions for TASK_InfoUpdate */
osThreadId_t TASK_InfoUpdateHandle;
const osThreadAttr_t TASK_InfoUpdate_attributes = {
  .name = "TASK_InfoUpdate",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void Task_10ms(void *argument);
void StartSendData(void *argument);
void Task_500ms(void *argument);
extern void Task_Chassis(void *argument);
extern void Task_Gimbal(void *argument);
extern void Task_Shoot(void *argument);
extern void Task_InfoUpdate(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	   
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of TASK_10ms */
  TASK_10msHandle = osThreadNew(Task_10ms, NULL, &TASK_10ms_attributes);

  /* creation of sendData */
  sendDataHandle = osThreadNew(StartSendData, NULL, &sendData_attributes);

  /* creation of TASK_500ms */
  TASK_500msHandle = osThreadNew(Task_500ms, NULL, &TASK_500ms_attributes);

  /* creation of TASK_Chassis */
  TASK_ChassisHandle = osThreadNew(Task_Chassis, NULL, &TASK_Chassis_attributes);

  /* creation of TASK_Gimbal */
  TASK_GimbalHandle = osThreadNew(Task_Gimbal, NULL, &TASK_Gimbal_attributes);

  /* creation of TASK_Shoot */
  TASK_ShootHandle = osThreadNew(Task_Shoot, NULL, &TASK_Shoot_attributes);

  /* creation of TASK_InfoUpdate */
  TASK_InfoUpdateHandle = osThreadNew(Task_InfoUpdate, NULL, &TASK_InfoUpdate_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Task_10ms */
/**
* @brief Function implementing the TASK_10ms thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_10ms */
void Task_10ms(void *argument)
{
  /* USER CODE BEGIN Task_10ms */
  /* Infinite loop */
  for(;;)
  {
		if(roboStatus.control_mode == Disable_mode){
			system_disable();
		}
		else{
			system_enable();
		}
	
		//10ms
		osDelay(10);
  }
  /* USER CODE END Task_10ms */
}

/* USER CODE BEGIN Header_StartSendData */
/**
* @brief Function implementing the sendData thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSendData */
void StartSendData(void *argument)
{
  /* USER CODE BEGIN StartSendData */
  /* Infinite loop */
  for(;;)
  {
//		 ANO_DT_Display(moto_chassis[0].speed_rpm,
//					-moto_chassis[1].speed_rpm, 
//					moto_chassis[2].speed_rpm,
//					-moto_chassis[3].speed_rpm,
//					set_spd[0],
//					-set_spd[1],
//					set_spd[2], 
//					-set_spd[3],0);
				 ANO_DT_Display(
					VisionRecvData.time,
					cur_pit[GIMBAL_Below]*100, 
				  set_pit[GIMBAL_Below]*100,
					VisionRecvData.yaw_angle*100,
					cur_yaw[GIMBAL_Below]*100, //5
					(cur_yaw[GIMBAL_Below]+VisionRecvData.yaw_angle)*100,
					set_yaw[GIMBAL_Below]*100, 
					(cur_pit[GIMBAL_Below]+VisionRecvData.pitch_angle)*100,
					0);
		//ANO_DT_Send_Udata(moto_chassis[3].angle,moto_chassis[3].speed_rpm,4,5,6,7,8,9,0);
		//VisualDisplay(mpu_data.yaw, mpu_data.pitch, mpu_data.roll, 0);  //bug待修 坐标VisualScope_Output串口发�?�处
		osDelay(10);
  }
  /* USER CODE END StartSendData */
}

/* USER CODE BEGIN Header_Task_500ms */
/**
* @brief Function implementing the TASK_500ms thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_500ms */
void Task_500ms(void *argument)
{
  /* USER CODE BEGIN Task_500ms */
  /* Infinite loop */
	uint8_t SendData[VISION_TX_LENGTH];
  for(;;)
  {
			VisionSendData.SOF = 0xA5;
			VisionSendData.CmdID = 0x01;
			//memcpy(&VisionSendData.yaw_angle, &cur_yaw[GIMBAL_Below], 4);
//			memcpy(&VisionSendData.pitch_angle, &cur_pit[GIMBAL_Below], 4);
		  VisionSendData.yaw_angle = cur_yaw[GIMBAL_Below];
			VisionSendData.pitch_angle = cur_pit[GIMBAL_Below];
			VisionSendData.yaw_speed = moto_yaw[GIMBAL_Below].speed_rpm;		 
			VisionSendData.pitch_speed = moto_pit[GIMBAL_Below].speed_rpm;
			VisionSendData.target_mode = 0; //����ģʽ{ 0:��ͨ���� 1:���� 2:��������}
			//Ԥ��
			VisionSendData.blank_a = 0;
			VisionSendData.blank_b = 0;
			VisionSendData.blank_c= 0;
			VisionSendData.blank_e= 0;
			VisionSendData.blank_f= 0;
			VisionSendData.TOF = 0xA6;
			memcpy(SendData, &VisionSendData, VISION_TX_LENGTH);
			HAL_UART_Transmit(&huart6, SendData, VISION_TX_LENGTH, 0xFF);
			//1ms
			osDelay(1);
  }
  /* USER CODE END Task_500ms */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
