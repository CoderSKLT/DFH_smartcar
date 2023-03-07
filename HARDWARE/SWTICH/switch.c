#include "switch.h" 
extern u8 MODE_flag;//ģʽ��־λ
//��������IO--C8��ʼ��
void Switch_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //�������룬�����������������1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void scan_for_mode(void)
{
	u8 i=0,all=0;
	for(all=40;all>0;all--)
	{	
		if(MODE_Switch!=RESET)//�������ڱպ�״̬
			i++;
		delay_ms(25);
	}
	if(i>30)//����1s�ڰ���״̬�պ�
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
