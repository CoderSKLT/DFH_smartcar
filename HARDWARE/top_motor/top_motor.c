#include "top_motor.h"

void project(int duty,u16 time)//����1����ת����ת:������2����ʱ
{
  if(duty>1&&duty<=840)
  {
    TIM_SetCompare2(TIM12,duty);
    delay_ms(5);
    PDout(13)=0;
    PDout(14)=1;
  }
  if(duty<-1&&duty>=-840)
  {
    TIM_SetCompare2(TIM12,0-duty);
    delay_ms(5);
    PDout(13)=1;
    PDout(14)=0;
  }
  delay_ms(time);
  //��λ�������
  PDout(13)=1;
  PDout(14)=1;
  delay_ms(5);
}

void Top_Motor_init(void)
/* 
  L298N��������ʹ�ܽ����ö��2��pwmͨ�����٣�PB15��TIM12-CH2
  ��������ͨIO��PD13,PD14������ת��
 */
{
	
	GPIO_InitTypeDef 					 GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // Ҫ�õ���һЩ�ṹ�������

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD,ENABLE);     //���� IO�ڵ�ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12 ,ENABLE);        //������ʱ����ʱ��
	
	////// IO��������
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_TIM12);           // TIM2���ĸ�ͨ��

	//// ��ʼ��GPIO
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //���ù���            
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //���츴�����          
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15 ;     
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       //��������������          
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                
  GPIO_Init(GPIOB,&GPIO_InitStructure);															

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                      
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;     
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                
  GPIO_Init(GPIOD,&GPIO_InitStructure);	

//��ʼ����ʱ��	
//����TIM2~TIM7��TIM12~TIM14��ʱ��ΪAPB1��ʱ�ӵ�������84M
//APB2����ʱ��Ϊ2��Ƶ��84M��TIM1��TIM8~TIM11��ʱ��ΪAPB2ʱ�ӵ�������168M��
//�������Ƶ��10K
//�ö��2��4��pwmͨ����PB15��TIM12-CH2,PF9,TIM14-CH1
	TIM_TimeBaseInitStructure.TIM_Period = 840-1; 
  TIM_TimeBaseInitStructure.TIM_Prescaler = 10-1; 
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
		
	// TIM2 CH1234��ʼ��
	TIM_OC2Init(TIM12,&TIM_OCInitStructure);  
	TIM_OC2PreloadConfig(TIM12 , TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM12,ENABLE);//ʹ��TIM12
}

