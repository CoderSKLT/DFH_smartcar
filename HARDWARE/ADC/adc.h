#ifndef __ADC_H
#define __ADC_H

#include "stm32f4xx.h"
#include "delay.h"
#include "sys.h"
//转换次数
//DMA传输配置
void DMA_Configuration(void);
void ADC3_Init(void);
void get_adc(float *temp);
#endif
