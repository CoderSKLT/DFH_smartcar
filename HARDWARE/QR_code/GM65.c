#include "GM65.h"
#include "string.h"


u8 a=0;
u8 k;
u8 j=0;
u8 res;

u8 usart_table_send[]={0x7E, 0x00, 0x08 ,0x01 ,0x00 ,0x02 ,0x01 ,0xAB ,0xCD};   //命令触发模式
u8 usart_table_get[10];
u8 timer=1;
u8 getQRcode(void)
{
	u8 QR_temp=0;
	for(timer=0;timer<20;timer++)
		{
			QR_ask();//二维码触发
			delay_ms(50);
			QR_temp=usart_table_get[0];
//			printf("%d\n",QRcode);
		}
		return QR_temp;
}
void QR_ask(void)
{
	if(a==0)
	{
		for(k=0;k<9;k++)
		{
			USART_SendData(USART6, usart_table_send[k]);
			while(USART_GetFlagStatus(USART6,USART_FLAG_TC)!=SET);  //检测是否发送完毕
			a=1;	
			memset(usart_table_get,0,sizeof(usart_table_get));			
		}

	}	
}

void USART6_Init(void)
{
	  //USART6_TX:PG14  
		//USART6_RX:PG9
		GPIO_InitTypeDef GPIO_InitStructure;     
		USART_InitTypeDef USART_InitStructure;   
		NVIC_InitTypeDef NVIC_InitStructure;    

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);  
		
	  //串口6对应引脚复用映射
  	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOG14复用为USART6
	  GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOG9复用为USART6
		//USART3_TX   PD8,CAMERA--P5--STM32---PD8             	 
		//USART3_RX	  PD9,CAMERA--P4--STM32---PD9
    //USART3端口配置
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_9; //GPIOD9与GPIOD8
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
		GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化PA9，PA10          

		//USART3 
		NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;  //?????0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		    //????2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			      					                 //IRQ????
		NVIC_Init(&NVIC_InitStructure);	                          //??????????VIC???

			//USART ?????
		USART_InitStructure.USART_BaudRate = 115200;                  //??????115200
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //???8?????
		USART_InitStructure.USART_StopBits = USART_StopBits_1;        //?????
		USART_InitStructure.USART_Parity = USART_Parity_No;           //??????
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //????????
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                  //????
		USART_Init(USART6, &USART_InitStructure);                     //?????1

		USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); //????
		USART_Cmd(USART6, ENABLE);                     //????1
		USART_ClearFlag(USART6, USART_FLAG_TC);        //???1????

}

void USART6_IRQHandler(void)
{

	if(USART_GetITStatus(USART6,USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART6,USART_IT_RXNE); 
		res=USART_ReceiveData(USART6);
		USART_SendData(USART6,res);

		if(res==0x31)
		{
			usart_table_get[j]=res;
		}
		else if(res==0x32)
		{
			usart_table_get[j]=res;
		}
		else if(res==0x33)
		{
			usart_table_get[j]=res;
		}
		else if(res==0x34)
		{
			usart_table_get[j]=res;
		}
		else if(res==0x35)
		{
			usart_table_get[j]=res;
		}
	
	}

}
