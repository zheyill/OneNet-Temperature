#include "stm32f10x.h"                  // Device header

#include "driver_JiaShiQi.h"

uint8_t JiaShiQi_status = 0;


/**  
  * @简要  加湿器继电器初始化
  * @参数  	无
  * @注意	无
  * @返回值 无  
  */
void JiaShiQi_init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = JiaShiQi_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(JiaShiQi_GPIO_Port, &GPIO_InitStructure);	
	
	/*设置GPIO初始化后的默认电平*/
	GPIO_WriteBit(JiaShiQi_GPIO_Port, JiaShiQi_GPIO_Pin, Bit_RESET);	
}

/**  
  * @简要  加湿器继电器开
  * @参数  	无
  * @注意	无
  * @返回值 无  
  */
void JiaShiQi_Control_ON(void)
{
	GPIO_WriteBit(JiaShiQi_GPIO_Port, JiaShiQi_GPIO_Pin, Bit_SET);
	JiaShiQi_status = 1;
}

/**  
  * @简要  加湿器继电器关
  * @参数  	无
  * @注意	无
  * @返回值 无  
  */
void JiaShiQi_Control_OFF(void)
{
	GPIO_WriteBit(JiaShiQi_GPIO_Port, JiaShiQi_GPIO_Pin, Bit_RESET);
	JiaShiQi_status = 0;
}
