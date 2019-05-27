//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2018-03-29
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(STM32F0ϵ��)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��3.3v��Դ
//              D0   ��PA9��SCL��
//              D1   ��PA7��SDA��
//              RES  ��PA6
//              DC   ��PA5
//              CS   ��PA4               
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/

#ifndef __LED_H
#define __LED_H

// #define u8 unsigned char
// #define u32 unsigned int
// #define LED_GPIO_CLKA   RCC_AHBPeriph_GPIOA 
// #define LED_GPIO_CLKB   RCC_AHBPeriph_GPIOB
// #define LED_PORT   	   GPIOB
// #define LED_PIN        GPIO_PIN_4
#define Max_Column	128
#define Max_Row		64
#define SIZE 16
void LED_Init(void);
// void LED_ON(void);
// void LED_OFF(void);

// #define LED_ON HAL_GPIO_WritePin(LED_PORT, LED_PIN,GPIO_PIN_SET)
// #define LED_OFF HAL_GPIO_WritePin(LED_PORT, LED_PIN,GPIO_PIN_RESET)


#define OLED_SCLK_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET)	//CLK  PB13
#define OLED_SCLK_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET)

#define OLED_SDIN_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET)	//DIN  PB15
#define OLED_SDIN_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET)

#define OLED_RST_Clr() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET)		//RES	PD8
#define OLED_RST_Set() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET)			//DC	PD9
#define OLED_DC_Set() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_SET)
 		     
#define OLED_CS_Clr()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_RESET)		//CS		PD10	// û�õ�
#define OLED_CS_Set()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, GPIO_PIN_SET)

#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 mode,u8 line);
extern void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size,u8 bufchar,u8 mode);
extern void OLED_ShowString(u8 x,u8 y, u8 *p,u8 mode,u8 line);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
extern void OLED_ShowLine(void);
extern void OLED_Show_Out1(u8 cnt,u8 x,u8 y,u32 c_out1V,u32 c_out1A,u8 cur);
extern void LEDLoop(void);
#endif  
