#include "Gear.h"
u8 gear=0;
/*
满周期20ms，period为2000
500-2500us为可调范围，对应定时器的arr可从50-250间变化，对应舵机角度360°
我们设置arr的每一个增量对应10us高电平时间变化
则有arr+1为舵机角度增量1.8°，20对应36°增量

*/
/*****************************硬件接口Gear1---B14********************************/
void Gear_Init(void)
{
	GPIO_InitTypeDef 					 GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef 	 TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  				 TIM_OCInitStructure;                   // 要用到的一些结构体的声明

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);     //开启 IO口的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);        //开启定时器的时钟
	
	////// IO复用配置
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM12);
	
	//// 初始化GPIO
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //复用功能            
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽复用输出          
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;     //TIM12的四个通道
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       //无上拉下拉输入          
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;                
  GPIO_Init(GPIOB,&GPIO_InitStructure);															
	
//初始化定时器	
//所以TIM2~TIM7、TIM12~TIM14的时钟为APB1的时钟的两倍即84M
//APB2总线时钟为2分频即84M，TIM1、TIM8~TIM11的时钟为APB2时钟的两倍即168M。
//马达驱动频率10K
	
	TIM_TimeBaseInitStructure.TIM_Period = 1999; //周期20ms，一个刻度对应10us
  TIM_TimeBaseInitStructure.TIM_Prescaler = 839; 
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
	TIM_OC1Init(TIM12,&TIM_OCInitStructure);  // TIM14 CH4初始化
	TIM_OC1PreloadConfig(TIM12 , TIM_OCPreload_Enable);

	TIM_Cmd(TIM12,ENABLE);//使能TIM14
}
void gear_One_tenth(void)
{
	//舵机位置=50（零位）+20（36度），x取值0-10
	TIM_SetCompare1(TIM12,50+20*gear);
	gear++;
//		delay_ms(1000);
}
