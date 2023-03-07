#include "top_motor.h"

void project(int duty,u16 time)//参数1：正转，反转:；参数2：延时
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
  //复位锁死电机
  PDout(13)=1;
  PDout(14)=1;
  delay_ms(5);
}

void Top_Motor_init(void)
/* 
  L298N驱动器，使能脚用用舵机2的pwm通道调速，PB15，TIM12-CH2
  用两个普通IO，PD13,PD14，控制转向
 */
{
	
	GPIO_InitTypeDef 					 GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // 要用到的一些结构体的声明

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD,ENABLE);     //开启 IO口的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12 ,ENABLE);        //开启定时器的时钟
	
	////// IO复用配置
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_TIM12);           // TIM2的四个通道

	//// 初始化GPIO
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //复用功能            
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽复用输出          
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15 ;     
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       //无上拉下拉输入          
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                
  GPIO_Init(GPIOB,&GPIO_InitStructure);															

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                      
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                    
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;     
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                
  GPIO_Init(GPIOD,&GPIO_InitStructure);	

//初始化定时器	
//所以TIM2~TIM7、TIM12~TIM14的时钟为APB1的时钟的两倍即84M
//APB2总线时钟为2分频即84M，TIM1、TIM8~TIM11的时钟为APB2时钟的两倍即168M。
//马达驱动频率10K
//用舵机2和4的pwm通道，PB15，TIM12-CH2,PF9,TIM14-CH1
	TIM_TimeBaseInitStructure.TIM_Period = 840-1; 
  TIM_TimeBaseInitStructure.TIM_Prescaler = 10-1; 
  TIM_TimeBaseInitStructure.TIM_ClockDivision = 0; 	
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;   
  TIM_TimeBaseInit(TIM12,&TIM_TimeBaseInitStructure);

	
	//////定时器的输出配置
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;              
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //开启OC输出到对应引脚
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; //互补输出使能，关闭OCN输出
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //输出极性高
  TIM_OCInitStructure.TIM_Pulse = 0;
		
	// TIM2 CH1234初始化
	TIM_OC2Init(TIM12,&TIM_OCInitStructure);  
	TIM_OC2PreloadConfig(TIM12 , TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM12,ENABLE);//使能TIM12
}

