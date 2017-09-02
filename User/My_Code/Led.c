#include "Led.h"

//初始化GPIO
void Led_Init(void)
{
	//初始化一个GPIO结构体空间
	GPIO_InitTypeDef	GPIO_InitStructure;
	//开启GPIO总线时钟
	RCC_APB2PeriphClockCmd(LED_BLUE_PORT_CLK | LED_RGB_PORT_CLK,ENABLE);
	//都配置为推挽输出，50MHZ速度
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//配置相应的管脚
	//三色灯
	GPIO_InitStructure.GPIO_Pin   = LED_R_PIN;
	GPIO_Init(LED_RGB_PORT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = LED_G_PIN;
	GPIO_Init(LED_RGB_PORT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = LED_B_PIN;
	GPIO_Init(LED_RGB_PORT,&GPIO_InitStructure);
	//单色蓝灯
	GPIO_InitStructure.GPIO_Pin   = LED_BLUE_PIN1;
	GPIO_Init(LED_BLUE_PORT,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = LED_BLUE_PIN2;
	GPIO_Init(LED_BLUE_PORT,&GPIO_InitStructure);
}

//灯控制程序
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

//灯测试程序
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

