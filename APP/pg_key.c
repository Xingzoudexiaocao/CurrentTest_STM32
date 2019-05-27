/***********************************************************************
文件名称：pg_key.c
功	  能：
编写时间：2018.09.18
编 写 人：wuliu
注    意：
***********************************************************************/

#include "includes.h"

/*
**********************************************************************************************************
											变量
**********************************************************************************************************
*/
static u8 ButtonAction1;    //按钮1动作
static u8 ButtonAction2;    //按钮2动作
static u8 ButtonAction3;    //按钮3动作
static u8 ButtonAction4;    //按钮4动作
static u8 ButtonAction5;    //按钮5动作
static u8 ButtonAction6;    //按钮6动作
static u8 ButtonAction7;    //按钮7动作
static u8 ButtonAction8;    //按钮8动作

u8 TestCompleteRf; //测试完成状态切换到空闲状态的标志
u8 Btn3LongPressFlag;
u8 Btn4LongPressFlag;
u8 Btn8LongPressFlag;

WIN last_win; //上次被选中的窗口
WIN cur_win;
u8 win_num = 4;

u8 SettingFlag; //被设置了

u8 cur_switch; //分钟和秒选项

u8 rrecord;
u8 WhichRecord;
u8 RecordNeedRefresh;

u8 CommNeedReflash = 0;

/*
**********************************************************************************************************
										   函数声明
**********************************************************************************************************
*/
static void DealKey1(void);
static void DealKey2(void);
static void DealKey3(void);
static void DealKey4(void);
static void DealKey5(void);
static void DealKey6(void);
static void DealKey7(void);
static void DealKey8(void);
static void ReadKeyStatus(void);
static void DealKeyLongPress(void);
static void UpdateWinNumber(void);

/*
**********************************************************************************************
*                               void switch_min_sec(void)
* Description: 时间设置分钟跟秒的选项切换
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void switch_min_sec(void)
{
	if(cur_switch == MIN)
	{
		//切换到秒
		cur_switch = SEC;
	}
	else
	{
		//切换到分钟
		cur_switch = MIN;
	}
}

/*
**********************************************************************************************
*                       void mode_option_change(bool mode)
* Description: 模式窗口选项改变(向上加或向下减)
* Arguments  : mode:   true:向上加 false:向下减
* Returns    :            
**********************************************************************************************
*/
void mode_option_change(bool mode)
{
	if(mode)
	{
		if(TestMode == 0x02)
		{
			TestMode = 0x08;
		}
		else
		{
			TestMode--;
		}
	}
	else
	{
		if(TestMode == 0x08)
		{
			TestMode = 0x02;
		}
		else
		{
			TestMode++;
		}
	}
	
	if(TestMode == 0x02)
	{
		win_num = 3;
	}
	else if(TestMode == 0x03)
	{
		win_num = 2;
	}
	else if(TestMode == 0x04)
	{
		win_num = 3;
	}
	else if(TestMode == 0x05)
	{
		win_num = 4;
	}
	else if(TestMode == 0x06)
	{
		win_num = 4;
	}
	else if(TestMode == 0x07)
	{
		win_num = 1;
	}
	else if(TestMode == 0x08)
	{
		win_num = 1;
	}
	
}

/*
**********************************************************************************************
*                       void vlevel_option_change(bool mode)
* Description: 电压等级窗口选项改变(向上加或向下减)
* Arguments  : mode:   true:向上加 false:向下减
* Returns    :            
**********************************************************************************************
*/
void vlevel_option_change(bool mode)
{
	if(mode)
	{
		if(TestVol == 0)
		{
			TestVol = 3;
		}
		else
		{
			TestVol--;
		}
	}
	else
	{
		if(TestVol == 3)
		{
			TestVol = 0;
		}
		else
		{
			TestVol++;
		}
	}
}

