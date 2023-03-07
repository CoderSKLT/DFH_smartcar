#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f4xx.h"

#include "usart.h"
void MOTOR_PWM_Init(void);

void set_motor(int pwm1, int pwm2,int pwm3 ,int pwm4);
#endif
