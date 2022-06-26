/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * 文件名  ：main.c
 * 描述    ：        
 * 硬件连接：
 * 弯曲传感器模块: VCC -> 5V; GND -> GND; DO -> P1.1;
 * 1602液晶显示屏：RS -> P2^5;RW -> P2^6;EN -> P2^7;D0~D7 ->P0口
 * 功能描述：液晶屏显示弯曲次数；

 * 淘宝    ：https://ilovemcu.taobao.com

**********************************************************************************/

#include "main.h"
#include "LCD1602.h"



//定义变量
unsigned char KEY_NUM = 0;   
int count = 0;
int state = 0;
	

//****************************************************
//主函数
//****************************************************
void main()
{
	Init_LCD1602();
	LCD1602_write_com(0x80);
	LCD1602_write_word("welcome to use!");
	while(1)
	{
    	scanSensor();
		if(KEY_NUM == 1)
		{
			KEY_NUM = 0;
			LCD1602_write_com(0x80+0x40);
			LCD1602_write_word("count =");
			LCD1602_write_data(count%10000/1000+0x30);
			LCD1602_write_data(count%1000/100+0x30);
			LCD1602_write_data(count%100/10+0x30);
			LCD1602_write_data(count%10+0x30);

			count++;
			
			if(state == 2)
			  state = 0;
		}

	}
}

void scanSensor()
{
//	SENSOR = 1;
	if(SENSOR == 1)
	{
		Delay_ms(10);
		if(SENSOR == 1) 
		{
		  while(SENSOR == 1);
		  KEY_NUM = 1;
		} 
	}
}


//****************************************************
//MS延时函数
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}

