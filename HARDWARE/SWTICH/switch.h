#ifndef __SWITCH_H
#define __SWITCH_H
#include "sys.h"
#include "led.h"
#include "delay.h"

//LED�˿ڶ���
#define MODE_Switch PCin(8)	// PC8	 
void scan_for_mode(void);
void Switch_Init(void);//��ʼ��		 				    
#endif
