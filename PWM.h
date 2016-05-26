#ifndef _PWM_H
#define _PWM_H

#include <STC12C5A60S2.h>

void PWM_Init();
void Set_PWM(unsigned char n);//设置占空比，输入1到10

#endif