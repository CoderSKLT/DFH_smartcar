/* CAM FUNC */
#include "usart3.h"
#include "gear.h"
#include "delay.h"
#include "math.h"
#include "pid.h"
#include "led.h"
#include "top_motor.h"
extern int v0;
extern float adc[2];
extern char CLEAR;
int Center=0;
int goal_center=190;//期望值
int area=0;//画面垂直象限的划分
u8 CAP=0;//CAP置位为1的条件，应该是前进垄道中树苗中心出现在area1,或者后退垄道中树苗中心出现在area3
u8 buff=0;
u8 full=0;//捕获标志位
extern u8 lane;//垄道计数
extern u8 PID_ENABLE;//使能标志
extern u8 SET_LOC;//位置调节使能参数,1侧边，2反向侧边
extern u8 SET_DIR;//方向调节使能参数
u8 SET_CAP=0;//中心捕捉标志位
extern float Kp,Ki,Kd;//实测效果最好，latest
extern float Kp_loc;//位置p控制参数
extern float Kd_loc;//位置d控制参数
extern float Kp_dir;//角度p控制参数
extern float Ki_dir;//角度i控制参数
extern float Kd_dir;//角度d控制参数
extern float set_loc_1;//单位cm,set_loc∈（4，30）cm,左侧
extern float set_loc_2;//前侧
extern float set_loc_3;//后侧
extern int v0;//基准速度
extern u8 status;
//捕获后二次定位
u8 NEXT(void)
{
	int n;
	u8 flag;
	n=0;
	flag=0;
	//前后走
	if(area==1)
	{
		set_motor(2900,2900,2900,2900);
		delay_ms(25);
		flag=0;
	}
	if (area==3)
	{
		set_motor(-2900,-2900,-2900,-2900);
		delay_ms(25);
		flag=0;
	}
	if (area==2)
	{
		//再转一下
		while(1)
			{
				n++;
				if(n>160)break;
				get_adc(adc);
				if(fabs(adc[0]-adc[1])<=1.0f)break;
				if(adc[1]-adc[0]>0.8f)
					set_motor(2500,-2500,-2500,2500);
				if(adc[0]-adc[1]>0.8f)
					set_motor(-2500,2500,2500,-2500);
				delay_ms(25);
			}
		n=0;
		//再平移一点
		while(1)
			{
				n++;
				if(n>160)break;
				get_adc(adc);
				if(fabs(adc[0]+adc[1]-24.0f)<=2.0f)break;
				if((adc[1]+adc[0])>25.6f)//近了
					set_motor(-2500,2500,-2500,2500);
				if((adc[0]+adc[1])<22.4f)//远了
					set_motor(2500,-2500,2500,-2500);
				delay_ms(25);
			}
		n=0;

		//再转一下
		while(1)
			{
				n++;
				if(n>160)break;
				get_adc(adc);
				if(fabs(adc[0]-adc[1])<=1.0f)break;
				if(adc[1]-adc[0]>0.8f)
					set_motor(2500,-2500,-2500,2500);
				if(adc[0]-adc[1]>0.8f)
					set_motor(-2500,2500,2500,-2500);
				delay_ms(25);
			}
		n=0;
		while (1)
		{
			n++;
			if(n>160)break;
			if(Center>=146&&Center<=200)break;
			if(Center<146)
			{
				set_motor(2500,2500,2500,2500);
			}
			if(Center>200)
			{
				set_motor(-2500,-2500,-2500,-2500);
			}
			delay_ms(20);
			flag=0;
		}
		n=0;
		set_motor(0,0,0,0);
		gear_One_tenth();
		delay_ms(600);
		project(839,800);
		delay_ms(1500);
		project(-839,800);
		delay_ms(100);
		DEAD_ZONE;
		full=0;//在接下来十个周期内不再接收识别
		if (lane%2==1)
		{
			set_motor(3200,3200,3200,3200);
			delay_ms(1000);
		}
		else if (lane%2==0&&lane!=6)
		{
			set_motor(-3200,-3200,-3200,-3200);
			delay_ms(1000);
		}
		DEAD_ZONE;
		CAP=0;
		flag=1;
		PID_INVOKE;
		CONTINUE(status,0);//继承状态
	}
	return flag;
}
//恢复中断前状态
void CONTINUE(u8 status,int time)
{
	switch (status)
		{
		case 0:
			AHEAD;
			break;
		case 1:
			BACK;
			break;
		case 2:
			LEFT;
			break;
		case 3:
			RIGHT;
			break;
		case 4:
			OP_RIGHT;
			break;
		case 6:
			FAST;
			break;
		case 7:
			OP_FAST;
			break;
		case 8:
			SLOW;
			break;
		case 9:
			OP_SLOW;
			break;
		default:
			break;
		}
	if(time)
		delay_ms(time);
}

