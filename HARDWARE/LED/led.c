#include "led.h" 
	    
//LED IO初始化
void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOF时钟

  //GPIOC8初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
  //GPIOC8初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);//GPIOC9设置高，灯灭
  
}






