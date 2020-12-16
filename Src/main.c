/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "System.h"
#include "./remote_control/remote_control.h"
#include "Vision.h"
#include "Task_InfoUpdate.h"
#include "Task_Gimbal.h"
#include "Visual_Scope.h"
#include "./ANO_DT/ANO_DT.h"
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

/* USER CODE BEGIN PV */
uint8_t ano_data_rec;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t originData[5] ={0};	//Debug串口接收到的原始数据
uint8_t DebugRevFlag;

uint8_t USART3_temp[VISION_LENGTH];
uint8_t UART6_temp[11];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART6_UART_Init();
  MX_CAN1_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_SPI5_Init();
  MX_TIM5_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_CAN2_Init();
  MX_UART7_Init();
  MX_USART3_UART_Init();
  MX_UART8_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

  system_Init();
	//HAL_UART_Receive_IT(&huart3,(uint8_t *)USART3_temp,VISION_LENGTH);
	HAL_UART_Receive_IT(&huart8,originData,5); 
	HAL_UART_Receive_IT(&huart6,(uint8_t *)UART6_temp,11); //串口�?螺仪				
	HAL_UART_Receive_IT(&huart3,(uint8_t *)USART3_temp,VISION_LENGTH); //视觉
	HAL_UART_Receive_IT(&huart7,&ano_data_rec,1); //串口7-匿名上位�?
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    while (1)
    {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    UNUSED(huart);
	
		if (huart->Instance == USART3)
		{
			Vision_Read_Data(USART3_temp);
			HAL_UART_Receive_IT(&huart3,(uint8_t *)USART3_temp,VISION_LENGTH);
		}
		
    if (huart->Instance == USART6)
    {
      if(UART6_temp[0]==0x55)       //帧头
			{
				switch(UART6_temp[1])
				{                
					 case 0x51: //标识这个包是加�?�度
							mpu_data.a[0] = ((short)(UART6_temp[3]<<8 | UART6_temp[2]))/32768.0*16;      //X轴加速度
							mpu_data.a[1] = ((short)(UART6_temp[5]<<8 | UART6_temp[4]))/32768.0*16;      //Y轴加速度
							mpu_data.a[2] = ((short)(UART6_temp[7]<<8 | UART6_temp[6]))/32768.0*16;      //Z轴加速度
							mpu_data.T    = ((short)(UART6_temp[9]<<8 | UART6_temp[8]))/340.0+36.25;      //温度
							break;
					 case 0x52: //标识这个包是角加速度
							mpu_data.w[0] = ((short)(UART6_temp[3]<<8| UART6_temp[2]))/32768.0*2000;      //X轴角速度
							mpu_data.w[1] = ((short)(UART6_temp[5]<<8| UART6_temp[4]))/32768.0*2000;      //Y轴角速度
							mpu_data.w[2] = ((short)(UART6_temp[7]<<8| UART6_temp[6]))/32768.0*2000;      //Z轴角速度
							mpu_data.T    = ((short)(UART6_temp[9]<<8| UART6_temp[8]))/340.0+36.25;      //温度
							break;
					 case 0x53: //标识这个包是角度
							mpu_data.real_roll = ((short)(UART6_temp[3]<<8| UART6_temp[2]))/32768.0*180;   //X轴滚转角（x 轴）
							mpu_data.real_pitch = ((short)(UART6_temp[5]<<8| UART6_temp[4]))/32768.0*180;   //Y轴俯仰角（y 轴）
							mpu_data.real_yaw = ((short)(UART6_temp[7]<<8| UART6_temp[6]))/32768.0*180;   //Z轴偏航角（z 轴）
							mpu_data.T        = ((short)(UART6_temp[9]<<8| UART6_temp[8]))/340.0+36.25;   //温度
							//printf("X轴角度：%.2f   Y轴角度：%.2f   Z轴角度：%.2f\r\n",angle[0],angle[1],angle[2]);
							break;
					 default:					 
						 break;
				}
			} 
			HAL_UART_Receive_IT(&huart6,(uint8_t *)UART6_temp,11);
    }
	if(huart->Instance == UART7)
	{
		ANO_DT_Data_Receive_Prepare(ano_data_rec);
		HAL_UART_Receive_IT(&huart7,&ano_data_rec,1);
	}
	if (huart->Instance == UART8)
    {
        DebugRevFlag += 1;
//        HAL_UART_Receive_IT(&huart6, ReadTemp, VISION_LENGTH);
    }
}
int fputc(int ch, FILE *f)
{
      HAL_UART_Transmit(&huart8, (uint8_t *)&ch,1, 0xFF);
      return ch;
}
int fgetc(FILE *f)
{
	uint8_t  ch;
	HAL_UART_Receive(&huart1,(uint8_t *)&ch, 1, 0xFFFF);
	return  ch;
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/