#ifndef __USART_H
#define __USART_H
#include "reg52.h" 
#include "delay.h"

void UART_init(void);//���ڳ�ʼ�� 9600
void set52_baudrate(float frequency, long int baudrate); //������Ϊ115200
void UARTSendByte(unsigned char DAT);	//���ڷ���һ�ֽ�����
void PrintCom(unsigned char *DAT); //��ӡ�����ַ�������

void UART_SendByte(unsigned char dat);
void UART_SendStr(unsigned char *s,unsigned char length);

#endif
