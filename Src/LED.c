#include "main.h"
#include "LED.h"
//#include "delay.h"	
#include "oledfont.h"  
#include "bmp.h"
#include "system.h"
// #include "TimDelay.h"
#include "KeyBoard_Drv.h"

 unsigned char V1F = 0;
 unsigned char V1H = 0;
 unsigned char A1F = 0;
 unsigned char A1H = 0;
 unsigned char W1F = 0;
 unsigned char W1H = 0;

void LED_Init(void)
{
	GPIO_InitTypeDef  gpioinitstruct;
	
    /* �����ʼ�� */ 
	gpioinitstruct.Pin    = GPIO_PIN_13 | GPIO_PIN_15;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &gpioinitstruct);
	
	gpioinitstruct.Pin    = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10;
  gpioinitstruct.Mode   = GPIO_MODE_OUTPUT_PP;
  gpioinitstruct.Pull   = GPIO_NOPULL;
  gpioinitstruct.Speed  = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct);
	
	OLED_RST_Set();
	HAL_Delay(20);		//delay_ms 100
	OLED_RST_Clr();
	HAL_Delay(40);		//delay_ms 200
	OLED_RST_Set(); 
					  
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x02,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xff,OLED_CMD); // Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 	
	
	
// 	OLED_ShowString(0,6,"DianLiu2:",0,0);
// 	OLED_ShowString(0,8,"DianLiu3:");
// 	OLED_ShowString(0,10,"DianLiu4:");
// 	OLED_ShowNum(0,4,0,5,16);
// 	OLED_DrawBMP(0,0,127,8,BMP1);		// ��ʾͼƬ
// 	for(i=0;i<7;i++)
// 		OLED_ShowCHinese(i*16,6,i);		// ��ʾ����
}

//��SSD1106д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 

//��SSD1106д��һ���ֽڡ�
//dat:ֻ������ʾ���ݣ�������ʾ��
//cmd:����
void OLED_WR_Byte_2(u8 dat,u8 cmd)
{	
	u8 i;			  
	if(cmd)
	  OLED_DC_Set();
	else 
	  OLED_DC_Clr();		  
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Clr();
		else 
			 OLED_SDIN_Set();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();
	OLED_DC_Set();   	  
} 
	void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);		// 
	OLED_WR_Byte((((x)&0xf0)>>4)|0x10,OLED_CMD);	// x+2
	OLED_WR_Byte(((x)&0x0f),OLED_CMD); 						// x+2
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x02,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //������ʾ
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//line:0,����ʾ�»��ߣ�1����ʾ�»���
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 mode,u8 line)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
		{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			{
				if(mode) {
					OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
				} else {
					OLED_WR_Byte_2(F8X16[c*16+i],OLED_DATA);
				}
			}
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			{
				if(mode) {
						if(line)
								OLED_WR_Byte(F8X16[c*16+i+8]|F_Line[16+i+8],OLED_DATA);
						else
								OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
				} else {
						if(line)
								OLED_WR_Byte_2(F8X16[c*16+i+8]|F_Line[16+i+8],OLED_DATA);
						else
								OLED_WR_Byte_2(F8X16[c*16+i+8],OLED_DATA);
				}
			}
		}
		else {	
			OLED_Set_Pos(x,y+1);
			for(i=0;i<6;i++)
			{
				if(mode) {
					OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				} else {
					OLED_WR_Byte_2(F6x8[c][i],OLED_DATA);
				}
			}
		}
}
//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//bufchar:����0�������ַ���"0"��" "
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size,u8 bufchar,u8 mode)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,bufchar,mode,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',mode,0); 
	}
} 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 mode,u8 line)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],mode,line);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//��ʾ����
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 


//��ָ��λ����ʾһ��11X21���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//���� 11/21 
void OLED_ShowLine(void)
{      	
		u8 i,j;
		for(j=0;j<7;j++)
		{
				OLED_Set_Pos(70+(j*8),3);	
				for(i=0;i<8;i++)
				{
						OLED_WR_Byte(F_Line[16+8+i],OLED_DATA);
				}
		}
		for(j=0;j<4;j++)
		{
				OLED_Set_Pos(70,4+(j));	
				for(i=0;i<8;i++)
				{
						OLED_WR_Byte(F_Line[i+((j%2)*8)],OLED_DATA);
				}
		}

}

