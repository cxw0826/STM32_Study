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
//�ļ�ϵͳ���Ժ���
FIL fnew;													/* file objects */
FATFS fs;													/* Work area (file system object) for logical drives */
FRESULT res_flash; 
UINT br, bw;            					/* File R/W count */
BYTE buffer[1024]={0};       		  /* file copy buffer */
BYTE textFileBuffer[] = "�������ļ��ļ�ϵͳ��ֲ��\r\n";
BYTE work[4096]; /* Work area (larger is better for processing time) */


void fatFs_Spi_Flash_Test(void)
{
	printf("This is a fatfs test demo \r\n");
	//���ⲿFlash�����ļ�ϵͳ
	res_flash = f_mount(&fs,"0:",1);
	printf("f_mount res_flash=%d \r\n",res_flash);
	//���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ
	if(res_flash ==FR_NO_FILESYSTEM)
	{
		res_flash=f_mkfs("0:",0,4096,work,sizeof(work));							//��ʽ��
		printf("\r\nmkfs res_flash=%d",res_flash);
		res_flash = f_mount(&fs,"0:",0);						//��ʽ������ȡ������
		res_flash = f_mount(&fs,"0:",1);						//���¹���
	}
	//�ļ�ϵͳ���ԣ�д����
	//���ļ�������ļ����μ��򴴽���
	res_flash = f_open(&fnew, "0:flashnewfile.txt", FA_CREATE_ALWAYS | FA_WRITE );
	
	if ( res_flash == FR_OK )
	{
		res_flash = f_write(&fnew, textFileBuffer, sizeof(textFileBuffer), &bw);
		f_close(&fnew);      
	}
	//������
	res_flash = f_open(&fnew, "0:flashnewfile.txt", FA_OPEN_EXISTING | FA_READ); 	 
	res_flash = f_read(&fnew, buffer, sizeof(buffer), &br); 
	printf("\r\n %s ", buffer);
	/* Close open files */
	res_flash = f_close(&fnew);	
	//����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ
	res_flash = f_mount(&fs,"0:",0);
	
	printf("fatFs_Spi_Flash_Test Success! \r\n");
	 
}



