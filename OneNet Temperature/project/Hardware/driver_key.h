#ifndef __driver_key__
#define __driver_key__


#include <stdint.h>

#define KEY1_ONCE (0x01 << 0)
#define KEY2_ONCE (0x01 << 1)

#define KEY1_TWICE (0x01 << 0)
#define KEY2_TWICE (0x01 << 1)

#define KEY1_MORE (0x01 << 0)
#define KEY2_MORE (0x01 << 1)

#define KEY1_GPIO_Port     	GPIOA
#define KEY1_Pin           	GPIO_Pin_8
			
#define KEY2_GPIO_Port    	GPIOB
#define KEY2_Pin          	GPIO_Pin_10


typedef struct{
	uint32_t once;		//单击
	uint32_t twice;		//双击
	uint32_t more;		//长按
}KeyNum;

extern KeyNum key_num;
// 按键处理函数结构体定义  
typedef struct {
	KeyNum (*get_number)(void);		//获得按键状态
    uint32_t (*get_tick)(uint8_t);         // 获取系统时间的函数指针
    void (*init)(void);                 // 初始化函数指针
    void (*callback)(void);             // 回调函数指针
} Key_Handler;

extern const Key_Handler *key;


#endif
