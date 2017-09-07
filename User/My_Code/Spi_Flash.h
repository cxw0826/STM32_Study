#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_

#include "stm32f10x.h"
#include "DMA_Uart.h"

//spi相关引脚定义
#define W25Q64_SPI_Port		GPIOA
#define W25Q64_SPI_NSS		GPIO_Pin_4
#define W25Q64_SPI_SCK		GPIO_Pin_5
#define W25Q64_SPI_MISO		GPIO_Pin_6
#define W25Q64_SPI_MOSI		GPIO_Pin_7
//原理图用的是SPI1
#define	W25Q64_SPIX			SPI1
//CS控制信号函数
#define Spi_Flash_Cs_Enable() GPIO_ResetBits(W25Q64_SPI_Port,W25Q64_SPI_NSS);
#define Spi_Flash_Cs_Disable() GPIO_SetBits(W25Q64_SPI_Port,W25Q64_SPI_NSS);
//定义相关操作参数
#define DummyByte 			0xFF
//W25Q64设备ID号存放寄存器
//byte1为读地址，byte2为manufacture ID
//byte3为memory type
//byte4为DeviceID
#define W25Q64_JEDECID_REG  0x9F	
//

void Spi_Flash_Init(void);
void Spi_Flash_Read_W25Q64_JEDEC_ID(void);



#endif
