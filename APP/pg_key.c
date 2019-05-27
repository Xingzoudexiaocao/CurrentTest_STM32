/***********************************************************************
�ļ����ƣ�pg_key.c
��	  �ܣ�
��дʱ�䣺2018.09.18
�� д �ˣ�wuliu
ע    �⣺
***********************************************************************/

#include "includes.h"

/*
**********************************************************************************************************
											����
**********************************************************************************************************
*/
static u8 ButtonAction1;    //��ť1����
static u8 ButtonAction2;    //��ť2����
static u8 ButtonAction3;    //��ť3����
static u8 ButtonAction4;    //��ť4����
static u8 ButtonAction5;    //��ť5����
static u8 ButtonAction6;    //��ť6����
static u8 ButtonAction7;    //��ť7����
static u8 ButtonAction8;    //��ť8����

u8 TestCompleteRf; //�������״̬�л�������״̬�ı�־
u8 Btn3LongPressFlag;
u8 Btn4LongPressFlag;
u8 Btn8LongPressFlag;

WIN last_win; //�ϴα�ѡ�еĴ���
WIN cur_win;
u8 win_num = 4;

u8 SettingFlag; //��������

u8 cur_switch; //���Ӻ���ѡ��

u8 rrecord;
u8 WhichRecord;
u8 RecordNeedRefresh;

u8 CommNeedReflash = 0;

