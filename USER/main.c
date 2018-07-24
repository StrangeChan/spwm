
#include "spwm.h"  
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "adc.h"

#include "key.h"
#include "lcd.h"
extern u32 adc_cal;
extern u32 adc_sum ;
extern u8 adc_over_flag ;
extern u16 adc_count ;
extern u32 adc_ref ;
extern uint8_t fre_out;
u32 adcave = 0;
u16 M_SET = 0;
u8 key_time = 0;
u8 keyup_flag = 0;
u8 key_xiaodou_flag = 0;
u16 slow_startflag=0;
u16 slowstartcount=0;
u8 YXJ_keydown_flag = 0;
u8 YXJ_new_flag = 0;
u8 change_numcount_flag = 0;
extern u8 change_sin_m_flag;
uint32_t sin_step_set = 0;
uint16_t sin_num_set = 0;        //半周期计数值

u16 rupt_fre[81]={
 0,0,0,0,0,0,0,0,0,0,			//20-29
 0,0,0,0,0,1,1,0,1,1,			//30-39
 1,1,1,1,1,1,1,1,1,1,			//40-49
 1,1,1,1,1,1,1,1,1,1,			//50-59
 1,1,1,1,1,1,1,1,1,1,			//60-69
 1,1,1,1,1,1,1,1,1,1,			//70-79
 1,1,1,1,1,1,0,1,1,0,			//80-89
 1,1,0,1,0,1,0,1,0,1,			//90-99
 0
};


u8 key;

#define SLOWSET 500   //软起动电压设置
#define ADC_SET 2100
#define V_REF_SET 1199 //25v电压采样点
#define DisableAllIRQs() __set_PRIMASK(1);__set_FAULTMASK(1)
#define EnableAllIRQs()  __set_PRIMASK(0);__set_FAULTMASK(0)


void oscilograph(void); 

int main(void)
{
		DisableAllIRQs();   //关中断
	  delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  	  //1组
	
		//sin_cal();
		SPWM_Init();
	//uart_init(9600);
	 // TIM3_Int_Init(999,71);//1ms
    Adc_Init();		  		//ADC初始化
		KEY_Init();
	//LCD_Init();
	
	LCD_Init();
	  EnableAllIRQs();  //开中断
	
	set_fre=50;
	
	Time_control=20000/(set_fre)/4+rupt_fre[set_fre-20];
	//Time_control=20000/(set_fre)/2;
	
	adc_count = 10000.0f/set_fre +0.4;
		while(1)
		{
//			YXJ_keydown_flag = KEY_Scan(0); 			 //不可连续按的扫描模式
//			if(YXJ_keydown_flag==1 && change_numcount_flag==0) //YXJ_UP 增加一个移相角增量
//			{
//				fre_out++;
//				if(fre_out > 100)
//				{
//					fre_out = 100;
//				}
//				sin_cal();
//			  	change_numcount_flag = 1;
//			}
//			if(YXJ_keydown_flag==2 && change_numcount_flag==0) //YXJ_DOWN 减少一个移相角增量
//			{		
//				fre_out--;
//				if(fre_out < 20)
//				{
//					fre_out = 20;
//				}			
//				sin_cal();
//				change_numcount_flag = 1;
//			}			

		oscilograph();	
		key=KEY_Scan(0);	//得到键值
		if(key)
		{		
			
			switch(key)
			{				 
					case WKUP_PRES:	//控制频率上升
						set_fre++;
						break; 
					case KEY1_PRES:	//控制频率为50
						set_fre=50;
						break;
					case KEY0_PRES:	//控制频率下降
						set_fre--;
						break;
			}
			if(set_fre>100) set_fre=100;
			if(set_fre<20) set_fre=20;
			
			
			
			Time_control=20000/(set_fre)/4+rupt_fre[set_fre-20];
			//Time_control=20000/(set_fre)/2;
			adc_count = 10000.0f/set_fre +0.4;
		}







		if(adc_over_flag==1)//99次采完了要做平均为实际的电压值
		{
			adc_ref = adc_sum / (adc_count<<1);
			adcave = adc_cal / (adc_count<<1);
			slowstartcount=0;
        if(!slow_startflag)
        {
         if((adcave < SLOWSET) && (M_SET < 700))  //软启
         {
          M_SET += 6;
         }
         else
         {
					slow_startflag=1;
         }
        }				
				if(slow_startflag)
				{
					if(adcave > V_REF_SET)
					{
						M_SET -= 4;
					}
					else
					{
						M_SET += 4;				
					}					
				}
				if(M_SET > 1010)
				{
					M_SET = 1010;
				}
				if(M_SET < 200)
				{
					M_SET = 200;
				}
				adc_sum	= 0;
				adc_cal = 0;
				adc_over_flag=0;
				if(change_sin_m_flag == 0)
				{
					change_sin_m_flag = 1;					
				}
			}			
		}
}

void oscilograph()
{
    LCD_ShowString(100,70,200,16,16,"set_fre:");
    LCD_ShowxNum(156,70,set_fre,4,16,0);//显示ADC的值
}


//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
//			IWDG_Feed();//喂狗
			if(keyup_flag == 1)
			{
				key_time++;				
			}				
			if(key_time == 10)
			{
				key_time = 0;
				keyup_flag = 0;	
				key_xiaodou_flag = 1;
			}
		}
}
