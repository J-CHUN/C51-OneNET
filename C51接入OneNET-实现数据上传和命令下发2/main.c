/**************************************************************************************
���ߣ�
ESP826601S: TX--P30 RX--P31  VCC--3.3V  GND--GND
DS18B20: IO--P37 
��Ҫ��д OneNET �ṩ�� ESP8266 �̼�
***************************************************************************************/
#include "reg52.h"    //���ļ��ж����˵�Ƭ����һЩ���⹦�ܼĴ���
#include "usart.h"
#include "delay.h"
#include "stdio.h"
#include "18b20.h"

sbit led=P2^0;	   //����Ƭ����P0.0�˿ڶ���Ϊled
unsigned char xdata dataBuf[36]= {0};//������ʾ������ʱ�洢����

//������ȫ�ֱ������ڴ��ڽ����жϸı���ֵ
char WIFI_OK = 0;  
char OneNET_OK = 0;

void main()
{
	int temp1;
	float temperature;
	Init_DS18B20();
	
	//UART_init();//���ڳ�ʼ��
	set52_baudrate(11.0592, 115200);//���ڳ�ʼ��
	DelayMs(1000);
	
	UART_SendStr("AT+CWJAP=XIAOCHUN02,3118003167\r\n",32); //�ֶ�����WiFi,�����ֱ�����ȵ����ơ�����
	while(WIFI_OK != 1);  //������WIFI��WIFI_OK==1

	UART_SendStr("AT+IOTCFG=879125822,486413,0713\r\n",33);//���õ�¼��Ϣ�������ֱ�����豸ID����ƷID����Ȩ��Ϣ
	while(OneNET_OK != 1);  //����½OneNET��OneNET_OK==1
	
	UART_SendStr("AT+IOTSEND=0,data,100\r\n",23);	//�����������������ֱ������������ֵ���͡���������������ֵ
	DelayMs(1000);
	
	while(1)
	{
		temp1=ReadTemperature();		 //��ȡ�¶�
		temperature=(float)temp1*0.0625; //�¶�ֵת��
		
		sprintf(dataBuf,"AT+IOTSEND=0,data,%.1f\r\n",temperature);  //���¶����ݷ���dataBuf������
		
		UART_SendStr(dataBuf,36);	//����������
		
		DelayMs(2000);
	}		
}
