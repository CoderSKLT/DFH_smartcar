#ifndef __SWITCH_H
#define __SWITCH_H
#include "sys.h"
#include "led.h"
#include "delay.h"

//LED端口定义
#define MODE_Switch PCin(8)	// PC8	 
void scan_for_mode(void);
void Switch_Init(void);//初始化		 				    
#endif