void OLED_Show_Out1(u8 cnt,u8 x,u8 y,u32 c_out1V,u32 c_out1A,u8 cur)
{
		u8 i,j,k;
		u8 n[4] = {0,0,0,0};
		u8 a[4] = {0,0,0,0};
		u8 w[4] = {0,0,0,0};
		u8 buf[66];
		n[0] = (c_out1V / 1000) % 10;
		n[1] = (c_out1V / 100) % 10;
		n[2] = (c_out1V / 10) % 10;
		n[3] = (c_out1V) % 10;
		a[0] = (c_out1A / 1000) % 10;
		a[1] = (c_out1A / 100) % 10;
		a[2] = (c_out1A / 10) % 10;
		a[3] = (c_out1A) % 10;
		w[0] = ((c_out1V * c_out1A /100) / 1000) % 10;
		w[1] = ((c_out1V * c_out1A / 100) / 100) % 10;
		w[2] = ((c_out1V * c_out1A / 100) / 10) % 10;
		w[3] = (c_out1V * c_out1A / 100) % 10;
		// ��ʾ��1�͵�2��
// 		for(k=0;k<2;k++) {
		if(cnt < 2) {
				k = cnt;
				// ����1Ϊ���ָ�������
				for(i=0;i<11;i++) {
						if(n[0] == 0) {		// ���Ϊ����0ʱ��ʾ�ո�
								if(cur == 1)
										buf[i] = ~F11X21[14*33+i+(k*11)];
								else
										buf[i] = F11X21[14*33+i+(k*11)];
						} else {		// ���Ϊ��Ϊ0ʱ������ʾ
								if(cur == 1)
										buf[i] = ~F11X21[n[0]*33+i+(k*11)];
								else
										buf[i] = F11X21[n[0]*33+i+(k*11)];
						}
				}
				// ����2Ϊ���ָ�������
				for(i=0;i<11;i++) {
						if(cur == 1)
								buf[11+i] = ~F11X21[n[1]*33+i+(k*11)];
						else
								buf[11+i] = F11X21[n[1]*33+i+(k*11)];
				}
				// ����3Ϊ . ��������
				for(i=0;i<11;i++) {
						buf[22+i] = F11X21[10*33+i+(k*11)];
				}
				// ����4Ϊ���ָ�������
				for(i=0;i<11;i++) {
						if(cur == 2)
								buf[33+i] = ~F11X21[n[2]*33+i+(k*11)];
						else
								buf[33+i] = F11X21[n[2]*33+i+(k*11)];
				}
				// ����5Ϊ���ָ�������
				for(i=0;i<11;i++) {
						if(cur == 2)
								buf[44+i] = ~F11X21[n[3]*33+i+(k*11)];
						else
								buf[44+i] = F11X21[n[3]*33+i+(k*11)];
				}
				// ����6Ϊ V ��������
				for(i=0;i<11;i++) {
						buf[55+i] = F11X21[11*33+i+(k*11)];
				}
				OLED_Set_Pos(x,y+k);	
				for(j=0;j<66;j++) {
						OLED_WR_Byte(buf[j],OLED_DATA);
				}
		}
		// ��ʾ������
		if(cnt == 2) {
				// ����1Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[i] = 0;
						if(n[0] == 0) {		// ���Ϊ����0ʱ��ʾ�ո�
								if(cur == 1)
										buf[i] |= 0x1F & (~F11X21[14*33+i+22]);
								else 
										buf[i] |= 0x1F & F11X21[14*33+i+22];
						} else {		// ���Ϊ��Ϊ0ʱ������ʾ
								if(cur == 1)
										buf[i] |= 0x1F & (~F11X21[n[0]*33+i+22]);
								else
										buf[i] |= 0x1F & F11X21[n[0]*33+i+22];
						}
						if(a[0] == 0) {		// ���Ϊ����0ʱ��ʾ�ո�
								if(cur == 3)
										buf[i] |= (0x07 & (~F11X21[14*33+i])) << 5;
								else 
										buf[i] |= (0x07 & F11X21[14*33+i]) << 5;
						} else {		// ���Ϊ��Ϊ0ʱ������ʾ
								if(cur == 3)
										buf[i] |= (0x07 & (~F11X21[a[0]*33+i])) << 5;
								else
										buf[i] |= (0x07 & F11X21[a[0]*33+i]) << 5;
						}
				}
				// ����2Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[11+i] = 0;
						if(cur == 1)
								buf[11+i] |= 0x1F & (~F11X21[n[1]*33+i+22]);
						else
								buf[11+i] |= 0x1F & F11X21[n[1]*33+i+22];
						if(cur == 3)
								buf[11+i] |= (0x07 & (~F11X21[a[1]*33+i])) << 5;
						else 
								buf[11+i] |= (0x07 & F11X21[a[1]*33+i]) << 5;
				}
				// ����3Ϊ . ��������
				for(i=0;i<11;i++) {
						buf[22+i] = 0;
						buf[22+i] |= 0x1F & F11X21[10*33+i+22];
						buf[22+i] |= (0x07 & F11X21[10*33+i]) << 5;
				}
				// ����4Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[33+i] = 0;
						if(cur == 2)
								buf[33+i] |= 0x1F & (~F11X21[n[2]*33+i+22]);
						else 
								buf[33+i] |= 0x1F & F11X21[n[2]*33+i+22];
						if(cur == 4)
								buf[33+i] |= (0x07 & (~F11X21[a[2]*33+i])) << 5;
						else 
								buf[33+i] |= (0x07 & F11X21[a[2]*33+i]) << 5;
				}
				// ����5Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[44+i] = 0;
						if(cur == 2)
								buf[44+i] |= 0x1F & (~F11X21[n[3]*33+i+22]);
						else 
								buf[44+i] |= 0x1F & F11X21[n[3]*33+i+22];
						if(cur == 4)
								buf[44+i] |= (0x07 & (~F11X21[a[3]*33+i])) << 5;
						else 
								buf[44+i] |= (0x07 & F11X21[a[3]*33+i]) << 5;
				}
				// ����6Ϊ V ��������
				for(i=0;i<11;i++) {
						buf[55+i] = 0;
						buf[55+i] |= 0x1F & F11X21[11*33+i+22];
						buf[55+i] |= (0x07 & F11X21[12*33+i]) << 5;
				}
				OLED_Set_Pos(x,y+2);	
				for(j=0;j<66;j++) {
						OLED_WR_Byte(buf[j],OLED_DATA);
				}
		}
		// ��ʾ��4�͵�5��
		if((cnt >= 3) && (cnt <= 4)) {
				k = cnt - 3;
				// ����1Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[i] = 0;
						if(a[0] == 0) {		// ���Ϊ����0ʱ��ʾ�ո�
								if(cur == 3) {
										buf[i] |= (0xF8 & (~F11X21[14*33+i+(k*11)])) >> 3;
										buf[i] |= (0x07 & (~F11X21[14*33+i+11+(k*11)])) << 5;
								} else {
										buf[i] |= (0xF8 & F11X21[14*33+i+(k*11)]) >> 3;
										buf[i] |= (0x07 & F11X21[14*33+i+11+(k*11)]) << 5;
								}
						} else {		// ���Ϊ��Ϊ0ʱ������ʾ
								if(cur == 3) {
										buf[i] |= (0xF8 & (~F11X21[a[0]*33+i+(k*11)])) >> 3;
										buf[i] |= (0x07 & (~F11X21[a[0]*33+i+11+(k*11)])) << 5;
								} else {
										buf[i] |= (0xF8 & F11X21[a[0]*33+i+(k*11)]) >> 3;
										buf[i] |= (0x07 & F11X21[a[0]*33+i+11+(k*11)]) << 5;
								}
						}
				}
				// ����2Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[11+i] = 0;
						if(cur == 3) {
								buf[11+i] |= (0xF8 & (~F11X21[a[1]*33+i+(k*11)])) >> 3;
								buf[11+i] |= (0x07 & (~F11X21[a[1]*33+i+11+(k*11)])) << 5;
						} else {
								buf[11+i] |= (0xF8 & F11X21[a[1]*33+i+(k*11)]) >> 3;
								buf[11+i] |= (0x07 & F11X21[a[1]*33+i+11+(k*11)]) << 5;
						}
				}
				// ����3Ϊ . ��������
				for(i=0;i<11;i++) {
						buf[22+i] = 0;
						buf[22+i] |= (0xF8 & F11X21[10*33+i+(k*11)]) >> 3;
						buf[22+i] |= (0x07 & F11X21[10*33+i+11+(k*11)]) << 5;
				}
				// ����4Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[33+i] = 0;
						if(cur == 4) {
								buf[33+i] |= (0xF8 & (~F11X21[a[2]*33+i+(k*11)])) >> 3;
								buf[33+i] |= (0x07 & (~F11X21[a[2]*33+i+11+(k*11)])) << 5;
						} else {
								buf[33+i] |= (0xF8 & F11X21[a[2]*33+i+(k*11)]) >> 3;
								buf[33+i] |= (0x07 & F11X21[a[2]*33+i+11+(k*11)]) << 5;
						}
				}
				// ����5Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[44+i] = 0;
						if(cur == 4) {
								buf[44+i] |= (0xF8 & (~F11X21[a[3]*33+i+(k*11)])) >> 3;
								buf[44+i] |= (0x07 & (~F11X21[a[3]*33+i+11+(k*11)])) << 5;
						} else {
								buf[44+i] |= (0xF8 & F11X21[a[3]*33+i+(k*11)]) >> 3;
								buf[44+i] |= (0x07 & F11X21[a[3]*33+i+11+(k*11)]) << 5;
						}
				}
				// ����6Ϊ A ��������
				for(i=0;i<11;i++) {
						buf[55+i] = 0;
						buf[55+i] |= (0xF8 & F11X21[12*33+i+(k*11)]) >> 3;
						buf[55+i] |= (0x07 & F11X21[12*33+i+11+(k*11)]) << 5;
				}
				OLED_Set_Pos(x,y+3+k);	
				for(j=0;j<66;j++) {
						OLED_WR_Byte(buf[j],OLED_DATA);
				}
		}
		// ��ʾ��6��
		if(cnt == 5) {
				// ����1Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[i] = 0;
						if(a[0] == 0) {		// ���Ϊ����0ʱ��ʾ�ո�
								if(cur == 3) {
										buf[i] |= (0x18 & (~F11X21[14*33+i+22])) >> 3;
								} else {
										buf[i] |= (0x18 & F11X21[14*33+i+22]) >> 3;
								}
						} else {		// ���Ϊ��Ϊ0ʱ������ʾ
								if(cur == 3) {
										buf[i] |= (0x18 & (~F11X21[a[0]*33+i+22])) >> 3;
								} else {
										buf[i] |= (0x18 & F11X21[a[0]*33+i+22]) >> 3;
								}
						}
						if(w[0] == 0) {		// ���Ϊ����0ʱ��ʾ�ո�
								buf[i] |= (0x3F & F11X21[14*33+i]) << 2;
						} else {		// ���Ϊ��Ϊ0ʱ������ʾ
								buf[i] |= (0x3F & F11X21[w[0]*33+i]) << 2;
						}
				}
				// ����2Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[11+i] = 0;
						if(cur == 3) {
								buf[11+i] |= (0x18 & (~F11X21[a[1]*33+i+22])) >> 3;
						} else {
								buf[11+i] |= (0x18 & F11X21[a[1]*33+i+22]) >> 3;
						}
						buf[11+i] |= (0x3F & F11X21[w[1]*33+i]) << 2;
				}
				// ����3Ϊ . ��������
				for(i=0;i<11;i++) {
						buf[22+i] = 0;
						buf[22+i] |= (0x18 & F11X21[10*33+i+22]) >> 3;
						buf[22+i] |= (0x3F & F11X21[10*33+i]) << 2;
				}
				// ����4Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[33+i] = 0;
						if(cur == 4) {
								buf[33+i] |= (0x18 & (~F11X21[a[2]*33+i+22])) >> 3;
						} else {
								buf[33+i] |= (0x18 & F11X21[a[2]*33+i+22]) >> 3;
						}
						buf[33+i] |= (0x3F & F11X21[w[2]*33+i]) << 2;
				}
				// ����5Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[44+i] = 0;
						if(cur == 4) {
								buf[44+i] |= (0x18 & (~F11X21[a[3]*33+i+22])) >> 3;
						} else {
								buf[44+i] |= (0x18 & F11X21[a[3]*33+i+22]) >> 3;
						}
						buf[44+i] |= (0x3F & F11X21[w[3]*33+i]) << 2;
				}
				// ����6Ϊ A �� W ��������
				for(i=0;i<11;i++) {
						buf[55+i] = 0;
						buf[55+i] |= (0x18 & F11X21[12*33+i+22]) >> 3;
						buf[55+i] |= (0x3F & F11X21[13*33+i]) << 2;
				}
				OLED_Set_Pos(x,y+5);	
				for(j=0;j<66;j++) {
						OLED_WR_Byte(buf[j],OLED_DATA);
				}
		}
		
		// ��ʾ��7�к͵�8��
		if((cnt >= 6) && (cnt <= 7)) {
				k = cnt - 6;
				// ����1Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[i] = 0;
						if(w[0] == 0) {		// ���Ϊ����0ʱ��ʾ�ո�
								buf[i] |= (0xC0 & F11X21[14*33+i+(k*11)]) >> 6;
								buf[i] |= (0x3F & F11X21[14*33+i+11+(k*11)]) << 2;
						} else {		// ���Ϊ��Ϊ0ʱ������ʾ
								buf[i] |= (0xC0 & F11X21[w[0]*33+i+(k*11)]) >> 6;
								buf[i] |= (0x3F & F11X21[w[0]*33+i+11+(k*11)]) << 2;
						}
				}
				// ����2Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[11+i] = 0;
						buf[11+i] |= (0xC0 & F11X21[w[1]*33+i+(k*11)]) >> 6;
						buf[11+i] |= (0x3F & F11X21[w[1]*33+i+11+(k*11)]) << 2;
				}
				// ����3Ϊ . ��������
				for(i=0;i<11;i++) {
						buf[22+i] = 0;
						buf[22+i] |= (0xC0 & F11X21[10*33+i+(k*11)]) >> 6;
						buf[22+i] |= (0x3F & F11X21[10*33+i+11+(k*11)]) << 2;
				}
				// ����4Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[33+i] = 0;
						buf[33+i] |= (0xC0 & F11X21[w[2]*33+i+(k*11)]) >> 6;
						buf[33+i] |= (0x3F & F11X21[w[2]*33+i+11+(k*11)]) << 2;
				}
				// ����5Ϊ���ָ�������
				for(i=0;i<11;i++) {
						buf[44+i] = 0;
						buf[44+i] |= (0xC0 & F11X21[w[3]*33+i+(k*11)]) >> 6;
						buf[44+i] |= (0x3F & F11X21[w[3]*33+i+11+(k*11)]) << 2;
				}
				// ����6Ϊ W ��������
				for(i=0;i<11;i++) {
						buf[55+i] = 0;
						buf[55+i] |= (0xC0 & F11X21[13*33+i+(k*11)]) >> 6;
						buf[55+i] |= (0x3F & F11X21[13*33+i+11+(k*11)]) << 2;
				}
				OLED_Set_Pos(x,y+6+k);	
				for(j=0;j<66;j++) {
						OLED_WR_Byte(buf[j],OLED_DATA);
				}
		}
}

void LEDLoop(void) {
	if(SysTime) {

	}
}
