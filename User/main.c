#include "stm32f10x.h"
#include "Led.h"
#include "Sys_Tick.h"
#include "DMA_Uart.h"
#include "Spi_Flash.h"


int main(void)
{	
	// 程序来到main函数之前，启动文件：statup_stm32f10x_hd.s已经调用
	// SystemInit()函数把系统时钟初始化成72MHZ
	// SystemInit()在system_stm32f10x.c中定义
	// 如果用户想修改系统时钟，可自行编写程序修改
	Sys_Tick_Init();
	Led_Init();
	DMA_Uart_Init();
	Spi_Flash_Init();
	
	//Led_RGB_Test();
	//DMA_TX_Uart_Test();
	//DMA_Uart_Recv_Send_Test();
	printf("SPI读ID测试!\r\n");
	Spi_Flash_Read_W25Q64_JEDEC_ID();
		
	while( 1 )
	{
	}
}

	


