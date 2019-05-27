//#include "main.h"
#include "lcd.h"
#include "font.h" 
#include "delay.h"	 

#include "stdarg.h"
#include "string.h"
//#include "ff.h"	

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniSTM32F103������
//2.4��/2.8��/3.5��/4.3��/7�� TFTҺ������	  
//֧������IC�ͺŰ���:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/
//                  SPFD5408/1505/B505/C505/NT35310/NT35510/SSD1963��		    
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2010/7/4
//�汾��V3.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved	
//********************************************************************************
//V1.2�޸�˵��
//֧����SPFD5408������,�����Һ��IDֱ�Ӵ�ӡ��HEX��ʽ.����鿴LCD����IC.
//V1.3
//�����˿���IO��֧��
//�޸��˱�����Ƶļ��ԣ�������V1.8���Ժ�Ŀ�����汾��
//����1.8�汾֮ǰ(������1.8)��Һ��ģ��,���޸�LCD_Init������LCD_LED=1;ΪLCD_LED=1;
//V1.4
//�޸���LCD_ShowChar������ʹ�û��㹦�ܻ��ַ���
//�����˺�������ʾ��֧��
//V1.5 20110730
//1,�޸���B505Һ������ɫ�����bug.
//2,�޸��˿���IO�������������÷�ʽ.
//V1.6 20111116
//1,�����LGDP4535Һ��������֧��
//V1.7 20120713
//1,����LCD_RD_DATA����
//2,���Ӷ�ILI9341��֧��
//3,����ILI9325�Ķ�����������
//4,����LCD_Scan_Dir����(����ʹ��)	  
//6,�����޸��˲���ԭ���ĺ���,����Ӧ9341�Ĳ���
//V1.8 20120905
//1,����LCD��Ҫ�������ýṹ��lcddev
//2,����LCD_Display_Dir����,֧�����ߺ������л�
//V1.9 20120911
//1,����RM68042������ID:6804��������6804��֧�ֺ�����ʾ����ԭ�򣺸ı�ɨ�跽ʽ��
//����6804��������ʧЧ���Թ��ܶ෽�������У���ʱ�޽⡣
//V2.0 20120924
//�ڲ�Ӳ����λ�������,ILI9341��ID��ȡ�ᱻ�����9300,�޸�LCD_Init,���޷�ʶ��
//�����������IDΪ9300/�Ƿ�ID��,ǿ��ָ������ICΪILI9341��ִ��9341�ĳ�ʼ����
//V2.1 20120930
//����ILI9325����ɫ��bug��
//V2.2 20121007
//����LCD_Scan_Dir��bug��
//V2.3 20130120
//����6804֧�ֺ�����ʾ
//V2.4 20131120
//1,����NT35310��ID:5310����������֧��
//2,����LCD_Set_Window����,�������ô���,�Կ������,�Ƚ�����,���Ǹú����ں���ʱ,��֧��6804.
//V2.5 20140211
//1,����NT35510��ID:5510����������֧��
//V2.6 20140504
//1,����ASCII 24*24�����֧��(���������û������������)  
//2,�޸Ĳ��ֺ�������,��֧��MDK -O2�Ż�
//3,���9341/35310/35510,дʱ������Ϊ���,�����ܵ�����ٶ�
//4,ȥ����SSD1289��֧��,��Ϊ1289ʵ����̫����,������Ҫ1us...��ֱ����.���ʺ�F4ʹ��
//5,����68042��C505��IC�Ķ���ɫ������bug.
//V2.7 20140710
//1,����LCD_Color_Fill������һ��bug. 
//2,����LCD_Scan_Dir������һ��bug.
//V2.8 20140721
//1,���MDKʹ��-O2�Ż�ʱLCD_ReadPoint��������ʧЧ������.
//2,����LCD_Scan_Dir����ʱ���õ�ɨ�跽ʽ��ʾ��ȫ��bug.
//V2.9 20141130
//1,������SSD1963 LCD��֧��.
//2,����LCD_SSD_BackLightSet����
//3,ȡ��ILI93XX��Rxx�Ĵ�������
//V3.0 20150423
//�޸�SSD1963 LCD������������.
//////////////////////////////////////////////////////////////////////////////////	 

 
//LCD�Ļ�����ɫ�ͱ���ɫ	   
u16 POINT_COLOR=0x0000;	//������ɫ
u16 BACK_COLOR=0xFFFF;  //����ɫ 

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;
	
////////////////////////////////////////////////////////
///**
// * @brief Use Serial Debug Wire SWO Pin to send out the
// *
// * @param pcBuff
// * @param length
// *
// * @return int
// */
//int SwdWrite(char * pcBuff,unsigned long length)
//{
//        int xBytesSent=0;
//        while (length)
//        {
//                ITM_SendChar((uint32_t)(*pcBuff));
//                length--;
//                pcBuff++;
//                xBytesSent++;
//        }
//        return xBytesSent++;
//}

///**
// * @brief private impleted printf function,this would save a lot
// * of rom&ram space compare to the built in printf
// * function in the C newlib.Carefully use it inside a
// * interrupt service routine.
// * @param fmt
// *
// * @return int
// */
//int printf(const char * fmt, ...)
//{


//        int length=0;
//        va_list va;

//        char pcBuff[200];

//        va_start(va, fmt);
//       // ts_formatstring(pcBuff, fmt, va);
//        vsprintf(pcBuff, fmt, va);
//        va_end(va);

//      // #ifdef SERIAL_DEBUG
//      // length = CONSOLE_UART.ulWrite(pcBuff,strlen(pcBuff));
//     // #endif
//     // #ifdef SWD_DEBUG
//        length = SwdWrite(pcBuff,strlen(pcBuff));
//     // #endif

//        return (length);
//}
////////////////////////////////////////////////////////

