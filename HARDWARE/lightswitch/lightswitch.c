#include "lightswitch.h"
 
//��volatileǰ׺������������Ż������
//���ж��ж�ȡ
void Lightswitch_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //�������룬�����������������1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}
//void get_LightSwitch(u8 *temp)//��pid��50ms�ж��е��ô˺���
//{
//	temp[0]=S1;
//	temp[1]=S2;
//	temp[2]=S3;
//	temp[3]=S4;
//}