/*
**********************************************************************************************
*                       void time1_option_change(bool mode)
* Description: 时间1窗口选项改变(向上加或向下减)
* Arguments  : mode:   true:向上加 false:向下减
* Returns    :            
**********************************************************************************************
*/
void time1_option_change(bool mode)
{ 	
	if(mode)
	{
		if(TestMode == 0x04)
		{
			if(CompR == 0)
			{
				CompR = 100000;
			}
			else
			{
				CompR--;
			}
		}
		else
		{
			if(cur_switch == MIN)
			{
				if(TestMode == 0x02)
				{
					if(TimeSS/60 == 0)
					{
						TimeSS += (60*59);
					}
					else
					{
						TimeSS -= 60;
					}
				}
				else if(TestMode == 0x05)
				{
					if(XSB_Time1/60 == 0)
					{
						XSB_Time1 += (60*59);
					}
					else
					{
						XSB_Time1 -= 60;
					}
				}
				else if(TestMode == 0x06)
				{
					if(JH_Time1/60 == 0)
					{
						JH_Time1 += (60*59);
					}
					else
					{
						JH_Time1 -= 60;
					}
				}
				else
				{

				}
			}
			else
			{
				if(TestMode == 0x02)
				{
					if(TimeSS%60 == 0)
					{
						TimeSS += 59;
					}
					else
					{
						TimeSS--;
					}
				}
				else if(TestMode == 0x05)
				{
					if(XSB_Time1/60 == 0)
					{
						XSB_Time1 += 59;
					}
					else
					{
						XSB_Time1 --;
					}
				}
				else if(TestMode == 0x06)
				{
					if(JH_Time1/60 == 0)
					{
						JH_Time1 +=  59;
					}
					else
					{
						JH_Time1 --;
					}
				}
				else
				{

				}
			}
		}
	}
	else
	{
		if(TestMode == 0x04)
		{
			if(CompR == 100000)
			{
				CompR = 0;
			}
			else
			{
				CompR++;
			}
		}
		else
		{
			if(cur_switch == MIN)
			{
				if(TestMode == 0x02)
				{
					if(TimeSS/60 == 59)
					{
						TimeSS -= (60*59);
					}
					else
					{
						TimeSS += 60;
					}
				}
				else if(TestMode == 0x05)
				{
					if(XSB_Time1/60 == 59)
					{
						XSB_Time1 -= (60*59);
					}
					else
					{
						XSB_Time1 += 60;
					}
				}
				else if(TestMode == 0x06)
				{
					if(JH_Time1/60 == 59)
					{
						JH_Time1 -= (60*59);
					}
					else
					{
						JH_Time1 += 60;
					}
				}
				else
				{

				}
			}
			else
			{
				if(TestMode == 0x02)
				{
					if(TimeSS%60 == 59)
					{
						TimeSS -= 59;
					}
					else
					{
						TimeSS++;
					}
				}
				else if(TestMode == 0x05)
				{
					if(XSB_Time1/60 == 59)
					{
						XSB_Time1 -= 59;
					}
					else
					{
						XSB_Time1++;
					}
				}
				else if(TestMode == 0x06)
				{
					if(JH_Time1/60 == 59)
					{
						JH_Time1 -= 59;
					}
					else
					{
						JH_Time1++;
					}
				}
				else
				{

				}
			} 
		}	
	}	
}