//д�Ĵ�������
//data:�Ĵ���ֵ
void LCD_WR_REG(u16 data)
{ 
	LCD_RS_CLR;//д��ַ  
 	LCD_CS_CLR; 
	DATAOUT(data); 
	LCD_WR_CLR; 
	LCD_WR_SET; 
 	LCD_CS_SET;   
}
//д���ݺ���
//�������LCD_WR_DATAX��,��ʱ�任�ռ�.
//data:�Ĵ���ֵ
void LCD_WR_DATAX(u16 data)
{
	LCD_RS_SET;
	LCD_CS_CLR;
	DATAOUT(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCD_CS_SET;
}

// ����Data GPIO Ϊ����������
void SetDataGPIOMode(bool input)
{
	 	GPIO_InitTypeDef GPIO_InitStructure;

		GPIO_InitStructure.Pin = GPIO_PIN_All;	//  PORTB�������
		if(input)
		{
			GPIO_InitStructure.Mode = GPIO_MODE_INPUT;  		// ��������
			GPIO_InitStructure.Pull   = GPIO_PULLUP;
		}
		else
		{
			GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;  // �������
			GPIO_InitStructure.Pull   = GPIO_NOPULL;
		}
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOB
	 
		if(input)
		{
// 			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All, GPIO_PIN_RESET);
			GPIOC->ODR=0X0000;     //ȫ�����0
		}
		else
		{
// 			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All, GPIO_PIN_SET);
			GPIOC->ODR=0XFFFF;    //ȫ�������
		}
}

//��LCD����
//����ֵ:������ֵ
u16 LCD_RD_DATA(void)
{										   
	u16 t;
//  	GPIOB->CRL=0X88888888; //PB0-7  ��������
// 	GPIOB->CRH=0X88888888; //PB8-15 ��������
// 	GPIOB->ODR=0X0000;     //ȫ�����0
	SetDataGPIOMode(true);
	
	LCD_RS_SET;
	LCD_CS_CLR;
	//��ȡ����(���Ĵ���ʱ,������Ҫ��2��)
	LCD_RD_CLR;
	if(lcddev.id==0X8989)delay_us(2);//FOR 8989,��ʱ2us					   
	t=DATAIN;  
	LCD_RD_SET;
	LCD_CS_SET; 

// 	GPIOB->CRL=0X33333333; //PB0-7  �������
// 	GPIOB->CRH=0X33333333; //PB8-15 �������
// 	GPIOB->ODR=0XFFFF;    //ȫ�������
		SetDataGPIOMode(false);
	return t;  
}
//д�Ĵ���
//LCD_Reg:�Ĵ������
//LCD_RegValue:Ҫд���ֵ
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg>>8); 
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATA(LCD_RegValue>>8);	
	LCD_WR_DATA(LCD_RegValue);	    		 
}   
//���Ĵ���
//LCD_Reg:�Ĵ������
//����ֵ:������ֵ
u16 LCD_ReadReg(u16 LCD_Reg)
{										   
 	LCD_WR_REG(LCD_Reg);  //д��Ҫ���ļĴ�����  
	return LCD_RD_DATA(); 
} 
//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
} 
//LCDдGRAM
//RGB_Code:��ɫֵ
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD_WR_DATA(RGB_Code);//дʮ��λGRAM
}
//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
u16 LCD_BGR2RGB(u16 c)
{
	u16  r,g,b,rgb;   
	b=(c>>0)&0x1f;
	g=(c>>5)&0x3f;
	r=(c>>11)&0x1f;	 
	rgb=(b<<11)+(g<<5)+(r<<0);		 
	return(rgb);
}	
//��mdk -O1ʱ���Ż�ʱ��Ҫ����
//��ʱi
void opt_delay(u8 i)
{
	while(i--);
}
//��ȡ��ĳ�����ɫֵ	 
//x,y:����
//����ֵ:�˵����ɫ
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r,g,b;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//�����˷�Χ,ֱ�ӷ���		   
	LCD_SetCursor(x,y);
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X1963)LCD_WR_REG(0X2E);//9341/6804/3510/1963 ���Ͷ�GRAMָ��
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2E00);	//5510 ���Ͷ�GRAMָ��
	else LCD_WR_REG(0X22);      		 			//����IC���Ͷ�GRAMָ��
// 	GPIOB->CRL=0X88888888; 							//PB0-7  ��������
// 	GPIOB->CRH=0X88888888; 							//PB8-15 ��������
// 	GPIOB->ODR=0XFFFF;     							//ȫ�������
	SetDataGPIOMode(true);
	
	LCD_RS_SET;
	LCD_CS_CLR;	    
	//��ȡ����(��GRAMʱ,��һ��Ϊ�ٶ�)	
	LCD_RD_CLR;		   
	opt_delay(2);									//��ʱ		
 	r=DATAIN;  										//ʵ��������ɫ 
	LCD_RD_SET;
	if(lcddev.id==0X1963)
	{
		LCD_CS_SET;
// 		GPIOB->CRL=0X33333333; 		//PB0-7  �������
// 		GPIOB->CRH=0X33333333; 		//PB8-15 �������
// 		GPIOB->ODR=0XFFFF;    		//ȫ�������  
		SetDataGPIOMode(false);
		return r;					//1963ֱ�Ӷ��Ϳ��� 
 	}
 	//dummy READ
	LCD_RD_CLR;					   
	opt_delay(2);//��ʱ					   
 	r=DATAIN;  	//ʵ��������ɫ
	LCD_RD_SET;
 	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)	//9341/NT35310/NT35510Ҫ��2�ζ���
	{	 
		LCD_RD_CLR;					   
		opt_delay(2);//��ʱ			   
		b=DATAIN;//��ȡ��ɫֵ  	  
	 	LCD_RD_SET;
		g=r&0XFF;//����9341,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
		g<<=8;
	}else if(lcddev.id==0X6804)
	{
		LCD_RD_CLR;					   
	 	LCD_RD_SET;
		r=DATAIN;//6804�ڶ��ζ�ȡ�Ĳ�����ʵֵ 
	}	 
	LCD_CS_SET;
