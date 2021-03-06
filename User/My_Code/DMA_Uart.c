#include "DMA_Uart.h"

//开辟一个发送buffer和接收buffer
u32	DMA_Uart_SendBuffer[DMA_Uart_Buffer_Size];
u32	DMA_Uart_RecevBuffer[DMA_Uart_Buffer_Size];

//初始化串口函数
void Uartx_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	//打开串口功能时钟
	RCC_APB2PeriphClockCmd(DMA_Uart_Func_CLK,ENABLE);
	//先初始化硬件管脚,启动AFIO
	RCC_APB2PeriphClockCmd(DMA_Uart_IO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin 	= DMA_Uart_TX_Pin;
	GPIO_Init(DMA_Uart_Port,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin		= DMA_Uart_RX_Pin;
	GPIO_Init(DMA_Uart_Port,&GPIO_InitStructure);
	//初始化串口1相关参数
	//设置波特率
	USART_InitStructure.USART_BaudRate	=	DMA_Uart_Baud_Rate;
	//无硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//发送接收模式
	USART_InitStructure.USART_Mode		=	USART_Mode_Rx | USART_Mode_Tx;
	//奇偶校验,无校验
	USART_InitStructure.USART_Parity	=	USART_Parity_No;
	//停止位1位
	USART_InitStructure.USART_StopBits	=	USART_StopBits_1;
	//字节长度8bit
	USART_InitStructure.USART_WordLength =  USART_WordLength_8b;
	//初始化参数
	USART_Init(DMA_Uartx,&USART_InitStructure);
	//使能串口1
	USART_Cmd(DMA_Uartx,ENABLE);
	//向DMA发出发送请求
	USART_DMACmd(DMA_Uartx,USART_DMAReq_Tx,ENABLE);
	//向DMA发出接收请求
	USART_DMACmd(DMA_Uartx,USART_DMAReq_Rx,ENABLE);
	
	//TXE发送中断,TC传输完成中断,RXNE接收中断,PE奇偶错误中断,可以是多个     
	USART_ITConfig(DMA_Uartx,USART_IT_RXNE | USART_IT_IDLE,ENABLE);  
}

//初始化DMA相关参数函数
void DMA_Config(void)
{
	DMA_InitTypeDef		DMA_InitStructure;
    //启动DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	//先初始化RX通道
	//配置RX buffer size
	DMA_InitStructure.DMA_BufferSize	=	DMA_Uart_Buffer_Size;
	//传输方向，外设作为传输源
	DMA_InitStructure.DMA_DIR			=	DMA_DIR_PeripheralSRC;
	//禁止内存到内存传输
	DMA_InitStructure.DMA_M2M			=	DMA_M2M_Disable;
	//内存地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA_Uart_RecevBuffer;
	//内存传输数据单元为字节
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//内存地址自增
	DMA_InitStructure.DMA_MemoryInc		=	DMA_MemoryInc_Enable;
	//DMA模式，不断循环
	//DMA_InitStructure.DMA_Mode			=	DMA_Mode_Circular;
	DMA_InitStructure.DMA_Mode			=	DMA_Mode_Normal;
	//外设(串口)基地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(DMA_Uartx->DR));
	//外设(串口)传输数据单元为字节
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//外设(串口)地址不增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//外设(串口)中断优先级，中
	DMA_InitStructure.DMA_Priority	=	DMA_Priority_Medium;
	//初始化接收
	DMA_Init(DMA_Uart_RX_Channel,&DMA_InitStructure);
	//使能DMA
	DMA_Cmd(DMA_Uart_RX_Channel,ENABLE);
	//允许DMA中断
    DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);

	
	//再初始化TX通道
	//配置TX buffer size
	DMA_InitStructure.DMA_BufferSize	=	DMA_Uart_Buffer_Size;
	//传输方向，内存作为传输源
	DMA_InitStructure.DMA_DIR			=	DMA_DIR_PeripheralDST;
	//禁止内存到内存传输
	DMA_InitStructure.DMA_M2M			=	DMA_M2M_Disable;
	//内存地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA_Uart_SendBuffer;
	//内存传输数据单元为字节
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//内存地址自增
	DMA_InitStructure.DMA_MemoryInc		=	DMA_MemoryInc_Enable;
	//DMA模式，不断循环
	//DMA_InitStructure.DMA_Mode			=	DMA_Mode_Circular;
	DMA_InitStructure.DMA_Mode			=	DMA_Mode_Normal;
	//外设(串口)基地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&(DMA_Uartx->DR));
	//外设(串口)传输数据单元为字节
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//外设(串口)地址不增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//外设(串口)中断优先级，高
	DMA_InitStructure.DMA_Priority	=	DMA_Priority_High;
	//初始化发送
	DMA_Init(DMA_Uart_TX_Channel,&DMA_InitStructure);
	
	//使能DMA
	//DMA_Cmd(DMA_Uart_TX_Channel,ENABLE);
	//允许DMA中断
    DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
}

void DMA_Nvic_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//配置UART1中断    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);  
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;               //通道设置为串口1中断    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;       //中断占先等级0    
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;              //中断响应优先级0    
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //打开中断    
	NVIC_Init(&NVIC_InitStructure);                                 //初始化 
	
    //DMA发送中断设置  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);  
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure); 
	
    //DMA接收中断设置  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);  
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure); 
}

