#ifndef __driver_JiaShiQi__
#define __driver_JiaShiQi__

#include <stdint.h>

/********** led1引脚定义 **********/
#define 	JiaShiQi_GPIO_Port 		GPIOB				
#define 	JiaShiQi_GPIO_Pin		GPIO_Pin_14

extern uint8_t  JiaShiQi_status;

void JiaShiQi_init(void);
void JiaShiQi_Control_ON(void);
void JiaShiQi_Control_OFF(void);

#endif
