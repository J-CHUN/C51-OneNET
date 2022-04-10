/**************************************************************************************
接线：
ESP826601S: TX--P30 RX--P31  VCC--3.3V  GND--GND
DS18B20: IO--P37 
需要烧写 OneNET 提供的 ESP8266 固件
***************************************************************************************/
#include "reg52.h"    //此文件中定义了单片机的一些特殊功能寄存器
#include "usart.h"
#include "delay.h"
#include "stdio.h"
#include "18b20.h"

sbit led=P2^0;	   //将单片机的P0.0端口定义为led
unsigned char xdata dataBuf[36]= {0};//定义显示区域临时存储数组

//定义两全局变量，在串口接收中断改变其值
char WIFI_OK = 0;  
char OneNET_OK = 0;

void main()
{
	int temp1;
	float temperature;
	Init_DS18B20();
	
	//UART_init();//串口初始化
	set52_baudrate(11.0592, 115200);//串口初始化
	DelayMs(1000);
	
	UART_SendStr("AT+CWJAP=XIAOCHUN02,3118003167\r\n",32); //手动连接WiFi,参数分别代表热点名称、密码
	while(WIFI_OK != 1);  //当连上WIFI后WIFI_OK==1

	UART_SendStr("AT+IOTCFG=879125822,486413,0713\r\n",33);//配置登录信息，参数分别代表设备ID、产品ID、鉴权信息
	while(OneNET_OK != 1);  //当登陆OneNET后OneNET_OK==1
	
	UART_SendStr("AT+IOTSEND=0,data,100\r\n",23);	//发送数据流，参数分别代表数据是数值类型、数据流名、数据值
	DelayMs(1000);
	
	while(1)
	{
		temp1=ReadTemperature();		 //读取温度
		temperature=(float)temp1*0.0625; //温度值转换
		
		sprintf(dataBuf,"AT+IOTSEND=0,data,%.1f\r\n",temperature);  //将温度数据放入dataBuf数组中
		
		UART_SendStr(dataBuf,36);	//发送数据流
		
		DelayMs(2000);
	}		
}
