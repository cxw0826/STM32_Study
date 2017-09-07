#include "Spi_Flash.h"

//初始化引脚和SPI功能
void Spi_Flash_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	SPI_InitTypeDef		SPI_InitStructure;

	//先初始化GPIO相关引脚
	//功能复用的，打开时钟
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
void Spi_Flash_Read_W25Q64_JEDEC_ID(void)
{
	u8 Manufator_ID = 0;
	u8 Menmory_Type_ID = 0;
	u8 Device_ID = 0;
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
	//打印出相应的ID信息
	printf("设备的Manufator_ID是:%x\r\n",Manufator_ID);
	printf("设备的Menmory_Type_ID是:%x\r\n",Menmory_Type_ID);
	printf("设备的Device_ID是:%x\r\n",Device_ID);
	while(1);
}
//读另外一个寄存器中的deviceID
void Spi_Flash_Read_W25Q64_ID2(void)
{
	u8 Device_ID = 0;
	//先使能cs管脚
	Spi_Flash_Cs_Enable();
	//写要读的地址
	Spi_Flash_SendByte(0xAB);
	Spi_Flash_ReadByte();
	Spi_Flash_ReadByte();
	Spi_Flash_ReadByte();
	Device_ID = Spi_Flash_ReadByte();
	//失能CS管脚
	Spi_Flash_Cs_Disable();
	printf("设备的Device_ID是:%x\r\n",Device_ID);
	while(1);
}








