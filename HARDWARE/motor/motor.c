#include "motor.h"
/*
TIM2_CH1~4--A5,B3,B10,B11
TIM5_CH3~4--A2,A3
TIM9_CH1~2--E5,E6
*/
//����ʱ�Ӻ����ڼ�������
//��Ϊϵͳ��ʼ��SystemInit�������ʼ��APB1����ʱ��Ϊ4��Ƶ��42M��
//����TIM2~TIM7��TIM12~TIM14��ʱ��ΪAPB1��ʱ�ӵ�������84M
//APB2����ʱ��Ϊ2��Ƶ��84M��TIM1��TIM8~TIM11��ʱ��ΪAPB2ʱ�ӵ�������168M��
//�ɴ˸������������ڵ�ʱ�����ߣ�����֪Tout=(arr+1)*(psc+1)/ʱ��Ƶ��.
//��ֵ8400
void set_motor(int pwm4, int pwm2,int pwm3 ,int pwm1)//��ǰ����ǰ���Һ����
{

	if (pwm1>=0)
	{
		TIM_SetCompare3(TIM5,0);	//PA3
    TIM_SetCompare4(TIM5,pwm1);   //PA2,��λ��
	}
	else if (pwm1 < 0)//��ת
	{
		TIM_SetCompare3(TIM5,-pwm1);	
    TIM_SetCompare4(TIM5,0);
	}
	//��ǰ��
	if (pwm2 >= 0)
	{
		TIM_SetCompare3(TIM2,0);	//PB10
    TIM_SetCompare4(TIM2,pwm2); //PB11
	}
	else if (pwm2 < 0)
	{
		TIM_SetCompare3(TIM2,-pwm2);
		TIM_SetCompare4(TIM2,0);
	}
	//��ǰ��
		if (pwm3 >= 0)
	{
		TIM_SetCompare1(TIM2,0);	//PB3
    TIM_SetCompare2(TIM2,pwm3); //PA5
	}
	else if (pwm3 < 0)
	{
		TIM_SetCompare1(TIM2,-pwm3);
		TIM_SetCompare2(TIM2,0);
	}
	//�Һ���
	if (pwm4 >= 0)
	{
		TIM_SetCompare2(TIM9,pwm4);	//PE6
    TIM_SetCompare1(TIM9,0);   //PE5
	}
	else if (pwm4 < 0)
	{
		TIM_SetCompare2(TIM9,0);	
    TIM_SetCompare1(TIM9,-pwm4);
	}
}


void MOTOR_PWM_Init(void) //  �����PWM�źų�ʼ������
{
	
	GPIO_InitTypeDef 					 GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // Ҫ�õ���һЩ�ṹ�������

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE,ENABLE);     //���� IO�ڵ�ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM5 ,ENABLE);        //������ʱ����ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);        //������ʱ����ʱ��
	
	////// IO��������
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);           // TIM2���ĸ�ͨ��
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_TIM2);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM5);          // TIM5��CH3,CH4
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM5);
	
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9);           //TIM9��CH1,CH2
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_TIM9);
	
	//// ��ʼ��GPIO
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //���ù���            
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //���츴�����          
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5;     
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       //��������������          
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                
  GPIO_Init(GPIOA,&GPIO_InitStructure);															
  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                      
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_10 | GPIO_Pin_11;     
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                
  GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                     
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                    
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6;     //TIM9��CH1,CH2
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                
  GPIO_Init(GPIOE,&GPIO_InitStructure);	
	
//��ʼ����ʱ��	
//����TIM2~TIM7��TIM12~TIM14��ʱ��ΪAPB1��ʱ�ӵ�������84M
//APB2����ʱ��Ϊ2��Ƶ��84M��TIM1��TIM8~TIM11��ʱ��ΪAPB2ʱ�ӵ�������168M��
//�������Ƶ��10K
	TIM_TimeBaseInitStructure.TIM_Period = 8399; 
  TIM_TimeBaseInitStructure.TIM_Prescaler = 0; 
  TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; 	
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;   
  TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_TimeBaseInitStructure.TIM_Period = 8399; 
  TIM_TimeBaseInitStructure.TIM_Prescaler = 0; 
  TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; 	
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;   
  TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	TIM_TimeBaseInitStructure.TIM_Period = 8399; 
  TIM_TimeBaseInitStructure.TIM_Prescaler = 1; 
  TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; 	
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;   
  TIM_TimeBaseInit(TIM9,&TIM_TimeBaseInitStructure);
	
	
	//////��ʱ�����������
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;              
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //����OC�������Ӧ����
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; //�������ʹ�ܣ��ر�OCN���
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //������Ը�
  TIM_OCInitStructure.TIM_Pulse = 0;
		
	// TIM2 CH1234��ʼ��
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);  
	TIM_OC1PreloadConfig(TIM2 , TIM_OCPreload_Enable);
	
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);  
	TIM_OC2PreloadConfig(TIM2 , TIM_OCPreload_Enable);
	
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);  
	TIM_OC3PreloadConfig(TIM2 , TIM_OCPreload_Enable);
	
	TIM_OC4Init(TIM2,&TIM_OCInitStructure);  
	TIM_OC4PreloadConfig(TIM2 , TIM_OCPreload_Enable);
	
	// TIM5 CH34��ʼ��
	TIM_OC3Init(TIM5,&TIM_OCInitStructure);  
	TIM_OC3PreloadConfig(TIM5 , TIM_OCPreload_Enable);
	TIM_OC4Init(TIM5,&TIM_OCInitStructure);  
	TIM_OC4PreloadConfig(TIM5 , TIM_OCPreload_Enable);
	
  //TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; //�ڿ���ʱ����� 
	
	// TIM9 CH12��ʼ��
	TIM_OC1Init(TIM9,&TIM_OCInitStructure);  // TIM9 CH1��ʼ��
	TIM_OC1PreloadConfig(TIM9 , TIM_OCPreload_Enable);
	
	TIM_OC2Init(TIM9,&TIM_OCInitStructure);  
	TIM_OC2PreloadConfig(TIM9 , TIM_OCPreload_Enable);
	
	
  TIM_Cmd(TIM2,ENABLE);//ʹ��TIM12
	TIM_Cmd(TIM5,ENABLE);//ʹ��TIM13
	TIM_Cmd(TIM9,ENABLE);//ʹ��TIM14
}

