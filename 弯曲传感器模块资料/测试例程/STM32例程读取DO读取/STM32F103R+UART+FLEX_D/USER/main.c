/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * 文件名  ：main.c
 * 描述    ：        
 * 硬件连接：
 * 弯曲传感器模块: VCC -> 3.3V; GND -> GND; DO -> PC15;
 * USB转串口模块：RX->PA9;TX->PA10;波特率：9600
 * 功能描述：测量电阻值、串口显示测量值；
 * 淘宝    ：https://ilovemcu.taobao.com
**********************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "FLEX.h"
#include "usart.h"

u8 state = 0;

int main(void)
{		
	delay_init();	
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
	FLEX_IO_Init();

	delay_ms(1000);

	printf("Test start\r\n");
	while(1)
	{
		if(FLEX_Scan(1) == 1 && state == 0)
		{
				printf("大于阀值\r\n");
				state = 1;
		}
		
		if(FLEX_Scan(1) == 0 && state == 1)
		{
				printf("低于阀值\r\n");
				state = 0;
		}
			
		delay_ms(500);
	}

}

