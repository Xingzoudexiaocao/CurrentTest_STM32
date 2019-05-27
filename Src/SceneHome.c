
/* THIS SAMPLE CODE IS PROVIDED AS IS AND IS SUBJECT TO ALTERATIONS. FUJITSU */
/* MICROELECTRONICS ACCEPTS NO RESPONSIBILITY OR LIABILITY FOR ANY ERRORS OR */
/* ELIGIBILITY FOR ANY PURPOSES.                                             */
/*            (C) Fujitsu Microelectronics (Shanghai) Co., LTD.              */
/*---------------------------------------------------------------------------
  SYSTEM.C
  - description
  - System init.watch dog init.timebase init...
-----------------------------------------------------------------------------*/

#include "main.h"
#include "system.h"
#include "SceneHome.h"
#include "LED.h"
#include "KeyBoard_Drv.h"
#include "Flash.h"

bool isLock = false;		// 是否锁定
bool isOut1 = false;		// 输出1是否可用
bool isOut2 = false;		// 输出2是否可用
u8 current = 0;	// 可调节选项

/*----------------------------- SUB ROUTINES --------------------------------*/
void SceneHomeInit(void)
{
	OLED_Display_On();
// 	OLED_ShowString(0,0," OUT1: |",1);
// 	OLED_ShowString(0,2,"25.00V |",1);
// 	OLED_ShowString(0,4," 1.99A |",1);
// 	OLED_ShowString(0,6,"49.75W |",1);
	OLED_ShowLine();
	OLED_ShowString(72,0,"Mod:CV",1,0);
	OLED_ShowString(72,2,"Normal",1,1);
	OLED_ShowString(72,4,"10.00V",1,0);
	OLED_ShowString(72,6," 0.88A",1,0);

}

u32 getPower(u32 V,u32 A)
{
		return V*A/100;
}

u8 isReverse(u8 buf)
{
		if(current == buf)
			return 0;
		return 1;
}

void controlOutOnOff(void)
{
		if(KeyShort_OnOff1)
		{
			ClearKey_OnOff1;
			if(isOut1)
				isOut1 = false;
			else
				isOut1 = true;
		}
		if(KeyShort_OnOff2)
		{
			ClearKey_OnOff2;
			if(isOut2)
				isOut2 = false;
			else
				isOut2 = true;
		}
}

u32 ChangeNum(u32 buf, u32 buf2)
{
		if(rotateCodeLeft)
		{
				rotateCodeLeft--;
				buf += buf2;
		}
		if(rotateCodeRight)
		{
				rotateCodeRight--;
				buf -= buf2;
		}
		return buf;
}

void controlSetCurrent(void)
{
		if(KeyShort_Current)
		{
			ClearKey_Current;
			if((current == 0) && (isOut1 == true))
				current = 1;
			else
				current = 0;
		}
		if(current)
		{
				if(KeyShort_Left)
				{
					ClearKey_Left;
					if(current > 1)
						current--;
				}
				if(KeyShort_Right)
				{
					ClearKey_Right;
					if(current < 4)
						current++;
				}
				switch(current)
				{
					case 1:	MemoryData.out1_V = ChangeNum(MemoryData.out1_V, 100); break;
					case 2:	MemoryData.out1_V = ChangeNum(MemoryData.out1_V, 1); break;
					case 3:	MemoryData.out1_A = ChangeNum(MemoryData.out1_A, 100); break;
					case 4:	MemoryData.out1_A = ChangeNum(MemoryData.out1_A, 1); break;
					default: break;
				}
		}
	
}

void controlDisplay(void)
{
		static u8 cntN = 0;

		if(isOut1)
		{
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8, GPIO_PIN_RESET);
				OLED_Show_Out1(cntN,0,0,MemoryData.out1_V,MemoryData.out1_A,current);
		}
		else
		{
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8, GPIO_PIN_SET);
				OLED_Show_Out1(cntN,0,0,0,0,current);
				current = 0;
		}
		
		cntN++;
		if(cntN >= 8)
				cntN = 0;

		if(isOut2)
		{
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12, GPIO_PIN_RESET);
				OLED_ShowNum(72,4,MemoryData.out2_V/100,2,16,' ',1);	// 显示电压2  
				OLED_ShowNum(96,4,MemoryData.out2_V%100,2,16,'0',1);	// 显示电压2  
				OLED_ShowNum(72,6,MemoryData.out2_A/100,2,16,' ',1);	// 显示电流2 
				OLED_ShowNum(96,6,MemoryData.out2_A%100,2,16,'0',1);	// 显示电流2   
		}
		else
		{
				HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12, GPIO_PIN_SET);
				OLED_ShowNum(72,4,0,2,16,' ',1);	// 显示电压2   
				OLED_ShowNum(96,4,0,2,16,'0',1);	// 显示电压2   
				OLED_ShowNum(72,6,0,2,16,' ',1);	// 显示电流2  
				OLED_ShowNum(96,6,0,2,16,'0',1);	// 显示电流2  
		}
	
}

void SceneHomeLoop(void)
{
		if(SysTime)
		{
				if(current == 0)
				{
						rotateCodeLeft = 0;
						rotateCodeRight = 0;
				}
				if(KeyLong_Lock)
				{
					ClearKey_Lock;
					if(isLock)
						isLock = false;
					else
						isLock = true;
				}
				if(!isLock)
				{
						OLED_ShowString(72,2,"Normal",1,1);
						controlOutOnOff();
						controlSetCurrent();
				}
				else
				{
						current = 0;
						OLED_ShowString(72,2," Lock ",1,1);
				}
				ClearAllKey
				
				controlDisplay();

		}
}
