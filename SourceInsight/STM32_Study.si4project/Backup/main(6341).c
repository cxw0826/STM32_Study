#include "stm32f10x.h"
#include "Led.h"
#include "Sys_Tick.h"
#include "DMA_Uart.h"


int main(void)
{	
	// ��������main����֮ǰ�������ļ���statup_stm32f10x_hd.s�Ѿ�����
	// SystemInit()������ϵͳʱ�ӳ�ʼ����72MHZ
	// SystemInit()��system_stm32f10x.c�ж���
	// ����û����޸�ϵͳʱ�ӣ������б�д�����޸�
	Sys_Tick_Init();
	Led_Init();
	DMA_Uart_Init();
	
	//Led_RGB_Test();
	DMA_Uart_Test();
		
	while( 1 )
	{
	}
}

	


