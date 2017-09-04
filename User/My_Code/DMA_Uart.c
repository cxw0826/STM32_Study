#include "DMA_Uart.h"

//����һ������buffer�ͽ���buffer
u8	DMA_Uart_SendBuffer[DMA_Uart_Buffer_Size];
u8	DMA_Uart_RecevBuffer[DMA_Uart_Buffer_Size];

//��ʼ�����ں���
void Uartx_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	//�򿪴��ڹ���ʱ��
	RCC_APB2PeriphClockCmd(DMA_Uart_Func_CLK,ENABLE);
	//�ȳ�ʼ��Ӳ���ܽ�,����AFIO
	RCC_APB2PeriphClockCmd(DMA_Uart_IO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin 	= DMA_Uart_TX_Pin;
	GPIO_Init(DMA_Uart_Port,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin		= DMA_Uart_RX_Pin;
	GPIO_Init(DMA_Uart_Port,&GPIO_InitStructure);
	//��ʼ������1��ز���
	//���ò�����
	USART_InitStructure.USART_BaudRate	=	DMA_Uart_Baud_Rate;
	//��Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//���ͽ���ģʽ
	USART_InitStructure.USART_Mode		=	USART_Mode_Rx | USART_Mode_Tx;
	//��żУ��,��У��
	USART_InitStructure.USART_Parity	=	USART_Parity_No;
	//ֹͣλ1λ
	USART_InitStructure.USART_StopBits	=	USART_StopBits_1;
	//�ֽڳ���8bit
	USART_InitStructure.USART_WordLength =  USART_WordLength_8b;
	//��ʼ������
	USART_Init(DMA_Uartx,&USART_InitStructure);
	//ʹ�ܴ���1
	USART_Cmd(DMA_Uartx,ENABLE);
	//��DMA������������
	USART_DMACmd(DMA_Uartx,USART_DMAReq_Tx,ENABLE);
	
	//TXE�����ж�,TC��������ж�,RXNE�����ж�,PE��ż�����ж�,�����Ƕ��     
	USART_ITConfig(DMA_Uartx,USART_IT_RXNE,ENABLE);  
}

//��ʼ��DMA��ز�������
void DMA_Config(void)
{
	DMA_InitTypeDef		DMA_InitStructure;
    //����DMAʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//�ȳ�ʼ��RXͨ��
	//����RX buffer size
	DMA_InitStructure.DMA_BufferSize	=	DMA_Uart_Buffer_Size;
	//���䷽��������Ϊ����Դ
	DMA_InitStructure.DMA_DIR			=	DMA_DIR_PeripheralSRC;
	//��ֹ�ڴ浽�ڴ洫��
	DMA_InitStructure.DMA_M2M			=	DMA_M2M_Disable;
	//�ڴ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA_Uart_RecevBuffer;
	//�ڴ洫�����ݵ�ԪΪ�ֽ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//�ڴ��ַ����
	DMA_InitStructure.DMA_MemoryInc		=	DMA_MemoryInc_Enable;
	//DMAģʽ������ѭ��
	DMA_InitStructure.DMA_Mode			=	DMA_Mode_Circular;
	//����(����)����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(DMA_Uartx->DR));
	//����(����)�������ݵ�ԪΪ�ֽ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//����(����)��ַ����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//����(����)�ж����ȼ�����
	DMA_InitStructure.DMA_Priority	=	DMA_Priority_Medium;
	//��ʼ������
	DMA_Init(DMA_Uart_RX_Channel,&DMA_InitStructure);
	//ʹ��DMA
	DMA_Cmd(DMA_Uart_RX_Channel,ENABLE);

	
	//�ٳ�ʼ��TXͨ��
	//����TX buffer size
	DMA_InitStructure.DMA_BufferSize	=	DMA_Uart_Buffer_Size;
	//���䷽���ڴ���Ϊ����Դ
	DMA_InitStructure.DMA_DIR			=	DMA_DIR_PeripheralDST;
	//��ֹ�ڴ浽�ڴ洫��
	DMA_InitStructure.DMA_M2M			=	DMA_M2M_Disable;
	//�ڴ��ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA_Uart_SendBuffer;
	//�ڴ洫�����ݵ�ԪΪ�ֽ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//�ڴ��ַ����
	DMA_InitStructure.DMA_MemoryInc		=	DMA_MemoryInc_Enable;
	//DMAģʽ������ѭ��
	//DMA_InitStructure.DMA_Mode			=	DMA_Mode_Circular;
	DMA_InitStructure.DMA_Mode			=	DMA_Mode_Normal;
	//����(����)����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(DMA_Uartx->DR));
	//����(����)�������ݵ�ԪΪ�ֽ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//����(����)��ַ����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//����(����)�ж����ȼ�����
	DMA_InitStructure.DMA_Priority	=	DMA_Priority_High;
	//��ʼ������
	DMA_Init(DMA_Uart_TX_Channel,&DMA_InitStructure);
	

	//ʹ��DMA
	//DMA_Cmd(DMA_Uart_TX_Channel,ENABLE);
	//����DMA�ж�
    DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
}

void DMA_Nvic_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//����UART1�ж�    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);  
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;               //ͨ������Ϊ����1�ж�    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;       //�ж�ռ�ȵȼ�0    
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;              //�ж���Ӧ���ȼ�0    
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //���ж�    
	NVIC_Init(&NVIC_InitStructure);                                 //��ʼ�� 
	
    //DMA�����ж�����  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);  
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure); 
	
    //DMA�����ж�����  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);  
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure); 
}

