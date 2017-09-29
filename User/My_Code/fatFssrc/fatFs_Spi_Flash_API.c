#include "fatFs_Spi_Flash_API.h"
//文件系统测试函数
FATFS	Spi_Fs;
FIL		Spi_File;
FRESULT	Spi_Fr_Res;

void Spi_File_W_R_Test(void);

//测试挂载和格式化
void Spi_Fs_Test(void)
{
	printf(">这是一个SPI文件系统测试函数!\r\n");
	Spi_Fr_Res = f_mount(&Spi_Fs,"0:", 1);
	if (Spi_Fr_Res == FR_NO_FILESYSTEM)
	{
		printf(">没有文件系统，格式化!\r\n");
		Spi_Fr_Res = f_mkfs("0:",0,4096);
		if (Spi_Fr_Res == FR_OK)
		{
			printf(">格式化成功！\r\n");
		}
		//取消挂载
		//Spi_Fr_Res = f_mount(&Spi_Fs,"0:", 0);
		Spi_Fr_Res = f_mount(NULL,"0:", 1);
		Delay_ms(10);
		Spi_Fr_Res = f_mount(&Spi_Fs,"0:", 0);
		if (Spi_Fr_Res == FR_OK)
		{
			printf(">挂载成功！\r\n");
		}
		else if(Spi_Fr_Res == FR_NO_FILESYSTEM)
		{
			printf(">挂载成功,格式化不成功！\r\n");
			while(1);
		}
		else
		{
			printf(">初始化错误！\r\n");
			while(1);
		}
	}
	else if(Spi_Fr_Res == FR_OK)
	{
		printf(">挂载成功，有文件系统，可直接读写！\r\n");
	}
	else
	{
		printf(">挂载错误，文件系统初始化不成功！\r\n");
		printf(">错误代码:%x\r\n",Spi_Fr_Res);
	}
	Spi_File_W_R_Test();
	while(1);
}

//测试文件创建和读写
void Spi_File_W_R_Test(void)
{
	u8 	Spi_Flash_TxBuffer[] = "This is a flash test program!中文据说也能写！\r\n";
	u8 	Spi_Flash_RxBuffer[RxBufferSize] = {0};
	UINT File_Bw_Num_W = 0;
	UINT File_Bw_Num_R = 0;
	
	Spi_Fr_Res = f_open(&Spi_File,"0:Test.txt",FA_CREATE_ALWAYS | FA_WRITE);//同时读写不行 | FA_READ);
	if(Spi_Fr_Res != FR_OK)
	{
		printf(">文件打开失败！\r\n");
		printf(">错误代码:%x\r\n",Spi_Fr_Res);
		while(1);
	}
	printf(">文件打开成功！\r\n");
	Spi_Fr_Res = f_write(&Spi_File,Spi_Flash_TxBuffer,RxBufferSize,&File_Bw_Num_W);
	if(Spi_Fr_Res != FR_OK)
	{
		printf(">文件写入失败！\r\n");
		printf(">错误代码:%x\r\n",Spi_Fr_Res);
		while(1);
	}
	printf(">文件写入成功！\r\n");
	Spi_Fr_Res = f_close(&Spi_File);
	
	Spi_Fr_Res = f_open(&Spi_File,"0:Test.txt",FA_OPEN_EXISTING | FA_READ);
	
	Spi_Fr_Res = f_read(&Spi_File,Spi_Flash_RxBuffer,RxBufferSize,&File_Bw_Num_R);
	if(Spi_Fr_Res != FR_OK)
	{
		printf(">文件读取失败！\r\n");
		printf(">错误代码:%x\r\n",Spi_Fr_Res);
		while(1);
	}
	printf(">文件读取成功！读取内容为：\r\n");
	printf("%s",Spi_Flash_RxBuffer);
	Spi_Fr_Res = f_close(&Spi_File);
	Spi_Fr_Res = f_mount(NULL,"0:",1);
	if(Spi_Fr_Res != FR_OK)
	{
		printf(">文件系统移植失败！\r\n");
		printf(">错误代码:%x\r\n",Spi_Fr_Res);
		while(1);
	}
	printf(">恭喜你，文件系统移植成功！\r\n");
	while(1);
	
}

