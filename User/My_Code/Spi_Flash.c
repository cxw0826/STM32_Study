#include "Spi_Flash.h"
#include "diskio.h"

//��ʼ�����ź�SPI����
void Spi_Flash_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	SPI_InitTypeDef		SPI_InitStructure;
	
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
u32 Spi_Flash_Read_JEDEC_ID(void)
{
	u8 	Manufator_ID = 0;
	u8 	Menmory_Type_ID = 0;
	u8 	Device_ID = 0;
	u32 DEV_ID = 0;
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
	DEV_ID = (Manufator_ID << 16) | (Menmory_Type_ID << 8) | Device_ID;
	//��ӡ����Ӧ��ID��Ϣ
	//printf("�豸��Manufator_ID��:%x\r\n",Manufator_ID);
	//printf("�豸��Menmory_Type_ID��:%x\r\n",Menmory_Type_ID);
	//printf("�豸��Device_ID��:%x\r\n",Device_ID);
	//printf("�豸��Device_ID��:%x\r\n",DEV_ID);
	return DEV_ID;
}
//�ȴ�������ɺ�����ԭ���ǻ�ȡ�豸��busy״̬λ
void Spi_Flash_Wait_Instruction_End(void)
{
	u8 Instruction_State;
	//��ʹ��cs�ܽ�
	Spi_Flash_Cs_Enable();
	//дҪ���ĵ�ַ
	Spi_Flash_SendByte(W25Q64_READ_STATE_CMD);
	do
	{
		Instruction_State = Spi_Flash_SendByte(DummyByte);
	}
	//���豸æ��ʱ��busy��1������01����1���ж���1����ѭ��
	while ((Instruction_State & W25Q64_WIP_FLAG) == SET);
	Spi_Flash_Cs_Disable();
}
//������һ���Ĵ����е�deviceID
void Spi_Flash_Read_ID2(void)
{
	u8 Device_ID = 0;
	Spi_Flash_Wait_Instruction_End();
	//��ʹ��cs�ܽ�
	Spi_Flash_Cs_Enable();
	//дҪ���ĵ�ַ
	Spi_Flash_SendByte(W25Q64_DEVICEID_REG);
	Spi_Flash_ReadByte();
	Spi_Flash_ReadByte();
	Spi_Flash_ReadByte();
	Device_ID = Spi_Flash_ReadByte();
	//ʧ��CS�ܽ�
	Spi_Flash_Cs_Disable();
	Spi_Flash_Wait_Instruction_End();
	printf("�豸��Device_ID��:%x\r\n",Device_ID);
	while(1);
}
//��ȥ��DEVICEд����
void Spi_Flash_Write_Enable(void)
{
	//������cs
	Spi_Flash_Cs_Enable();
	//д��д������ָ��
	Spi_Flash_SendByte(W25Q64_WRITE_ENABLE);
	//����cs
	Spi_Flash_Cs_Disable();
}
//����һ��sector
void Spi_Flash_Erase_Sector(u32 Sector_Num)
{
	Spi_Flash_Write_Enable();
	Spi_Flash_Wait_Instruction_End();
	Spi_Flash_Cs_Enable();
	//д��sector����ָ��
	Spi_Flash_SendByte(W25Q64_SECTOR_ERASE_CMD);
	//���ŷ��͵�ַ�Ӹߵ���
	//Spi_Flash_SendByte((Sector_Num & 0xFF000000)>>24);
	Spi_Flash_SendByte((Sector_Num & 0xFF0000)>>16);
	Spi_Flash_SendByte((Sector_Num & 0xFF00)>>8);
	Spi_Flash_SendByte( Sector_Num & 0xFF);
	//����cs
	Spi_Flash_Cs_Disable();
	Spi_Flash_Wait_Instruction_End();
}
//дҳ����
void Spi_Flash_Write_Page(u32 Addr,u8 *Data,u32 NumOfByte)
{
	Spi_Flash_Wait_Instruction_End();
	Spi_Flash_Write_Enable();
	//�ж��Ƿ񳬳���д��Χ
	//����д��������ǰ�������
	if(NumOfByte>W25Q64_PAGE_SIZE)
	{
		//һҳ��256BYTES�����Ҫд�Ĵ���256BYTE�����������д256BYTE
		NumOfByte = W25Q64_PAGE_SIZE;
	}
	Spi_Flash_Cs_Enable();
	//����дpageָ��
	Spi_Flash_SendByte(W25Q64_PAGE_WRITE_CMD);
	//����д��ַ
	Spi_Flash_SendByte((Addr & 0xFF0000) >> 16);
	Spi_Flash_SendByte((Addr & 0xFF00) >> 8);
	Spi_Flash_SendByte( Addr & 0xFF);
	//ѭ��д������
	while(NumOfByte--)
	{
		Spi_Flash_SendByte(*Data);
		Data++;
	}
	Spi_Flash_Cs_Disable();
	Spi_Flash_Wait_Instruction_End();
}
//дBUFFER����
void Spi_Flash_Write_Buffer(u32 ADDR,u8* DATA,u32 NumOfByte)
{
	//����һ���ж���ҳ
	u8 NumOfPage = 0;
	//����һҳ�������ж���
	u8 NumOfSingleData = 0;
	//ҳ��ַ
	u8 PageAddr = 0;
	//�ܹ�Ҫд����ҳ
	u8 PageTotalCnt = 0;
	//�м����
	u8 CntTmp = 0;

	//W25Q64һ���洢��ַ��1��BYTE
	PageAddr = ADDR % W25Q64_PAGE_SIZE;
	//��ʼд����һҳ��ʣ���ٿռ�
	PageTotalCnt = W25Q64_PAGE_SIZE - PageAddr;
	NumOfPage = NumOfByte / W25Q64_PAGE_SIZE;
	NumOfSingleData = NumOfByte % W25Q64_PAGE_SIZE;

	//�����ʼ��ַ��ҳ��������
	if(PageAddr == 0)
	{
		//д�����ݲ���1ҳ
		if(NumOfPage == 0)
		{
			Spi_Flash_Write_Page(ADDR, DATA, NumOfByte);
		}
		//д�����ݳ���1ҳ
		else
		{
			//��д����ҳ
			while(NumOfPage--)
			{
				Spi_Flash_Write_Page(ADDR, DATA, W25Q64_PAGE_SIZE);
				ADDR = ADDR + W25Q64_PAGE_SIZE;
				DATA = DATA + W25Q64_PAGE_SIZE;
			}
			//��дʣ�µĵ�������
			Spi_Flash_Write_Page(ADDR, DATA, NumOfSingleData);
		}
	}
	//���д��ĵ�ַ�ǲ������
	else
	{
		//���Ҫд������ݲ���һҳ
		if(NumOfPage == 0)
		{
			//�ж�Ҫд��������Ƿ񳬳�һҳʣ�µĿռ�
			if(NumOfSingleData > PageTotalCnt)
			{
				//����һҳд���������
				CntTmp = NumOfSingleData - PageTotalCnt;
				//дһҳʣ�µĿռ�
				Spi_Flash_Write_Page(ADDR, DATA, PageTotalCnt);
				//��ַָ����һҳ��ʼ
				ADDR = ADDR + PageTotalCnt;
				//����ָ����һҳ��ʼ
				DATA = DATA + PageTotalCnt;
				//����һҳ��дʣ�µ�����
				Spi_Flash_Write_Page(ADDR, DATA, CntTmp);
			}
			//���Ҫд��������һҳ��װ����ֱ��д
			else
			{
				Spi_Flash_Write_Page(ADDR, DATA, NumOfByte);
			}
		}
		//���д����1ҳ
		else
		{
			//�ȼ��㲻����Ĳ�������
			NumOfByte = NumOfByte - PageTotalCnt;
			//�ټ���һ����ʣ����ҳ
			NumOfPage = NumOfByte / W25Q64_PAGE_SIZE;
			//�ټ��㻹ʣ���ٸ���������
			NumOfSingleData = NumOfPage % W25Q64_PAGE_SIZE;
			//��д�����������
			Spi_Flash_Write_Page(ADDR, DATA, PageTotalCnt);
			//��ַָ����һҳ
			ADDR = ADDR + PageTotalCnt;
			//����ָ����һҳ
			DATA = DATA + PageTotalCnt;
			//ѭ��д������ҳ����
			while(NumOfPage--)
			{
				Spi_Flash_Write_Page(ADDR, DATA, W25Q64_PAGE_SIZE);
				//��ַָ����һҳ
				ADDR = ADDR + W25Q64_PAGE_SIZE;
				//����ָ����һҳ
				DATA = DATA + W25Q64_PAGE_SIZE;
			}
			//���ж��Ƿ��е���������
			if(NumOfSingleData != 0)
			{
				Spi_Flash_Write_Page(ADDR, DATA, NumOfSingleData);
			}
		}
	}
}
//��buffer����
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


