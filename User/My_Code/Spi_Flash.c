#include "Spi_Flash.h"

//��ʼ�����ź�SPI����
void Spi_Flash_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	SPI_InitTypeDef		SPI_InitStructure;

	//�ȳ�ʼ��GPIO�������
	//���ܸ��õģ���ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//NSS���ų�ʼ��Ҫ��OUT_PP,��������
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
	
	//��ʼ����ɺ�Ҫ��ʧ��CS����,��ȥ����SPI����������
	Spi_Flash_Cs_Disable();
	
	//����SPIʱ����ز���
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	//������Ԥ��Ƶ,SPI����Ƶ��,��APB2���߷�Ƶ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	//�ڶ���ʱ����ȡ����
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	//����ʱ�Ӽ��ԣ�����Ϊ��
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	//����CRC����Ķ���ֵ���ֲ�д7
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	//SPI����֡�ṹ,8λ֡�ṹ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	//SPI���䷽��˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	//��λ��ǰ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	//��ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	//NSS��SSIλ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	//��ʼ������
	SPI_Init(W25Q64_SPIX, &SPI_InitStructure);
	//ʹ��SPI
	SPI_Cmd(W25Q64_SPIX,ENABLE);
}

//���豸����һ���ֽ�
u8 Spi_Flash_SendByte(u8 Byte)
{
	//���ж��Ƿ������
	while(SPI_I2S_GetFlagStatus(W25Q64_SPIX,SPI_I2S_FLAG_TXE) == RESET);
	//����һ��byte
	SPI_I2S_SendData(W25Q64_SPIX,Byte);
	//�ȴ��������
	while(SPI_I2S_GetFlagStatus(W25Q64_SPIX,SPI_I2S_FLAG_RXNE) == RESET);
	//���ػ�ȡ��������
	return SPI_I2S_ReceiveData(W25Q64_SPIX);
}
//���豸��һ���ֽ�
u8 Spi_Flash_ReadByte(void)
{
	return	(Spi_Flash_SendByte(DummyByte));
}
//��ȡ�豸deviceID
void Spi_Flash_Read_W25Q64_JEDEC_ID(void)
{
	u8 Manufator_ID = 0;
	u8 Menmory_Type_ID = 0;
	u8 Device_ID = 0;
	//��ʹ��cs�ܽ�
	Spi_Flash_Cs_Enable();
	//дҪ���ĵ�ַ
	Spi_Flash_SendByte(W25Q64_JEDECID_REG);
	//��Manufator_ID
	Manufator_ID = Spi_Flash_ReadByte();
	//��Menmory_Type_ID
	Menmory_Type_ID = Spi_Flash_ReadByte();
	//��Device_ID
	Device_ID = Spi_Flash_ReadByte();
	//ʧ��CS�ܽ�
	Spi_Flash_Cs_Disable();
	//��ӡ����Ӧ��ID��Ϣ
	printf("�豸��Manufator_ID��:%x\r\n",Manufator_ID);
	printf("�豸��Menmory_Type_ID��:%x\r\n",Menmory_Type_ID);
	printf("�豸��Device_ID��:%x\r\n",Device_ID);
	while(1);
}
//������һ���Ĵ����е�deviceID
void Spi_Flash_Read_W25Q64_ID2(void)
{
	u8 Device_ID = 0;
	//��ʹ��cs�ܽ�
	Spi_Flash_Cs_Enable();
	//дҪ���ĵ�ַ
	Spi_Flash_SendByte(0xAB);
	Spi_Flash_ReadByte();
	Spi_Flash_ReadByte();
	Spi_Flash_ReadByte();
	Device_ID = Spi_Flash_ReadByte();
	//ʧ��CS�ܽ�
	Spi_Flash_Cs_Disable();
	printf("�豸��Device_ID��:%x\r\n",Device_ID);
	while(1);
}








