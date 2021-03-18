#ifndef _NR_PRINTF_H_
#define _NR_PRINTF_H_

#ifdef __STM32F1xx_HAL_H
#include "stm32f1xx_hal.h"
#endif

#ifdef __STM32F4xx_HAL_H
#include "stm32f4xx_hal.h"
#endif

#include <stdio.h>
#include "usart.h"

#define MY_UART huart3

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#endif
/************************END OF FILE************************/
