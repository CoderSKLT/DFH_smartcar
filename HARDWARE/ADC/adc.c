#include "adc.h"
#define ADC_Nbr 2
__IO uint16_t ADC_Value[ADC_Nbr]={0};//0��ǰ�࣬1�Ǻ�࣬2��ƨ�ɣ�3��ͷ
void get_adc(float *temp)
{
	char i=0;
	temp[0]=temp[1]=0;
	while(i!=10)
	{
	i++;
	if(DMA_GetFlagStatus(DMA2_Stream0,DMA_FLAG_TCIF0)!=RESET)/*�������*/
		{
			DMA_ClearFlag(DMA2_Stream0,DMA_FLAG_TCIF0);/*����*/
				temp[0]+=ADC_Value[0];
				temp[1]+=ADC_Value[1];
		}
	  delay_us(100);
	}
	i=0;
	temp[0]/=10.0f;
	temp[1]/=10.0f;//10��ȡ��ֵ

	temp[0]=12.87f/(temp[0]*3.3f/4095+0.01186f);
	temp[1]=12.87f/(temp[1]*3.3f/4095+0.01186f);
//arm�ı�����ͨ���ڼ��㸡�����˳���ʱת��˫���ȵļ��㣬
//�ڸ����������f��׺����0.01->0.01f,����ǿ��ת��Ϊfloatȥ���㣬���������Լ��ٴ���ռ䣬���ӳ���ִ��Ч��
}
//DMA��������
void DMA_Configuration(void) 
{
				DMA_InitTypeDef DMA_InitStructure;

				RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);/*DMA2��ʱ��ʹ��*/
				while(DMA_GetCmdStatus(DMA2_Stream0)!=DISABLE);/*�ȴ�DMA��������*/


				DMA_InitStructure.DMA_Channel = DMA_Channel_2;/*DMAͨ��0*/
				DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_BASE+0x4C;/*�����ַ*/
				DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)ADC_Value;/*��ȡ����ַ*/
				DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;/*��������赽�ڴ�*/
				DMA_InitStructure.DMA_BufferSize = ADC_Nbr;/*���ݴ��������Ϊ4*/
				DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/*��ַ������*/
				DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;/*��ַ����*/
				DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;/*���ݳ��Ȱ���*/
				DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;/*���ݳ��Ȱ���*/
				DMA_InitStructure.DMA_Priority = DMA_Priority_High;/*�����ȼ�*/
				DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;/*ѭ��ģʽ*/
				DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;/*��ֹFIFO*/
				DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;/*FIFO��ֵ*/
				DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;/*���δ���*/
				DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;/*���δ���*/
				DMA_Init(DMA2_Stream0,&DMA_InitStructure);/**/
				DMA_Cmd(DMA2_Stream0,ENABLE);//����DMA����
}

void ADC3_Init(void)
{
				GPIO_InitTypeDef GPIO_InitStructure;
				ADC_CommonInitTypeDef ADC_CommonInitStructure;
				ADC_InitTypeDef ADC_InitStructure;

				RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);//ʹ��GPIOAʱ��
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);//ʹ��ADCʱ��
				DMA_Configuration();
				/*��ʼ��ADC1ͨ��3 ��IO��*/
				GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;/*ģ������*/
				GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3|GPIO_Pin_6;/*ͨ��4*/
				GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;/*����������*/
				GPIO_Init(GPIOF,&GPIO_InitStructure);/*��ʼ��*/
				
				ADC_TempSensorVrefintCmd(DISABLE);/*ʹ���ڲ��¶ȴ�����*/

				/*ͨ�ÿ��ƼĴ���������*/
				ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;/*DMAʧ��*/
				ADC_CommonInitStructure.ADC_Mode          = ADC_Mode_Independent;/*����ģʽ*/
				ADC_CommonInitStructure.ADC_Prescaler     = ADC_Prescaler_Div8;/*APB2��4��Ƶ ��84/4=21M*/
				ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;/*���������׶ε���ʱ5��ʱ��*/
				ADC_CommonInit(&ADC_CommonInitStructure);/*��ʼ��*/
				/*��ʼ��ADC1*/
				ADC_InitStructure.ADC_Resolution  = ADC_Resolution_12b;/*12λģʽ*/
				ADC_InitStructure.ADC_ScanConvMode = ENABLE;/*ɨ��ģʽ*/
				ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;/*��ֹ����ת��*/
				ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;/*��ֹ������� ʹ���������*/
				ADC_InitStructure.ADC_DataAlign    = ADC_DataAlign_Right;/*�Ҷ���*/
				ADC_InitStructure.ADC_NbrOfConversion = ADC_Nbr;/*ͨ����Ŀ*/
				ADC_Init(ADC3,&ADC_InitStructure);/*��ʼ��*/

				//FF6,F3,F5,F4
				ADC_RegularChannelConfig(ADC3,ADC_Channel_4,1,ADC_SampleTime_480Cycles);/*���ù���ͨ��16 3������ ����ʱ�� */
				ADC_RegularChannelConfig(ADC3,ADC_Channel_9,2,ADC_SampleTime_480Cycles);/*���ù���ͨ��3 2������ ����ʱ�� */
//				ADC_RegularChannelConfig(ADC3,ADC_Channel_15,3,ADC_SampleTime_480Cycles);/*���ù���ͨ��2 1������ ����ʱ�� */
//				ADC_RegularChannelConfig(ADC3,ADC_Channel_14,4,ADC_SampleTime_480Cycles);/*���ù���ͨ��2 1������ ����ʱ�� */
				
				ADC_DMARequestAfterLastTransferCmd(ADC3,ENABLE);//Դ���ݱ仯ʱ����DMA����
				ADC_DMACmd(ADC3,ENABLE);//ʹ��ADC���� 
				ADC_Cmd(ADC3,ENABLE);/*����ת��*/
				ADC_SoftwareStartConv(ADC3);/*�������ת��*/
}
