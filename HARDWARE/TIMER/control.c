#include "control.h"

u16 r1,r2,time,set_fre;
u16 Time_control;
u8 sin_counter;
u16 sin_chart[100]={
 0x000,0x001,0x004,0x009,0x010,0x019,0x023,0x030,0x03F,0x04F
,0x061,0x075,0x08A,0x0A1,0x0B9,0x0D2,0x0ED,0x109,0x125,0x143
,0x161,0x180,0x19F,0x1BF,0x1DF,0x1FF,0x21F,0x23F,0x25F,0x27E
,0x29D,0x2BB,0x2D9,0x2F5,0x311,0x32C,0x345,0x35D,0x374,0x389
,0x39D,0x3AF,0x3BF,0x3CE,0x3DB,0x3E5,0x3EE,0x3F5,0x3FA,0x3FD
,0x3FE,0x3FD,0x3FA,0x3F5,0x3EE,0x3E5,0x3DB,0x3CE,0x3BF,0x3AF
,0x39D,0x389,0x374,0x35D,0x345,0x32C,0x311,0x2F5,0x2D9,0x2BB
,0x29D,0x27E,0x25F,0x23F,0x21F,0x1FF,0x1DF,0x1BF,0x19F,0x180
,0x161,0x143,0x125,0x109,0x0ED,0x0D2,0x0B9,0x0A1,0x08A,0x075
,0x061,0x04F,0x03F,0x030,0x023,0x019,0x010,0x009,0x004,0x001};

//float out_P=60,out_I=0.00,out_D=0;
//float speed_P=3,speed_I=2,speed_D=0.7;

//s16 err[4],err_speed[3],output,set_fre,err_speed_I;
 
 

//定时器4中断服务程序
void TIM4_IRQHandler(void)   //TIM4中断
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
		{
//            time++;
//            if (time>=Time_control) {
//                time=0;
                sin_counter++;
                if (sin_counter==200) 
					sin_counter=0;
				
                if (sin_counter<100)
				{
                    r1=sin_chart[sin_counter];
                    r2=0;
                }
                else 
				{
                    r2=sin_chart[sin_counter-100];
                    r1=0;
                }
				
                r1=r1*2;
                r2=r2*2;
                TIM2->CNT=0;
				TIM3->CNT = 0;
				TIM4->CNT = 0;
                TIM_SetCompare2(TIM3,r1);
                TIM_SetCompare4(TIM2,900);

                
            }
            TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志 
            

		//}

}

//void out_pid(void)
//{
//    err[3]=err[2];
//    err[2]=err[1];
//    err[1]=err[0];
//    err[0]=ad_get-up_adress;
//    set_speed =out_P*err[0]+out_D*(err[0]-err[1]);

//    if (output>400) output=400;
//    if (output<-400) output=-400;
//    if (output>0) {r1=output+40; r2=0;}  
//    if (output<0) {r1=0; r2=-output+40;} 
//}

//void speed_pid(void)
//{

//    err_speed[2]=err_speed[1];
//    err_speed[1]=err_speed[0];

////    err_speed[0]=count1-set_speed;
////    err_speed_I+=err_speed[0];
////    if (err_speed_I>200) err_speed_I=200;
////    if (err_speed_I<-200) err_speed_I=-200;
////    output = speed_I*err_speed_I+speed_P*err_speed[0]+speed_D*(err_speed[0]-err_speed[1]);
////    if (output>400) output=400;
////    if (output<-400) output=-400;
////    if (output>0) {r1=output; r2=0;}  
////    if (output<0) {r1=0; r2=-output;} 
//    
//    
//    if (set_speed<0) err_speed[0]=count1+set_speed;
//    if (set_speed>0) err_speed[0]=count1-set_speed;
//   
//    err_speed_I+=err_speed[0];
//    if (err_speed_I>300) err_speed_I=300;
//    if (err_speed_I<-300) err_speed_I=-300;
//    output = speed_I*err_speed_I+speed_P*err_speed[0]+speed_D*(err_speed[0]-err_speed[1]);
//    if (output>600) output=600;
//    if (output<-600) output=-600;
//    if (set_speed>0) {r1=-output; r2=0;}  
//    if (set_speed<0) {r1=0; r2=-output;} 
//    if (set_speed==0) {err_speed_I=0;r1=0;r2=0;}
//    
//}
    
 
    
