#include "NR_Printf.h"

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  HAL_UART_Transmit(&MY_UART, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
/************************END OF FILE************************/
