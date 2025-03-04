#include "stm32f10x.h"                  // Device header
#include "driver_key.h"
#include "stdio.h"

/**
  * @简要   配置按键信息
  */
#define NUM_KEYS 2              //按键个数
#define DOUBLE_CLICK_TIME  250  // 双击最大间隔时间（ms）  
#define LONG_PRESS_TIME  500  	// 长按最小持续时间（ms）

/**
  * @简要   函数声明
  */
void key_scan(void);
void key_init(void);
uint32_t key_get_tick_1ms(uint8_t value);
KeyNum key_get_number(void);

/**
  * @简要   GPIO端口
  */  
const GPIO_TypeDef* key_GPIO_Ports[NUM_KEYS] = 
{  
    KEY1_GPIO_Port,KEY2_GPIO_Port
};  

/**
  * @简要   GPIO引脚
  */
const uint16_t key_GPIO_Pins[NUM_KEYS] = 
{  
    KEY1_Pin,KEY2_Pin
};

/**
  * @简要   按键状态定义  
  */ 
typedef enum 
{  
    KEY_RELEASED,  				//松开
    KEY_PRESSED,  				//按下
    KEY_SINGLE_CLICK,  			//单击
    KEY_DOUBLE_CLICK,  			//双击
    KEY_LONG_PRESS  			//长按
} Key_State; 
 
/**
  * @简要   按键结构体定义     
  */ 
typedef struct 
{  
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_PIN;              // 按键连接的GPIO引脚  
	Key_State state;         		// 按键状态  
	uint32_t press_time;       		// 按下时间  
	uint32_t release_time;    		// 释放时间 
	uint8_t click_count;           	// 连续点击次数  
	uint32_t num;					// 按键键值
} Key_TypeDef;  

/**
  * @简要   按键函数指针     
  */ 
const Key_Handler *key = &(const Key_Handler)
{
    .get_tick = key_get_tick_1ms,				//获取系统时间滴答
    .init = key_init,					//按键初始化
    .callback = key_scan,				//按键扫描回调函数
	.get_number = key_get_number,		//获取键值
};

/**
  * @简要 按键数组     
  */ 
static Key_TypeDef keys[NUM_KEYS]; 

/**
  * @简要   按键数值初始化     
  */ 
KeyNum key_num = {0,0,0};


/**
  * @简要   初始化按键配置
  * @说明   该函数对每个按键的GPIO端口和引脚进行初始化，并将按键状态设置为未按下
  * @参数   无
  * @返回值 无
  */
void key_init(void) 
{  
		/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = KEY1_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(KEY1_GPIO_Port, &GPIO_InitStructure);		//配置KEY1引脚
	
	GPIO_InitStructure.GPIO_Pin = KEY2_Pin;
	GPIO_Init(KEY2_GPIO_Port, &GPIO_InitStructure);	//配置KEY2引脚

	
    for (int i = 0; i < NUM_KEYS; i++) 
	{  
        keys[i].GPIOx = (GPIO_TypeDef*)key_GPIO_Ports[i];  
        keys[i].GPIO_PIN = key_GPIO_Pins[i];  
        keys[i].state = KEY_RELEASED;  
        keys[i].click_count = 0;  
		keys[i].num = 0x01 << i;
    }  
}  

/**
  * @简要   按键时钟tick
  * @说明   每1ms执行一次
  * @参数   自增tick，根据需求填写，默认填1或者0
  * @返回值 tick
  */
uint32_t key_get_tick_1ms(uint8_t value)
{
	static uint32_t tick = 0;
	tick += value;
	return tick;
}

/**
  * @简要   定时器扫描按键
  * @说明   定时器消抖扫描并检测按键状态
  * @参数   建议20ms执行一次
  * @返回值 无
  */
void key_scan(void) {  
    uint32_t current_time = key->get_tick(0);  // 获取当前时间  
    for (int i = 0; i < NUM_KEYS; i++) 	//遍历所有按键
	{  
		Key_TypeDef *key = &keys[i];  
		uint8_t current_state = GPIO_ReadInputDataBit(key->GPIOx, key->GPIO_PIN);  // 读取按键状态 
        if (current_state == 0) 	// 按键按下
		{    
            if (key->state == KEY_RELEASED) 	// 如果之前是松开状态
			{  
                key->press_time = current_time;  // 记录按下时间
                key->state = KEY_PRESSED;  	//更新按键状态为按下
            }  	
        } 
		else  // 按键释放 
		{   
            if (key->state == KEY_PRESSED) // 如果之前是按下状态
			{  
                key->release_time = current_time;  // 记录释放时间

                uint32_t press_duration = key->release_time - key->press_time;   // 计算按下持续时间

                if (press_duration >= LONG_PRESS_TIME) // 如果按下时间超过长按阈值
				{  
                    key->state = KEY_LONG_PRESS; // 更新状态为长按
                    key_num.more |= keys[i].num;	// 标记长按事件
                } 
				else //如果按下时间在长按阈值范围内
				{  
                    key->click_count++;  // 增加点击计数
                }  
                // 复位按键状态  
                key->state = KEY_RELEASED;  
            }  
        }
		
        if (key->click_count)  // 如果有点击计数
		{
            // 距离下一次按下时间大于 DOUBLE_CLICK_TIME 可认为是单击
            if (key->click_count == 1 && current_time - key->release_time > DOUBLE_CLICK_TIME) 
			{
                key->click_count = 0;  		// 重置点击计数
                key_num.once |= keys[i].num;			// 标记单击事件
            }
            // 否则 在 DOUBLE_CLICK_TIME 时间段内按几下算几连击
            else if (key->click_count >= 2 && current_time - key->release_time > DOUBLE_CLICK_TIME)
			{
                key->click_count = 0;   // 重置点击计数
                key_num.twice |= keys[i].num;	// 标记双击事件
            }                                   
        }
    }  
}  

/**
  * @简要   获取按键状态
  * @说明   返回当前各类按键的键值
  * @参数   无
  * @返回值 按键的键值
  */
KeyNum key_get_number(void) 
{
    KeyNum temp = key_num;
    key_num.once = 0;
    key_num.twice = 0;
    key_num.more = 0;
    return temp;
}