/*
**********************************************************************************************************
										   ��������
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
* Description: ʱ�����÷��Ӹ����ѡ���л�
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void switch_min_sec(void)
{
	if(cur_switch == MIN)
	{
		//�л�����
		cur_switch = SEC;
	}
	else
	{
		//�л�������
		cur_switch = MIN;
	}
}

/*
**********************************************************************************************
*                       void mode_option_change(bool mode)
* Description: ģʽ����ѡ��ı�(���ϼӻ����¼�)
* Arguments  : mode:   true:���ϼ� false:���¼�
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
* Description: ��ѹ�ȼ�����ѡ��ı�(���ϼӻ����¼�)
* Arguments  : mode:   true:���ϼ� false:���¼�
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
* Description: ʱ��1����ѡ��ı�(���ϼӻ����¼�)
* Arguments  : mode:   true:���ϼ� false:���¼�
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
* Description: ʱ��2����ѡ��ı�(���ϼӻ����¼�)
* Arguments  : mode:   true:���ϼ� false:���¼�
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
* Description: ��ǰѡ��ı�(���ϼӻ����¼�)
* Arguments  : re_min����ǰ�����ĸ�����
*			 : mode:   true:���ϼ� false:���¼�
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
* Description: ��������
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void pg_key_poll(void)
{
	ReadKeyStatus(); /* ������״̬ */
	
	DealKeyLongPress(); /* ��������״̬���� */
	
	DealKey1(); /*������1 */ 
	DealKey2(); /*������2 */
	DealKey3(); /*������3 */
	DealKey4(); /*������4 */
	DealKey5(); /*������5 */
	DealKey6(); /*������6 */
	DealKey7(); /*������7 */
	DealKey8(); /*������8 */
	
	UpdateWinNumber();
	
}
/*
**********************************************************************************************
*                                   static void UpdateWinNumber(void)
* Description: �������ô��ڸ���
*							 ��ͬģʽ�����ô��ڸ����ǲ�һ����
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
* Description: ��������״̬����
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
* Description: ������״̬
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
* Description: ����1����(���ÿ���ת��ť)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey1(void)
{
	/* ����ģʽ������������������,������,�Ķ���¼ʱ�ð������������� */
	if((Communicate == BLUETOOTH) || (CommFlag == 1) || (TestState == 1) || (TestState == 3))
	{
		return;
	}
	
	/* �̰���ת���¸��� */
	if(ButtonAction1 == BUTTON_WAS_PRESSED) /* ���ÿ���ת��ť */
	{
		last_win = cur_win; /* �����ϴα�ѡ�еĿ� */

		if(cur_win == win_num)
		{
			cur_win = MODE_WIN;
		}
		else
		{
			cur_win++;
		}
		
		SettingFlag = 1;	/* ���ÿ򱻸ı� */
		
		/* ���Խ������л�������ģʽ */
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
* Description: ����2����(ʱ����Ӻ����л���ť)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey2(void)
{
	/* ����ģʽ������������������,������,�Ķ���¼ʱ�ð������������� */
	if((Communicate == BLUETOOTH) || (CommFlag == 1) || (TestState == 1) || (TestState == 3))
	{
		return;
	}
	
	/* �̰����Ӻ���֮���л� */
	if(ButtonAction2 == BUTTON_WAS_PRESSED)
	{
		if(cur_win == TIME1_WIN || cur_win == TIME2_WIN)
		{
			if((TestMode == 2) || (TestMode == 5) || (TestMode == 6))
			{
				switch_min_sec(); /* �л����Ӻ��� */
				SettingFlag = 1; /* ���ñ��ı��� */
			}
		}
		
		/* ���Խ������л�������ģʽ */
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
* Description: ����3����(��¼����/�Ķ���ť)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey3(void)
{
	/* ����ģʽ������������������ʱ�ð������������� */
	if((Communicate == BLUETOOTH) || (CommFlag == 1))
	{
		return;
	}
	
	if(ButtonAction3 == BUTTON_WAS_PRESSED)
	{
		if(TestState == 1) /* ���Թ����� */
		{
			SaveOneRecord(); /* �����¼ */	
		}
	}
	
	if((ButtonAction3 == BUTTON_IS_HELD) && (Btn3LongPressFlag == 0))
	{
		if(TestState == 0)
		{
			WhichRecord = 1;		/* �ӵ�һ����¼���� */	
			RecordNeedRefresh = 1;	/* ��¼��ˢ�� */
			TestStateSet(3); 			/* �������¼״̬ */	
			if(cur_win != NONE_WIN)
			{
				last_win = cur_win; /* �����ϴα�ѡ�еĿ� */
				cur_win = NONE_WIN;
				SettingFlag = 1; /* ���ñ��ı��� */
			}
		}
		else if(TestState == 3)
		{
			RecordNeedRefresh = 1;	/* ��¼��ˢ�� */
			TestStateSet(0); /* �˳�����¼״̬ */
		}
		
		Btn3LongPressFlag = 1;
		
		/* ���Խ������л�������ģʽ */
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
* Description: ����4����(����/���ذ�ť)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey4(void)
{
	/* ����ʱ�ð������������� */
	if((TestState == 1) || (TestState == 3))
	{
		return;
	}
	
	if((ButtonAction4 == BUTTON_IS_HELD) && (Btn4LongPressFlag == 0))
	{
		if(CommFlag == 0)
		{
			BTStateSet(1); /* �������� */
			CommNeedReflash = 1;
		}
		else 
		{
			BTStateSet(0); /* �˳����� */
			CommNeedReflash = 1;
		}
		
		Btn4LongPressFlag = 1;
		
		/* ���Խ������л�������ģʽ */
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
* Description: ����5����(���ϰ�ť)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey5(void)
{
	/* ����ģʽ,�����������������м�����ʱ�ð������������� */
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
			RecordNeedRefresh = 1;		/* ��¼��ˢ�� */
		}
		else
		{
			cur_option_change(cur_win, true);
			SettingFlag = 1; /* ���ñ��ı��� */
			SaveDataFlag = 1; /* �������ò��� */
		}
		
		/* ���Խ������л�������ģʽ */
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
* Description: ����6����(���°�ť)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey6(void)
{
	/* ����ģʽ,�����������������м�����ʱ�ð������������� */
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
			RecordNeedRefresh = 1;		/* ��¼��ˢ�� */
		}
		else
		{
			cur_option_change(cur_win, false);
			SettingFlag = 1; /* ���ñ��ı��� */
			SaveDataFlag = 1;  /* �������ò��� */
		}
		
		/* ���Խ������л�������ģʽ */
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
* Description: ����7����(ȷ�ϰ�ť)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey7(void)
{
	/* ����ģʽ,�����������������м�����ʱ�ð������������� */
	if((Communicate == BLUETOOTH) || (CommFlag == 1) || (TestState == 1))
	{
		return;
	}
	
	if(ButtonAction7 == BUTTON_WAS_PRESSED)
	{
		if(TestState == 3)
		{
			RecordNeedRefresh = 1;		/* ��¼��ˢ�� */
			TestStateSet(0); /* �˳�����¼״̬ */
		}
		else
		{
			if(cur_win != NONE_WIN)
			{
				last_win = cur_win; /* �����ϴα�ѡ�еĿ� */
				cur_win = NONE_WIN;
				SettingFlag = 1; /* ���ñ��ı��� */
			}
		}
		
		/* ���Խ������л�������ģʽ */
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
* Description: ����8����(���԰�ť)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void DealKey8(void)
{
	/* ����ģʽ,����¼ʱ�ð������������� */
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
			StartTestSet(1); /* ������������ */
			if(cur_win != NONE_WIN)
			{
				last_win = cur_win; /* �����ϴα�ѡ�еĿ� */
				cur_win = NONE_WIN;
				SettingFlag = 1; /* ���ñ��ı��� */
			}
		}
		
		Btn8LongPressFlag = 1;
		
		/* ���Խ������л�������ģʽ */
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
//	u8 ButtonAction1;    //��ť1����
//	u8 ButtonAction2;    //��ť2����
//	u8 ButtonAction3;    //��ť3����
//	u8 ButtonAction4;    //��ť4����
//	u8 ButtonAction5;    //��ť5����
//	u8 ButtonAction6;    //��ť6����
//	u8 ButtonAction7;    //��ť7����
//	u8 ButtonAction8;    //��ť8����
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
//		�������������
//	*/
//	if(Communicate == BLUETOOTH)
//	{
//		/*
//		����/���ذ�ť����,ǿ���л�������
//		*/
//		if((ButtonAction4 == BUTTON_IS_HELD) && (Button4ReleaseFlag == 0))
//		{
//			if(TestState == 0)  /* �ڲ����в����л������� */
//			{
//				Communicate = LOCAL;
//				CommFlag = 0; /* �˳����� */
//				CommNeedReflash = 1;
//				Button4ReleaseFlag = 1;
//			}
//		}
//		
//		/*
//		���԰�ť����,ǿ��ֹͣ����
//		*/
//		if(ButtonAction8 == BUTTON_IS_HELD && (Button8ReleaseFlag == 0))
//		{
//			if(TestState == 1)
//			{
//				StopTestSet(1); /* ����ֹͣ���� */
//			}
//			
//			Button8ReleaseFlag = 1;
//		}
//	}
//	else
//	{
//		/*
//			����Ǳ��ز���
//		*/
//		
//		if(TestState == 1) /* ������ */
//		{
//			if(ButtonAction3 == BUTTON_WAS_PRESSED)
//			{
//				SaveOneRecord(); /* �����¼ */
//			}
//			/*
//			���԰�ť����,ǿ��ֹͣ����
//			*/
//			if(ButtonAction8 == BUTTON_IS_HELD && (Button8ReleaseFlag == 0))
//			{
//				if(TestState == 1)
//				{
//					StopTestSet(1); /* ����ֹͣ���� */
//				}
//				
//				Button8ReleaseFlag = 1;
//			}
//		}
//		else if(TestState == 0) 
//		{
//			/*
//			���ÿ���ת��ť
//			*/
//			if(ButtonAction1 == BUTTON_WAS_PRESSED)
//			{
//				last_win = cur_win; /* �����ϴα�ѡ�еĿ� */
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
//				SettingFlag = 1;	/* ���ÿ򱻸ı� */
//			}
//			
//			/*
//			ʱ����Ӻ����л���ť
//			*/
//			if(ButtonAction2 == BUTTON_WAS_PRESSED)
//			{
//				if(cur_win == TIME1_WIN || cur_win == TIME2_WIN)
//				{
//					if((TestMode == 2) || (TestMode == 5) || (TestMode == 6))
//					{
//						switch_min_sec(); /* �л����Ӻ��� */
//						SettingFlag = 1; /* ���ñ��ı��� */
//					}
//				}
//			}

//			/*
//			��¼�����Ķ���ť
//			*/
//			if(ButtonAction3 == BUTTON_WAS_PRESSED)
//			{
//				if(rrecord == 0)
//				{
//					rrecord = 1;				/* ���ڶ���¼��־ */
//					WhichRecord = 1;			/* �ӵ�һ����¼���� */
//				//	ReadRecord(WhichRecord);	
//					RecordNeedRefresh = 1;		/* ��¼��ˢ�� */
//					
//					if(cur_win != NONE_WIN)
//					{
//						last_win = cur_win; /* �����ϴα�ѡ�еĿ� */
//						cur_win = NONE_WIN;
//						SettingFlag = 1; /* ���ñ��ı��� */
//					}
//				}
//			}

//			/*
//			����/����
//			*/
//			if((ButtonAction4 == BUTTON_IS_HELD) && (Button4ReleaseFlag == 0))
//			{
//				Communicate = BLUETOOTH;
//				CommFlag = 1; /* �������� */
//				CommNeedReflash = 1;
//				Button4ReleaseFlag = 1;
//			}

//			/*
//			���ϰ�ť
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
//					RecordNeedRefresh = 1;		/* ��¼��ˢ�� */
//				}
//				else
//				{
//					cur_option_change(cur_win, true);
//					SettingFlag = 1; /* ���ñ��ı��� */
//				}
//			}

//			/*
//			���°�ť
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
//					RecordNeedRefresh = 1;		/* ��¼��ˢ�� */
//				}
//				else
//				{
//					cur_option_change(cur_win, false);
//					SettingFlag = 1; /* ���ñ��ı��� */
//				}	
//			}

//			/*
//			ȷ�ϰ�ť
//			*/
//			if(ButtonAction7 == BUTTON_WAS_PRESSED)
//			{
//				if(cur_win != NONE_WIN)
//				{
//					last_win = cur_win; /* �����ϴα�ѡ�еĿ� */
//					cur_win = NONE_WIN;
//					SettingFlag = 1; /* ���ñ��ı��� */
//				}
//				if(rrecord) //���ڶ���¼
//				{
//					rrecord = 0;
//				//	ExitReadRecord();
//					RecordNeedRefresh = 1;		/* ��¼��ˢ�� */
//				}
//			}

//			/*
//			���԰�ť
//			*/
//			if(ButtonAction8 == BUTTON_IS_HELD && (Button8ReleaseFlag == 0))
//			{
//				StartTestSet(1); /* ������������ */
//				
//				if(cur_win != NONE_WIN)
//				{
//					last_win = cur_win; /* �����ϴα�ѡ�еĿ� */
//					cur_win = NONE_WIN;
//					SettingFlag = 1; /* ���ñ��ı��� */
//				}
//				if(rrecord) //���ڶ���¼
//				{ 
//					rrecord = 0;
//				//	ExitReadRecord();
//					RecordNeedRefresh = 1;		/* ��¼��ˢ�� */
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
   




