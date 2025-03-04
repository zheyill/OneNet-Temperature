#ifndef __driver_ChuangLian__
#define __driver_ChuangLian__

#include <stdint.h>

/********** led1引脚定义 **********/
#define 	ChuangLian_GPIO_Port 		GPIOB				
#define 	ChuangLian_GPIO_Pin			GPIO_Pin_11 

extern uint8_t  ChuangLian_status;

void ChuangLian_init(void);
void ChuangLian_Control_ON(void);
void ChuangLian_Control_OFF(void);

#endif
