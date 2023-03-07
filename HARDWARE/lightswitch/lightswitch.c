#include "lightswitch.h"
 
//加volatile前缀，避免编译器优化后出错
//在中断中读取
void Lightswitch_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //上拉输入，正常情况传感器返回1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}
//void get_LightSwitch(u8 *temp)//在pid的50ms中断中调用此函数
//{
//	temp[0]=S1;
//	temp[1]=S2;
//	temp[2]=S3;
//	temp[3]=S4;
//}




