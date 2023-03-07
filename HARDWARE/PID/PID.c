#include"pid.h"
#include"math.h"
char CLEAR=0;
u8 status=0;
u8 SET_LOC=0;//λ�õ���ʹ�ܲ���,1��ߣ�2������
u8 SET_DIR=0;//�������ʹ�ܲ���
u8 PID_ENABLE=0;
extern u8 SET_CAP;//���Ĳ�׽��־λ
volatile char flag=0;//���ű�־λ��0��1������λ�ֱ����ÿֻ���ת���ķ������Ӧ���ٶȡ�pwm���޷�
float adc[2]={0};
extern u8 full;//����ͷ�����־λ
extern u8 status;//��ʻ��־λ
//extern	u8 LightSwitch[4];
/*
λ�ÿ���p����
*/
int A=0,B=0,C=0,D=0;
extern float Kp_loc;//λ��p���Ʋ���
extern float Ki_loc;//λ��i���Ʋ���
extern float Kd_loc;//λ��d���Ʋ���
extern float Kp_dir;//�Ƕ�p���Ʋ���
extern float Ki_dir;//�Ƕ�i���Ʋ���
extern float Kd_dir;//�Ƕ�d���Ʋ���
extern float set_loc_1;
extern float set_loc_2;
extern float set_loc_3;
extern int v0;
extern u8 SET_LOC;//λ��У׼����
extern u8 SET_DIR;//����У׼����
void set_loc_A(float set_location,float adc1,float adc2)//�����ߺ�ֵ����
{
	float error=0,last_error=0,prev_error=0;
	int t=0;
	if(CLEAR==1)
	{
		error=0,last_error=0,prev_error=0;
	}
	CLEAR=0;
	error=set_location-(adc1+adc2)/2;
	if(status==6||status==8||status==9&&error<-1.8)
		A=(int)((Kp_loc+6*fabs(error))*(error-last_error)+Kd_loc*(error-2*last_error+prev_error));
	else
		A=(int)(Kp_loc*(error-last_error)+Kd_loc*(error-2*last_error+prev_error));
	t+=A;
	if(error>=0)//���������1,3�����ٶȷŴ�
	{
	    set_pid(v0+t,v0,v0+t,v0);
	}
	else
	{
		set_pid(v0,v0-t,v0,v0-t);
	}
	prev_error=last_error;
	last_error=error;
}
void set_loc_B(float set_location,float adc1,float adc2)//�����ߺ�ֵ����
{
	float error=0,last_error=0,prev_error=0;
	int t=0;
	if(CLEAR==1)
	{
		error=0,last_error=0,prev_error=0;
	}
	CLEAR=0;
	error=set_location-(adc1+adc2)/2;
	if(status==6||status==8||status==9&&error<-1.8)
		A=(int)((Kp_loc+6*fabs(error))*(error-last_error)+Kd_loc*(error-2*last_error+prev_error));
	else
		A=(int)(Kp_loc*(error-last_error)+Kd_loc*(error-2*last_error+prev_error));
	t+=A;
	if(error>=0)//���������1,3�����ٶȷŴ�
	{
	    set_pid(v0,v0-t,v0,v0-t);
	}
	else
	{
		set_pid(v0+t,v0,v0+t,v0);
	}
	prev_error=last_error;
	last_error=error;
}
void set_loc_C(float set_location,int Ultra_x)//����ǰ���ֵ����,adc[3]
{
	float error=0,last_error=0,prev_error=0;
	int t=0;
	if(CLEAR==1)
	{
		error=0,last_error=0,prev_error=0;
	}
	CLEAR=0;
	error=set_location-Ultra_x/10.0;
	if(status==6||status==8||status==9&&error<-1.8)
		C=(int)((Kp_loc+fabs(error))*(error-last_error)+Kd_loc*(error-2*last_error+prev_error));
	else
		C=(int)(Kp_loc*(error-last_error)+Kd_loc*(error-2*last_error+prev_error));
	t+=C;
	if(error>=0)//���������2��4�����ٶȷŴ�
	{
	   	set_pid(v0,-v0-t,v0,-v0-t);
	}
	else
	{
		set_pid(v0-t,-v0,v0-t,-v0);//t�Ǹ�ֵ
	}
	prev_error=last_error;
	last_error=error;
}	
void set_loc_D(float set_location,int Ultra_x)//�������ֵ����,adc[2]
{
	float error=0,last_error=0,prev_error=0;
	int t=0;
	if(CLEAR==1)
	{
		error=0,last_error=0,prev_error=0;
	}
	CLEAR=0;
	error=set_location-Ultra_x/10.0;
	if(status==6||status==8||status==9&&error<-1.8)
		D=(int)((Kp_loc+fabs(error))*(error-last_error)+Kd_loc*(error-2*last_error+prev_error));
	else
		D=(int)(Kp_loc*(error-last_error)+Kd_loc*(error-2*last_error+prev_error));
	t+=D;
	if(error>=0)//���������1,3�����ٶȷŴ�
	{
	   set_pid(v0+t,-v0,v0+t,-v0);
	}
	else
	{
		set_pid(v0,-v0+t,v0,-v0+t);//t�Ǹ�ֵ
	}
	prev_error=last_error;
	last_error=error;
}	
/****************�����������****************/
void Adjust_dir(void)
{
	while(SET_DIR)
		{
			delay_ms(1800);//����1.6s�ĵ���
			PID_SLEEP;//���ж�ʱ�رյ���
			if(fabs(adc[0]-adc[1])<0.8)	
			{
				delay_ms(60);
				if(fabs(adc[0]-adc[1])<0.8)	
				{
					delay_ms(60);
				if(fabs(adc[0]-adc[1])<0.8)	{SET_DIR=0;CLEAR=1;break;}
				}
			}
			PID_INVOKE;
		}
}
void set_dir(float adc1,float adc2)
{
	float error=0,last_error=0,prev_error=0;
	int t=0;	
	if(CLEAR==1)
	{
		error=0,last_error=0,prev_error=0;
	}
	CLEAR=0;
	error=(adc1-adc2);
	A=(int)(Kp_dir*(error-last_error)+Ki_dir*error+Kd_dir*(error-2*last_error+prev_error));
	t+=A;
//	printf("%f",Kp_loc*error);
	if(error>=0)//ǰ��������,t>0
	{
		set_pid(-t,t,t,-t);
	}
	else
	{
		set_pid(-t,t,t,-t);
	}
	prev_error=last_error;
	last_error=error;
//  }
}
/*
�ٶȿ���pid����
*/
extern int Encoder[4];
//int sum=0,i=0;
extern float Kp,Ki,Kd;
static int setpoint[4]={0}; //�趨Ŀ��,��ֵΪ0
/*
�ٶȼ��㹫ʽ��������Ҫ���ñ�����������ֵ����50ms���������£�29��һ��תһȦ������
*/
PIDtypedef PID0;	 //PID�ṹ��
PIDtypedef PID1;
PIDtypedef PID2;
PIDtypedef PID3;

