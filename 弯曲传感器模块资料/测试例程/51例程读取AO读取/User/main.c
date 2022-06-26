/********************(C) COPRIGHT 2019 Crownto electronic **************************
 * �ļ���  ��main.c
 * ����    ��        
 * Ӳ�����ӣ�
 * OLED :CS -> P2.4; RST -> P2.2; DC -> P2.3;SCL -> P2.0;SDIN -> P2.1;
 * ����������ģ��: VCC -> 5V; GND -> GND; AO -> P1.3;
 * ������������������ֵ������Һ����ʾ��

 * �Ա�    ��https://ilovemcu.taobao.com
**********************************************************************************/

#include <STC12C5A60S2.h>
#include <intrins.h>
#include "oled.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#define	uchar	unsigned char
#define uint	unsigned int
	


#define MAIN_Fosc		11059200L	//������ʱ��, ģ�⴮�ںͺ���ʱ���Զ���Ӧ��5~35MHZ  22118400L 


#define ADC_OFF()	ADC_CONTR = 0
#define ADC_ON		(1 << 7)
#define ADC_90T		(3 << 5)
#define ADC_180T	(2 << 5)
#define ADC_360T	(1 << 5)
#define ADC_540T	0
#define ADC_FLAG	(1 << 4)	//�����0
#define ADC_START	(1 << 3)	//�Զ���0
#define ADC_CH0		0
#define ADC_CH1		1
#define ADC_CH2		2
#define ADC_CH3		3
#define ADC_CH4		4
#define ADC_CH5		5
#define ADC_CH6		6
#define ADC_CH7		7

/**************��������***************************/

const float VCC =5.0; // ģ�鹩���ѹ��ADC�ο���ѹΪV
const float R_DIV =100000.0; // ��ѹ����Ϊ100K��

const float STRAIGHT_RESISTANCE =28320.0; // ƽֱʱ�ĵ���ֵ  ���޸�
const float BEND_RESISTANCE =48763.0; // 90������ʱ�ĵ���ֵ  ���޸�

int flexADC;
float flexV=0.0;
float flexR=0.0;
float angle=0.0;


unsigned char V_disbuff[5]={0}; 
unsigned char R_disbuff[7]={0}; 
unsigned char A_disbuff[5]={0};

char xdata buf1[60];
//void DelayUs(unsigned int time);   //��ʱ�Ӻ���
//void Delay1(unsigned int time);		 //��ʱ������
void  delay_ms(unsigned int ms);

uint	adc10_start(uchar channel);	//channel = 0~7
long map(long x, long in_min, long in_max, long out_min, long out_max);
void Uart1Sends(uchar *str);
/*
 * ��������Get_Voltage_Value
 * ����  ����ȡADC0809��ת����ѹֵ
 *         
 * ����  ����
 * ���  ����
 * ˵��  ��
 *						AIN0(0X40):��������
 *						AIN1(0X41):����
 *					  AIN2(0X42):����������
 *					  AIN3(0X43):��������
 */
void BEND_Value_Conversion()
{
	  flexV=(flexADC*VCC)/1024;
	  flexR= R_DIV * (VCC / flexV-1.0);
	
		/*��ʾ��ѹֵ*/
		V_disbuff[0]=(long int)(flexV*1000)/1000+'0';
		V_disbuff[1]=(long int)(flexV*1000)%1000/100+'0';	
		V_disbuff[2]=(long int)(flexV*1000)%100/10+'0';
		V_disbuff[3]=(long int)(flexV*1000)%10+'0';
	
	  OLED_ShowString(36,2,V_disbuff);
	
   /*��ʾ����ֵ*/
	  R_disbuff[0]=(long int)(flexR)/100000+'0';
	  R_disbuff[1]=(long int)(flexR)%100000/10000+'0';
	  R_disbuff[2]=(long int)(flexR)%10000/1000+'0';	
	  R_disbuff[3]=(long int)(flexR)%1000/100+'0';
	  R_disbuff[4]=(long int)(flexR)%100/10+'0';;
	  R_disbuff[5]=(long int)(flexR)%10+'0';
		
		OLED_ShowString(36,4,R_disbuff);
    /*��ʾ�Ƕ�ֵ*/
		if(angle<0)
		{
		 	A_disbuff[0]='-';	
		  A_disbuff[1]=(int)(abs(angle))/100+'0';	
		  A_disbuff[2]=(int)(abs(angle))%100/10+'0';
		  A_disbuff[3]=(int)(abs(angle))%10+'0';
		}
		else
		{
			A_disbuff[0]=' ';	
		  A_disbuff[1]=(int)(angle)/100+'0';	
		  A_disbuff[2]=(int)(angle)%100/10+'0';
		  A_disbuff[3]=(int)(angle)%10+'0';
		}

		
		OLED_ShowString(27,6,A_disbuff);
	
}
 
