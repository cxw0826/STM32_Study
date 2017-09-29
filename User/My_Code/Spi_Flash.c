#include "Spi_Flash.h"
#include "diskio.h"

//初始化引脚和SPI功能
void Spi_Flash_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	SPI_InitTypeDef		SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//NSS引脚初始化要是OUT_PP,否则会出错
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin	  = W25Q64_SPI_NSS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(W25Q64_SPI_Port,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin	  = W25Q64_SPI_SCK;
	GPIO_Init(W25Q64_SPI_Port,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin	  = W25Q64_SPI_MISO;
	GPIO_Init(W25Q64_SPI_Port,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin	  = W25Q64_SPI_MOSI;
	GPIO_Init(W25Q64_SPI_Port,&GPIO_InitStructure);
	
	//初始化完成后要先失能CS引脚,再去配置SPI，否则会出错
	Spi_Flash_Cs_Disable();
	
	//配置SPI时序相关参数
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	//波特率预分频,SPI工作频率,由APB2总线分频而来
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	//第二个时钟沿取数据
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	//空闲时钟极性，设置为高
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	//用于CRC计算的多项值，手册写7
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	//SPI传输帧结构,8位帧结构
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	//SPI传输方向，双线双向全双工
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	//高位在前
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	//主模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	//NSS由SSI位控制
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	//初始化参数
	SPI_Init(W25Q64_SPIX, &SPI_InitStructure);
	//使能SPI
	SPI_Cmd(W25Q64_SPIX,ENABLE);
}

//向设备发送一个字节
u8 Spi_Flash_SendByte(u8 Byte)
{
	//先判断是否发送完成
	while(SPI_I2S_GetFlagStatus(W25Q64_SPIX,SPI_I2S_FLAG_TXE) == RESET);
	//发送一个byte
	SPI_I2S_SendData(W25Q64_SPIX,Byte);
	//等待接收完成
	while(SPI_I2S_GetFlagStatus(W25Q64_SPIX,SPI_I2S_FLAG_RXNE) == RESET);
	//返回获取到的数据
	return SPI_I2S_ReceiveData(W25Q64_SPIX);
}
//向设备读一个字节
u8 Spi_Flash_ReadByte(void)
{
	return	(Spi_Flash_SendByte(DummyByte));
}
//读取设备deviceID
u32 Spi_Flash_Read_JEDEC_ID(void)
{
	u8 	Manufator_ID = 0;
	u8 	Menmory_Type_ID = 0;
	u8 	Device_ID = 0;
	u32 DEV_ID = 0;
	//先使能cs管脚
	Spi_Flash_Cs_Enable();
	//写要读的地址
	Spi_Flash_SendByte(W25Q64_JEDECID_REG);
	//读Manufator_ID
	Manufator_ID = Spi_Flash_ReadByte();
	//读Menmory_Type_ID
	Menmory_Type_ID = Spi_Flash_ReadByte();
	//读Device_ID
	Device_ID = Spi_Flash_ReadByte();
	//失能CS管脚
	Spi_Flash_Cs_Disable();
	DEV_ID = (Manufator_ID << 16) | (Menmory_Type_ID << 8) | Device_ID;
	//打印出相应的ID信息
	//printf("设备的Manufator_ID是:%x\r\n",Manufator_ID);
	//printf("设备的Menmory_Type_ID是:%x\r\n",Menmory_Type_ID);
	//printf("设备的Device_ID是:%x\r\n",Device_ID);
	//printf("设备的Device_ID是:%x\r\n",DEV_ID);
	return DEV_ID;
}
//等待操作完成函数，原理是获取设备的busy状态位
void Spi_Flash_Wait_Instruction_End(void)
{
	u8 Instruction_State;
	//先使能cs管脚
	Spi_Flash_Cs_Enable();
	//写要读的地址
	Spi_Flash_SendByte(W25Q64_READ_STATE_CMD);
	do
	{
		Instruction_State = Spi_Flash_SendByte(DummyByte);
	}
	//当设备忙的时候，busy置1，与上01还是1，判断是1继续循环
	while ((Instruction_State & W25Q64_WIP_FLAG) == SET);
	Spi_Flash_Cs_Disable();
}
//读另外一个寄存器中的deviceID
void Spi_Flash_Read_ID2(void)
{
	u8 Device_ID = 0;
	Spi_Flash_Wait_Instruction_End();
	//先使能cs管脚
	Spi_Flash_Cs_Enable();
	//写要读的地址
	Spi_Flash_SendByte(W25Q64_DEVICEID_REG);
	Spi_Flash_ReadByte();
	Spi_Flash_ReadByte();
	Spi_Flash_ReadByte();
	Device_ID = Spi_Flash_ReadByte();
	//失能CS管脚
	Spi_Flash_Cs_Disable();
	Spi_Flash_Wait_Instruction_End();
	printf("设备的Device_ID是:%x\r\n",Device_ID);
	while(1);
}
//先去除DEVICE写保护
void Spi_Flash_Write_Enable(void)
{
	//先拉低cs
	Spi_Flash_Cs_Enable();
	//写入写不保护指令
	Spi_Flash_SendByte(W25Q64_WRITE_ENABLE);
	//拉高cs
	Spi_Flash_Cs_Disable();
}
//擦除一个sector
void Spi_Flash_Erase_Sector(u32 Sector_Num)
{
	Spi_Flash_Write_Enable();
	Spi_Flash_Wait_Instruction_End();
	Spi_Flash_Cs_Enable();
	//写入sector擦除指令
	Spi_Flash_SendByte(W25Q64_SECTOR_ERASE_CMD);
	//跟着发送地址从高到低
	//Spi_Flash_SendByte((Sector_Num & 0xFF000000)>>24);
	Spi_Flash_SendByte((Sector_Num & 0xFF0000)>>16);
	Spi_Flash_SendByte((Sector_Num & 0xFF00)>>8);
	Spi_Flash_SendByte( Sector_Num & 0xFF);
	//拉高cs
	Spi_Flash_Cs_Disable();
	Spi_Flash_Wait_Instruction_End();
}
//写页函数
void Spi_Flash_Write_Page(u32 Addr,u8 *Data,u32 NumOfByte)
{
	Spi_Flash_Wait_Instruction_End();
	Spi_Flash_Write_Enable();
	//判断是否超出了写范围
	//避免写操作覆盖前面的数据
	if(NumOfByte>W25Q64_PAGE_SIZE)
	{
		//一页有256BYTES，如果要写的大于256BYTE，就设置最大写256BYTE
		NumOfByte = W25Q64_PAGE_SIZE;
	}
	Spi_Flash_Cs_Enable();
	//发出写page指令
	Spi_Flash_SendByte(W25Q64_PAGE_WRITE_CMD);
	//跟上写地址
	Spi_Flash_SendByte((Addr & 0xFF0000) >> 16);
	Spi_Flash_SendByte((Addr & 0xFF00) >> 8);
	Spi_Flash_SendByte( Addr & 0xFF);
	//循环写入数据
	while(NumOfByte--)
	{
		Spi_Flash_SendByte(*Data);
		Data++;
	}
	Spi_Flash_Cs_Disable();
	Spi_Flash_Wait_Instruction_End();
}
//写BUFFER数据
void Spi_Flash_Write_Buffer(u32 ADDR,u8* DATA,u32 NumOfByte)
{
	//数据一共有多少页
	u8 NumOfPage = 0;
	//不够一页的数据有多少
	u8 NumOfSingleData = 0;
	//页地址
	u8 PageAddr = 0;
	//总共要写多少页
	u8 PageTotalCnt = 0;
	//中间变量
	u8 CntTmp = 0;

	//W25Q64一个存储地址是1个BYTE
	PageAddr = ADDR % W25Q64_PAGE_SIZE;
	//开始写的那一页还剩多少空间
	PageTotalCnt = W25Q64_PAGE_SIZE - PageAddr;
	NumOfPage = NumOfByte / W25Q64_PAGE_SIZE;
	NumOfSingleData = NumOfByte % W25Q64_PAGE_SIZE;

	//如果起始地址是页的整数倍
	if(PageAddr == 0)
	{
		//写入数据不够1页
		if(NumOfPage == 0)
		{
			Spi_Flash_Write_Page(ADDR, DATA, NumOfByte);
		}
		//写入数据超过1页
		else
		{
			//先写整数页
			while(NumOfPage--)
			{
				Spi_Flash_Write_Page(ADDR, DATA, W25Q64_PAGE_SIZE);
				ADDR = ADDR + W25Q64_PAGE_SIZE;
				DATA = DATA + W25Q64_PAGE_SIZE;
			}
			//再写剩下的单个数据
			Spi_Flash_Write_Page(ADDR, DATA, NumOfSingleData);
		}
	}
	//如果写入的地址是不对齐的
	else
	{
		//如果要写入的数据不够一页
		if(NumOfPage == 0)
		{
			//判断要写入的数据是否超出一页剩下的空间
			if(NumOfSingleData > PageTotalCnt)
			{
				//缓存一页写不完的数据
				CntTmp = NumOfSingleData - PageTotalCnt;
				//写一页剩下的空间
				Spi_Flash_Write_Page(ADDR, DATA, PageTotalCnt);
				//地址指向下一页起始
				ADDR = ADDR + PageTotalCnt;
				//数据指向下一页起始
				DATA = DATA + PageTotalCnt;
				//在下一页中写剩下的内容
				Spi_Flash_Write_Page(ADDR, DATA, CntTmp);
			}
			//如果要写的数据这一页够装，就直接写
			else
			{
				Spi_Flash_Write_Page(ADDR, DATA, NumOfByte);
			}
		}
		//如果写超过1页
		else
		{
			//先计算不对齐的部分数据
			NumOfByte = NumOfByte - PageTotalCnt;
			//再计算一共还剩多少页
			NumOfPage = NumOfByte / W25Q64_PAGE_SIZE;
			//再计算还剩多少个单个数据
			NumOfSingleData = NumOfPage % W25Q64_PAGE_SIZE;
			//先写不对齐的数据
			Spi_Flash_Write_Page(ADDR, DATA, PageTotalCnt);
			//地址指向下一页
			ADDR = ADDR + PageTotalCnt;
			//数据指向下一页
			DATA = DATA + PageTotalCnt;
			//循环写入整数页数据
			while(NumOfPage--)
			{
				Spi_Flash_Write_Page(ADDR, DATA, W25Q64_PAGE_SIZE);
				//地址指向下一页
				ADDR = ADDR + W25Q64_PAGE_SIZE;
				//数据指向下一页
				DATA = DATA + W25Q64_PAGE_SIZE;
			}
			//再判断是否还有单个的数据
			if(NumOfSingleData != 0)
			{
				Spi_Flash_Write_Page(ADDR, DATA, NumOfSingleData);
			}
		}
	}
}
//读buffer函数
void Spi_Flash_Read_Buffer(u32 ADDR,u8 *DATA,u32 NumOfRead)
{
	Spi_Flash_Wait_Instruction_End();
	Spi_Flash_Cs_Enable();
	Spi_Flash_SendByte(W25Q64_READ_DATA);
	Spi_Flash_SendByte((ADDR & 0xFF0000)>>16);
	Spi_Flash_SendByte((ADDR & 0xFF00)>>8);
	Spi_Flash_SendByte( ADDR & 0xFF);
	while(NumOfRead--)
	{
		*DATA = Spi_Flash_SendByte(DummyByte);
		DATA++;
	}
	Spi_Flash_Cs_Disable();
	Spi_Flash_Wait_Instruction_End();
}


//SPI_FLASH测试函数
void Spi_Flash_Test(void)
{
	u8 Spi_Flash_TxBuffer[] = "This is a flash test program!";
	u8 Spi_Flash_RxBuffer[RxBufferSize];
	u32 Spi_Print_Cnt;
	printf("SPI读ID测试!\r\n");
	Spi_Print_Cnt = Spi_Flash_Read_JEDEC_ID();
	printf("spi页擦除\r\n");
	Spi_Flash_Erase_Sector(W25Q64_MEMORY_SECTOR_0);
	printf("spi写buffer\r\n");
	Spi_Flash_Write_Buffer(W25Q64_MEMORY_SECTOR_0,Spi_Flash_TxBuffer,RxBufferSize);
	printf("写入的数据为：%s \r\n", Spi_Flash_TxBuffer);
	Spi_Flash_Read_Buffer(W25Q64_MEMORY_SECTOR_0,Spi_Flash_RxBuffer,RxBufferSize);
	printf("SPI读出来的数据是：\r\n");
	for(Spi_Print_Cnt=0;Spi_Print_Cnt<RxBufferSize;Spi_Print_Cnt++)
	{
		printf("%c",Spi_Flash_RxBuffer[Spi_Print_Cnt]);
	}
	printf("\r\n");
	printf("打印完毕，SPI测试完毕!\r\n");
}

//SPI_FLASH测试函数

//----------------------------------------------------------
//下面函数是FATFS文件系统接口函数
//
//
//----------------------------------------------------------

static volatile DSTATUS fatFs_Spi_Flash_Stat = STA_NOINIT;	/* Physical drive status */


//文件系统初始化函数
DSTATUS fatFs_Spi_Flash_Init(void)
{
	Spi_Flash_Init();
	if(W25Q64_DEVICE_ID == Spi_Flash_Read_JEDEC_ID())
	{
		//printf("W25Q64_FLASH_ID == Spi_Flash_Read_ID1()\r\n");
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
	if(W25Q64_DEVICE_ID == Spi_Flash_Read_JEDEC_ID())
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
	//sector+=512;//扇区偏移，外部Flash文件系统空间放在外部Flash后面6M空间
	Spi_Flash_Read_Buffer(sector << 12,buff,count<<12);
	
	return RES_OK;
}
//文件系统写接口
DRESULT fatFs_Spi_Flash_Write(BYTE *buff, DWORD sector, UINT count)
{
	u32 write_addr;  
	//sector+=512;//扇区偏移，外部Flash文件系统空间放在外部Flash后面6M空间
	write_addr = sector<<12;    
	Spi_Flash_Erase_Sector(write_addr);
	Spi_Flash_Write_Buffer(write_addr,buff,count<<12);
	return RES_OK;
}