// 	GPIOB->CRL=0X33333333; 		//PB0-7  �������
// 	GPIOB->CRH=0X33333333; 		//PB8-15 �������
// 	GPIOB->ODR=0XFFFF;    		//ȫ�������  
	SetDataGPIOMode(false);
	
	if(lcddev.id==0X9325||lcddev.id==0X4535||lcddev.id==0X4531||lcddev.id==0X8989||lcddev.id==0XB505)return r;	//�⼸��ICֱ�ӷ�����ɫֵ
	else if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X5510)return (((r>>11)<<11)|((g>>10)<<5)|(b>>11));//ILI9341/NT35310/NT35510��Ҫ��ʽת��һ��
	else return LCD_BGR2RGB(r);	//����IC
}		 
//LCD������ʾ
void LCD_DisplayOn(void)
{					   
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X1963)LCD_WR_REG(0X29);	//������ʾ
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2900);	//������ʾ
	else LCD_WriteReg(0X07,0x0173); 				 	//������ʾ
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	   
	if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X1963)LCD_WR_REG(0X28);	//�ر���ʾ
	else if(lcddev.id==0X5510)LCD_WR_REG(0X2800);	//�ر���ʾ
	else LCD_WriteReg(0X07,0x0);//�ر���ʾ 
}   
//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{	 
 	if(lcddev.id==0X9341||lcddev.id==0X5310)
	{		    
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); 		
	}else if(lcddev.id==0X6804)
	{
		if(lcddev.dir==1)Xpos=lcddev.width-1-Xpos;//����ʱ����
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); 
	}else if(lcddev.id==0X1963)
	{  			 		
		if(lcddev.dir==0)//x������Ҫ�任
		{
			Xpos=lcddev.width-1-Xpos;
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0); 		
			LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF);		 	 
		}else
		{
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 		
			LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);		 	 			
		}	
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); 		
		LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF); 			 		
		
	}else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(Xpos>>8); 		
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(Xpos&0XFF);			 
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(Ypos>>8);  		
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(Ypos&0XFF);			
	}else
	{
		if(lcddev.dir==1)Xpos=lcddev.width-1-Xpos;//������ʵ���ǵ�תx,y����
		LCD_WriteReg(lcddev.setxcmd, Xpos);
		LCD_WriteReg(lcddev.setycmd, Ypos);
	}	 
} 		 
//����LCD���Զ�ɨ�跽��
//ע��:�����������ܻ��ܵ��˺������õ�Ӱ��(������9341/6804����������),
//����,һ������ΪL2R_U2D����,�������Ϊ����ɨ�跽ʽ,���ܵ�����ʾ������.
//dir:0~7,����8������(���嶨���lcd.h)
//9320/9325/9328/4531/4535/1505/b505/5408/9341/5310/5510/1963��IC�Ѿ�ʵ�ʲ���	   	   
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u16 dirreg=0;
	u16 temp;  
	if((lcddev.dir==1&&lcddev.id!=0X6804&&lcddev.id!=0X1963)||(lcddev.dir==0&&lcddev.id==0X1963))//����ʱ����6804��1963���ı�ɨ�跽������ʱ1963�ı䷽��
	{			   
		switch(dir)//����ת��
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	} 
	if(lcddev.id==0x9341||lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X5510||lcddev.id==0X1963)//9341/6804/5310/5510/1963,���⴦��
	{
		switch(dir)
		{
			case L2R_U2D://������,���ϵ���
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		if(lcddev.id==0X5510)dirreg=0X3600;
		else dirreg=0X36;
 		if((lcddev.id!=0X5310)&&(lcddev.id!=0X5510)&&(lcddev.id!=0X1963))regval|=0X08;//5310/5510/1963����ҪBGR   
		if(lcddev.id==0X6804)regval|=0x02;//6804��BIT6��9341�ķ���	   
		LCD_WriteReg(dirreg,regval);
		if(lcddev.id!=0X1963)//1963�������괦��
		{
			if(regval&0X20)
			{
				if(lcddev.width<lcddev.height)//����X,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}else  
			{
				if(lcddev.width>lcddev.height)//����X,Y
				{
					temp=lcddev.width;
					lcddev.width=lcddev.height;
					lcddev.height=temp;
				}
			}  
		}
		if(lcddev.id==0X5510)
		{
			LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA((lcddev.width-1)>>8); 
			LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA((lcddev.width-1)&0XFF); 
			LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(0); 
			LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA((lcddev.height-1)>>8); 
			LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA((lcddev.height-1)&0XFF);
		}else
		{
			LCD_WR_REG(lcddev.setxcmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
			LCD_WR_REG(lcddev.setycmd); 
			LCD_WR_DATA(0);LCD_WR_DATA(0);
			LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
		}
  	}else 
	{
		switch(dir)
		{
			case L2R_U2D://������,���ϵ���
				regval|=(1<<5)|(1<<4)|(0<<3); 
				break;
			case L2R_D2U://������,���µ���
				regval|=(0<<5)|(1<<4)|(0<<3); 
				break;
			case R2L_U2D://���ҵ���,���ϵ���
				regval|=(1<<5)|(0<<4)|(0<<3);
				break;
			case R2L_D2U://���ҵ���,���µ���
				regval|=(0<<5)|(0<<4)|(0<<3); 
				break;	 
			case U2D_L2R://���ϵ���,������
				regval|=(1<<5)|(1<<4)|(1<<3); 
				break;
			case U2D_R2L://���ϵ���,���ҵ���
				regval|=(1<<5)|(0<<4)|(1<<3); 
				break;
			case D2U_L2R://���µ���,������
				regval|=(0<<5)|(1<<4)|(1<<3); 
				break;
			case D2U_R2L://���µ���,���ҵ���
				regval|=(0<<5)|(0<<4)|(1<<3); 
				break;	 
		} 
		dirreg=0X03;
		regval|=1<<12; 
		LCD_WriteReg(dirreg,regval);
	}
}     
//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y)
{
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD_WR_DATA(POINT_COLOR); 
}	 
//���ٻ���
//x,y:����
//color:��ɫ
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
{	   
	LCD_WR_REG(0x00); 
	LCD_WR_REG(0x20); 
	LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF); 
	LCD_WR_REG(0x00); 
	LCD_WR_REG(0x21); 
	LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 	
//	if(lcddev.id==0X9341||lcddev.id==0X5310)
//	{
//		LCD_WR_REG(lcddev.setxcmd); 
//		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);  			 
//		LCD_WR_REG(lcddev.setycmd); 
//		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 		 	 
//	}else if(lcddev.id==0X5510)
//	{
//		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(x>>8);  
//		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(x&0XFF);	  
//		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(y>>8);  
//		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(y&0XFF); 
//	}else if(lcddev.id==0X1963)
//	{
//		if(lcddev.dir==0)x=lcddev.width-1-x;
//		LCD_WR_REG(lcddev.setxcmd); 
//		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF); 		
//		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF); 		
//		LCD_WR_REG(lcddev.setycmd); 
//		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 		
//		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 		
//	}else if(lcddev.id==0X6804)
//	{		    
//		if(lcddev.dir==1)x=lcddev.width-1-x;//����ʱ����
//		LCD_WR_REG(lcddev.setxcmd); 
//		LCD_WR_DATA(x>>8);LCD_WR_DATA(x&0XFF);			 
//		LCD_WR_REG(lcddev.setycmd); 
//		LCD_WR_DATA(y>>8);LCD_WR_DATA(y&0XFF); 		
//	}else
//	{
// 		if(lcddev.dir==1)x=lcddev.width-1-x;//������ʵ���ǵ�תx,y����
//		LCD_WriteReg(lcddev.setxcmd,x);
//		LCD_WriteReg(lcddev.setycmd,y);
//	}	
	LCD_WR_REG(0x00); 
	LCD_WR_REG(0x22); 
	LCD_WR_DATA(color>>8);	
	LCD_WR_DATA(color);		//д����
}
//SSD1963 ��������
//pwm:����ȼ�,0~100.Խ��Խ��.
void LCD_SSD_BackLightSet(u8 pwm)
{	
	LCD_WR_REG(0xBE);	//����PWM���
	LCD_WR_DATA(0x05);	//1����PWMƵ��
	LCD_WR_DATA(pwm*2.55);//2����PWMռ�ձ�
	LCD_WR_DATA(0x01);	//3����C
	LCD_WR_DATA(0xFF);	//4����D
	LCD_WR_DATA(0x00);	//5����E
	LCD_WR_DATA(0x00);	//6����F
}
//����LCD��ʾ����
//dir:0,������1,����
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)			//����
	{
		lcddev.dir=0;	//����
		lcddev.width=240;
		lcddev.height=320;
		if(lcddev.id==0X9341||lcddev.id==0X6804||lcddev.id==0X5310)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
			if(lcddev.id==0X6804||lcddev.id==0X5310)
			{
				lcddev.width=320;
				lcddev.height=480;
			}
		}else if(lcddev.id==0x5510)
		{
			lcddev.wramcmd=0X2C00;
	 		lcddev.setxcmd=0X2A00;
			lcddev.setycmd=0X2B00; 
			lcddev.width=480;
			lcddev.height=800;
		}else if(lcddev.id==0X1963)
		{
			lcddev.wramcmd=0X2C;	//����д��GRAM��ָ�� 
			lcddev.setxcmd=0X2B;	//����дX����ָ��
			lcddev.setycmd=0X2A;	//����дY����ָ��
			lcddev.width=480;		//���ÿ��480
			lcddev.height=800;		//���ø߶�800  
		}else
		{
			lcddev.wramcmd=0X22;
	 		lcddev.setxcmd=0X20;
			lcddev.setycmd=0X21;  
		}
	}else 				//����
	{	  				
		lcddev.dir=1;	//����
		lcddev.width=320;
		lcddev.height=240;
		if(lcddev.id==0X9341||lcddev.id==0X5310)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
		}else if(lcddev.id==0X6804)	 
		{
 			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2B;
			lcddev.setycmd=0X2A; 
		}else if(lcddev.id==0x5510)
		{
			lcddev.wramcmd=0X2C00;
	 		lcddev.setxcmd=0X2A00;
			lcddev.setycmd=0X2B00; 
			lcddev.width=800;
			lcddev.height=480;
		}else if(lcddev.id==0X1963)
		{
			lcddev.wramcmd=0X2C;	//����д��GRAM��ָ�� 
			lcddev.setxcmd=0X2A;	//����дX����ָ��
			lcddev.setycmd=0X2B;	//����дY����ָ��
			lcddev.width=800;		//���ÿ��800
			lcddev.height=480;		//���ø߶�480  
		}else
		{
			lcddev.wramcmd=0X22;
	 		lcddev.setxcmd=0X21;
			lcddev.setycmd=0X20;  
		}
		if(lcddev.id==0X6804||lcddev.id==0X5310)
		{ 	 
			lcddev.width=480;
			lcddev.height=320; 			
		}
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//Ĭ��ɨ�跽��
}	 
//���ô���,���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//width,height:���ڿ�Ⱥ͸߶�,�������0!!
//�����С:width*height. 
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height)
{    
	u8 hsareg,heareg,vsareg,veareg;
	u16 hsaval,heaval,vsaval,veaval; 
	u16 twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;
	if(lcddev.id==0X9341||lcddev.id==0X5310||lcddev.id==0X6804||(lcddev.dir==1&&lcddev.id==0X1963))
	{
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(sx>>8); 
		LCD_WR_DATA(sx&0XFF);	 
		LCD_WR_DATA(twidth>>8); 
		LCD_WR_DATA(twidth&0XFF);  
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(sy>>8); 
		LCD_WR_DATA(sy&0XFF); 
		LCD_WR_DATA(theight>>8); 
		LCD_WR_DATA(theight&0XFF); 
	}else if(lcddev.id==0X1963)//1963�������⴦��
	{
		sx=lcddev.width-width-sx; 
		height=sy+height-1; 
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(sx>>8); 
		LCD_WR_DATA(sx&0XFF);	 
		LCD_WR_DATA((sx+width-1)>>8); 
		LCD_WR_DATA((sx+width-1)&0XFF);  
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(sy>>8); 
		LCD_WR_DATA(sy&0XFF); 
		LCD_WR_DATA(height>>8); 
		LCD_WR_DATA(height&0XFF); 		
	}else if(lcddev.id==0X5510)
	{
		LCD_WR_REG(lcddev.setxcmd);LCD_WR_DATA(sx>>8);  
		LCD_WR_REG(lcddev.setxcmd+1);LCD_WR_DATA(sx&0XFF);	  
		LCD_WR_REG(lcddev.setxcmd+2);LCD_WR_DATA(twidth>>8);   
		LCD_WR_REG(lcddev.setxcmd+3);LCD_WR_DATA(twidth&0XFF);   
		LCD_WR_REG(lcddev.setycmd);LCD_WR_DATA(sy>>8);   
		LCD_WR_REG(lcddev.setycmd+1);LCD_WR_DATA(sy&0XFF);  
		LCD_WR_REG(lcddev.setycmd+2);LCD_WR_DATA(theight>>8);   
		LCD_WR_REG(lcddev.setycmd+3);LCD_WR_DATA(theight&0XFF);  
	}else	//��������IC
	{
		if(lcddev.dir==1)//����
		{
			//����ֵ
			hsaval=sy;				
			heaval=theight;
			vsaval=lcddev.width-twidth-1;
			veaval=lcddev.width-sx-1;				
		}else
		{ 
			hsaval=sx;				
			heaval=twidth;
			vsaval=sy;
			veaval=theight;
		} 
		hsareg=0X50;heareg=0X51;//ˮƽ���򴰿ڼĴ���
		vsareg=0X52;veareg=0X53;//��ֱ���򴰿ڼĴ���	   							  
		//���üĴ���ֵ
		LCD_WriteReg(hsareg,hsaval);
		LCD_WriteReg(heareg,heaval);
		LCD_WriteReg(vsareg,vsaval);
		LCD_WriteReg(veareg,veaval);		
		LCD_SetCursor(sx,sy);	//���ù��λ��
	}
}
/*
**************************************************************************************************
*            void LCD_SetPointPixel(uint16_t usX,uint16_t usY,uint16_t usColor)	
*
* Description: ��LCD��ʾ����ĳһ������ĳ����ɫ��������
* Arguments  :           
*             usX �����ض�ɨ�跽���´��ڵ����X����
  *           usY �����ض�ɨ�跽���´��ڵ����Y����
  *           usWidth �����ڵĿ��
  *           usHeight �����ڵĸ߶�
  *           usColor ����ɫ

* Returns    : 
**************************************************************************************************
*/

