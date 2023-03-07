#include "switch.h" 
extern u8 MODE_flag;//模式标志位
//按键读入IO--C8初始化
void Switch_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //上拉输入，正常情况传感器返回1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void scan_for_mode(void)
{
	u8 i=0,all=0;
	for(all=40;all>0;all--)
	{	
		if(MODE_Switch!=RESET)//按键处于闭合状态
			i++;
		delay_ms(25);
	}
	if(i>30)//连续1s内按键状态闭合
	{
		MODE_Led=0;
		MODE_flag=1;
	}
	else 
	{
		MODE_flag=0;
		MODE_Led=1;
	}
}