//SPI_FLASH���Ժ���
void Spi_Flash_Test(void)
{
	u8 Spi_Flash_TxBuffer[] = "This is a flash test program!";
	u8 Spi_Flash_RxBuffer[RxBufferSize];
	u32 Spi_Print_Cnt;
	printf("SPI��ID����!\r\n");
	Spi_Print_Cnt = Spi_Flash_Read_JEDEC_ID();
	printf("spiҳ����\r\n");
	Spi_Flash_Erase_Sector(W25Q64_MEMORY_SECTOR_0);
	printf("spiдbuffer\r\n");
	Spi_Flash_Write_Buffer(W25Q64_MEMORY_SECTOR_0,Spi_Flash_TxBuffer,RxBufferSize);
	printf("д�������Ϊ��%s \r\n", Spi_Flash_TxBuffer);
	Spi_Flash_Read_Buffer(W25Q64_MEMORY_SECTOR_0,Spi_Flash_RxBuffer,RxBufferSize);
	printf("SPI�������������ǣ�\r\n");
	for(Spi_Print_Cnt=0;Spi_Print_Cnt<RxBufferSize;Spi_Print_Cnt++)
	{
		printf("%c",Spi_Flash_RxBuffer[Spi_Print_Cnt]);
	}
	printf("\r\n");
	printf("��ӡ��ϣ�SPI�������!\r\n");
}

//SPI_FLASH���Ժ���

//----------------------------------------------------------
//���溯����FATFS�ļ�ϵͳ�ӿں���
//
//
//----------------------------------------------------------

static volatile DSTATUS fatFs_Spi_Flash_Stat = STA_NOINIT;	/* Physical drive status */


//�ļ�ϵͳ��ʼ������
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

//�ļ�ϵͳ��ѯ״̬����
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
	//sector+=512;//����ƫ�ƣ��ⲿFlash�ļ�ϵͳ�ռ�����ⲿFlash����6M�ռ�
	Spi_Flash_Read_Buffer(sector << 12,buff,count<<12);
	
	return RES_OK;
}
//�ļ�ϵͳд�ӿ�
DRESULT fatFs_Spi_Flash_Write(BYTE *buff, DWORD sector, UINT count)
{
	u32 write_addr;  
	//sector+=512;//����ƫ�ƣ��ⲿFlash�ļ�ϵͳ�ռ�����ⲿFlash����6M�ռ�
	write_addr = sector<<12;    
	Spi_Flash_Erase_Sector(write_addr);
	Spi_Flash_Write_Buffer(write_addr,buff,count<<12);
	return RES_OK;
}