void LCD_SetPointPixel(uint16_t usX,uint16_t usY,uint16_t usColor)	
{	
	if((usX<lcddev.width)&&(usY<lcddev.height))
  {
		LCD_Set_Window(usX,usY,1,1);
		LCD_Fill(usX,usY,usX,usY,usColor);
//		LCD_FillColor(1,usColor);
	}
}
//��ʼ��lcd
//�ó�ʼ���������Գ�ʼ������ALIENTEK��Ʒ��LCDҺ����
//������ռ�ýϴ�flash,�û����Ը����Լ���ʵ�����,ɾ��δ�õ���LCD��ʼ������.�Խ�ʡ�ռ�.
void Use_Before(void)
{
		int a = 0;
		// RST
		delay_us(1000);	
	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
    delay_us(1000);					   
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); 	 
    delay_us(1000);	
	
		LCD_WriteReg(0x0000,0x0000);
//	 a = *(__IO u16 *) (Bank1_LCD_D); 
//   printf("ID=%04x\r\n",a);
//	a = *(__IO u16 *) (Bank1_LCD_D); 
//   printf("ID=%04x\r\n",a);
//	a = *(__IO u16 *) (Bank1_LCD_D); 
//   printf("ID=%04x\r\n",a);
//	a = *(__IO u16 *) (Bank1_LCD_D); 
//   printf("ID=%04x\r\n",LCD_ReadReg(0x0000));
//	a = LCD_ReadReg(0x0000);
	while(a != 0xDF05)
	{
		a = LCD_ReadReg(0x0000);
	}
	LCD_WriteReg(0x0000,0x0000); 
	LCD_WriteReg(0x0000,0x0000); 
	LCD_WriteReg(0x0000,0x0000); 
	LCD_WriteReg(0x0000,0x0000); 
	LCD_WriteReg(0x0010,0x0600);      // SLP=0, 
	delay_us(30); 
	LCD_WriteReg(0x0007,0x0000);               
	LCD_WriteReg(0x0012,0x011A); 
	LCD_WriteReg(0x00A4,0x0001); 
	LCD_WriteReg(0x0008,0x020E);    // FP,BP 
	LCD_WriteReg(0x000A,0x0008); 
	LCD_WriteReg(0x000D,0x0008); 
	LCD_WriteReg(0x0030,0x0707); 
	LCD_WriteReg(0x0031,0x0007); 
	LCD_WriteReg(0x0032,0x0603);   
	LCD_WriteReg(0x0033,0x0700);   
	LCD_WriteReg(0x0034,0x0202);   
	LCD_WriteReg(0x0035,0x0002);   
	LCD_WriteReg(0x0036,0x1E00);   
	LCD_WriteReg(0x0037,0x0707);   
	LCD_WriteReg(0x0038,0x0000);   
	LCD_WriteReg(0x0039,0x0000);   
	LCD_WriteReg(0x003A,0x0707);   
	LCD_WriteReg(0x003B,0x0000);   
	LCD_WriteReg(0x003C,0x0007);   
	LCD_WriteReg(0x003D,0x0000);   
	delay_us(30);     
	LCD_WriteReg(0x0011,0x0007);                        
	LCD_WriteReg(0x0060,0xA700); 
	LCD_WriteReg(0x0090,0x0016);    // DIVI, RTNI 
	LCD_WriteReg(0x0017,0x0001);   
	LCD_WriteReg(0x0019,0x0000); // TBT[1:0] 
	delay_us(30);       
	LCD_WriteReg(0x0010,0x16B0);                                      
	LCD_WriteReg(0x0012,0x011A);   
	delay_us(30);       
	LCD_WriteReg(0x0013,0x1400); // VDV[4:0]       
	LCD_WriteReg(0x002A,0x000E); // VCMSEL, VCM2[4:0] 
	LCD_WriteReg(0x0029,0x000E); // VCM1[4:0]              Delay(30); 
	LCD_WriteReg(0x0012,0x013A); // VCOMR[0], VREG1R, PSON, PON, VRH[3:0] 
	delay_us(100); 
	LCD_WriteReg(0x0050,0x0000);   
	LCD_WriteReg(0x0051,0x00EF);   
	LCD_WriteReg(0x0052,0x0000);   
	LCD_WriteReg(0x0053,0x013F);   
	LCD_WriteReg(0x0020,0x0000);   
	LCD_WriteReg(0x0021,0x0000);                       
	LCD_WriteReg(0x0061,0x0000);   
	LCD_WriteReg(0x006A,0x0000);   
	LCD_WriteReg(0x0080,0x0000);   
	LCD_WriteReg(0x0081,0x0000);   
	LCD_WriteReg(0x0082,0x0000);   
	LCD_WriteReg(0x0083,0x0000);   
	LCD_WriteReg(0x0084,0x0000);   
	LCD_WriteReg(0x0085,0x0000);                   
	LCD_WriteReg(0x0092,0x0300);   
	LCD_WriteReg(0x0093,0x0005);   
	LCD_WriteReg(0x0095,0x0000);   
	LCD_WriteReg(0x0097,0x0000);   
	LCD_WriteReg(0x0098,0x0000);   
	LCD_WriteReg(0x0001,0x0100);   
	LCD_WriteReg(0x0002,0x0700);   
	LCD_WriteReg(0x0003,0x1030);   
	LCD_WriteReg(0x0004,0x0000);   
	LCD_WriteReg(0x000C,0x0000);   
	LCD_WriteReg(0x000F,0x0000);   
	delay_us(30);       
	LCD_WriteReg(0x0007,0x0001); 
	LCD_WriteReg(0x0007,0x0021);   
	delay_us(30); 
	LCD_WriteReg(0x0010,0x16B0);    // Write final user’s setting values to BT bits 
	LCD_WriteReg(0x0011,0x0007);    // Write final user’s setting values to VC bits 
	delay_us(30); 
	LCD_WriteReg(0x0007,0x0061);   
	delay_us(30); 
	LCD_WriteReg(0x0007,0x0173);   
	delay_us(30); 

	LCD_WriteReg( 0x01, 0x0000 );	// 0x0100
	LCD_WriteReg( 0x03, 0x0038 );	// 0x1038
