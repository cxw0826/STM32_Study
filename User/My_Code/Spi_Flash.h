#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_

#include "stm32f10x.h"
#include "DMA_Uart.h"
<<<<<<< HEAD
#include "ff.h"
#include "diskio.h"
#include "integer.h"
=======
#include "integer.h"
#include "ff.h"
#include "diskio.h"
>>>>>>> 238c1cde672cec4ea517a2ee3e54741207880b25

//spi相关引脚定义
#define W25Q64_SPI_Port		GPIOA

#define W25Q64_SPI_NSS		GPIO_Pin_4
#define W25Q64_SPI_SCK		GPIO_Pin_5
#define W25Q64_SPI_MISO		GPIO_Pin_6
#define W25Q64_SPI_MOSI		GPIO_Pin_7
//原理图用的是SPI1
#define	W25Q64_SPIX			SPI1
//CS控制信号函数
#define Spi_Flash_Cs_Enable() GPIO_ResetBits(W25Q64_SPI_Port,W25Q64_SPI_NSS)
#define Spi_Flash_Cs_Disable() GPIO_SetBits(W25Q64_SPI_Port,W25Q64_SPI_NSS)
//定义相关操作参数
#define DummyByte 			0xFF
#define W25Q64_WIP_FLAG		0x01
#define	W25Q64_PAGE_SIZE	256
#define  countof(a)      (sizeof(a) / sizeof(*(a)))
#define  RxBufferSize (countof(Spi_Flash_TxBuffer)-1)

//W25Q64设备ID号存放寄存器
//byte1为读地址，byte2为manufacture ID
//byte3为memory type
//byte4为DeviceID
#define W25Q64_JEDECID_REG  		0x9F	
#define W25Q64_DEVICEID_REG  		0xAB
#define W25Q64_DEVICE_ID			0xEF4017
//定义操作W25Q64指令
#define W25Q64_PAGE_WRITE_CMD		0x02
#define W25Q64_READ_DATA			0x03
#define W25Q64_WRITE_DISABLE		0x04
#define W25Q64_READ_STATE_CMD		0x05
#define W25Q64_WRITE_ENABLE			0x06
#define	W25Q64_SECTOR_ERASE_CMD		0x20

//定义W25Q64的16个sector 地址
#define W25Q64_MEMORY_SECTOR_0		0x000000
#define W25Q64_MEMORY_SECTOR_1		0x001000
#define W25Q64_MEMORY_SECTOR_2		0x002000
#define W25Q64_MEMORY_SECTOR_3		0x003000
#define W25Q64_MEMORY_SECTOR_4		0x004000
#define W25Q64_MEMORY_SECTOR_5		0x005000
#define W25Q64_MEMORY_SECTOR_6		0x006000
#define W25Q64_MEMORY_SECTOR_7		0x007000
#define W25Q64_MEMORY_SECTOR_8		0x008000
#define W25Q64_MEMORY_SECTOR_9		0x009000
#define W25Q64_MEMORY_SECTOR_10		0x00A000
#define W25Q64_MEMORY_SECTOR_11		0x00B000
#define W25Q64_MEMORY_SECTOR_12		0x00C000
#define W25Q64_MEMORY_SECTOR_13		0x00D000
#define W25Q64_MEMORY_SECTOR_14		0x00E000
#define W25Q64_FLASH_ID             0XEF4017    //W25Q64

void Spi_Flash_Init(void);
<<<<<<< HEAD
u32	 Spi_Flash_Read_JEDEC_ID(void);
void Spi_Flash_Erase_Sector(u32 Sector_Num);
void Spi_Flash_Write_Buffer(u32 ADDR, u8 * DATA, u32 NumOfByte);
void Spi_Flash_Read_Buffer(u32 ADDR,u8 *DATA,u32 NumOfRead);
void Spi_Flash_Test(void);
//
//#define	FF_USE_MKFS 1
//#define FF_FS_READONLY 0
//
=======
void Spi_Flash_Read_JEDEC_ID(void);
u32 Spi_Flash_Read_ID1(void);
void Spi_Flash_Erase_Sector(u32 Sector_Num);
void Spi_Flash_Write_Buffer(const BYTE *DATA,DWORD ADDR,UINT NumOfByte);
void Spi_Flash_Read_Buffer(BYTE *DATA,DWORD ADDR,UINT NumOfRead);
//void Spi_Flash_Write_Buffer(u32 ADDR, u8 * DATA, u32 NumOfByte);
//void Spi_Flash_Read_Buffer(u32 ADDR,u8 *DATA,u32 NumOfRead);
//void Spi_Flash_Test(void);

void fatFs_Spi_Flash_Test(void);
>>>>>>> 238c1cde672cec4ea517a2ee3e54741207880b25
DSTATUS fatFs_Spi_Flash_Init(void);
DSTATUS fatFs_Spi_Flash_Status(void);
DRESULT fatFs_Spi_Flash_Ioctl(BYTE cmd,char *buff);
DRESULT fatFs_Spi_Flash_Read(BYTE *buff, DWORD sector, UINT count);
<<<<<<< HEAD
DRESULT fatFs_Spi_Flash_Write(BYTE *buff, DWORD sector, UINT count);
=======
DRESULT fatFs_Spi_Flash_Write(const BYTE *buff, DWORD sector, UINT count);

>>>>>>> 238c1cde672cec4ea517a2ee3e54741207880b25



#endif
