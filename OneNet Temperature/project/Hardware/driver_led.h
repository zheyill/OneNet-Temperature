#ifndef __driver_led__
#define __driver_led__

#include <stdint.h>

#define 	LED1 		(0x01 << 0)
#define 	LED2 		(0x01 << 1)
#define 	LED3 		(0x01 << 2)
	
#define 	LED_ON		1
#define 	LED_OFF		0 	

/********** led1引脚定义 **********/
#define 	LED1_GPIO_Port 		GPIOB				
#define 	LED1_GPIO_Pin		GPIO_Pin_11 	

/********** led2引脚定义 **********/				
#define 	LED2_GPIO_Port 		GPIOB			
#define 	LED2_GPIO_Pin		GPIO_Pin_14 

/********** led3引脚定义 **********/				
#define 	LED3_GPIO_Port 		GPIOA			
#define 	LED3_GPIO_Pin		GPIO_Pin_11 	

void led_init(void);
void led_set_status(uint32_t led, uint8_t status);
void led_blink(uint32_t led, uint16_t period, uint8_t duty);

#endif