void USART3_Init(void)
{
	 	//USART3_TX:PD8
		//USART3_RX:PD9
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	  //串口3对应引脚复用映射
  		GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); //GPIOA9复用为USART1
	    GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //GPIOA10复用为USART1
		//USART3_TX   PD8,CAMERA--P5--STM32---PD8
		//USART3_RX	  PD9,CAMERA--P4--STM32---PD9
    	//USART3端口配置
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; //GPIOD9与GPIOD8
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
		GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PA9，PA10

		//USART3
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
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
		USART_Init(USART3, &USART_InitStructure);                     //?????1

		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //????
		USART_Cmd(USART3, ENABLE);                     //????1
		USART_ClearFlag(USART3, USART_FLAG_TC);        //???1????



}

void USART3_IRQHandler()
{
		u8 com_data;
		u8 i;
		static u8 RxCounter=0;//接收序列号
		static u16 RxBuffer[8]={0};//8个16bit缓冲区
		static u8 RxState = 0;	//数据帧在包中的位置


		if( USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
		{
				USART_ClearITPendingBit(USART3,USART_IT_RXNE);
				com_data = USART_ReceiveData(USART3);

				if(RxState==0&&com_data==0x2C)  //目前应接收头帧，且数据是0x2C
				{
					RxState=1;//迭换到次帧
					RxBuffer[RxCounter++]=com_data;//寄存到缓存
				}

				else if(RxState==1&&com_data==0x12)  //目前应接收次帧，且数据是0x12
				{
					RxState=2;//迭换到位置帧
					RxBuffer[RxCounter++]=com_data;
				}

				else if(RxState==2)	//目前应接收位置帧
				{
					RxBuffer[RxCounter++]=com_data;
					RxState=3;//迭换到尾帧
					area=RxBuffer[RxCounter-1];//数据存到尾帧前一字节
				}
				else if(RxState==3)	//目前应接收位置帧
				{
					RxBuffer[RxCounter++]=com_data;
					RxState=4;//迭换到尾帧
					Center=RxBuffer[RxCounter-1];//数据存到尾帧前一字节
				}
				else if(RxState==4&&com_data==0x5B)	//接收尾帧
				{
					RxBuffer[RxCounter++]=com_data;
					USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);//关闭中断线，开始处理事件
/***************************USER CODE BEGIN******************************/
					//这个中断优先级要比PID中断低，以便被PID抢断，进行运动控制
						if(full==0)
						{
							if(lane%2&&(area==1||area==2)&&CAP==0)
							{	
								PID_SLEEP;
								set_motor(0,0,0,0);
								delay_ms(400);
								if (area==1)
								{
									set_motor(2000,2000,2000,2000);
									delay_ms(240);
								}
								set_motor(0,0,0,0);
								//转一下
								while(1)
									{
										get_adc(adc);
										if(fabs(adc[0]-adc[1])<=2.0f)break;
										if(adc[1]-adc[0]>1.8f)
											set_motor(2000,-2000,-2000,2000);
										if(adc[0]-adc[1]>1.8f)
											set_motor(-2000,2000,2000,-2000);
										delay_ms(50);
									}
								CAP=1;
							}
						if(lane%2==0&&lane!=6&&(area==3||area==2)&&CAP==0)
							{
								PID_SLEEP;
								set_motor(0,0,0,0);
								delay_ms(400);
								if (area==3)
								{
									set_motor(-2000,-2000,-2000,-2000);
									delay_ms(240);
								}
								set_motor(0,0,0,0);
								//转一下
								while(1)
									{
										get_adc(adc);
										if(fabs(adc[0]-adc[1])<=2.0f)break;
										if(adc[1]-adc[0]>1.8f)
											set_motor(2000,-2000,-2000,2000);
										if(adc[0]-adc[1]>1.8f)
											set_motor(-2000,2000,2000,-2000);
										delay_ms(50);
									}
								CAP=1;
							}
						}
/***************************USER CODE END********************************/
					RxCounter = 0;
					RxState = 0;
					area=0;
					USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//释放中断线
				}
				else  //不符合以上规则，数据包有误，复位，等待下一波数据
				{
					RxState = 0;
					RxCounter=0;
					Center=0;
					for(i=0;i<8;i++)
					{
						RxBuffer[i]=0x00;      //清空缓存区
					}
				}
		}
}


