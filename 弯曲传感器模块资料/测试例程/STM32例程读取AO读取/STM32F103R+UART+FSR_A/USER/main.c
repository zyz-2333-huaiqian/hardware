/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * 文件名  ：main.c
 * 描述    ：        
 * 硬件连接：
 * 弯曲传感器模块: VCC -> 5V; GND -> GND; AO -> PA1;
 * USB转串口模块：RX->PA9;TX->PA10;波特率：9600
 * 功能描述：测量电阻值、串口显示测量值；
 * 作者    ：冠拓电子
 * 淘宝    ：https://ilovemcu.taobao.com
**********************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "FSR.h"
#include "usart.h"
#include "adc.h"
#include <stdio.h>
#include <string.h>

int flexADC;
float flexV=0.0;
float flexR=0.0;
float angle=0.0;

const float VCC =3.3; // 模块供电电压，ADC参考电压为V
const float R_DIV =100000.0; // 分压电阻为100KΩ

const float STRAIGHT_RESISTANCE =28320.0; // 平直时的电阻值  需修改
const float BEND_RESISTANCE =48763.0; // 90度弯曲时的电阻值  需修改


long map(long x, long in_min, long in_max, long out_min, long out_max);

int main(void)
{		
	delay_init();	
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
	Adc_Init();

	while(1)
	{
		flexADC = Get_Adc_Average(1,10);	//10次平均值
		flexV=(flexADC*VCC)/4096;
	  flexR= R_DIV * (VCC / flexV-1.0);
		angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,0, 90.0);	
	  printf("Voltage:%f V\r\n",flexV);
		printf("Resistance:%f ohms\r\n",flexR);
		printf("Bend:%f degrees\r\n",angle);
	  delay_ms(500);
	}

}


long map(long x, long in_min, long in_max, long out_min, long out_max) {
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

