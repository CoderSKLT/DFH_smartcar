#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H

#include "sys.h"
#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "delay.h"
#include "usart.h"
//�źŴ�����
#define TRIGfront	PDout(2)
//�źŽ��տ�
#define ECHOfront	PDin(3)
//�źŴ�����
#define TRIGrear	PDout(0)
//�źŽ��տ�
#define ECHOrear	PDin(1)
int Ultra_ave(int Ultra_x);
void Ultra_Init(void);


int Ultra(u8 Ultra_x);//����ֵ������ֵ

#endif
