#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "control.h"
#include "lcd.h"
#include "usart.h"	 
#include "adc.h"
#include "timer.h"

 
void oscilograph(void); 
void ADC_get(void);

u8 i=0;
u16 adcx,ad_all;
u16 ad[51];
u16 x=20,y=120,y_old=0;
u16 count1;
 
u16 ad_get;
/************************************************
 ALIENTEK精英STM32开发板实验17
 ADC 实验   
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

   
int main(void)
{

    vu8 key=0,len,t;	
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			     //LED端口初始化
	LCD_Init();			 	
 	Adc_Init();		  		//ADC初始化
    TIM3_PWM_Init(1800*2-1,0);	 //不分频。PWM频率=72000000/1800=40Khz
    TIM2_PWM_Init(1800*2-1,0);	 //不分频。PWM频率=72000000/1800=40Khz
    TIM1_Init();
    TIM4_Int_Init(100,71);
	KEY_Init();         	//初始化与按键连接的硬件接口
    set_fre=50;
	
	//Time_control=500000/(set_fre*Sample);
	//TIM4->ARR = 10;
	while(1)
	{ 

        //ADC_get();
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
            Time_control=1000000/((set_fre+2)*Sample);
			TIM4->ARR = Time_control;
        }
//        if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
//			printf("\r\n您发送的消息为:\r\n\r\n");
//			for(t=0;t<len;t++)
//			{
//				USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
//			}
//            if (USART_RX_BUF[0]=='-')   set_speed=-(100*(USART_RX_BUF[1]-'0')+10*(USART_RX_BUF[2]-'0')+(USART_RX_BUF[3]-'0'));
//            else set_speed=100*(USART_RX_BUF[0]-'0')+10*(USART_RX_BUF[1]-'0')+(USART_RX_BUF[2]-'0');
//			USART_RX_STA=0;

//		}
        if (set_fre>100) set_fre=100;

	}
}
 









void ADC_get()
{
    u8 j;
        i++;
		adcx=Get_Adc_Average(ADC_Channel_1,3);
        ad[i]=adcx-3000;
        if (i==50) i=0;    
        ad_all=0;
        for (j=1;j<51;j++) ad_all += ad[j];
        ad_get=(ad_all/50)+3000;
}

void oscilograph()
{
    LCD_ShowString(100,70,200,16,16,"set_fre:");
    LCD_ShowxNum(156,70,set_fre,4,16,0);//显示ADC的值
    
//    LCD_ShowString(100,50,200,16,16,"speed:");
//    LCD_ShowxNum(156,50,count1,4,16,0);//显示编码1
//    
//    LCD_ShowString(100,30,200,16,16,"r2:");
//    LCD_ShowxNum(156,30,r2,4,16,0);//显示编码1
//    
//    LCD_ShowString(100,10,200,16,16,"set:");
//    if (set_speed<0) {LCD_ShowString(150,10,200,16,16,"-");LCD_ShowxNum(156,10,-set_speed,4,16,0);}//显示编码1
//    else {LCD_ShowxNum(156,10,set_speed,4,16,0); LCD_ShowString(150,10,200,16,16," ");}
//		
//        y_old=y;
//        if (set_speed>0) y=210-count1;
//        if (set_speed<=0) y=210+count1;
//        x++;
//        if (x==250)
//        {
//            x=20;
//            LCD_Clear(WHITE);
//        }
//        LCD_DrawLine(x-1,y_old,x,y);
//                USART_SendData(USART1, count1);
//        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
}



