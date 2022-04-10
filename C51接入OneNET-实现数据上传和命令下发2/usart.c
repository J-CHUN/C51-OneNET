#include "usart.h"
//#define FOSC 22118400L      //System frequency
#define FOSC   11059200L      //System frequency
unsigned  int baud=9600;       //UART������
sbit led=P2^0;	   //����Ƭ����P0.0�˿ڶ���Ϊled
/************************************************************************
�� �� ���� ���ڳ�ʼ��
���������� 51��Ƭ�����ڳ�ʼ������
���غ����� none
����˵���� none
**************************************************************************/
void UART_init(void)
{
    SCON = 0x50;             // SCON: ģʽ 1, 8-bit UART, ʹ�ܽ��� 
    TMOD = 0x20;            // TMOD: timer 1, mode 2, 8-bit ��װ
    TH1 = TL1 = -(FOSC/12/32/baud); //TH1:  ��װֵ 9600 ������ ���� 11.0592MHz
    TR1 = 1;                //TR1:  timer 1 ��  
    ES = 1;                 //�򿪴����ж�
    EA = 1;                 //�����ж�
}
/**�Զ��岨���ʲ�������**/
/*
frequency������Ƶ�ʣ�11.0592
baudrate�� ��Ҫ�Ĳ�����,115200
*/
void set52_baudrate(float frequency, long int baudrate)
{
	unsigned int itmp;
	unsigned char tlow,thigh;

	TMOD =0x20;
	itmp=(int)(65536-(frequency*1000000)/(baudrate*32));
	thigh=itmp/256;
	tlow=itmp%256;
	SCON=0x5c;
	T2CON=0x30;
	RCAP2H=thigh;
	RCAP2L=tlow;
	TH2=thigh;
	TL2=tlow;
	TR2=1;  //set ok
	//REN=1;
	ET2=1;  //T2�ж�
	EA=1;	//���ж�
	ES=1;	//�����ж�
	PS=1;	//
}

/************************************************************************
���������� 	���ڷ���һ�ֽ�����
��ڲ�����	DAT:�����͵�����
�� �� ֵ�� 	none
����˵����	none
**************************************************************************/
void UARTSendByte(unsigned char DAT)
{
	ES  =  0;
	TI=0;
	SBUF = DAT;
	while(TI==0);
	TI=0;
	ES = 1;
}


/************************************************************************
���������� ���ڷ����ַ�������
��ڲ����� 	*DAT���ַ���ָ��
�� �� ֵ�� none
����˵���� API ���ⲿʹ�ã�ֱ�ۣ�
**************************************************************************/
void PrintCom(unsigned char *DAT)
{
	while(*DAT)
	{
	 	UARTSendByte(*DAT++);
	}	
}


void UART_SendByte(unsigned char dat)
{
	unsigned char time_out;
	time_out=0x00;
	SBUF = dat;			  //�����ݷ���SBUF��
	while((!TI)&&(time_out<100))  //����Ƿ��ͳ�ȥ
	{time_out++;DelayUs2x(10);}	//δ���ͳ�ȥ ���ж�����ʱ
	TI = 0;						//���ti��־
}

void UART_SendStr(unsigned char *s,unsigned char length)
{
	unsigned char NUM;
	NUM=0x00;
	while(NUM<length)	//���ͳ��ȶԱ�
	{
		UART_SendByte(*s);  //���ɵ��ֽ�����
		s++;		  //ָ��++
		NUM++;		  //��һ��++
  	 }
}
/*
6.	�������
���أ�+CMD,x,cmd
X�������
cmd������
*/
static  char  uart_buf[32]={0};   //���ڱ��洮������
static  unsigned char uart_cnt=0; //���ڶ�λ�������ݵ�λ��
extern char WIFI_OK;              //�ж�WIFI�Ƿ�����
extern char OneNET_OK;            //�ж�OneNET�Ƿ��½
void UART_SER (void) interrupt 4 	//�����жϷ������
{
	if(RI)                        //�ж��ǽ����жϲ���
	{
		RI=0;                      //��־λ����
		//���մ�������
		uart_buf[uart_cnt] =SBUF;  
		uart_cnt++;

		
		//�ж��Ƿ����·���ָ��
		if(uart_buf[uart_cnt-4]=='L'&&uart_buf[uart_cnt-3]=='E'&&uart_buf[uart_cnt-2]=='D'&&uart_buf[uart_cnt-1] == '0')  
		{
			led = 0;       //����
			uart_cnt = 0;  //���㣬���¼���
		}
		else if(uart_buf[uart_cnt-4]=='L'&&uart_buf[uart_cnt-3]=='E'&&uart_buf[uart_cnt-2]=='D'&&uart_buf[uart_cnt-1] == '1')  
		{
			led = 1;       //�ص�
			uart_cnt = 0;
		}
		//�����������
		
		
		
		//WIFI���ӳɹ���᷵���ַ�����+Event:WIFI GOT IP
		else if(uart_buf[uart_cnt-4]=='T'&&uart_buf[uart_cnt-3]==' '&&uart_buf[uart_cnt-2]=='I'&&uart_buf[uart_cnt-1] == 'P')
		{
			WIFI_OK = 1;
			uart_cnt = 0;
		}
		//OneNET�ɹ���½��᷵���ַ�����+Event:Connect:0
		else if(uart_buf[uart_cnt-4]=='c'&&uart_buf[uart_cnt-3]=='t'&&uart_buf[uart_cnt-2]==':'&&uart_buf[uart_cnt-1] == '0')
		{
			OneNET_OK = 1;
			uart_cnt = 0;
		}
		
		if(uart_cnt>28)   //��ֹ����Խ��
		{
			uart_cnt = 0;
		}
	}
	if(TI)  //����Ƿ��ͱ�־λ������
	TI=0;
} 





