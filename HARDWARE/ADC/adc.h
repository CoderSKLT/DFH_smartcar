#ifndef __ADC_H
#define __ADC_H

#include "stm32f4xx.h"
#include "delay.h"
#include "sys.h"
//ת������
//DMA��������
void DMA_Configuration(void);
void ADC3_Init(void);
void get_adc(float *temp);
#endif
