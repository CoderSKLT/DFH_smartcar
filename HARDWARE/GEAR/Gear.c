#include "Gear.h"
u8 gear=0;
/*
������20ms��periodΪ2000
500-2500usΪ�ɵ���Χ����Ӧ��ʱ����arr�ɴ�50-250��仯����Ӧ����Ƕ�360��
��������arr��ÿһ��������Ӧ10us�ߵ�ƽʱ��仯
����arr+1Ϊ����Ƕ�����1.8�㣬20��Ӧ36������

*/
/*****************************Ӳ���ӿ�Gear1---B14********************************/
void Gear_Init(void)
{
	GPIO_InitTypeDef 					 GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // Ҫ�õ���һЩ�ṹ�������

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);     //���� IO�ڵ�ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);        //������ʱ����ʱ��
	
	////// IO��������
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM12);
	
	//// ��ʼ��GPIO
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //���ù���            
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //���츴�����          
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;     //TIM12���ĸ�ͨ��
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       //��������������          
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                
  GPIO_Init(GPIOB,&GPIO_InitStructure);															
	
//��ʼ����ʱ��	
//����TIM2~TIM7��TIM12~TIM14��ʱ��ΪAPB1��ʱ�ӵ�������84M
//APB2����ʱ��Ϊ2��Ƶ��84M��TIM1��TIM8~TIM11��ʱ��ΪAPB2ʱ�ӵ�������168M��
//�������Ƶ��10K
	
	TIM_TimeBaseInitStructure.TIM_Period = 1999; //����20ms��һ���̶ȶ�Ӧ10us
  TIM_TimeBaseInitStructure.TIM_Prescaler = 839; 
  TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; 	
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;   
  TIM_TimeBaseInit(TIM12,&TIM_TimeBaseInitStructure);
	
	//////��ʱ�����������
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //����OC�������Ӧ����
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; //�������ʹ�ܣ��ر�OCN���
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //������Ը�
  TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM12,&TIM_OCInitStructure);  // TIM14 CH4��ʼ��
	TIM_OC1PreloadConfig(TIM12 , TIM_OCPreload_Enable);

	TIM_Cmd(TIM12,ENABLE);//ʹ��TIM14
}
void gear_One_tenth(void)
{
	//���λ��=50����λ��+20��36�ȣ���xȡֵ0-10
	TIM_SetCompare1(TIM12,50+20*gear);
	gear++;
//		delay_ms(1000);
}
