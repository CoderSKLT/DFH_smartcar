#include "led.h" 
	    
//LED IO��ʼ��
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOFʱ��

  //GPIOC8��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
  //GPIOC8��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIO
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);//GPIOC9���øߣ�����
  
}





