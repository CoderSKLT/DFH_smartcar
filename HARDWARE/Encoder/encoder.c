#include "encoder.h"
/*
10.3��ʱ�޸����ݣ���TIM5��A0,1
*/
int Encoder[4]={0};
//float Speed=0;
//u16 i=0;
void Encoder_Init(void)
{  
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4|RCC_APB1Periph_TIM6,ENABLE); 
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB2Periph_TIM8,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);
	
//GPIO�˿�����
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//   	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//��©���
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;           //��������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

		GPIO_Init(GPIOA, &GPIO_InitStructure);
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);           //TIM1��CH1,CH2
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);
	  GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3);           //TIM3��CH1,CH2
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM3);
	  GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4);           //TIM4��CH1,CH2
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_TIM4);
	  GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);           //TIM8��CH1,CH2
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);
//������ģʽ����--------------------------------------------------------------                 

    TIM_EncoderInterfaceConfig(TIM1,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//����ģʽ3
    TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//����ģʽ3
    TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//����ģʽ3
		TIM_EncoderInterfaceConfig(TIM8,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//����ģʽ3
		
    TIM_ICStructInit(&TIM_ICInitStructure); 
    TIM_ICInitStructure.TIM_ICFilter = 6;//�˲���ֵ
    TIM_ICInit(TIM1, &TIM_ICInitStructure);
		TIM_ICInit(TIM3, &TIM_ICInitStructure);
		TIM_ICInit(TIM4, &TIM_ICInitStructure);
		TIM_ICInit(TIM8, &TIM_ICInitStructure);

    //��������ж�
		TIM_ITConfig(TIM1,TIM_IT_Update,DISABLE); //TIM1����ж�
		TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE); //TIM3����ж�
		TIM_ITConfig(TIM4,TIM_IT_Update,DISABLE); //TIM4����ж�
		TIM_ITConfig(TIM8,TIM_IT_Update,DISABLE); //TIM5����ж�
		
		TIM1->CNT=15000;
		TIM3->CNT=15000;
		TIM4->CNT=15000;
		TIM8->CNT=15000;
		TIM_Cmd(TIM1, ENABLE); 
		TIM_Cmd(TIM3, ENABLE); 
		TIM_Cmd(TIM4, ENABLE); 
		TIM_Cmd(TIM8, ENABLE);
//TIME DOMAIN 50ms--------------------------------------------------------
		TIM_TimeBaseInitStructure.TIM_Period = 4199;  //��װ��ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler=999;  //Ԥ��ƵTIM6��Ƶ84M
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; //ʱ�ӷָ�

    TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);//��ʼ��TIM6
//����ж�����--------------------------------------------------------------  
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //TIM6����ж�
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update); 
		
    NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; 
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		TIM_SetCounter(TIM6,0); //TIM6->CNT=0
		TIM_Cmd(TIM6, ENABLE);
}

void Caculate_Encoder(void) 
{

	Encoder[0]=TIM8->CNT-15000;
	Encoder[1]=TIM4->CNT-15000;
	Encoder[2]=TIM3->CNT-15000;
	Encoder[3]=TIM1->CNT-15000;

	TIM8->CNT=15000;
	TIM4->CNT=15000;
	TIM3->CNT=15000;
	TIM1->CNT=15000;
	
	Encoder[0]/=4;
	Encoder[1]/=4;
	Encoder[2]/=4;
	Encoder[3]/=4;
	
}
