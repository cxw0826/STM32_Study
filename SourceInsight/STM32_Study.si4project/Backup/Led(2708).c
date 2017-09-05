#include "Led.h"

//��ʼ��GPIO
void Led_Init(void)
{
	//��ʼ��һ��GPIO�ṹ��ռ�
	GPIO_InitTypeDef	GPIO_InitStructure;
	//����GPIO����ʱ��
	RCC_APB2PeriphClockCmd(LED_BLUE_PORT_CLK | LED_RGB_PORT_CLK,ENABLE);
	//������Ϊ���������50MHZ�ٶ�
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//������Ӧ�Ĺܽ�
	//��ɫ��
	GPIO_InitStructure.GPIO_Pin   = LED_R_PIN;
	GPIO_Init(LED_RGB_PORT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = LED_G_PIN;
	GPIO_Init(LED_RGB_PORT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = LED_B_PIN;
	GPIO_Init(LED_RGB_PORT,&GPIO_InitStructure);
	//��ɫ����
	GPIO_InitStructure.GPIO_Pin   = LED_BLUE_PIN1;
	GPIO_Init(LED_BLUE_PORT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = LED_BLUE_PIN2;
	GPIO_Init(LED_BLUE_PORT,&GPIO_InitStructure);
}

//�ƿ��Ƴ���
void Led_RGB_R_CTRL(u32 state)
{
	if(state == LED_ON)
	{
		GPIO_ResetBits(LED_RGB_PORT,LED_R_PIN);
	}
	else
	{
		GPIO_SetBits(LED_RGB_PORT,LED_R_PIN);
	}
}

void Led_RGB_G_CTRL(u32 state)
{
	if(state == LED_ON)
	{
		GPIO_ResetBits(LED_RGB_PORT,LED_G_PIN);
	}
	else
	{
		GPIO_SetBits(LED_RGB_PORT,LED_G_PIN);
	}
}

void Led_RGB_B_CTRL(u32 state)
{
	if(state == LED_ON)
	{
		GPIO_ResetBits(LED_RGB_PORT,LED_B_PIN);
	}
	else
	{
		GPIO_SetBits(LED_RGB_PORT,LED_B_PIN);
	}
}

void Led_BLUE1_CTRL(u32 state)
{
	if(state == LED_ON)
	{
		GPIO_ResetBits(LED_BLUE_PORT,LED_BLUE_PIN1);
	}
	else
	{
		GPIO_SetBits(LED_BLUE_PORT,LED_BLUE_PIN1);
	}
}

void Led_BLUE2_CTRL(u32 state)
{
	if(state == LED_ON)
	{
		GPIO_ResetBits(LED_BLUE_PORT,LED_BLUE_PIN2);
	}
	else
	{
		GPIO_SetBits(LED_BLUE_PORT,LED_BLUE_PIN2);
	}
}

//�Ʋ��Գ���
void Led_RGB_Test(void)
{
	while(1)
	{
		Led_BLUE1_CTRL(LED_ON);
		Led_BLUE2_CTRL(LED_OFF);
		Led_RGB_R_CTRL(LED_OFF);
		Led_RGB_G_CTRL(LED_OFF);
		Led_RGB_B_CTRL(LED_OFF);
		Delay_ms(300);
		Led_BLUE1_CTRL(LED_OFF);
		Led_BLUE2_CTRL(LED_ON);
		Led_RGB_R_CTRL(LED_OFF);
		Led_RGB_G_CTRL(LED_OFF);
		Led_RGB_B_CTRL(LED_OFF);
		Delay_ms(300);
		Led_BLUE1_CTRL(LED_OFF);
		Led_BLUE2_CTRL(LED_OFF);
		Led_RGB_R_CTRL(LED_ON);
		Led_RGB_G_CTRL(LED_OFF);
		Led_RGB_B_CTRL(LED_OFF);
		Delay_ms(300);
		Led_BLUE1_CTRL(LED_OFF);
		Led_BLUE2_CTRL(LED_OFF);
		Led_RGB_R_CTRL(LED_OFF);
		Led_RGB_G_CTRL(LED_ON);
		Led_RGB_B_CTRL(LED_OFF);
		Delay_ms(300);
		Led_BLUE1_CTRL(LED_OFF);
		Led_BLUE2_CTRL(LED_OFF);
		Led_RGB_R_CTRL(LED_OFF);
		Led_RGB_G_CTRL(LED_OFF);
		Led_RGB_B_CTRL(LED_ON);
		Delay_ms(300);
	}
}