//Lcd_Light_ON;
	LCD_LED;
//while(1)
//{
	delay_ms(5);
	LCD_Clear(WHITE);//��һ��������������Ч�������Ǳ����
	delay_ms(5);
	LCD_Clear(CYAN);
	delay_ms(5);
	LCD_Clear(YELLOW);
	delay_ms(5);
	LCD_Clear(RED);
	delay_ms(5);
//	LCD_Clear(GBLUE);
//	delay_ms(5);
//	LCD_Clear(WHITE);
//	delay_ms(5);

//	LCD_Clear(WHITE);
//	delay_ms(5);
//	LCD_Clear(BLACK);
//	delay_ms(5);

// 	LCD_Fill_Pic(0,0,240,320,gImage_MM1_240);Delay(10000000);
// 	LCD_Fill_Pic(0,0,320,240,gImage_MM3_240);Delay(10000000);
//}
}
void LCD_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
	
//  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); //ʹ��PORTB,Cʱ�Ӻ�AFIOʱ��
// 	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//����SWD��ʧ��JTAG
	
	GPIO_InitStructure.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;	   ///PORTC6~10�����������
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;   
	GPIO_InitStructure.Pull   = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure); //GPIOC	

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);

	GPIO_InitStructure.Pin = GPIO_PIN_All;	//  PORTB�������
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure); //GPIOB
 
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All, GPIO_PIN_SET);

	Use_Before();
}  		  

