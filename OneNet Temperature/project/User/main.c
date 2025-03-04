#include "stm32f10x.h"                  // Device header
#include "driver_wireless.h"
#include "driver_dht11.h"
#include "driver_JiaShiQi.h"
#include "driver_ChuangLian.h"
#include "driver_DiaoDeng.h"
#include "driver_key.h"
#include "timer.h"
#include "serial.h"
#include "delay.h"
#include <stdio.h>


#define WIRELESS_SEND_TIME 	3000	//上报数据时间间隔（单位：毫秒）
#define SENSOR_READ_TIME	3000    //获取传感器数据时间间隔（单位：毫秒）
#define KEY_SCAN_TIME		20    	//按键扫描（单位：毫秒）

uint8_t wireless_send_flag = 0;     //上报数据标志位
uint8_t sensor_read_flag = 0;		//读取传感器数据标志位

int main(void)
{
	Serial_Init(115200);            //初始化串口1，用来打印系统运行状态与事件
	timer2_init(100, 720);          //定时器二定时中断初始化，用来定时执行任务
	wireless_init();                //无线模块初始化并连接onenet mqtt服务器
	JiaShiQi_init();                //加湿器继电器初始化
	ChuangLian_init();              //窗帘继电器初始化
	DiaoDeng_init(100, 720);		//吊灯PWM控制初始化
	dht11_init();                   //温湿度传感器dht11初始化
	key->init();
	while(1)
	{
		KeyNum key_value = key->get_number();		//获取按键键值
		if(key_value.once & KEY2_ONCE)       //单击按键一
		{
			JiaShiQi_Control_ON();           //打开加湿器
		}
		if(key_value.more & KEY2_MORE)       //长按按键二
		{
			JiaShiQi_Control_OFF();          //关闭加湿器
		}
		if(sensor_read_flag == 1)			//读取传感器数据标志位
		{
			sensor_read_flag = 0;
			dht11_get_data(&dht11_data);	//获取温湿度数据
		}
		
		if(wireless_send_flag == 1) 		//数据上报标志位
		{
			wireless_send_flag = 0;
			wireless_system_handler();		//执行无线模块相关事件
			wireless_onenet_data_handler();	//处理有关onenet的数据
			if((WirelessStatus.error_code & ERROR_MQTT_CONNECT) == 0)  wireless_publish_data(); 	//发布数据测试	
		}		
		if(wireless_get_receive_flag() == W_OK)		//无线模块接收到数据
		{
			wireless_receive_data_handler();		//接收数据处理函数
		}		
	}
}

/**
  * @简要：USART1中断函数
  * @参数：无
  * @注意：此函数为中断函数，无需调用，中断触发后自动执行
  *        函数名为预留的指定名称，可以从启动文件复制
  *        请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  *@返回值 无 
  */
void USART1_IRQHandler(void)
{
	uint8_t data = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)		
	{
		data = USART_ReceiveData(USART1);
		usart1_receive_callback(data);			
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);																	
	}
}

/**  
  * @简要  串口2接收中断服务函数
  * @参数  	无
  * @注意	将无线模块接收函数放在此中断的接收中
  * @返回值 无  
  */
void USART2_IRQHandler(void)
{
	uint8_t data = 0;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)		
	{
		data = USART_ReceiveData(USART2);	
		//Serial_SendByte(data);           //取消注释可以将esp8266发送的数据通过串口一打印
		wireless_receive_callback(data);	/* 将无线模块接收数据函数放在串口接收中断中，接收数据 */		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);																	
	}
}

/**  
  * @简要  定时器2中断服务函数
  * @参数  	1ms
  * @注意	定时设置标志位 ,
  * @返回值 无  
  */
void TIM2_IRQHandler(void)
{
	static uint16_t wireless_send_timer = 0;
	static uint16_t sensor_read_timer = 0;
	static uint16_t key_scan_timer = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		if(wireless_send_timer++ == WIRELESS_SEND_TIME) {wireless_send_timer = 0;wireless_send_flag = 1;}
		if(sensor_read_timer++ == SENSOR_READ_TIME) {sensor_read_timer = 0;sensor_read_flag = 1;}
		if(key_scan_timer++ == KEY_SCAN_TIME){key_scan_timer = 0; key->callback();}
		key->get_tick(1);
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
