#include "stm32f10x.h"                  // Device header

#include "driver_ChuangLian.h"

uint8_t  ChuangLian_status = 0;

/**  
  * @简要  窗帘继电器初始化
  * @参数  	无
  * @注意	无
  * @返回值 无  
  */
void ChuangLian_init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = ChuangLian_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(ChuangLian_GPIO_Port, &GPIO_InitStructure);	
	
	/*设置GPIO初始化后的默认电平*/
	GPIO_WriteBit(ChuangLian_GPIO_Port, ChuangLian_GPIO_Pin, Bit_RESET);	
}

/**  
  * @简要  窗帘继电器开
  * @参数  	无
  * @注意	无
  * @返回值 无  
  */
void ChuangLian_Control_ON(void)
{
	GPIO_WriteBit(ChuangLian_GPIO_Port, ChuangLian_GPIO_Pin, Bit_SET);
	ChuangLian_status = 1;
}

/**  
  * @简要  窗帘继电器关
  * @参数  	无
  * @注意	无
  * @返回值 无  
  */
void ChuangLian_Control_OFF(void)
{
	GPIO_WriteBit(ChuangLian_GPIO_Port, ChuangLian_GPIO_Pin, Bit_RESET);
	ChuangLian_status = 0;
}
