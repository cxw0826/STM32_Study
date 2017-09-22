#include "fatFs_Spi_Flash_API.h"
static volatile DSTATUS fatFs_Spi_Flash_Stat = STA_NOINIT;	/* Physical drive status */

//SPI_FLASH���Ժ���

//----------------------------------------------------------
//���溯����FATFS�ļ�ϵͳ�ӿں���
//
//
//----------------------------------------------------------
//�ļ�ϵͳ��ʼ������
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

//�ļ�ϵͳ��ѯ״̬����
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
//�ļ�ϵͳIO���ƺ���
DRESULT fatFs_Spi_Flash_Ioctl(BYTE cmd,char *buff)
{
	switch (cmd)
	{
		case GET_SECTOR_SIZE:
			*(WORD * )buff = 4096;		//flash��Сд��ԪΪҳ��256�ֽڣ��˴�ȡ2ҳΪһ����д��λ
			break;
		case GET_BLOCK_SIZE:
			*(DWORD * )buff = 1;		//flash��4kΪ��С������λ
			break;
		case GET_SECTOR_COUNT:
			*(DWORD * )buff = 1536;		//sector����
			break;
		case CTRL_SYNC:
			break;
		default:
			break;
	}
	
	return RES_OK;
}
//�ļ�ϵͳ���ӿ�
DRESULT fatFs_Spi_Flash_Read(BYTE *buff, DWORD sector, UINT count)
{
	if ((fatFs_Spi_Flash_Stat & STA_NOINIT)) {
		return RES_NOTRDY;
	}
	sector+=512;//����ƫ�ƣ��ⲿFlash�ļ�ϵͳ�ռ�����ⲿFlash����6M�ռ�
	Spi_Flash_Read_Buffer(buff, sector << 12, count<<12);
	
	return RES_OK;
}
//�ļ�ϵͳд�ӿ�
DRESULT fatFs_Spi_Flash_Write(const BYTE *buff, DWORD sector, UINT count)
{
	uint32_t write_addr;  
	sector+=512;//����ƫ�ƣ��ⲿFlash�ļ�ϵͳ�ռ�����ⲿFlash����6M�ռ�
	write_addr = sector<<12;    
	Spi_Flash_Erase_Sector(write_addr);
	Spi_Flash_Write_Buffer(buff,write_addr,4096);
	return RES_OK;
}



