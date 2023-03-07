#include "motor.h"
/*
TIM2_CH1~4--A5,B3,B10,B11
TIM5_CH3~4--A2,A3
TIM9_CH1~2--E5,E6
*/
//关于时钟和周期计算问题
//因为系统初始化SystemInit函数里初始化APB1总线时钟为4分频即42M，
//所以TIM2~TIM7、TIM12~TIM14的时钟为APB1的时钟的两倍即84M
//APB2总线时钟为2分频即84M，TIM1、TIM8~TIM11的时钟为APB2时钟的两倍即168M。
//由此根据外设所挂在的时钟总线，即可知Tout=(arr+1)*(psc+1)/时钟频率.
//幅值8400
void set_motor(int pwm4, int pwm2,int pwm3 ,int pwm1)//右前，左前，右后，左后
{

	if (pwm1>=0)
	{
		TIM_SetCompare3(TIM5,0);	//PA3
    TIM_SetCompare4(TIM5,pwm1);   //PA2,正位移
	}
	else if (pwm1 < 0)//反转
	{
		TIM_SetCompare3(TIM5,-pwm1);	
    TIM_SetCompare4(TIM5,0);
	}
	//左前轮
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
	//右前轮
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
	//右后轮
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


void MOTOR_PWM_Init(void) //  电机的PWM信号初始化函数
{
	
	GPIO_InitTypeDef 					 GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // 要用到的一些结构体的声明

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE,ENABLE);     //开启 IO口的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM5 ,ENABLE);        //开启定时器的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE);        //开启定时器的时钟
	
	////// IO复用配置
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);           // TIM2的四个通道
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_TIM2);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM5);          // TIM5的CH3,CH4
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM5);
	
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_TIM9);           //TIM9的CH1,CH2
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_TIM9);
	
	//// 初始化GPIO
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //复用功能            
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽复用输出          
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5;     
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       //无上拉下拉输入          
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
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6;     //TIM9的CH1,CH2
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;                 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                
  GPIO_Init(GPIOE,&GPIO_InitStructure);	
	
//初始化定时器	
//所以TIM2~TIM7、TIM12~TIM14的时钟为APB1的时钟的两倍即84M
//APB2总线时钟为2分频即84M，TIM1、TIM8~TIM11的时钟为APB2时钟的两倍即168M。
//马达驱动频率10K
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
	
	
	//////定时器的输出配置
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;              
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //开启OC输出到对应引脚
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; //互补输出使能，关闭OCN输出
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //输出极性高
  TIM_OCInitStructure.TIM_Pulse = 0;
		
	// TIM2 CH1234初始化
	TIM_OC1Init(TIM2,&TIM_OCInitStructure);  
	TIM_OC1PreloadConfig(TIM2 , TIM_OCPreload_Enable);
	
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);  
	TIM_OC2PreloadConfig(TIM2 , TIM_OCPreload_Enable);
	
	TIM_OC3Init(TIM2,&TIM_OCInitStructure);  
	TIM_OC3PreloadConfig(TIM2 , TIM_OCPreload_Enable);
	
	TIM_OC4Init(TIM2,&TIM_OCInitStructure);  
	TIM_OC4PreloadConfig(TIM2 , TIM_OCPreload_Enable);
	
	// TIM5 CH34初始化
	TIM_OC3Init(TIM5,&TIM_OCInitStructure);  
	TIM_OC3PreloadConfig(TIM5 , TIM_OCPreload_Enable);
	TIM_OC4Init(TIM5,&TIM_OCInitStructure);  
	TIM_OC4PreloadConfig(TIM5 , TIM_OCPreload_Enable);
	
  //TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; //在空闲时输出低 
	
	// TIM9 CH12初始化
	TIM_OC1Init(TIM9,&TIM_OCInitStructure);  // TIM9 CH1初始化
	TIM_OC1PreloadConfig(TIM9 , TIM_OCPreload_Enable);
	
	TIM_OC2Init(TIM9,&TIM_OCInitStructure);  
	TIM_OC2PreloadConfig(TIM9 , TIM_OCPreload_Enable);
	
	
  TIM_Cmd(TIM2,ENABLE);//使能TIM12
	TIM_Cmd(TIM5,ENABLE);//使能TIM13
	TIM_Cmd(TIM9,ENABLE);//使能TIM14
}

