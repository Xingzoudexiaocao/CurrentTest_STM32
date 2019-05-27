#ifndef __TIMDELAY_h
#define __TIMDELAY_h

typedef unsigned char       CALC_TYPE;  

#define CALC_TYPE_S  1
#define CALC_TYPE_MS 2
#define CALC_TYPE_US 3

extern void TIM4_Init_Query(CALC_TYPE type);
extern void Delay_1us(u16 m);
extern void Delay_1ms(u16 m);

#endif
