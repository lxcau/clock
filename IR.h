                 #ifndef _IR_H_
#define _IR_H_

#include <STC12C5A60S2.h>
#include "delay.h"

sbit IR=P3^2;

void IrCordPro(void);
void Ir_Work(void);
void EX0init(void);
void TIM0init(void);
void IR_Init(void);


#endif