/*
**********************************************************************************************
*                       void time2_option_change(bool mode)
* Description: 时间2窗口选项改变(向上加或向下减)
* Arguments  : mode:   true:向上加 false:向下减
* Returns    :            
**********************************************************************************************
*/
void time2_option_change(bool mode)
{
	if(mode)
	{ 
		if(cur_switch == MIN)
		{
			if(TestMode == 0x05)
			{
				if(XSB_Time2/60 == 0)
				{
					XSB_Time2 += (60*59);
				}
				else
				{
					XSB_Time2 -= 60;
				}
			}
			else if(TestMode == 0x06)
			{
				if(JH_Time2/60 == 0)
				{
					JH_Time2 += (60*59);
				}
				else
				{
					JH_Time2 -= 60;
				}
			}
			else
			{

			}
		}
		else
		{
			if(TestMode == 0x05)
			{
				if(XSB_Time2/60 == 0)
				{
					XSB_Time2 += 59;
				}
				else
				{
					XSB_Time2 --;
				}
			}
			else if(TestMode == 0x06)
			{
				if(JH_Time2/60 == 0)
				{
					JH_Time2 +=  59;
				}
				else
				{
					JH_Time2 --;
				}
			}
			else
			{

			}
		}			
	}
	else
	{
		if(cur_switch == MIN)
		{
			if(TestMode == 0x05)
			{
				if(XSB_Time2/60 == 59)
				{
					XSB_Time2 -= (60*59);
				}
				else
				{
					XSB_Time2 += 60;
				}
			}
			else if(TestMode == 0x06)
			{
				if(JH_Time2/60 == 59)
				{
					JH_Time2 -= (60*59);
				}
				else
				{
					JH_Time2 += 60;
				}
			}
			else
			{

			}
		}
		else
		{
			if(TestMode == 0x05)
			{
				if(XSB_Time2/60 == 59)
				{
					XSB_Time2 -= 59;
				}
				else
				{
					XSB_Time2++;
				}
			}
			else if(TestMode == 0x06)
			{
				if(JH_Time2/60 == 59)
				{
					JH_Time2 -= 59;
				}
				else
				{
					JH_Time2++;
				}
			}
			else
			{

			}
		} 			
	}	
}
/*
**********************************************************************************************
*                       void cur_option_change(win re_win, bool mode)
* Description: 当前选项改变(向上加或向下减)
* Arguments  : re_min：当前的在哪个窗口
*			 : mode:   true:向上加 false:向下减
* Returns    :            
**********************************************************************************************
*/
void cur_option_change(WIN re_win, bool mode)
{
	switch(re_win)
	{
		case NONE_WIN:
		{	
		}break;
		case MODE_WIN:
		{
			mode_option_change(mode);
		}break;
		case VLEVEL_WIN:
		{
			vlevel_option_change(mode);
		}break;
		case TIME1_WIN:
		{
			time1_option_change(mode);
		}break;
		case TIME2_WIN:
		{
			time2_option_change(mode);
		}break;
		default:
		{}break;
	}
}
/*
**********************************************************************************************
*                                   void SaveOneRecord(void) 
* Description: 
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void SaveOneRecord(void)
{
	CreatOneRecord();
}


/*
**********************************************************************************************
*                                   void KeyPoll(void)
* Description: 按键处理
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void pg_key_poll(void)
{
	ReadKeyStatus(); /* 读按键状态 */
	
	DealKeyLongPress(); /* 按键长按状态处理 */
	
	DealKey1(); /*处理按键1 */ 
	DealKey2(); /*处理按键2 */
	DealKey3(); /*处理按键3 */
	DealKey4(); /*处理按键4 */
	DealKey5(); /*处理按键5 */
	DealKey6(); /*处理按键6 */
	DealKey7(); /*处理按键7 */
	DealKey8(); /*处理按键8 */
	
	UpdateWinNumber();
	
}
/*
**********************************************************************************************
*                                   static void UpdateWinNumber(void)
* Description: 更新设置窗口个数
*							 不同模式下设置窗口个数是不一样的
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void UpdateWinNumber(void)
{
	if(TestMode == 0x02)
	{
		win_num = 3;
	}
	else if(TestMode == 0x03)
	{
		win_num = 2;
	}
	else if(TestMode == 0x04)
	{
		win_num = 3;
	}
	else if(TestMode == 0x05)
	{
		win_num = 4;
	}
	else if(TestMode == 0x06)
	{
		win_num = 4;
	}
	else if(TestMode == 0x07)
	{
		win_num = 1;
	}
	else if(TestMode == 0x08)
	{
		win_num = 1;
	}
}
/*
**********************************************************************************************
*                                   static void DealKeyLongPress(void)
* Description: 按键长按状态处理
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKeyLongPress(void)
{
	if(Btn3LongPressFlag)
	{
		if((ButtonAction3 == BUTTON_WAS_RELEASED) || (ButtonAction3 == BUTTON_HELD_LONG_RELEASED))
		{
			Btn3LongPressFlag = 0;
		}
	}
	/*---------------------------------------------------------------------------------------------*/
	if(Btn4LongPressFlag)
	{
		if((ButtonAction4 == BUTTON_WAS_RELEASED) || (ButtonAction4 == BUTTON_HELD_LONG_RELEASED))
		{
			Btn4LongPressFlag = 0;
		}
	}
	/*---------------------------------------------------------------------------------------------*/
	if(Btn8LongPressFlag)
	{
		if((ButtonAction8 == BUTTON_WAS_RELEASED) || (ButtonAction8 == BUTTON_HELD_LONG_RELEASED))
		{
			Btn8LongPressFlag = 0;
		}
	}
}
/*
**********************************************************************************************
*                                   static void ReadKeyStatus(void)
* Description: 读按键状态
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void ReadKeyStatus(void)
{
	ButtonAction1=Button_Read(BUTTON1);
	ButtonAction2=Button_Read(BUTTON2);	
	ButtonAction3=Button_Read(BUTTON3);	
	ButtonAction4=Button_Read(BUTTON4);	
	ButtonAction5=Button_Read(BUTTON5);
	ButtonAction6=Button_Read(BUTTON6);	
	ButtonAction7=Button_Read(BUTTON7);	
	ButtonAction8=Button_Read(BUTTON8);
}
/*
**********************************************************************************************
*                                   static void DealKey1(void)
* Description: 按键1处理(设置框跳转按钮)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey1(void)
{
	/* 蓝牙模式或者蓝牙请求连接中,测试中,阅读记录时该按键都不起作用 */
	if((Communicate == BLUETOOTH) || (CommFlag == 1) || (TestState == 1) || (TestState == 3))
	{
		return;
	}
	
	/* 短按跳转到下个框 */
	if(ButtonAction1 == BUTTON_WAS_PRESSED) /* 设置框跳转按钮 */
	{
		last_win = cur_win; /* 保存上次被选中的框 */

		if(cur_win == win_num)
		{
			cur_win = MODE_WIN;
		}
		else
		{
			cur_win++;
		}
		
		SettingFlag = 1;	/* 设置框被改变 */
		
		/* 测试结束后切换到空闲模式 */
		if(TestState == 2) 
		{
			TestStateSet(0);
			TestCompleteRf = 1;
		}
	}
}
/*
**********************************************************************************************
*                                   static void DealKey2(void)
* Description: 按键2处理(时间分钟和秒切换按钮)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey2(void)
{
	/* 蓝牙模式或者蓝牙请求连接中,测试中,阅读记录时该按键都不起作用 */
	if((Communicate == BLUETOOTH) || (CommFlag == 1) || (TestState == 1) || (TestState == 3))
	{
		return;
	}
	
	/* 短按分钟和秒之间切换 */
	if(ButtonAction2 == BUTTON_WAS_PRESSED)
	{
		if(cur_win == TIME1_WIN || cur_win == TIME2_WIN)
		{
			if((TestMode == 2) || (TestMode == 5) || (TestMode == 6))
			{
				switch_min_sec(); /* 切换分钟和秒 */
				SettingFlag = 1; /* 设置被改变了 */
			}
		}
		
		/* 测试结束后切换到空闲模式 */
		if(TestState == 2) 
		{
			TestStateSet(0);
			TestCompleteRf = 1;
		}
	}
}
/*
**********************************************************************************************
*                                   static void DealKey3(void)
* Description: 按键3处理(记录保存/阅读按钮)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey3(void)
{
	/* 蓝牙模式或者蓝牙请求连接中时该按键都不起作用 */
	if((Communicate == BLUETOOTH) || (CommFlag == 1))
	{
		return;
	}
	
	if(ButtonAction3 == BUTTON_WAS_PRESSED)
	{
		if(TestState == 1) /* 测试过程中 */
		{
			SaveOneRecord(); /* 保存记录 */	
		}
	}
	
	if((ButtonAction3 == BUTTON_IS_HELD) && (Btn3LongPressFlag == 0))
	{
		if(TestState == 0)
		{
			WhichRecord = 1;		/* 从第一个记录看起 */	
			RecordNeedRefresh = 1;	/* 记录需刷新 */
			TestStateSet(3); 			/* 进入读记录状态 */	
			if(cur_win != NONE_WIN)
			{
				last_win = cur_win; /* 保存上次被选中的框 */
				cur_win = NONE_WIN;
				SettingFlag = 1; /* 设置被改变了 */
			}
		}
		else if(TestState == 3)
		{
			RecordNeedRefresh = 1;	/* 记录需刷新 */
			TestStateSet(0); /* 退出读记录状态 */
		}
		
		Btn3LongPressFlag = 1;
		
		/* 测试结束后切换到空闲模式 */
		if(TestState == 2) 
		{
			TestStateSet(0); 
			TestCompleteRf = 1;
		}
	}		
}
/*
**********************************************************************************************
*                                   static void DealKey3(void)
* Description: 按键4处理(蓝牙/本地按钮)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey4(void)
{
	/* 测试时该按键都不起作用 */
	if((TestState == 1) || (TestState == 3))
	{
		return;
	}
	
	if((ButtonAction4 == BUTTON_IS_HELD) && (Btn4LongPressFlag == 0))
	{
		if(CommFlag == 0)
		{
			BTStateSet(1); /* 请求连接 */
			CommNeedReflash = 1;
		}
		else 
		{
			BTStateSet(0); /* 退出连接 */
			CommNeedReflash = 1;
		}
		
		Btn4LongPressFlag = 1;
		
		/* 测试结束后切换到空闲模式 */
		if(TestState == 2) 
		{
			TestStateSet(0); 
			TestCompleteRf = 1;
		}
	}		
}
/*
**********************************************************************************************
*                                   static void DealKey5(void)
* Description: 按键5处理(向上按钮)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey5(void)
{
	/* 蓝牙模式,或者蓝牙请求连接中及测试时该按键都不起作用 */
	if((Communicate == BLUETOOTH) || (CommFlag == 1) || (TestState == 1))
	{
		return;
	}
	
	if(ButtonAction5 == BUTTON_WAS_PRESSED)
	{
		if(TestState == 3)
		{
			WhichRecord++;
			if(WhichRecord > Records)
			{
				WhichRecord = 1;
			}
			RecordNeedRefresh = 1;		/* 记录需刷新 */
		}
		else
		{
			cur_option_change(cur_win, true);
			SettingFlag = 1; /* 设置被改变了 */
			SaveDataFlag = 1; /* 保存设置参数 */
		}
		
		/* 测试结束后切换到空闲模式 */
		if(TestState == 2) 
		{
			TestStateSet(0);
			TestCompleteRf = 1;
		}
	}
}
/*
**********************************************************************************************
*                                   static void DealKey6(void)
* Description: 按键6处理(向下按钮)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey6(void)
{
	/* 蓝牙模式,或者蓝牙请求连接中及测试时该按键都不起作用 */
	if((Communicate == BLUETOOTH) || (CommFlag == 1) || (TestState == 1))
	{
		return;
	}
	
	if(ButtonAction6 == BUTTON_WAS_PRESSED)
	{
		if(TestState == 3)
		{
			WhichRecord--;
			if(WhichRecord < 1)
			{
				WhichRecord = Records;
			}
			RecordNeedRefresh = 1;		/* 记录需刷新 */
		}
		else
		{
			cur_option_change(cur_win, false);
			SettingFlag = 1; /* 设置被改变了 */
			SaveDataFlag = 1;  /* 保存设置参数 */
		}
		
		/* 测试结束后切换到空闲模式 */
		if(TestState == 2) 
		{
			TestStateSet(0);
			TestCompleteRf = 1;
		}
	}
}
/*
**********************************************************************************************
*                                   static void DealKey7(void)
* Description: 按键7处理(确认按钮)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey7(void)
{
	/* 蓝牙模式,或者蓝牙请求连接中及测试时该按键都不起作用 */
	if((Communicate == BLUETOOTH) || (CommFlag == 1) || (TestState == 1))
	{
		return;
	}
	
	if(ButtonAction7 == BUTTON_WAS_PRESSED)
	{
		if(TestState == 3)
		{
			RecordNeedRefresh = 1;		/* 记录需刷新 */
			TestStateSet(0); /* 退出读记录状态 */
		}
		else
		{
			if(cur_win != NONE_WIN)
			{
				last_win = cur_win; /* 保存上次被选中的框 */
				cur_win = NONE_WIN;
				SettingFlag = 1; /* 设置被改变了 */
			}
		}
		
		/* 测试结束后切换到空闲模式 */
		if(TestState == 2) 
		{
			TestStateSet(0);
			TestCompleteRf = 1;
		}
	}
}
/*
**********************************************************************************************
*                                   static void DealKey8(void)
* Description: 按键8处理(测试按钮)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey8(void)
{
	/* 蓝牙模式,读记录时该按键都不起作用 */
	if((CommFlag == 1) || (TestState == 3))
	{
		return;
	}
	
	if((ButtonAction8 == BUTTON_IS_HELD) && (Btn8LongPressFlag == 0))
	{
		if((Communicate == BLUETOOTH) || (TestState == 1))
		{
			StopTestSet(1);
		}
		else
		{
			StartTestSet(1); /* 请求启动测试 */
			if(cur_win != NONE_WIN)
			{
				last_win = cur_win; /* 保存上次被选中的框 */
				cur_win = NONE_WIN;
				SettingFlag = 1; /* 设置被改变了 */
			}
		}
		
		Btn8LongPressFlag = 1;
		
		/* 测试结束后切换到空闲模式 */
		if(TestState == 2) 
		{
			TestStateSet(0);
			TestCompleteRf = 1;
		}
	}		
}
///*
//**********************************************************************************************
//*                                   void pg_key_poll(void) 
//* Description: 
//* Arguments  : 
//* Returns    :            
//**********************************************************************************************
//*/
//void pg_key_poll(void) 
//{
//	u8 ButtonAction1;    //按钮1动作
//	u8 ButtonAction2;    //按钮2动作
//	u8 ButtonAction3;    //按钮3动作
//	u8 ButtonAction4;    //按钮4动作
//	u8 ButtonAction5;    //按钮5动作
//	u8 ButtonAction6;    //按钮6动作
//	u8 ButtonAction7;    //按钮7动作
//	u8 ButtonAction8;    //按钮8动作
//	
//	ButtonAction1=Button_Read(BUTTON1);
//	ButtonAction2=Button_Read(BUTTON2);	
//	ButtonAction3=Button_Read(BUTTON3);	
//	ButtonAction4=Button_Read(BUTTON4);	
//	ButtonAction5=Button_Read(BUTTON5);
//	ButtonAction6=Button_Read(BUTTON6);	
//	ButtonAction7=Button_Read(BUTTON7);	
//	ButtonAction8=Button_Read(BUTTON8);	
//	
//	if(Button3ReleaseFlag)
//	{
//		if((ButtonAction3 == BUTTON_WAS_RELEASED) || (ButtonAction3 == BUTTON_HELD_LONG_RELEASED))
//		{
//			Button3ReleaseFlag = 0;
//		}
//	}
//	
//	if(Button4ReleaseFlag)
//	{
//		if((ButtonAction4 == BUTTON_WAS_RELEASED) || (ButtonAction4 == BUTTON_HELD_LONG_RELEASED))
//		{
//			Button4ReleaseFlag = 0;
//		}
//	}

