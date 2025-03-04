#ifndef __driver_DiaoDeng__
#define __driver_DiaoDeng__

#include <stdint.h>

/********** led1引脚定义 **********/
#define 	DiaoDeng_GPIO_Port 		GPIOA				
#define 	DiaoDeng_GPIO_Pin		GPIO_Pin_11

extern uint8_t DiaoDeng_status;

void DiaoDeng_init(uint16_t period, uint16_t prescaler);
void DiaoDeng_Control_PWM(uint8_t duty);

#endif
