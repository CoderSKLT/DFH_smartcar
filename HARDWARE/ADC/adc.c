#include "adc.h"
#define ADC_Nbr 2
__IO uint16_t ADC_Value[ADC_Nbr]={0};//0是前侧，1是后侧，2是屁股，3是头
void get_adc(float *temp)
{
	char i=0;
	temp[0]=temp[1]=0;
	while(i!=10)
	{
	i++;
	if(DMA_GetFlagStatus(DMA2_Stream0,DMA_FLAG_TCIF0)!=RESET)/*传输完成*/
		{
			DMA_ClearFlag(DMA2_Stream0,DMA_FLAG_TCIF0);/*清零*/
				temp[0]+=ADC_Value[0];
				temp[1]+=ADC_Value[1];
		}
	  delay_us(100);
	}
	i=0;
	temp[0]/=10.0f;
	temp[1]/=10.0f;//10次取均值

	temp[0]=12.87f/(temp[0]*3.3f/4095+0.01186f);
	temp[1]=12.87f/(temp[1]*3.3f/4095+0.01186f);
//arm的编译器通常在计算浮点数乘除法时转成双精度的计算，
//在浮点数后添加f后缀。如0.01->0.01f,可以强制转化为float去计算，这样还可以减少代码空间，增加程序执行效率
}
//DMA传输配置
void DMA_Configuration(void) 
{
				DMA_InitTypeDef DMA_InitStructure;

				RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);/*DMA2的时钟使能*/
				while(DMA_GetCmdStatus(DMA2_Stream0)!=DISABLE);/*等待DMA可以配置*/


				DMA_InitStructure.DMA_Channel = DMA_Channel_2;/*DMA通道0*/
				DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_BASE+0x4C;/*外设地址*/
				DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)ADC_Value;/*存取器地址*/
				DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;/*方向从外设到内存*/
				DMA_InitStructure.DMA_BufferSize = ADC_Nbr;/*数据传输的数量为4*/
				DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/*地址不增加*/
				DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;/*地址增加*/
				DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;/*数据长度半字*/
				DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;/*数据长度半字*/
				DMA_InitStructure.DMA_Priority = DMA_Priority_High;/*高优先级*/
				DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;/*循环模式*/
				DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;/*禁止FIFO*/
				DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;/*FIFO的值*/
				DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;/*单次传输*/
				DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;/*单次传输*/
				DMA_Init(DMA2_Stream0,&DMA_InitStructure);/**/
				DMA_Cmd(DMA2_Stream0,ENABLE);//开启DMA传输
}

void ADC3_Init(void)
{
				GPIO_InitTypeDef GPIO_InitStructure;
				ADC_CommonInitTypeDef ADC_CommonInitStructure;
				ADC_InitTypeDef ADC_InitStructure;

				RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);//使能GPIOA时钟
				RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3,ENABLE);//使能ADC时钟
				DMA_Configuration();
				/*初始化ADC1通道3 的IO口*/
				GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;/*模拟输入*/
				GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3|GPIO_Pin_6;/*通道4*/
				GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;/*不带上下拉*/
				GPIO_Init(GPIOF,&GPIO_InitStructure);/*初始化*/
				
				ADC_TempSensorVrefintCmd(DISABLE);/*使能内部温度传感器*/

				/*通用控制寄存器的配置*/
				ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;/*DMA失能*/
				ADC_CommonInitStructure.ADC_Mode          = ADC_Mode_Independent;/*独立模式*/
				ADC_CommonInitStructure.ADC_Prescaler     = ADC_Prescaler_Div8;/*APB2的4分频 即84/4=21M*/
				ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;/*两个采样阶段的延时5个时钟*/
				ADC_CommonInit(&ADC_CommonInitStructure);/*初始化*/
				/*初始化ADC1*/
				ADC_InitStructure.ADC_Resolution  = ADC_Resolution_12b;/*12位模式*/
				ADC_InitStructure.ADC_ScanConvMode = ENABLE;/*扫描模式*/
				ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;/*禁止连续转换*/
				ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;/*禁止触发检测 使用软件触发*/
				ADC_InitStructure.ADC_DataAlign    = ADC_DataAlign_Right;/*右对齐*/
				ADC_InitStructure.ADC_NbrOfConversion = ADC_Nbr;/*通道数目*/
				ADC_Init(ADC3,&ADC_InitStructure);/*初始化*/

				//FF6,F3,F5,F4
				ADC_RegularChannelConfig(ADC3,ADC_Channel_4,1,ADC_SampleTime_480Cycles);/*设置规则通道16 3个序列 采样时间 */
				ADC_RegularChannelConfig(ADC3,ADC_Channel_9,2,ADC_SampleTime_480Cycles);/*设置规则通道3 2个序列 采样时间 */
//				ADC_RegularChannelConfig(ADC3,ADC_Channel_15,3,ADC_SampleTime_480Cycles);/*设置规则通道2 1个序列 采样时间 */
//				ADC_RegularChannelConfig(ADC3,ADC_Channel_14,4,ADC_SampleTime_480Cycles);/*设置规则通道2 1个序列 采样时间 */
				
				ADC_DMARequestAfterLastTransferCmd(ADC3,ENABLE);//源数据变化时开启DMA传输
				ADC_DMACmd(ADC3,ENABLE);//使能ADC传输 
				ADC_Cmd(ADC3,ENABLE);/*开启转换*/
				ADC_SoftwareStartConv(ADC3);/*启动软件转换*/
}
