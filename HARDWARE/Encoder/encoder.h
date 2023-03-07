#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"
#include "stm32f4xx.h"
#include "usart.h"
#include "pid.h"
void Encoder_Init(void);

void Caculate_Encoder(void);
#endif


