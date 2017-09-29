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
//文件系统测试函数
FIL fnew;													/* file objects */
FATFS fs;													/* Work area (file system object) for logical drives */
FRESULT res_flash; 
UINT br, bw;            					/* File R/W count */
BYTE buffer[1024]={0};       		  /* file copy buffer */
BYTE textFileBuffer[] = "陈兴旺的简单文件系统移植！\r\n";
BYTE work[4096]; /* Work area (larger is better for processing time) */


void fatFs_Spi_Flash_Test(void)
{
	printf("This is a fatfs test demo \r\n");
	//在外部Flash挂载文件系统
	res_flash = f_mount(&fs,"0:",1);
	printf("f_mount res_flash=%d \r\n",res_flash);
	//如果没有文件系统就格式化创建创建文件系统
	if(res_flash ==FR_NO_FILESYSTEM)
	{
		res_flash=f_mkfs("0:",0,4096,work,sizeof(work));							//格式化
		printf("\r\nmkfs res_flash=%d",res_flash);
		res_flash = f_mount(&fs,"0:",0);						//格式化后，先取消挂载
		res_flash = f_mount(&fs,"0:",1);						//重新挂载
	}
	//文件系统测试，写测试
	//打开文件，如果文件不参加则创建它
	res_flash = f_open(&fnew, "0:flashnewfile.txt", FA_CREATE_ALWAYS | FA_WRITE );
	
	if ( res_flash == FR_OK )
	{
		res_flash = f_write(&fnew, textFileBuffer, sizeof(textFileBuffer), &bw);
		f_close(&fnew);      
	}
	//读测试
	res_flash = f_open(&fnew, "0:flashnewfile.txt", FA_OPEN_EXISTING | FA_READ); 	 
	res_flash = f_read(&fnew, buffer, sizeof(buffer), &br); 
	printf("\r\n %s ", buffer);
	/* Close open files */
	res_flash = f_close(&fnew);	
	//不再使用文件系统，取消挂载文件系统
	res_flash = f_mount(&fs,"0:",0);
	
	printf("fatFs_Spi_Flash_Test Success! \r\n");
	 
}



