/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * �ļ���  ��main.c
 * ����    ��        
 * Ӳ�����ӣ�
 * ����������ģ��: VCC -> 5V; GND -> GND; AO -> PA1;
 * USBת����ģ�飺RX->PA9;TX->PA10;�����ʣ�9600
 * ������������������ֵ��������ʾ����ֵ��
 * ����    �����ص���
 * �Ա�    ��https://ilovemcu.taobao.com
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

const float VCC =3.3; // ģ�鹩���ѹ��ADC�ο���ѹΪV
const float R_DIV =100000.0; // ��ѹ����Ϊ100K��

const float STRAIGHT_RESISTANCE =28320.0; // ƽֱʱ�ĵ���ֵ  ���޸�
const float BEND_RESISTANCE =48763.0; // 90������ʱ�ĵ���ֵ  ���޸�


long map(long x, long in_min, long in_max, long out_min, long out_max);

int main(void)
{		
	delay_init();	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	Adc_Init();

	while(1)
	{
		flexADC = Get_Adc_Average(1,10);	//10��ƽ��ֵ
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

