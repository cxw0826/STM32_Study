#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"
#include "Sys_Tick.h"

//LED���ǵ͵�ƽ����
//��ɫ�ƶ˿ں�
#define	LED_RGB_PORT_CLK	RCC_APB2Periph_GPIOB
#define	LED_RGB_PORT		GPIOB
#define LED_R_PIN			GPIO_Pin_5
#define LED_G_PIN			GPIO_Pin_0
#define LED_B_PIN			GPIO_Pin_1
//����ɫ�˿�
#define	LED_BLUE_PORT_CLK	RCC_APB2Periph_GPIOF
#define LED_BLUE_PORT		GPIOF
#define	LED_BLUE_PIN1		GPIO_Pin_7
#define	LED_BLUE_PIN2		GPIO_Pin_8
//����״̬
#define	LED_ON	0
#define	LED_OFF	1


void Led_Init(void);
void Led_RGB_Test(void);


#endif
