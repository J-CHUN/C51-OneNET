#ifndef __USART_H
#define __USART_H
#include "reg52.h" 
#include "delay.h"

void UART_init(void);//串口初始化 9600
void set52_baudrate(float frequency, long int baudrate); //可设置为115200
void UARTSendByte(unsigned char DAT);	//串口发送一字节数据
void PrintCom(unsigned char *DAT); //打印串口字符串数据

void UART_SendByte(unsigned char dat);
void UART_SendStr(unsigned char *s,unsigned char length);

#endif
