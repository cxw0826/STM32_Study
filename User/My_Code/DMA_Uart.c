#include "DMA_Uart.h"

//开辟一个发送buffer和接收buffer
u8	DMA_Uart_SendBuffer[DMA_Uart_Buffer_Size];
u8	DMA_Uart_RecevBuffer[DMA_Uart_Buffer_Size];

//初始化串口函数
void Uartx_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	USART_InitTypeDef	USART_InitStructure;
	//先初始化硬件管脚
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
	//打开串口功能时钟
	RCC_APB2PeriphClockCmd(DMA_Uart_Func_CLK,ENABLE);
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
}

//初始化DMA相关参数函数
void DMA_Config(void)
{
	DMA_InitTypeDef		DMA_InitStructure;
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
	DMA_InitStructure.DMA_Mode			=	DMA_Mode_Circular;
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
	DMA_InitStructure.DMA_Mode			=	DMA_Mode_Circular;
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
	DMA_Cmd(DMA_Uart_TX_Channel,ENABLE);
}

//初始化程序入口
void DMA_Uart_Init(void)
{
	Uartx_Config();
	DMA_Config();
}

//串口DMA传输测试程序
void DMA_Uart_Test(void)
{
	u8	Data_Cnt = 0;
	//往SendBuffer填充数据
	for(Data_Cnt=0;Data_Cnt<DMA_Uart_Buffer_Size;Data_Cnt++)
	{
		DMA_Uart_SendBuffer[Data_Cnt] = 0x30 + Data_Cnt;
	}
	//向DMA发出发送请求
	USART_DMACmd(DMA_Uartx,USART_DMAReq_Tx,ENABLE);
	while(1)
	{
	}
}
