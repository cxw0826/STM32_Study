#include "Sys_Tick.h"

static u32 Delay_Time;
//初始化系统滴答时钟函数
void Sys_Tick_Init(void)
{
	/* SystemCoreClock = 72,000,000HZ
	 * SystemCoreClock / 1000    1ms中断一次
	 * SystemCoreClock / 100000	 10us中断一次
	 * SystemCoreClock / 1000000 1us中断一次
	 */
	//SysTick_Config配置systick函数，成功返回0，重装值超过预设返回1
	if(SysTick_Config(SystemCoreClock / 1000000))
	{
		while(1);//出错卡死
	}
	// 关闭滴答定时器  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

//systick延时函数，延时时间1us
void Delay_us(u32 nUS)
{
	//将要延时次数装载到计数器
	Delay_Time = nUS;
	//使能滴答定时器，每次延时都要使能
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
	//等待计数器将计数置0,如果还没置0，卡死，达到延时效果
	while(Delay_Time !=0);
}

//延时函数，延时时间1ms
void Delay_ms(u32 nMS)
{
	//延时1000us即1ms
	Delay_us(1000*nMS);
}

//滴答时钟中断函数
void SysTick_Handler(void)
{
	if(Delay_Time != 0x00)
	{
		Delay_Time--;
	}
}





