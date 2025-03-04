#include "driver_led.h"

#include "stm32f10x.h"     //驱动头文件

/**
  * @简要：led初始化
  * @参数：无
  * @注意：无
  *@返回值 无 
  */
void led_init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LED1_GPIO_Port, &GPIO_InitStructure);		//配置LED1引脚
	
	GPIO_InitStructure.GPIO_Pin = LED2_GPIO_Pin;
	GPIO_Init(LED2_GPIO_Port, &GPIO_InitStructure);	//配置LED2引脚
	
	GPIO_InitStructure.GPIO_Pin = LED3_GPIO_Pin;
	GPIO_Init(LED3_GPIO_Port, &GPIO_InitStructure);	//配置LED3引脚
	
	/*设置GPIO初始化后的默认电平*/
	GPIO_WriteBit(LED1_GPIO_Port, LED1_GPIO_Pin, Bit_RESET);	
	GPIO_WriteBit(LED2_GPIO_Port, LED2_GPIO_Pin, Bit_RESET);
	GPIO_WriteBit(LED3_GPIO_Port, LED3_GPIO_Pin, Bit_RESET);
}

/**  
  * @简要  led设置状态
  * @参数  	uint32_t led：led引脚
			uint8_t status：led状态
  * @注意	无
  * @返回值 无  
  */
void led_set_status(uint32_t led, uint8_t status)
{
	if((led & LED1) == LED1) GPIO_WriteBit(LED1_GPIO_Port, LED1_GPIO_Pin, (BitAction)status);
	if((led & LED2) == LED2) GPIO_WriteBit(LED2_GPIO_Port, LED2_GPIO_Pin, (BitAction)status);
	if((led & LED3) == LED3) GPIO_WriteBit(LED3_GPIO_Port, LED3_GPIO_Pin, (BitAction)status);
}

/**  
  * @简要  led闪烁
  * @参数  	uint32_t led：led引脚
			uint16_t period：led闪烁周期
			uint16_t duty：led闪烁占空比
  * @注意	无
  * @返回值 无  
  */
void led_blink(uint32_t led, uint16_t period, uint8_t duty)
{
	static uint16_t tick = 0;
	uint16_t duty_cycle = period / 100.0 * duty;
	
	if(tick >= period) tick = 0;
	if(tick <= duty_cycle) 
		led_set_status(led, LED_ON);	
	else  
		led_set_status(led, LED_OFF);
	tick++;
}

