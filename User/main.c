#include "stm32f10x.h"
#include "Led.h"
#include "Sys_Tick.h"
#include "DMA_Uart.h"
#include "Spi_Flash.h"

int main(void)
{	
	// ��������main����֮ǰ�������ļ���statup_stm32f10x_hd.s�Ѿ�����
	// SystemInit()������ϵͳʱ�ӳ�ʼ����72MHZ
	// SystemInit()��system_stm32f10x.c�ж���
	// ����û����޸�ϵͳʱ�ӣ������б�д�����޸�
	
	// ����ֵ����д1024�����ڴ����
	// u16 Spi_Flash_Read_Buffer[10];
	
	Sys_Tick_Init();
	Led_Init();
	DMA_Uart_Init();
	Spi_Flash_Init();
	
	//Led_RGB_Test();
	//DMA_TX_Uart_Test();
	//DMA_Uart_Recv_Send_Test();
	Spi_Flash_Test();
	
		
	while( 1 )
	{
	}
}

	


