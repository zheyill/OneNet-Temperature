#include "stm32f10x.h"                  // Device header

#include "driver_DiaoDeng.h"
#include "timer.h"

uint8_t DiaoDeng_status = 0;

/**  
  * @简要  吊灯PWM脉宽初始化
  * @参数  	uint16_t period:周期
			uint16_t prescaler:分频系数
  * @注意	无
  * @返回值 无  
  */
void DiaoDeng_init(uint16_t period, uint16_t prescaler)
{
	time1_pwm_init(period, prescaler);
	DiaoDeng_Control_PWM(0);
}

/**  
  * @简要  吊灯PWM控制
  * @参数  	uint8_t duty：占空比（0~100）
  * @注意	无
  * @返回值 无  
  */
void DiaoDeng_Control_PWM(uint8_t duty)
{
	DiaoDeng_status = duty;
	TIM_SetCompare4(TIM1, duty);		//设置CCR1的值
}