//	if(Button8ReleaseFlag)
//	{
//		if((ButtonAction8 == BUTTON_WAS_RELEASED) || (ButtonAction8 == BUTTON_HELD_LONG_RELEASED))
//		{
//			Button8ReleaseFlag = 0;
//		}
//	}

//	/*
//		如果是蓝牙操作
//	*/
//	if(Communicate == BLUETOOTH)
//	{
//		/*
//		蓝牙/本地按钮长按,强制切换到本地
//		*/
//		if((ButtonAction4 == BUTTON_IS_HELD) && (Button4ReleaseFlag == 0))
//		{
//			if(TestState == 0)  /* 在测试中不能切换到本地 */
//			{
//				Communicate = LOCAL;
//				CommFlag = 0; /* 退出连接 */
//				CommNeedReflash = 1;
//				Button4ReleaseFlag = 1;
//			}
//		}
//		
//		/*
//		测试按钮长按,强制停止测试
//		*/
//		if(ButtonAction8 == BUTTON_IS_HELD && (Button8ReleaseFlag == 0))
//		{
//			if(TestState == 1)
//			{
//				StopTestSet(1); /* 请求停止测试 */
//			}
//			
//			Button8ReleaseFlag = 1;
//		}
//	}
//	else
//	{
//		/*
//			如果是本地操作
//		*/
//		
//		if(TestState == 1) /* 测试中 */
//		{
//			if(ButtonAction3 == BUTTON_WAS_PRESSED)
//			{
//				SaveOneRecord(); /* 保存记录 */
//			}
//			/*
//			测试按钮长按,强制停止测试
//			*/
//			if(ButtonAction8 == BUTTON_IS_HELD && (Button8ReleaseFlag == 0))
//			{
//				if(TestState == 1)
//				{
//					StopTestSet(1); /* 请求停止测试 */
//				}
//				
//				Button8ReleaseFlag = 1;
//			}
//		}
//		else if(TestState == 0) 
//		{
//			/*
//			设置框跳转按钮
//			*/
//			if(ButtonAction1 == BUTTON_WAS_PRESSED)
//			{
//				last_win = cur_win; /* 保存上次被选中的框 */
//		
//				if(cur_win == win_num)
//				{
//					cur_win = MODE_WIN;
//				}
//				else
//				{
//					cur_win++;
//				}
//				
//				SettingFlag = 1;	/* 设置框被改变 */
//			}
//			
//			/*
//			时间分钟和秒切换按钮
//			*/
//			if(ButtonAction2 == BUTTON_WAS_PRESSED)
//			{
//				if(cur_win == TIME1_WIN || cur_win == TIME2_WIN)
//				{
//					if((TestMode == 2) || (TestMode == 5) || (TestMode == 6))
//					{
//						switch_min_sec(); /* 切换分钟和秒 */
//						SettingFlag = 1; /* 设置被改变了 */
//					}
//				}
//			}

