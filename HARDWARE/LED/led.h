#ifndef __LED_H
#define __LED_H
#include "sys.h"



//LED端口定义
#define WORK_Led PBout(8)	// PB8
#define MODE_Led PCout(9)	// PC9	 

void LED_Init(void);//初始化		 				    
#endif
