#include "fatFs_Spi_Flash_API.h"
//�ļ�ϵͳ���Ժ���
FATFS	Spi_Fs;
FIL		Spi_File;
FRESULT	Spi_Fr_Res;

void Spi_File_W_R_Test(void);

//���Թ��غ͸�ʽ��
void Spi_Fs_Test(void)
{
	printf(">����һ��SPI�ļ�ϵͳ���Ժ���!\r\n");
	Spi_Fr_Res = f_mount(&Spi_Fs,"0:", 1);
	if (Spi_Fr_Res == FR_NO_FILESYSTEM)
	{
		printf(">û���ļ�ϵͳ����ʽ��!\r\n");
		Spi_Fr_Res = f_mkfs("0:",0,4096);
		if (Spi_Fr_Res == FR_OK)
		{
			printf(">��ʽ���ɹ���\r\n");
		}
		//ȡ������
		//Spi_Fr_Res = f_mount(&Spi_Fs,"0:", 0);
		Spi_Fr_Res = f_mount(NULL,"0:", 1);
		Delay_ms(10);
		Spi_Fr_Res = f_mount(&Spi_Fs,"0:", 0);
		if (Spi_Fr_Res == FR_OK)
		{
			printf(">���سɹ���\r\n");
		}
		else if(Spi_Fr_Res == FR_NO_FILESYSTEM)
		{
			printf(">���سɹ�,��ʽ�����ɹ���\r\n");
			while(1);
		}
		else
		{
			printf(">��ʼ������\r\n");
			while(1);
		}
	}
	else if(Spi_Fr_Res == FR_OK)
	{
		printf(">���سɹ������ļ�ϵͳ����ֱ�Ӷ�д��\r\n");
	}
	else
	{
		printf(">���ش����ļ�ϵͳ��ʼ�����ɹ���\r\n");
		printf(">�������:%x\r\n",Spi_Fr_Res);
	}
	Spi_File_W_R_Test();
	while(1);
}

//�����ļ������Ͷ�д
void Spi_File_W_R_Test(void)
{
	u8 	Spi_Flash_TxBuffer[] = "This is a flash test program!���ľ�˵Ҳ��д��\r\n";
	u8 	Spi_Flash_RxBuffer[RxBufferSize] = {0};
	UINT File_Bw_Num_W = 0;
	UINT File_Bw_Num_R = 0;
	
	Spi_Fr_Res = f_open(&Spi_File,"0:Test.txt",FA_CREATE_ALWAYS | FA_WRITE);//ͬʱ��д���� | FA_READ);
	if(Spi_Fr_Res != FR_OK)
	{
		printf(">�ļ���ʧ�ܣ�\r\n");
		printf(">�������:%x\r\n",Spi_Fr_Res);
		while(1);
	}
	printf(">�ļ��򿪳ɹ���\r\n");
	Spi_Fr_Res = f_write(&Spi_File,Spi_Flash_TxBuffer,RxBufferSize,&File_Bw_Num_W);
	if(Spi_Fr_Res != FR_OK)
	{
		printf(">�ļ�д��ʧ�ܣ�\r\n");
		printf(">�������:%x\r\n",Spi_Fr_Res);
		while(1);
	}
	printf(">�ļ�д��ɹ���\r\n");
	Spi_Fr_Res = f_close(&Spi_File);
	
	Spi_Fr_Res = f_open(&Spi_File,"0:Test.txt",FA_OPEN_EXISTING | FA_READ);
	
	Spi_Fr_Res = f_read(&Spi_File,Spi_Flash_RxBuffer,RxBufferSize,&File_Bw_Num_R);
	if(Spi_Fr_Res != FR_OK)
	{
		printf(">�ļ���ȡʧ�ܣ�\r\n");
		printf(">�������:%x\r\n",Spi_Fr_Res);
		while(1);
	}
	printf(">�ļ���ȡ�ɹ�����ȡ����Ϊ��\r\n");
	printf("%s",Spi_Flash_RxBuffer);
	Spi_Fr_Res = f_close(&Spi_File);
	Spi_Fr_Res = f_mount(NULL,"0:",1);
	if(Spi_Fr_Res != FR_OK)
	{
		printf(">�ļ�ϵͳ��ֲʧ�ܣ�\r\n");
		printf(">�������:%x\r\n",Spi_Fr_Res);
		while(1);
	}
	printf(">��ϲ�㣬�ļ�ϵͳ��ֲ�ɹ���\r\n");
	while(1);
	
}

