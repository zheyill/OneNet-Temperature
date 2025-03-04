#ifndef __driver_dht11__
#define __driver_dht11__

#include <stdint.h>

typedef struct
{
	float temp;
	uint8_t humi;
}DHT11_TypeDef;

/********** dht11引脚定义 **********/				
#define 	DHT11_GPIO_Port 		GPIOB			
#define 	DHT11_GPIO_Pin			GPIO_Pin_3 

extern DHT11_TypeDef dht11_data;

/**  
  * @简要  dht11引脚，状态初始化
  */ 
uint8_t dht11_init(void);

/**  
  * @简要  向DHT11的数据引脚写入电平状态
  */  
static void dht11_write_pin(uint8_t PinState);
  
/**  
  * @简要  从DHT11的数据引脚读取电平状态  
  */  
static uint8_t dht11_read_pin(void);

/**  
  * @简要  启动DHT11传感器通信 
  */  
static uint8_t dht11_read_start(void);

/**  
  * @简要  从DHT11读取一个字节的数据 
  */  
static uint8_t dht11_read_byte(void);

/**  
  * @简要  从DHT11读取数据
  */  
static void dht11_read_data(void);

/**  
  * @简要  获得dht11温湿度数据
  */ 
void dht11_get_data(DHT11_TypeDef *data);

#endif