void send_data(void)
{
  	memset(buf1, 0, sizeof(buf1));
	  sprintf(buf1, "Voltage:%f V\r\n",flexV);
	  Uart1Sends(buf1);
	  memset(buf1, 0, sizeof(buf1));
	  sprintf(buf1, "Resistance:%f ohms\r\n",flexR);
	  Uart1Sends(buf1);
	  memset(buf1, 0, sizeof(buf1));
	  sprintf(buf1, "Bend:%f degrees\r\n",angle);
	  Uart1Sends(buf1);
	  delay_ms(500);
}

void StartUART( void )
{  							//������9600
     SCON = 0x50;
     TMOD = 0x20;
     TH1 = 0xFD;
     TL1 = 0xFD; 
     PCON = 0x00;
     TR1 = 1;
}
//**********ͨ�����ڽ����յ����ݷ��͸�PC��**************
void R_S_Byte(uchar R_Byte)
{	
	 SBUF = R_Byte;  
     while( TI == 0 );				//��ѯ��
  	 TI = 0;    
}
//���п���������char�����飬������ֹ��/0��ֹͣ
void Uart1Sends(uchar *str)
{
	while(*str!='\0')
	{
		SBUF=*str;
		while(!TI);//�ȴ���������źţ�TI=1������
		TI=0;
		str++;
	}
}
/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */
void main(void)
{ 
	StartUART();
	OLED_Init();			//��ʼ��OLED  
	OLED_Clear()  	; 

	P1ASF = 0x0F;			//12C5A60AD/S2ϵ��ģ������(AD)ѡ��
	ADC_CONTR = ADC_360T | ADC_ON;

	OLED_ShowCHinese(0,0,0);
	OLED_ShowCHinese(18,0,1);
	OLED_ShowCHinese(36,0,2);
	OLED_ShowCHinese(54,0,3);
	OLED_ShowCHinese(72,0,4);
	OLED_ShowCHinese(90,0,5);
	OLED_ShowCHinese(108,0,6);
	OLED_ShowString(0,2,"V:");	
	OLED_ShowString(0,4,"R:");
  OLED_ShowString(0,6,"A:");	
	OLED_ShowString(71,2,"mv");	
	OLED_ShowCHinese(90,4,7);
  OLED_ShowCHinese(63,6,8);	
	
	
	while(1)
	{
			flexADC = adc10_start(3);	
      angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,0, 90.0);		
			BEND_Value_Conversion();
		  send_data();
	}
}

//map����
long map(long x, long in_min, long in_max, long out_min, long out_max) {
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

///********************* ��һ��ADCת�� *******************/
uint	adc10_start(uchar channel)	//channel = 0~7
{
	uint	adc;
	uchar	i;

	ADC_RES = 0;
	ADC_RESL = 0;

	ADC_CONTR = (ADC_CONTR & 0xe0) | ADC_START | channel; 

	i = 250;
	do{
		if(ADC_CONTR & ADC_FLAG)
		{
			ADC_CONTR &= ~ADC_FLAG;
			adc = (uint)ADC_RES;
			adc = (adc << 2) | (ADC_RESL & 3);
			return	adc;
		}
	}while(--i);
	return	1024;
}




