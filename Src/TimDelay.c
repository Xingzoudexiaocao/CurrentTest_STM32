#include "main.h"

#include "string.h"
#include "system.h"
#include "TimDelay.h"


void TIM4_Init_Query(CALC_TYPE type)  
{  
    TIM_TimeBaseInitTypeDef Tim4;  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  
    Tim4.TIM_Period=1; //???  
    if(type==CALC_TYPE_S) //��ʱ��sΪ��λʱ��ʱ��Ƶ��57600Hz���ⲿ��Ҫ1250�μ�ʱ
    {  
        Tim4.TIM_Prescaler=57600-1; //Ԥ��Ƶ 72MHz / 57600= 1250Hz  
    }else if(type==CALC_TYPE_MS)  
    {  
        Tim4.TIM_Prescaler=2880-1; //25000Hz ,��ʱ������25��Ϊms  
    }else if(type==CALC_TYPE_US)  
    {     
        Tim4.TIM_Prescaler=72-1; //1MHz ,����1��Ϊus  
    }else  
    {  
        Tim4.TIM_Prescaler=7200-1;  
    }  
    Tim4.TIM_ClockDivision=0;  
    Tim4.TIM_CounterMode=TIM_CounterMode_Down; // ���¼���
    TIM_TimeBaseInit(TIM4,&Tim4);         
}  
  
static void TIM4_S_CALC(u32 s)  
{  
    u16 counter=(s*1250)&0xFFFF; //ǰ�ᶨʱ��ʱ��Ϊ1250Hz  
    TIM_Cmd(TIM4,ENABLE);  
    TIM_SetCounter(TIM4,counter); //���ü���ֵ
      
    while(counter>1)  
    {  
        counter=TIM_GetCounter(TIM4);  
    }  
    TIM_Cmd(TIM4,DISABLE);  
}  
  
static void TIM4_MS_CALC(u32 ms)  
{  
    u16 counter=(ms*25)&0xFFFF;   
    TIM_Cmd(TIM4,ENABLE);  
    TIM_SetCounter(TIM4,counter); //���ü���ֵ
      
    while(counter>1)  
    {  
        counter=TIM_GetCounter(TIM4);  
    }  
    TIM_Cmd(TIM4,DISABLE);  
}  
  
static void TIM4_US_CALC(u32 us)  
{  
    u16 counter=us&0xffff;  
    TIM_Cmd(TIM4,ENABLE);  
    TIM_SetCounter(TIM4,counter); //���ü���ֵ
  
    while(counter>1)  
    {  
        counter=TIM_GetCounter(TIM4);  
    }  
    TIM_Cmd(TIM4,DISABLE);  
}  

void Delay_1us(u16 m)
{
		TIM4_US_CALC(m);
}

void Delay_1ms(u16 m)
{
		TIM4_MS_CALC(m);
}
// int main()  
// {  
//     SystemInit(); //�⺯������ʼ��ϵͳʱ��Դѡ��,PLL��    
//   
// #ifdef _DEBUG  
//     Init_PD11();  
// #endif  
//     TIM5_Init_Query(CALC_TYPE_US);  
//   
//     while(1)  
//     {  
//         PD11_U; //��������
//         TIM5_US_CALC(1);   //TIM5_MS_CALC(1); ������ʱ, TIM5_S_CALC(1); �뼶����ʱ   
//         PD11_D;  
//         TIM5_US_CALC(1);  
//     }  
// } 
