#include "fatFs_Spi_Flash_API.h"
static volatile DSTATUS fatFs_Spi_Flash_Stat = STA_NOINIT;	/* Physical drive status */

//SPI_FLASH测试函数

//----------------------------------------------------------
//下面函数是FATFS文件系统接口函数
//
//
//----------------------------------------------------------
//文件系统初始化函数
DSTATUS fatFs_Spi_Flash_Init(void)
{
	Spi_Flash_Init();
	if(W25Q64_FLASH_ID == Spi_Flash_Read_ID1())
	{
		printf("W25Q64_FLASH_ID == Spi_Flash_Read_ID1()\r\n");
		return fatFs_Spi_Flash_Stat &= ~STA_NOINIT;
	}
	else
	{
		return fatFs_Spi_Flash_Stat |=  STA_NOINIT;
	}
}

//文件系统查询状态函数
DSTATUS fatFs_Spi_Flash_Status(void)
{
	if(W25Q64_FLASH_ID == Spi_Flash_Read_ID1())
	{
		return fatFs_Spi_Flash_Stat &= ~STA_NOINIT;
	}
	else
	{
		return fatFs_Spi_Flash_Stat |=  STA_NOINIT;
	}
}
//文件系统IO控制函数
DRESULT fatFs_Spi_Flash_Ioctl(BYTE cmd,char *buff)
{
	switch (cmd)
	{
		case GET_SECTOR_SIZE:
			*(WORD * )buff = 4096;		//flash最小写单元为页，256字节，此处取2页为一个读写单位
			break;
		case GET_BLOCK_SIZE:
			*(DWORD * )buff = 1;		//flash以4k为最小擦除单位
			break;
		case GET_SECTOR_COUNT:
			*(DWORD * )buff = 1536;		//sector数量
			break;
		case CTRL_SYNC:
			break;
		default:
			break;
	}
	
	return RES_OK;
}
//文件系统读接口
DRESULT fatFs_Spi_Flash_Read(BYTE *buff, DWORD sector, UINT count)
{
	if ((fatFs_Spi_Flash_Stat & STA_NOINIT)) {
		return RES_NOTRDY;
	}
	sector+=512;//扇区偏移，外部Flash文件系统空间放在外部Flash后面6M空间
	Spi_Flash_Read_Buffer(buff, sector << 12, count<<12);
	
	return RES_OK;
}
//文件系统写接口
DRESULT fatFs_Spi_Flash_Write(const BYTE *buff, DWORD sector, UINT count)
{
	uint32_t write_addr;  
	sector+=512;//扇区偏移，外部Flash文件系统空间放在外部Flash后面6M空间
	write_addr = sector<<12;    
	Spi_Flash_Erase_Sector(write_addr);
	Spi_Flash_Write_Buffer(buff,write_addr,4096);
	return RES_OK;
}



