#ifndef __TOP_MOTOR_H
#define __TOP_MOTOR_H
#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
void Top_Motor_init(void);
void project(int duty,u16 time);//参数1：正转，反转:；参数2：延时
#endif
