#include "Sys_Tick.h"

static u32 Delay_Time;
//��ʼ��ϵͳ�δ�ʱ�Ӻ���
void Sys_Tick_Init(void)
{
	/* SystemCoreClock = 72,000,000HZ
	 * SystemCoreClock / 1000    1ms�ж�һ��
	 * SystemCoreClock / 100000	 10us�ж�һ��
	 * SystemCoreClock / 1000000 1us�ж�һ��
	 */
	//SysTick_Config����systick�������ɹ�����0����װֵ����Ԥ�践��1
	if(SysTick_Config(SystemCoreClock / 1000000))
	{
		while(1);//������
	}
	// �رյδ�ʱ��  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

//systick��ʱ��������ʱʱ��1us
void Delay_us(u32 nUS)
{
	//��Ҫ��ʱ����װ�ص�������
	Delay_Time = nUS;
	//ʹ�ܵδ�ʱ����ÿ����ʱ��Ҫʹ��
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;
	//�ȴ���������������0,�����û��0���������ﵽ��ʱЧ��
	while(Delay_Time !=0);
}

//��ʱ��������ʱʱ��1ms
void Delay_ms(u32 nMS)
{
	//��ʱ1000us��1ms
	Delay_us(1000*nMS);
}

//�δ�ʱ���жϺ���
void SysTick_Handler(void)
{
	if(Delay_Time != 0x00)
	{
		Delay_Time--;
	}
}





