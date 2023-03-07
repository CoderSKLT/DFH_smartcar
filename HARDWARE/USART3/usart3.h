#ifndef __USART3_H
#define __USART3_H
#include "stm32f4xx.h"
#include "usart.h"	

void USART3_Init(void);
void CONTINUE(u8 status,int time);
void center(void);
u8 NEXT(void);
#endif