void PID_Init(PIDtypedef* PIDx)
{
  PIDx->proportion=Kp;//��������
  PIDx->integral=Ki;//���ֳ���
  PIDx->derivative=Kd;//΢�ֳ��� 
  PIDx->last_error=0;//e[-1]
  PIDx->prev_error=0;//e[-2]
}
void PID_INIT(void)
{
  	PID_Init(&PID0);
	PID_Init(&PID1);
	PID_Init(&PID2);
	PID_Init(&PID3);
	PID_ENABLE=1;
}
int inc_PID0(PIDtypedef* PIDx)
{
	static int iError=0;
  static long OutPWM0;
	iError=setpoint[0]-Encoder[0];
  OutPWM0+=PIDx->proportion*( iError-PIDx->last_error) +PIDx->integral*iError +PIDx->derivative*(iError-2*PIDx->last_error+PIDx->prev_error);
	PIDx->prev_error=PIDx->last_error; 
	PIDx->last_error=iError;//�������

	OutPWM0=limits_pwm(OutPWM0,0);//����޷�
//  printf("0==%d\n",setpoint[0]);
	return OutPWM0;
}
int inc_PID1(PIDtypedef* PIDx)
{
	static int iError1=0;
  static long OutPWM1=0;
	iError1=setpoint[1]-Encoder[1];
  OutPWM1+= PIDx->proportion*( iError1-PIDx->last_error) +PIDx->integral*iError1 +PIDx->derivative*(iError1-2*PIDx->last_error+PIDx->prev_error);
	PIDx->prev_error=PIDx->last_error; 
	PIDx->last_error=iError1;//�������
	OutPWM1=limits_pwm(OutPWM1,1);//����޷�
	// printf("1==%d\n",setpoint[1]);
	return OutPWM1;
}
int inc_PID2(PIDtypedef* PIDx)
{
	static int iError2=0;
  static long OutPWM2=0;
	iError2=setpoint[2]-Encoder[2];
  OutPWM2+= PIDx->proportion*( iError2-PIDx->last_error) +PIDx->integral*iError2 +PIDx->derivative*(iError2-2*PIDx->last_error+PIDx->prev_error);
	PIDx->prev_error=PIDx->last_error; 
	PIDx->last_error=iError2;//�������
	OutPWM2=limits_pwm(OutPWM2,2);//����޷�
//	printf("2==%d\n",setpoint[2]);
	return OutPWM2;
}
int inc_PID3(PIDtypedef* PIDx)
{
	static int iError3=0;
  static long OutPWM3=0;
	iError3=setpoint[3]-Encoder[3];
  OutPWM3+= PIDx->proportion*( iError3-PIDx->last_error) +PIDx->integral*iError3 +PIDx->derivative*(iError3-2*PIDx->last_error+PIDx->prev_error);
	PIDx->prev_error=PIDx->last_error; 
	PIDx->last_error=iError3;//�������
	OutPWM3=limits_pwm(OutPWM3,3);//����޷�
//	printf("3==%d\n",setpoint[3]);
	return OutPWM3;
}
//����޷�����
int limits_pwm(int limit_pwm,u8 i)
{
	if((flag>>i)&0x1)//�����룬&������0���жϸ���λ��|������1
  {  
     if(limit_pwm<=-8000)
        limit_pwm=-8000;
		 if(limit_pwm>=-1000)
        limit_pwm=-1000;
	}
	else//������
	{
		if(limit_pwm>=8000)
        limit_pwm=8000;
    if(limit_pwm<=0)
        limit_pwm=0;
	}
		return limit_pwm;
}
void set_pid(int target0,int target1,int target2,int target3)
{
	//ʹ��flag����λ�߼�״̬�б������򣬵�ĳһ�������������ʱ����ֻ���ͬʱ��������
	char i=0;
	char temp[4]={0xe,0xd,0xb,0x7};//̽������
	volatile char last_flag=0,now_flag=0;
	setpoint[0]=target0;
	setpoint[1]=target1;
	setpoint[2]=target2;
 	setpoint[3]=target3;
	last_flag=flag;
	for(i=0;i<4;i++)
	{
		//Ӱ���־λ
		if(setpoint[i]>=0) flag=flag&temp[i];//���ĳλĿ��ֵΪ�Ǹ�������Ѹ�λ����
			else flag=flag|(~temp[i]);//������1
		//�޷�
		if(setpoint[i]>=0)//������,�޷��ڡ�0��90��
		{
			if(setpoint[i]>=90)
				setpoint[i]=90;
			if(setpoint[i]<=0)
				setpoint[i]=0;
		}
		if(setpoint[i]<=0)//������,�޷��ڡ�-90��-5��
		{
			if(setpoint[i]<=-90)
				setpoint[i]=-90;
			if(setpoint[i]>=0)
				setpoint[i]=0;
		}
	}
	if(last_flag!=flag)
		DEAD_ZONE;//�������ת��Ҫ�仯��ȫ������һ������
		
}
int pid0=0,pid1=0,pid2=0,pid3=0;
int i=0;
//��TIM6���жϷ������н��б�����������PID���� 
//һȦ312�����壬ÿ50ms
void TIM6_DAC_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET)
    {
	if(full>0)//���˥��������Ϊ�˱����ظ�ɨ��ͬһ��ֲ��
		full--; 
//			i++;
//		 if(i==20)
//		 {
		 if(PID_ENABLE)
		   {
				 
				if(SET_LOC==1&&SET_DIR==0)
				{
				get_adc(adc);
				set_loc_A(set_loc_1,adc[0],adc[1]);
				SET_DIR=0;//����ͬʱִ��
				}
				if(SET_LOC==2&&SET_DIR==0)
				{
				get_adc(adc);
				set_loc_B(set_loc_1,adc[0],adc[1]);
				SET_DIR=0;//����ͬʱִ��
				}
				if(SET_LOC==3&&SET_DIR==0)
				{
				set_loc_C(set_loc_2,Ultra(0));
				SET_DIR=0;//����ͬʱִ��
				}
				if(SET_LOC==4&&SET_DIR==0)
				{
				set_loc_D(set_loc_3,Ultra(1));
				SET_DIR=0;//����ͬʱִ��
				}
				if(SET_DIR==1&&SET_LOC==0)
				{
				get_adc(adc);
				set_dir(adc[0],adc[1]);
				SET_LOC=0;//����ͬʱִ��
				}
    			//  printf("adc0=%3.3f\n,adc1=%3.3f\n",adc[0],adc[1]);
//				 printf("DIR_MODE:%d\n",SET_DIR);
				 Caculate_Encoder();
				 pid0=inc_PID0(&PID0);
				 pid1=inc_PID1(&PID1);
				 pid2=inc_PID2(&PID2);
				 pid3=inc_PID3(&PID3);
				 set_motor(pid0,pid1,pid2,pid3);
// 			 	get_LightSwitch(LightSwitch);//��ȡ��翪��״̬
//			 }
//		 Caculate_Encoder();//���ƽ��Ҫע�͵�
// 		 printf("Encoder0=%d,Encoder1=%d,Encoder2=%d,Encoder3=%d,goal=%d\r\n",Encoder[0],Encoder[1],Encoder[2],Encoder[3],20);
//		 i=0;
			 
			Encoder[0]=Encoder[1]=Encoder[2]=Encoder[3]=0;
			}
		 TIM_ClearITPendingBit(TIM6,TIM_IT_Update); 
	}
    
}

 