/************************************************
��������Lcdд��ʼ����
���ܣ�����Lcd�������� ִ��д����
************************************************/
void Lcd_WR_Start(void)
{
	LCD_WR_REG(0x00); 
	LCD_WR_REG(0x22); 
//*(__IO u16 *) (Bank1_LCD_C) = 0x00;
//	*(__IO u16 *) (Bank1_LCD_C) = 0x22;
}
/*************************************************
��������Lcd�����㶨λ����
���ܣ�ָ��320240Һ���ϵ�һ����Ϊд���ݵ���ʼ��
��ڲ�����x ���� 0~239
          y ���� 0~319
����ֵ����
*************************************************/
void Lcd_SetCursor(u16 x,u16 y)
{ 
	//�����ת
	LCD_WriteReg(0x20,y);//ˮƽ����
	LCD_WriteReg(0x21,x);//��ֱ����  
}

//��ʼȫ��д,���õ�ַ
void Lcd_Start(void)
{  
	Lcd_SetCursor(0,239);     
	LCD_WriteReg(0x0050,0x00);//ˮƽ GRAM��ʼλ��
	LCD_WriteReg(0x0051,239);//ˮƽGRAM��ֹλ��
	LCD_WriteReg(0x0052,0x00);//��ֱGRAM��ʼλ��
	LCD_WriteReg(0x0053,319);//��ֱGRAM��ֹλ��   
	Lcd_WR_Start();
}

