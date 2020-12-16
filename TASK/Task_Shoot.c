#include "Task_Shoot.h"
#include "GUN_MOTOR.h"


void Task_Shoot(void *argument)
{
		shoot_init();
		for(;;){
		
				osDelay(3);
		}
		
}

void shoot_init(){
	//初始化摩擦轮
	GUN_MOTOR_SHOOT(30);
}

void shoot_up(){
      
}

void shoot_down(){
				
}