//			/*
//			记录保存阅读按钮
//			*/
//			if(ButtonAction3 == BUTTON_WAS_PRESSED)
//			{
//				if(rrecord == 0)
//				{
//					rrecord = 1;				/* 正在读记录标志 */
//					WhichRecord = 1;			/* 从第一个记录看起 */
//				//	ReadRecord(WhichRecord);	
//					RecordNeedRefresh = 1;		/* 记录需刷新 */
//					
//					if(cur_win != NONE_WIN)
//					{
//						last_win = cur_win; /* 保存上次被选中的框 */
//						cur_win = NONE_WIN;
//						SettingFlag = 1; /* 设置被改变了 */
//					}
//				}
//			}

//			/*
//			蓝牙/本地
//			*/
//			if((ButtonAction4 == BUTTON_IS_HELD) && (Button4ReleaseFlag == 0))
//			{
//				Communicate = BLUETOOTH;
//				CommFlag = 1; /* 请求连接 */
//				CommNeedReflash = 1;
//				Button4ReleaseFlag = 1;
//			}

//			/*
//			向上按钮
//			*/
//			if(ButtonAction5 == BUTTON_WAS_PRESSED)
//			{
//				if(rrecord)
//				{
//					WhichRecord++;
//					if(WhichRecord > Records)
//					{
//						WhichRecord = 1;
//					}
//				//	ReadRecord(WhichRecord);
//					RecordNeedRefresh = 1;		/* 记录需刷新 */
//				}
//				else
//				{
//					cur_option_change(cur_win, true);
//					SettingFlag = 1; /* 设置被改变了 */
//				}
//			}