//��������
//color:Ҫ���������ɫ
//void LCD_Clear(u16 color)
//{
//	u32 index=0;      
//	u32 totalpoint=lcddev.width;
//	totalpoint*=lcddev.height; 			//�õ��ܵ���
//	if((lcddev.id==0X6804)&&(lcddev.dir==1))//6804������ʱ�����⴦��  
//	{						    
// 		lcddev.dir=0;	 
// 		lcddev.setxcmd=0X2A;
//		lcddev.setycmd=0X2B;  	 			
//		LCD_SetCursor(0x00,0x0000);		//���ù��λ��  
// 		lcddev.dir=1;	 
//  		lcddev.setxcmd=0X2B;
//		lcddev.setycmd=0X2A;  	 
// 	}else LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
//	LCD_WriteRAM_Prepare();     		//��ʼд��GRAM	  	  
//	for(index=0;index<totalpoint;index++)LCD_WR_DATA(color);	
//}  
/**********************************************
��������Lcdȫ����������
���ܣ���Lcd������Ϊָ����ɫ
��ڲ�����color ָ��Lcdȫ����ɫ RGB(5-6-5)
����ֵ����
***********************************************/
void LCD_Clear(u16 Color)
{
	u32 temp;
	Lcd_Start();
	for (temp = 0; temp < 76800; temp++)
	{
		LCD_WR_DATA(Color>>8);	
		LCD_WR_DATA(Color);	
//		*(__IO u16 *) (Bank1_LCD_D) = Color>>8;
//		*(__IO u16 *) (Bank1_LCD_D) = Color;
	}
}


//��ָ�����������ָ����ɫ
//�����С:(xend-xsta+1)*(yend-ysta+1)
//xsta
//color:Ҫ������ɫ
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	u16 temp;
	if((lcddev.id==0X6804)&&(lcddev.dir==1))	//6804������ʱ�����⴦��  
	{
		temp=sx;
		sx=sy;
		sy=lcddev.width-ex-1;	  
		ex=ey;
		ey=lcddev.width-temp-1;
 		lcddev.dir=0;	 
 		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 			
		LCD_Fill(sx,sy,ex,ey,color);  
 		lcddev.dir=1;	 
  		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;  	 
 	}else
	{
		xlen=ex-sx+1;	 
		for(i=sy;i<=ey;i++)
		{
		 	LCD_SetCursor(sx,i);      				//���ù��λ�� 
			LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
			for(j=0;j<xlen;j++)LCD_WR_DATA(color);	//���ù��λ�� 	    
		}
	}
}  
//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
	u16 height,width;
	u16 i,j;
	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		for(j=0;j<width;j++)LCD_WR_DATA(color[i*width+j]);//д������ 
	}	  
} 
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  		LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
    u8 temp,t1,t;
	u16 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)
				LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)
				LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=320)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=240)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p)
{         
	u8 x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}

u16 ssd1289_GetPoint(u16 x,u8 y)
{
	 u16 a = 0;
	
	LCD_WR_REG(0x4f);
	LCD_WR_DATA(x);	
//	*(__IO u16 *) (Bank1_LCD_C) = 0x4f;	
//	*(__IO u16 *) (Bank1_LCD_D) = x;
	LCD_WR_REG(0x4e);
	LCD_WR_DATA(y);	
//	*(__IO u16 *) (Bank1_LCD_C) = 0x4e;	
//	*(__IO u16 *) (Bank1_LCD_D) = y;
	LCD_WR_REG(0x22);
//	*(__IO u16 *) (Bank1_LCD_C) = 0x22;

	a = LCD_RD_DATA();
//	 a = *(__IO u16 *) (Bank1_LCD_D); 
   return(a);	  
}

static void WriteComm(u16 CMD)
{			
	LCD_WR_REG(CMD);
//	*(__IO u16 *) (Bank1_LCD_C) = CMD;
}
static void WriteData(u16 tem_data)
{			
	LCD_WR_DATA(tem_data);	
//	*(__IO u16 *) (Bank1_LCD_D) = tem_data;
}

/**********************************************
��������Lcd��ѡ����
���ܣ�ѡ��Lcd��ָ���ľ�������

ע�⣺xStart�� yStart������Ļ����ת���ı䣬λ���Ǿ��ο���ĸ���

��ڲ�����xStart x�������ʼ��
          ySrart y�������ֹ��
          xLong Ҫѡ�����ε�x���򳤶�
          yLong  Ҫѡ�����ε�y���򳤶�
����ֵ����
***********************************************/
void Lcd_ColorBox(u16 xStart,u16 yStart,u16 xLong,u16 yLong,u16 Color)
{
	u32 temp;

// 	BlockWrite(xStart,xStart+xLong-1,yStart,yStart+yLong-1);
	for (temp=0; temp<xLong*yLong; temp++)
	{
		LCD_WR_DATA(Color);
//		*(__IO u16 *) (Bank1_LCD_D) = Color;
	}
}