//初始化程序入口
void DMA_Uart_Init(void)
{
	Uartx_Config();
	DMA_Config();
	DMA_Nvic_Config();
}

/*******************************************************************************
* Function Name  : 重定义系统putchar函数int fputc(int ch, FILE *f)
* Description    : 串口发一个字节
* Input          : int ch, FILE *f
* Output         : 
* Return         : int ch
* 这个是使用printf的关键
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

//串口DMA传输测试程序
void DMA_TX_Uart_Test(void)

{
	u8	Data_Cnt = 0;
	
	//往SendBuffer填充数据
	for(Data_Cnt=0;Data_Cnt<DMA_Uart_Buffer_Size;Data_Cnt++)
	{
		DMA_Uart_SendBuffer[Data_Cnt] = 0x30 + Data_Cnt;
	}
	
	printf("Start DMA1:\r\n");
	//使能DMA通道
	DMA_Cmd(DMA_Uart_TX_Channel,ENABLE);
	//如果发送完成，关闭DMA通道
	//while(DMA_GetFlagStatus(DMA1_FLAG_TC4) == RESET);
	//DMA_Cmd(DMA_Uart_TX_Channel,DISABLE);

	Delay_ms(500);
	Delay_ms(500);
	//往SendBuffer填充数据
	for(Data_Cnt=0;Data_Cnt<DMA_Uart_Buffer_Size;Data_Cnt++)
	{
		DMA_Uart_SendBuffer[Data_Cnt] = 0x31 + Data_Cnt;
	}
	//设置传输数据长度  
	DMA_SetCurrDataCounter(DMA_Uart_TX_Channel,DMA_Uart_Buffer_Size);
	
	//向DMA发出发送请求
	printf("\r\n");
	printf("Start DMA2:\r\n");
	//normal模式下DMA传输完成后要手动关闭DMA通道,需要重新打开
	DMA_Cmd(DMA_Uart_TX_Channel,ENABLE);
	
	while(1);
}

//DMA发送中断处理函数
void DMA1_Channel4_IRQHandler(void)  
{
	//判断是否为传输完成中断
	if(DMA_GetFlagStatus(DMA1_FLAG_TC4) != RESET)
	{
		//先关闭DMA发送通道
		DMA_Cmd(DMA_Uart_TX_Channel,DISABLE);
		//清除标志位
		DMA_ClearFlag(DMA1_FLAG_GL4);
		//重新设置传输数据长度  
		DMA_SetCurrDataCounter(DMA_Uart_TX_Channel,DMA_Uart_Buffer_Size);
	}
}  

u8 DMA_Uart_Recv_Data_Process_Finish = 0;


//串口1接收中断处理函数
//主要处理空闲中断后，Rbuff数据copy到Tbuff
void USART1_IRQHandler(void)
{
	/*
	//如果收到了空闲中断，说明串口传输已经完成
	if(USART_GetFlagStatus(DMA_Uartx, USART_FLAG_IDLE) != RESET)
	{
		//清除中断标志
		USART_ClearFlag(DMA_Uartx,USART_FLAG_IDLE | USART_FLAG_RXNE);
	}
	*/
}

//串口接收后发送测试函数
void DMA_Uart_Recv_Send_Test(void)
{
	memset(DMA_Uart_SendBuffer,0,sizeof(DMA_Uart_SendBuffer));
	memset(DMA_Uart_RecevBuffer,0,sizeof(DMA_Uart_SendBuffer));
	printf("Test OK!");
	while(1)
	{
		//是否接收完毕
		if(DMA_Uart_Recv_Data_Process_Finish)
		{
			memset(DMA_Uart_SendBuffer,0,sizeof(DMA_Uart_SendBuffer));
			//将RxBuffer数据拷贝到TxBuffer
			memcpy(DMA_Uart_SendBuffer,DMA_Uart_RecevBuffer,sizeof(DMA_Uart_SendBuffer));
			DMA_Uart_Recv_Data_Process_Finish = 0;
			//接收完毕后打开DMA发送通道
			DMA_Cmd(DMA_Uart_TX_Channel,ENABLE);
			Delay_ms(100);
			DMA_Cmd(DMA_Uart_RX_Channel,ENABLE);
		}
	}
}

void DMA1_Channel5_IRQHandler(void)
{
	//判断是否为传输完成中断
	if(DMA_GetFlagStatus(DMA1_FLAG_TC5) != RESET)
	{
		//先关闭DMA接收通道
		DMA_Cmd(DMA_Uart_RX_Channel,DISABLE);
		//清除标志位
		DMA_ClearFlag(DMA1_FLAG_GL5);
		//重新设置传输数据长度  
		DMA_SetCurrDataCounter(DMA_Uart_RX_Channel,DMA_Uart_Buffer_Size);
		//打开DMA接收通道
		//DMA_Cmd(DMA_Uart_RX_Channel,ENABLE);
		//给出数据接收处理完毕信号
		DMA_Uart_Recv_Data_Process_Finish = 1;
	}
}

