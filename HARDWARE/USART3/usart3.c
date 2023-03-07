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
int goal_center=190;//����ֵ
int area=0;//���洹ֱ���޵Ļ���
u8 CAP=0;//CAP��λΪ1��������Ӧ����ǰ��¢�����������ĳ�����area1,���ߺ���¢�����������ĳ�����area3
u8 buff=0;
u8 full=0;//�����־λ
extern u8 lane;//¢������
extern u8 PID_ENABLE;//ʹ�ܱ�־
extern u8 SET_LOC;//λ�õ���ʹ�ܲ���,1��ߣ�2������
extern u8 SET_DIR;//�������ʹ�ܲ���
u8 SET_CAP=0;//���Ĳ�׽��־λ
extern float Kp,Ki,Kd;//ʵ��Ч����ã�latest
extern float Kp_loc;//λ��p���Ʋ���
extern float Kd_loc;//λ��d���Ʋ���
extern float Kp_dir;//�Ƕ�p���Ʋ���
extern float Ki_dir;//�Ƕ�i���Ʋ���
extern float Kd_dir;//�Ƕ�d���Ʋ���
extern float set_loc_1;//��λcm,set_loc�ʣ�4��30��cm,���
extern float set_loc_2;//ǰ��
extern float set_loc_3;//���
extern int v0;//��׼�ٶ�
extern u8 status;
//�������ζ�λ
u8 NEXT(void)
{
	int n;
	u8 flag;
	n=0;
	flag=0;
	//ǰ����
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
		//��תһ��
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
		//��ƽ��һ��
		while(1)
			{
				n++;
				if(n>160)break;
				get_adc(adc);
				if(fabs(adc[0]+adc[1]-24.0f)<=2.0f)break;
				if((adc[1]+adc[0])>25.6f)//����
					set_motor(-2500,2500,-2500,2500);
				if((adc[0]+adc[1])<22.4f)//Զ��
					set_motor(2500,-2500,2500,-2500);
				delay_ms(25);
			}
		n=0;

		//��תһ��
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
		full=0;//�ڽ�����ʮ�������ڲ��ٽ���ʶ��
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
		CONTINUE(status,0);//�̳�״̬
	}
	return flag;
}
//�ָ��ж�ǰ״̬
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

	  //����3��Ӧ���Ÿ���ӳ��
  		GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); //GPIOA9����ΪUSART1
	    GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //GPIOA10����ΪUSART1
		//USART3_TX   PD8,CAMERA--P5--STM32---PD8
		//USART3_RX	  PD9,CAMERA--P4--STM32---PD9
    	//USART3�˿�����
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; //GPIOD9��GPIOD8
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
		GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PA9��PA10

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
		static u8 RxCounter=0;//�������к�
		static u16 RxBuffer[8]={0};//8��16bit������
		static u8 RxState = 0;	//����֡�ڰ��е�λ��


		if( USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
		{
				USART_ClearITPendingBit(USART3,USART_IT_RXNE);
				com_data = USART_ReceiveData(USART3);

				if(RxState==0&&com_data==0x2C)  //ĿǰӦ����ͷ֡����������0x2C
				{
					RxState=1;//��������֡
					RxBuffer[RxCounter++]=com_data;//�Ĵ浽����
				}

				else if(RxState==1&&com_data==0x12)  //ĿǰӦ���մ�֡����������0x12
				{
					RxState=2;//������λ��֡
					RxBuffer[RxCounter++]=com_data;
				}

				else if(RxState==2)	//ĿǰӦ����λ��֡
				{
					RxBuffer[RxCounter++]=com_data;
					RxState=3;//������β֡
					area=RxBuffer[RxCounter-1];//���ݴ浽β֡ǰһ�ֽ�
				}
				else if(RxState==3)	//ĿǰӦ����λ��֡
				{
					RxBuffer[RxCounter++]=com_data;
					RxState=4;//������β֡
					Center=RxBuffer[RxCounter-1];//���ݴ浽β֡ǰһ�ֽ�
				}
				else if(RxState==4&&com_data==0x5B)	//����β֡
				{
					RxBuffer[RxCounter++]=com_data;
					USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);//�ر��ж��ߣ���ʼ�����¼�
/***************************USER CODE BEGIN******************************/
					//����ж����ȼ�Ҫ��PID�жϵͣ��Ա㱻PID���ϣ������˶�����
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
								//תһ��
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
								//תһ��
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
					USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//�ͷ��ж���
				}
				else  //���������Ϲ������ݰ����󣬸�λ���ȴ���һ������
				{
					RxState = 0;
					RxCounter=0;
					Center=0;
					for(i=0;i<8;i++)
					{
						RxBuffer[i]=0x00;      //��ջ�����
					}
				}
		}
}