/******************************************
��������Lcdͼ�����100*100
���ܣ���Lcdָ��λ�����ͼ��
��ڲ�����Index ҪѰַ�ļĴ�����ַ
          ConfigTemp д������ݻ�����ֵ
******************************************/
void LCD_Fill_Pic(u16 x, u16 y,u16 pic_H, u16 pic_V, const unsigned char* pic)
{
  unsigned long i;
	unsigned int j;

// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0x0a); //�����������½ǿ�ʼ�������ң����µ���
	Lcd_Start();
	for (i = 0; i < pic_H*pic_V*2; i++)
	{
		LCD_WR_DATA(pic[i]);
//		*(__IO u16 *) (Bank1_LCD_D) = pic[i];
	}
// 	WriteComm(0x36); //Set_address_mode
// 	WriteData(0xaa);
}

void DrawPixel(u16 x, u16 y, u16 Color)
{
// 	BlockWrite(x,x+1,y,y+1);
	LCD_WR_DATA(Color);
//	*(__IO u16 *) (Bank1_LCD_D) = Color;
}


//typedef __packed struct
//{
//	u8  pic_head[2];				//1
//	u16 pic_size_l;			    //2
//	u16 pic_size_h;			    //3
//	u16 pic_nc1;				    //4
//	u16 pic_nc2;				    //5
//	u16 pic_data_address_l;	    //6
//	u16 pic_data_address_h;		//7	
//	u16 pic_message_head_len_l;	//8
//	u16 pic_message_head_len_h;	//9
//	u16 pic_w_l;					//10
//	u16 pic_w_h;				    //11
//	u16 pic_h_l;				    //12
//	u16 pic_h_h;				    //13	
//	u16 pic_bit;				    //14
//	u16 pic_dip;				    //15
//	u16 pic_zip_l;			    //16
//	u16 pic_zip_h;			    //17
//	u16 pic_data_size_l;		    //18
//	u16 pic_data_size_h;		    //19
//	u16 pic_dipx_l;			    //20
//	u16 pic_dipx_h;			    //21	
//	u16 pic_dipy_l;			    //22
//	u16 pic_dipy_h;			    //23
//	u16 pic_color_index_l;	    //24
//	u16 pic_color_index_h;	    //25
//	u16 pic_other_l;			    //26
//	u16 pic_other_h;			    //27
//	u16 pic_color_p01;		    //28
//	u16 pic_color_p02;		    //29
//	u16 pic_color_p03;		    //30
//	u16 pic_color_p04;		    //31
//	u16 pic_color_p05;		    //32
//	u16 pic_color_p06;		    //33
//	u16 pic_color_p07;		    //34
//	u16 pic_color_p08;			//35			
//}BMP_HEAD;

//BMP_HEAD bmp;

// 


////��������С��Χ����ʾͼƬ
//char display_picture(char *filename)
//{
//	u16 ReadValue;
//	FATFS fs;            // Work area (file system object) for logical drive
//	FIL fsrc;      			// file objects
//	u8 buffer[2048]; 		// file copy buffer
//	FRESULT res;         // FatFs function common result code
//	UINT br;         		// File R/W count
//	u16 r_data,g_data,b_data;	
//	u32	 tx,ty,temp;

//  f_mount(0, &fs);

//  res = f_open(&fsrc, filename, FA_OPEN_EXISTING | FA_READ);	 //����ͼƬ�ļ���
//  if(res==FR_NO_FILE||res==FR_INVALID_NAME){
//	 //f_close(&fsrc);
//     f_mount(0, NULL);
//	 return 0;
//  }

//  if(res!=FR_OK){
//     f_mount(0, NULL);
//			SD_Init();
//	 return 0;
//  }

//  res = f_read(&fsrc, &bmp, sizeof(bmp), &br);

//  if(br!=sizeof(bmp)){
//     f_close(&fsrc);
//     f_mount(0, NULL);
//	 return 0;
//  }

//  if((bmp.pic_head[0]=='B')&&(bmp.pic_head[1]=='M'))
//  {
//	res = f_lseek(&fsrc, ((bmp.pic_data_address_h<<16)|bmp.pic_data_address_l));
//	if(res!=FR_OK){
//     f_close(&fsrc);
//     f_mount(0, NULL);
//	 return 0;
//    }
//	if(bmp.pic_h_l<240||bmp.pic_w_l<320)
//	{
//	LCD_Clear(0);

//	Lcd_SetCursor((320-bmp.pic_w_l)/2,(240-bmp.pic_h_l)/2);
//		    //x,y����Ե� 
//	LCD_WriteReg(0x0052,(320-bmp.pic_w_l)/2);//��ֱ GRAM��ʼλ��
//	LCD_WriteReg(0x0053,(320-bmp.pic_w_l)/2+bmp.pic_w_l-1);//��ֱGRAM��ֹλ��

//	LCD_WriteReg(0x0050,(240-bmp.pic_h_l)/2);//ˮƽGRAM��ʼλ��
//	LCD_WriteReg(0x0051,(240-bmp.pic_h_l)/2+bmp.pic_h_l-1);//ˮƽGRAM��ֹλ�� 
//	Lcd_WR_Start();
//	}
////     else 
//	ty=0;
//	for (tx = 0; tx < bmp.pic_h_l; tx++)
//	{
//	f_read(&fsrc, buffer, (bmp.pic_w_l)*3, &br);
//	for(ty=0;ty<bmp.pic_w_l;ty++)
//		{
//			r_data = *(ty*3 +2+buffer);
//			g_data = *(ty*3 +1+buffer);
//			b_data = *(ty*3 +  buffer);			
//			ReadValue=(r_data & 0xF8) << 8 | (g_data & 0xFC) << 3 | b_data >> 3; 
//			LCD_WR_DATA(ReadValue);
////      *(__IO u16 *) (Bank1_LCD_D) = ReadValue;                   
//		}
//	}	
//		f_close(&fsrc);
//   }
//  f_mount(0, NULL);
//  return 1;

//} 




























