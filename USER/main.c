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
 ALIENTEK��ӢSTM32������ʵ��17
 ADC ʵ��   
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

   
int main(void)
{

    vu8 key=0,len,t;	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();			 	
 	Adc_Init();		  		//ADC��ʼ��
    TIM3_PWM_Init(1800*2-1,0);	 //����Ƶ��PWMƵ��=72000000/1800=40Khz
    TIM2_PWM_Init(1800*2-1,0);	 //����Ƶ��PWMƵ��=72000000/1800=40Khz
    TIM1_Init();
    TIM4_Int_Init(100,71);
	KEY_Init();         	//��ʼ���밴�����ӵ�Ӳ���ӿ�
    set_fre=50;
	
	//Time_control=500000/(set_fre*Sample);
	//TIM4->ARR = 10;
	while(1)
	{ 

        //ADC_get();
        oscilograph();	
        


        key=KEY_Scan(0);	//�õ���ֵ
	   	if(key)
		{		
			
			switch(key)
			{				 
				case WKUP_PRES:	//����Ƶ������
                    set_fre++;
					break; 
				case KEY1_PRES:	//����Ƶ��Ϊ50
					set_fre=50;
					break;
				case KEY0_PRES:	//����Ƶ���½�
					set_fre--;
					break;
			}
            Time_control=1000000/((set_fre+2)*Sample);
			TIM4->ARR = Time_control;
        }
//        if(USART_RX_STA&0x8000)
//		{					   
//			len=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
//			printf("\r\n�����͵���ϢΪ:\r\n\r\n");
//			for(t=0;t<len;t++)
//			{
//				USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������
//				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
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
    LCD_ShowxNum(156,70,set_fre,4,16,0);//��ʾADC��ֵ
    
//    LCD_ShowString(100,50,200,16,16,"speed:");
//    LCD_ShowxNum(156,50,count1,4,16,0);//��ʾ����1
//    
//    LCD_ShowString(100,30,200,16,16,"r2:");
//    LCD_ShowxNum(156,30,r2,4,16,0);//��ʾ����1
//    
//    LCD_ShowString(100,10,200,16,16,"set:");
//    if (set_speed<0) {LCD_ShowString(150,10,200,16,16,"-");LCD_ShowxNum(156,10,-set_speed,4,16,0);}//��ʾ����1
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
//        while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
}



