#include "driver_dht11.h"
#include "delay.h"
#include "stm32f10x.h"     //驱动头文件

/**
  * @简要：dht11数据结构体
  * @注意：无
  */
DHT11_TypeDef dht11_data = {0, 0};

/**
  * @简要：dht11单总线读取5个8位数据
  * @注意：无
  */
static uint8_t humidity_high, humidity_low, temperature_high, temperature_low, check_data;

/**  
  * @简要  dht11引脚，状态初始化  
  * @参数  无
  * @返回值 0：初始化成功
			1：初始化失败
  */ 
uint8_t dht11_init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOB的时钟
	
	/*由于PB3默认JTAG功能，这里需要复用为普通的GPIO口*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); // 关闭JTAG功能，只使用SWD调试
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;		//配置开漏输出
	GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStructure);		//配置dht11引脚	
	return dht11_read_start();
}

/**  
  * @简要  向DHT11的数据引脚写入电平状态  
  * @参数  PinState：要写入的数据引脚状态
  * @返回值 无  
  */  
static void dht11_write_pin(uint8_t PinState)  
{  
	GPIO_WriteBit(DHT11_GPIO_Port, DHT11_GPIO_Pin, (BitAction)PinState);  
}  
  
/**  
  * @简要  从DHT11的数据引脚读取电平状态
  * @参数  无
  * @返回值 读取到的数据引脚电平状态（0 或 1）  
  */  
static uint8_t dht11_read_pin(void)  
{  
	return GPIO_ReadInputDataBit(DHT11_GPIO_Port, DHT11_GPIO_Pin);  
} 

/**  
  * @简要  启动DHT11传感器通信
  * @参数  无
  * @返回值 0 表示成功启动通信，1 表示启动失败  
  */  
static uint8_t dht11_read_start(void)  
{  
	uint16_t timeOut = 65535;     //超时时间
	uint8_t ACK = 1;  
	dht11_write_pin(0);  	delay_ms(20);  
	dht11_write_pin(1);   	delay_us(30);  
	ACK = dht11_read_pin();  
	if(ACK == 0)  
	{  
		while(dht11_read_pin() == 0 && timeOut) timeOut--; 
		if(timeOut == 0) ACK = 1;
		timeOut = 65535;		
		while(dht11_read_pin() == 1 && timeOut)	timeOut--;  
		if(timeOut == 0) ACK = 1;
	}  
	return ACK; 
}

/**  
  * @简要  从DHT11读取一个字节的数据
  * @参数  无
  * @返回值 读取到的字节数据  
  */  
static uint8_t dht11_read_byte(void)  
{  
	uint8_t Byte = 0x00;  
	uint8_t i = 0;  
	for(i=0;i<8;i++)  
	{  
		while(dht11_read_pin() == 0);  
		delay_us(40);  
		if(dht11_read_pin() == 1) Byte |= (0x80 >> i);  
		while(dht11_read_pin() == 1);  
	}  
	return Byte;  
} 

/**  
  * @简要  从DHT11读取数据
  * @参数  无
  * @返回值 无  
  */  
static void dht11_read_data(void)  
{  
	if(dht11_read_start() == 0)  
	{  
		humidity_high = dht11_read_byte();  
		humidity_low = dht11_read_byte();  
		temperature_high = dht11_read_byte();  
		temperature_low = dht11_read_byte();  
		check_data = dht11_read_byte();  
	}
	
	if((humidity_high + humidity_low + temperature_high + temperature_low) != check_data )        //如果数据不对
	{
		humidity_high = humidity_low = temperature_high = temperature_low = check_data = 0;
	}
	delay_us(54); // 等待DHT11数据发送完毕   
} 

/**  
  * @简要  获得dht11温湿度数据
  * @参数  DHT11_TypeDef *data：数据结构体
  * @返回值 无  
  */ 
void dht11_get_data(DHT11_TypeDef *data)
{
	dht11_read_data();
	data->temp = ((temperature_high * 10.0f) +  temperature_low)	/ 10.0f;
	data->humi = humidity_high;
}

