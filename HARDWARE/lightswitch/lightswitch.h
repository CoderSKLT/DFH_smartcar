#ifndef _LIGHTSWITCH_H_
#define _LIGHTSWITCH_H_

#include "sys.h"
//�����Ͻ���ʼ��˳ʱ��ΪS1234
void Lightswitch_Init(void);
void get_LightSwitch(u8 *temp);

#define S1 PFin(14)
#define S2 PFin(15)
#define S3 PFin(12)
#define S4 PFin(13)

#endif

