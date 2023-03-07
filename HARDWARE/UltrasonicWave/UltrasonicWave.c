#include "UltrasonicWave.h"
/*
   ��������ţ���ǰ1�����2
*/
void Ultra_Init(void)
{   
	  GPIO_InitTypeDef  GPIO_InitStructure;
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	   //�źŴ����� TRIG  D8  ���	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;          //�������
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(GPIOD , &GPIO_InitStructure);
	
	  //�źŽ��տ� ECHO  D9  ����
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_3;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;           //��������
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
		//Ultra_xָʾǰ��೬��������ѡ��0ǰ1��
    u32 count=0;
	  if(Ultra_x==0)
		{
			TRIGfront=1;			//����
			delay_us(40);
			TRIGfront=0;
			TIM_SetCounter(TIM7, 0);	
			TIM_Cmd(TIM7, ENABLE);
			while(ECHOfront==0)			//�ȴ��յ��ź�
			{
					if(TIM_GetCounter(TIM7)>1176)			//���ִ���
				{
					 TIM_Cmd(TIM7, DISABLE);
					 return -1;
				}
			}
			TIM_SetCounter(TIM7, 0);	//���ܵ��źſ�ʼ��ʱ
			while(ECHOfront==1)
			{
				 if(TIM_GetCounter(TIM7)>8000)
				{
					 TIM_Cmd(TIM7, DISABLE);			//����20cm
					 return 0;
				}
			}
		}

	  if(Ultra_x==1)
		{
			TRIGrear=1;			//����
			delay_us(40);
			TRIGrear=0;
			TIM_SetCounter(TIM7, 0);	
			TIM_Cmd(TIM7, ENABLE);
			while(ECHOrear==0)			//�ȴ��յ��ź�
			{
					if(TIM_GetCounter(TIM7)>1176)			//���ִ���
				{
					 TIM_Cmd(TIM7, DISABLE);
					 return -1;
				}
			}
			TIM_SetCounter(TIM7, 0);	//���ܵ��źſ�ʼ��ʱ
			while(ECHOrear==1)
			{
				 if(TIM_GetCounter(TIM7)>8000)
				{
					 TIM_Cmd(TIM7, DISABLE);			//����20cm
					 return 0;
				}
			}
		}
		 count=TIM_GetCounter(TIM7);				//�������
		 TIM_Cmd(TIM7, DISABLE);
		 //����340m/s  ������λus  �����λmm   
		 return  (int)(count*0.17); 
}




//u8 Ultra_5_get(int a,int b) 
//{
//	  u8 j=0,k=0;
//		a=b;
//		if(a>60&&a<100)//����������������쳣�ź�Ӱ��
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


