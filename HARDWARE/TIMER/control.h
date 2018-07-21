#ifndef __CONTROL_H
#define __CONTROL_H	
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "timer.h"
#define up_adress 4008

#define Sample 200
extern u16 ad_get;
//extern s16 err[4],err_I,output;
extern u16 r1,r2;
void TIM4_IRQHandler(void);
void out_pid(void);
void speed_pid(void);
extern u16 set_fre,Time_control;






#endif  
