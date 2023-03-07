#ifndef __GM65_H
#define __GM65_H 
#include "sys.h"
#include "stm32f4xx.h"
#include "usart.h"
u8 getQRcode(void);
void USART6_Init(void);
void QR_ask(void);

extern u8 usart_table_get[10];
extern u8 res;


#endif