//��ʼ���������
void DMA_Uart_Init(void)
{
	Uartx_Config();
	DMA_Config();
	DMA_Nvic_Config();
}

/*******************************************************************************
* Function Name  : �ض���ϵͳputchar����int fputc(int ch, FILE *f)
* Description    : ���ڷ�һ���ֽ�
* Input          : int ch, FILE *f
* Output         : 
* Return         : int ch
* �����ʹ��printf�Ĺؼ�
*******************************************************************************/
int fputc(int ch, FILE *f)
{
    //USART_SendData(USART1, (u8) ch);
    USART1->DR = (u8) ch;
    
    /* Loop until the end of transmission */
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {
    }
    return ch;
}

//����DMA������Գ���
void DMA_TX_Uart_Test(void)

{
	u8	Data_Cnt = 0;
	
	//��SendBuffer�������
	for(Data_Cnt=0;Data_Cnt<DMA_Uart_Buffer_Size;Data_Cnt++)
	{
		DMA_Uart_SendBuffer[Data_Cnt] = 0x30 + Data_Cnt;
	}
	
	printf("Start DMA1:\r\n");
	//ʹ��DMAͨ��
	DMA_Cmd(DMA_Uart_TX_Channel,ENABLE);
	//���������ɣ��ر�DMAͨ��
	//while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET);
	//DMA_Cmd(DMA_Uart_TX_Channel,DISABLE);

	Delay_ms(500);
	Delay_ms(500);
	//��SendBuffer�������
	for(Data_Cnt=0;Data_Cnt<DMA_Uart_Buffer_Size;Data_Cnt++)
	{
		DMA_Uart_SendBuffer[Data_Cnt] = 0x31 + Data_Cnt;
	}
	//���ô������ݳ���  
	DMA_SetCurrDataCounter(DMA_Uart_TX_Channel,DMA_Uart_Buffer_Size);
	
	//��DMA������������
	printf("\r\n");
	printf("Start DMA2:\r\n");
	//normalģʽ��DMA������ɺ�Ҫ�ֶ��ر�DMAͨ��,��Ҫ���´�
	DMA_Cmd(DMA_Uart_TX_Channel,ENABLE);
	
	while(1);
}


void DMA1_Channel4_IRQHandler(void)  
{ 
	DMA_Cmd(DMA_Uart_TX_Channel,DISABLE);
	DMA_ClearFlag(DMA1_FLAG_TC4);
	//if(DMA_GetFlagStatus(DMA1_FLAG_TC4)==SET)
	/*
	if(0)
	{
        USART_DMACmd(USART1, USART_DMAReq_Tx, DISABLE);
        USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
		//�����־λ  
		DMA_ClearFlag(DMA1_FLAG_TC4);
		//�ر�DMA   
		DMA_Cmd(DMA1_Channel4,DISABLE);
		DMA_Uart_TX_Flag = 1;
	}     
	*/
}  

void DMA1_Channel5_IRQHandler(void)
{
	
}

