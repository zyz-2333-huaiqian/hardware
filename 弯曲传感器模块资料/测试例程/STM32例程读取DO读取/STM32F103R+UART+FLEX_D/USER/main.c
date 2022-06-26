/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * �ļ���  ��main.c
 * ����    ��        
 * Ӳ�����ӣ�
 * ����������ģ��: VCC -> 3.3V; GND -> GND; DO -> PC15;
 * USBת����ģ�飺RX->PA9;TX->PA10;�����ʣ�9600
 * ������������������ֵ��������ʾ����ֵ��
 * �Ա�    ��https://ilovemcu.taobao.com
**********************************************************************************/
#include "stm32f10x.h"
#include "delay.h"
#include "FLEX.h"
#include "usart.h"

u8 state = 0;

int main(void)
{		
	delay_init();	
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	FLEX_IO_Init();

	delay_ms(1000);

	printf("Test start\r\n");
	while(1)
	{
		if(FLEX_Scan(1) == 1 && state == 0)
		{
				printf("���ڷ�ֵ\r\n");
				state = 1;
		}
		
		if(FLEX_Scan(1) == 0 && state == 1)
		{
				printf("���ڷ�ֵ\r\n");
				state = 0;
		}
			
		delay_ms(500);
	}

}