//			/*
//			向下按钮
//			*/
//			if(ButtonAction6 == BUTTON_WAS_PRESSED)
//			{
//				if(rrecord)
//				{
//					WhichRecord--;
//					if(WhichRecord < 1)
//					{
//						WhichRecord = Records;
//					}
//				//	ReadRecord(WhichRecord);
//					RecordNeedRefresh = 1;		/* 记录需刷新 */
//				}
//				else
//				{
//					cur_option_change(cur_win, false);
//					SettingFlag = 1; /* 设置被改变了 */
//				}	
//			}

//			/*
//			确认按钮
//			*/
//			if(ButtonAction7 == BUTTON_WAS_PRESSED)
//			{
//				if(cur_win != NONE_WIN)
//				{
//					last_win = cur_win; /* 保存上次被选中的框 */
//					cur_win = NONE_WIN;
//					SettingFlag = 1; /* 设置被改变了 */
//				}
//				if(rrecord) //正在读记录
//				{
//					rrecord = 0;
//				//	ExitReadRecord();
//					RecordNeedRefresh = 1;		/* 记录需刷新 */
//				}
//			}

//			/*
//			测试按钮
//			*/
//			if(ButtonAction8 == BUTTON_IS_HELD && (Button8ReleaseFlag == 0))
//			{
//				StartTestSet(1); /* 请求启动测试 */
//				
//				if(cur_win != NONE_WIN)
//				{
//					last_win = cur_win; /* 保存上次被选中的框 */
//					cur_win = NONE_WIN;
//					SettingFlag = 1; /* 设置被改变了 */
//				}
//				if(rrecord) //正在读记录
//				{ 
//					rrecord = 0;
//				//	ExitReadRecord();
//					RecordNeedRefresh = 1;		/* 记录需刷新 */
//				}
//				
//				Button8ReleaseFlag = 1;
//			}
//		}
//	}
//	
//	if(TestMode == 0x02)
//	{
//		win_num = 3;
//	}
//	else if(TestMode == 0x03)
//	{
//		win_num = 2;
//	}
//	else if(TestMode == 0x04)
//	{
//		win_num = 3;
//	}
//	else if(TestMode == 0x05)
//	{
//		win_num = 4;
//	}
//	else if(TestMode == 0x06)
//	{
//		win_num = 4;
//	}
//	else if(TestMode == 0x07)
//	{
//		win_num = 1;
//	}
//	else if(TestMode == 0x08)
//	{
//		win_num = 1;
//	}

//	
//	
//}
   




