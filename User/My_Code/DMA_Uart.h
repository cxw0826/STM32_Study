#ifndef _DMA_UART_H_
#define _DMA_UART_H_

#include "stm32f10x.h"
#include <stdio.h>
#include "Sys_Tick.h"

//定义串口和DMA相关参数
#define DMA_Uart_Baud_Rate			115200
#define DMA_Uartx					USART1
#define DMA_Uart_Func_CLK			RCC_APB2Periph_USART1
#define DMA_Uart_IO_CLK				(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO)
#define DMA_Uart_Port				GPIOA
#define DMA_Uart_RX_Pin				GPIO_Pin_10
#define DMA_Uart_TX_Pin				GPIO_Pin_9
#define DMA_Uart_IRQ				USART1_IRQn
#define	DMA_Uart_INT_FUNC			USART1_IRQHandler
//定义USART1 DMA通道号
#define DMA_Uart_TX_Channel			DMA1_Channel4
#define DMA_Uart_RX_Channel			DMA1_Channel5
//定义BUFFER大小
#define DMA_Uart_Buffer_Size		10

void DMA_Uart_Init(void);
void DMA_TX_Uart_Test(void);




#endif
