#ifndef __SYSTEM_H
#define	__SYSTEM_H
//System.c负责参数初始化，外设初始化
#include <stdio.h>
#include "stdbool.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "mytype.h"
//#include "Vision.h"

#define abs(x) ((x)>0? (x):(-(x)))
int32_t constrain_int32_t(int32_t amt, int32_t low, int32_t high);
int16_t constrain_int16_t(int16_t amt, int16_t low, int16_t high);
float constrain_float(float amt, float low, float high);
int constrain(int amt, int low, int high);

void system_disable(void);
void system_enable(void);
void system_Init(void);
#endif 



