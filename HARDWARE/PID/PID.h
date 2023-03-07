#ifndef __PID_H
#define __PID_H
#include "stm32f4xx.h"
#include "usart.h"
#include "encoder.h"
#include "motor.h"
#include "adc.h"
#include "lightswitch.h"
#include "UltrasonicWave.h"
#define DEAD_ZONE {set_motor(0,0,0,0);delay_ms(1);}//以防H桥损坏
#define PID_INVOKE {PID_ENABLE=1;CLEAR=1;}
#define PID_SLEEP {PID_ENABLE=0;set_motor(0,0,0,0);CLEAR=1;}

#define AHEAD {SET_LOC=0;SET_DIR=0;set_pid(25,25,25,25);status=0;CLEAR=1;}
#define BACK {SET_LOC=0;SET_DIR=0;set_pid(-25,-25,-25,-25);status=1;CLEAR=1;}
#define LEFT {SET_LOC=0;SET_DIR=0;set_pid(-20,20,-20,20);status=2;CLEAR=1;}
#define RIGHT {SET_LOC=3;SET_DIR=0;v0=25;Kp_loc=0.6;Kd_loc=0.6;status=3;CLEAR=1;}
#define OP_RIGHT {SET_LOC=4;SET_DIR=0;v0=25;Kp_loc=0.6;Kd_loc=0.6;status=4;CLEAR=1;}
#define STAY {SET_LOC=0;SET_DIR=0;set_pid(0,0,0,0);delay_ms(100);CLEAR=1;}
#define FAST {SET_LOC=1;SET_DIR=0;v0=40;Kp_loc=1.6;Kd_loc=0.8;status=6;CLEAR=1;}
#define OP_FAST {SET_LOC=2;SET_DIR=0;v0=-50;Kp_loc=3;Kd_loc=1;status=7;CLEAR=1;}
#define SLOW {SET_LOC=1;SET_DIR=0;v0=25;Kp_loc=1.6;Kd_loc=0.8;status=8;CLEAR=1;}
#define OP_SLOW {SET_LOC=2;SET_DIR=0;v0=-25;Kp_loc=1.6;Kd_loc=0.8;status=9;CLEAR=1;}

typedef struct 
{
 float proportion ;//比例常数
 float integral ;//积分常数
 float derivative;//微分常数
 int last_error;//e[-1]
 int prev_error;//e[-2]
}PIDtypedef;
void Struct_DEinit(PIDtypedef* PIDx);//结构墙体
void Adjust_dir(void);
void set_dir(float adc1,float adc2);//方向设定
void set_loc_A(float set_location,float adc1,float adc2);//侧边距离设定
int inc_PID0(PIDtypedef* PIDx);
int inc_PID1(PIDtypedef* PIDx);
int inc_PID2(PIDtypedef* PIDx);
int inc_PID3(PIDtypedef* PIDx);
void PID_Init(PIDtypedef* PIDx);
int limits_pwm(int limit_pwm,u8 i);
/**************初始化只调用下面这个即可****************/
void PID_INIT(void);
/*************调节输出只需调用这个改变参数****************/
void set_pid(int target0,int target1,int target2,int target3);
#endif
