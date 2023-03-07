#include "UltrasonicWave.h"
/*
   超声波编号，左前1，左后2
*/
void Ultra_Init(void)
{   
	  GPIO_InitTypeDef  GPIO_InitStructure;
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	   //信号触发口 TRIG  D8  输出	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOD , &GPIO_InitStructure);
	
	  //信号接收口 ECHO  D9  输入
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_3;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;           //下拉输入
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOD , &GPIO_InitStructure);
	
    TIM_TimeBaseStructure.TIM_Period = 65535;
    TIM_TimeBaseStructure.TIM_Prescaler = (84 - 1);				//84M / 84 = 1us
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	  TRIGfront=0;
		TIM_Cmd(TIM7, ENABLE);
}

int Ultra(u8 Ultra_x)
{
		//Ultra_x指示前后侧超声波测距的选择0前1后
    u32 count=0;
	  if(Ultra_x==0)
		{
			TRIGfront=1;			//触发
			delay_us(40);
			TRIGfront=0;
			TIM_SetCounter(TIM7, 0);	
			TIM_Cmd(TIM7, ENABLE);
			while(ECHOfront==0)			//等待收到信号
			{
					if(TIM_GetCounter(TIM7)>1176)			//出现错误
				{
					 TIM_Cmd(TIM7, DISABLE);
					 return -1;
				}
			}
			TIM_SetCounter(TIM7, 0);	//接受到信号开始计时
			while(ECHOfront==1)
			{
				 if(TIM_GetCounter(TIM7)>8000)
				{
					 TIM_Cmd(TIM7, DISABLE);			//超出20cm
					 return 0;
				}
			}
		}

	  if(Ultra_x==1)
		{
			TRIGrear=1;			//触发
			delay_us(40);
			TRIGrear=0;
			TIM_SetCounter(TIM7, 0);	
			TIM_Cmd(TIM7, ENABLE);
			while(ECHOrear==0)			//等待收到信号
			{
					if(TIM_GetCounter(TIM7)>1176)			//出现错误
				{
					 TIM_Cmd(TIM7, DISABLE);
					 return -1;
				}
			}
			TIM_SetCounter(TIM7, 0);	//接受到信号开始计时
			while(ECHOrear==1)
			{
				 if(TIM_GetCounter(TIM7)>8000)
				{
					 TIM_Cmd(TIM7, DISABLE);			//超出20cm
					 return 0;
				}
			}
		}
		 count=TIM_GetCounter(TIM7);				//保存计数
		 TIM_Cmd(TIM7, DISABLE);
		 //声速340m/s  计数单位us  结果单位mm   
		 return  (int)(count*0.17); 
}




//u8 Ultra_5_get(int a,int b) 
//{
//	  u8 j=0,k=0;
//		a=b;
//		if(a>60&&a<100)//三层过采样，避免异常信号影响
//		{
//			j++;
//			a=b;delay_ms(100);
//			printf("%d\n",a);
//				if(a>60&&a<140)
//				{
//					j++;
//					a=b;delay_ms(100);
//						if(a>60&&a<140)
//						{  
//							j++;
//							a=b; delay_ms(100);
//							  if(a>60&&a<140)
//								{  
//									j++;
//									a=b;delay_ms(100);
//									  if(a>60&&a<140)
//										{
//											j++;
//											a=b;delay_ms(100);
//										}
//								}
//						}
//				}
//		}
//		printf("%d\n",j);
//		k=j;
//		j=0;
//		return k;
//}


