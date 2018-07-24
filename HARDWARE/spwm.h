#ifndef __SPWM_H
#define __SPWM_H 	
#include "sys.h"

#define ZBF 3600   //72000000/20000  20K载波频率
#define PI  3.14159
#define DTCOUNT 1   //72M频率，死区时间为多少个时钟周期 
#define M 0.7


extern u16 r1,r2,r0;

extern u16 set_fre,Time_control;



void SPWM_Init(void) ;
void sinxcalculate(void);
void sinxcalculate_once(void);
void TIM4_IRQHandler(void); 
void sin_cal(void);


#endif
