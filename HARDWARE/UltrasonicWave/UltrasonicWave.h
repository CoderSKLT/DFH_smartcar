#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H

#include "sys.h"
#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "delay.h"
#include "usart.h"
//信号触发口
#define TRIGfront	PDout(2)
//信号接收口
#define ECHOfront	PDin(3)
//信号触发口
#define TRIGrear	PDout(0)
//信号接收口
#define ECHOrear	PDin(1)
int Ultra_ave(int Ultra_x);
void Ultra_Init(void);


int Ultra(u8 Ultra_x);//返回值即距离值

#endif
