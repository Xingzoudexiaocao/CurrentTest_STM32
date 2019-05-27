/***********************************************************************
文件名称：pg_rf.c
功	  能：页面创建
编写时间：2018.08.20
编 写 人：wuliu
注    意：
***********************************************************************/
#include "main.h"
#include "../lvgl/lvgl.h"
#include "pg_rf.h"

/*------------------------------------------------------------------------------------------*/

u8 Disp_ID[10];				//编号

u8 Disp_Bugle;				//喇叭标志


u8 Disp_switch; //分钟还是秒选项

u8 Disp_Mode;	 //=2  定时Timing measurement
				 //=3  连续Continuous measurement
				 //=4  比较
				 //=5	 吸收比
				 //=6  极化指数
				 //=7  直流电压
				 //=8  交流电压
				 
COMM Disp_Comm;	 /* 蓝牙/本地 */
u8 Disp_WhichRecord; /* 第几条记录 */
u8 Disp_records; /* 记录总数 */
				 

u8 Disp_Vol;    //=0 500V  
               //=1 1000V
               //=2 1500V
               //=3 2500V
							 
u32 Disp_CompR;		 //比较电阻值

u16 Disp_TimeSS; 	 //定时时间

u16 Disp_JH_Time1;	 //极化指数时间1
u16 Disp_JH_Time2;	 //极化指数时间2

u16 Disp_XSB_Time1; //吸收比时间1
u16 Disp_XSB_Time2; //吸收比时间2

u8 Disp_BatState;  

u8 Disp_BatVol; 

u16 Disp_Timeing;    //NowTime

u32 Disp_Value;	//测试值

u16 Disp_Test_Result; //测试结果

u8 Dis_Step;
u16 Dis_Dly;
u8 RefreshStep;

/*----------------------------------------*/
u8 TestingComplete;

#define REFRESH_TIMER 1
static u8 RefreshEnable = 0;

u8 refresh_step = 0;

/*----------------------------------------------------------------------------------------*/
static void BTRefresh(void);
static void SettingRefresh(void);
static void MustBeRefresh(void);
static void WholeRefresh(void);
static void RRcordStatusRefreshDisp(void);
static void TestingStatusRefreshDisp(void);
static void IdleStatusRefreshDisp(void);

/*
**********************************************************************************************
*                                   void ID_label_refresh(void) 
* Description: ID的标签刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void ID_label_refresh(u8 *id)
{
	lv_label_set_text(ID_label,(const char *)id);
}
/*
**********************************************************************************************
*                                   void record_label_refresh(void) 
* Description: 记录标签刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void record_label_refresh(u8 which_record, u8 record_num) 
{
	char strBuff[50];
	
	snprintf(strBuff, sizeof(strBuff), "记录 %d/%d", which_record, record_num);
	lv_label_set_text(record_label, strBuff);
}
/*
**********************************************************************************************
*                                   void bluetooth_icon_refresh(COMM mode) 
* Description: 蓝牙/本地标签刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void bluetooth_icon_refresh(COMM mode) 
{
	if(mode) //蓝牙
	{
		lv_label_set_text(bluetooth_label, SYMBOL_BLUETOOTH);
	}
	else //本地
	{
		lv_label_set_text(bluetooth_label, SYMBOL_KEYBOARD);
	}
	lv_obj_align(bluetooth_label, bg , LV_ALIGN_IN_TOP_RIGHT, -72, 3);
}

/*
**********************************************************************************************
*                                   void bugle_label_refresh(void) 
* Description: 喇叭标签刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void bugle_label_refresh(u8 bugle) 
{
	if(bugle)
	{
		if(bugle_label->hidden == 1)
		{
			lv_obj_set_hidden(bugle_label, false);
		}
	}
	else
	{
		if(bugle_label->hidden == 0)
		{
			lv_obj_set_hidden(bugle_label, true);
		}
	}
}
/*
**********************************************************************************************
*                                   void battery_icon_refresh(BAT state) 
* Description: 电池状态刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void battery_icon_refresh(u8 state)
{
	switch(state)
	{
		case BAT_EMPTY:
		{
			lv_label_set_text(battery_label, SYMBOL_BATTERY_EMPTY);
		}break;
		case BAT_1:
		{
			lv_label_set_text(battery_label, SYMBOL_BATTERY_1);
		}break;
		case BAT_2:
		{
			lv_label_set_text(battery_label, SYMBOL_BATTERY_2);
		}break;
		case BAT_3:
		{
			lv_label_set_text(battery_label, SYMBOL_BATTERY_3);
		}break;
		case BAT_FULL:
		{
			lv_label_set_text(battery_label, SYMBOL_BATTERY_FULL);
		}break;
		default:
		{}break;
	}
}
/*
**********************************************************************************************
*                                   void ACorDC_label_refresh(AC_DC acdc) 
* Description: AC/DC的标签刷新(运行时不需要刷新的)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void ACorDC_label_refresh(AC_DC acdc)
{
	if(acdc == AC)
	{
		if(ACorDC_label->hidden == 1)
		{
			lv_obj_set_hidden(ACorDC_label, false);
		}
		lv_label_set_text(ACorDC_label, "AC");
	}
	else if(acdc == DC)
	{
		if(ACorDC_label->hidden == 1)
		{
			lv_obj_set_hidden(ACorDC_label, false);
		}
		lv_label_set_text(ACorDC_label, "DC");
	}
	else
	{
		if(ACorDC_label->hidden == 0)
		{
			lv_obj_set_hidden(ACorDC_label, true);
		}
	}
	
}
/*
**********************************************************************************************
*                                   void ACorDC_label_refresh_lojic(void) 
* Description: AC/DC的标签刷新逻辑(运行时不需要刷新的)
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void ACorDC_label_refresh_lojic(u8 mode) 
{
	if(mode == 0x07)
	{
		ACorDC_label_refresh(DC);
	}
	else if(mode == 0x08)
	{
		ACorDC_label_refresh(AC);
	}
	else
	{
		ACorDC_label_refresh(OTHER);
	}
}
/*
**********************************************************************************************
*                                   void curtime_label_refresh(u32 time) 
* Description: 时间刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void curtime_label_refresh(u32 time) 
{
	char strBuff[10];

	uint8_t hours = time/60; //小时
	uint8_t min = time - (time/60) * 60; //分钟

	if((min < 10) && (hours < 10))
	{
		snprintf(strBuff, 6, "0%d:0%d", hours, min);
	}
	else if(min < 10)
	{
		snprintf(strBuff, 6, "%d:0%d", hours, min);
	}
	else if(hours < 10)
	{
		snprintf(strBuff, 6, "0%d:%d", hours, min);
	}
	else
	{
		snprintf(strBuff, 6, "%d:%d", hours, min);
	}

	lv_label_set_text(curtime_label, strBuff);
	lv_label_set_style(curtime_label, &curtime_style);
}
/*
**********************************************************************************************
*                                   void curtestvalue_label_refresh(u32 data) 
* Description: 电阻测试值标签刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void curtestvalue_res_label_refresh(u32 data) 
{
	char strBuff[10];

	uint32_t temp = data;

	if(temp < 100000) //MΩ
	{
		if((temp/100) > 100)	//100.12MΩ
		{
			snprintf(strBuff, 7, "%d.%d%d", (temp/100), (temp%100)/10, (temp%100)%10);
		}
		else if((temp/100) > 10) //010.12MΩ
		{
			snprintf(strBuff, 7, "%d.%d%d0", (temp/100), (temp%100)/10, (temp%100)%10);
		}
		else //001.12MΩ
		{
			snprintf(strBuff, 7, "%d.%d%d00", (temp/100), (temp%100)/10, (temp%100)%10);
		}

	}
	else if(temp <= 10000000) //100GΩ
	{
		if((temp/100000) > 99) //100GΩ
		{
			snprintf(strBuff, 7, "%d.%d%d", 100, 0, 0);
		}
		if((temp/100000) > 10)	//99.123GΩ
		{
			snprintf(strBuff, 7, "%d.%d%d%d", (temp/100000), (temp%100000)/10000, (temp%100000)%10000/1000, (((temp%100000)%10000)%1000)/100);
		}
		else //1.1234MΩ
		{
			snprintf(strBuff, 7, "%d.%d%d%d%d", temp/100000, (temp%100000)/10000, (temp%100000)%10000/1000, (((temp%100000)%10000)%1000)/100, ((((temp%100000)%10000)%1000)%100)/10);
		}
	}
	else
	{
		snprintf(strBuff, 7, "%s","-----");
	}

	lv_label_set_text(curtestvalue_label, strBuff);
	lv_label_set_style(curtestvalue_label, &curtestvalue_style);
}
/*
**********************************************************************************************
*                                   void curtestvalue_label_refresh(u32 data) 
* Description: 电压测试值标签刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void curtestvalue_vol_label_refresh(u32 data) 
{
	char strBuff[10];
	u32 temp;
	
	temp = data/100;
	
	if(temp >= 10000)
	{
		snprintf(strBuff, 7, "%d", temp);
	}
	else if(temp >= 1000)
	{
		snprintf(strBuff, 7, "%d.%d", temp, data%100);
	}
	else if(temp >= 100)
	{
		snprintf(strBuff, 7, "%d.%d%d", temp, (data%100)/10, (data%100)%10);
	}
	else if(temp >= 10)
	{
		snprintf(strBuff, 7, "%d.%d%d0", temp, (data%100)/10, (data%100)%10);
	}
	else 
	{
		snprintf(strBuff, 7, "%d.%d%d00", temp, (data%100)/10, (data%100)%10);
	}
	
}
/*
**********************************************************************************************
*                                   void curtestvalue_label_refresh(u32 data) 
* Description: 测试值标签刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void curtestvalue_label_refresh(u8 mode, u32 test_value)
{
	if(mode == 0x07 || mode == 0x08)
	{
		curtestvalue_vol_label_refresh(test_value);
	}
	else
	{
		curtestvalue_res_label_refresh(test_value);
	}
}

/*
**********************************************************************************************
*                                   void curvalueunit_label_refresh(u8 uint) 
* Description: 测试值单位刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void curvalueunit_label_refresh(u8 unit)
{
	if(unit == 0)
	{
		lv_label_set_text(curvalueunit_label, "V"); //显示V
	}
	else if(unit == 1)
	{
		lv_label_set_text(curvalueunit_label, "MΩ"); //显示MΩ
	}
	else
	{
		lv_label_set_text(curvalueunit_label, "GΩ"); //显示GΩ
	}
	lv_obj_align(curvalueunit_label, mainwin , LV_ALIGN_IN_TOP_RIGHT, -15, 75);
}
/*
**********************************************************************************************
*                                   void curvalueunit_label_refresh_lojic(PAGE rf) 
* Description: 测试值单位刷新逻辑
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void curvalueunit_label_refresh_lojic(u8 mode, u32 test_value) 
{
	if(mode == 0x07 || mode == 0x08)
	{
		curvalueunit_label_refresh(0);
	}
	else
	{
		if(test_value < 100000) //MΩ
		{
			curvalueunit_label_refresh(1);
		}
		else
		{
			curvalueunit_label_refresh(2);
		}
	}
}
/*
**********************************************************************************************
*                                   void PI_label_refresh(u32 PI, u8 state) 
* Description: PI标签刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void PI_label_refresh(u32 PI, u8 state)
{
	char str[10];
	
	if(state == 0) //测试中
	{
		snprintf(str, sizeof(str), "PI: ----");
		lv_label_set_text(PISI_label, str);
	}
	else	//测试完成
	{
		snprintf(str, sizeof(str), "PI: %d" ,PI);
		lv_label_set_text(PISI_label, str);
	}
}
/*
**********************************************************************************************
*                                   void SI_label_refresh(u32 SI, u8 state) 
* Description: SI标签刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void SI_label_refresh(u32 SI, u8 state)
{
	char str[10];
	
	if(state == 0) //测试中
	{
		snprintf(str, sizeof(str), "SI: ----");
		lv_label_set_text(PISI_label, str);
	}
	else	//测试完成
	{
		snprintf(str, sizeof(str), "SI: %d" ,SI);
		lv_label_set_text(PISI_label, str);
	}
}
/*
**********************************************************************************************
*                        void lineScale_refresh_value(uint32_t Data)
* Description: 更新电阻值，更新刻度显示
* Arguments  : Data:电阻值
* Returns    :
**********************************************************************************************
*/
void lineScale_refresh_value(uint32_t Data)
{
	uint32_t *ext = lv_obj_allocate_ext_attr(lineScale, sizeof(uint32_t));
	*ext = Data;
	lv_obj_refresh_ext_size(lineScale);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
**********************************************************************************************
*                       void time_turnto_resist(bool mode)
* Description: 比较模式下的窗口变化
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time_turnto_resist(u32 Data, bool mode)
{
	char str[10];
	if(mode)
	{
		lv_obj_set_hidden(time1_label, false);
		
		lv_obj_set_hidden(time1_min_label, true);
		lv_obj_set_hidden(time1_mid_label, true);	//分钟和秒中的“:”隐藏起来
		lv_obj_set_hidden(time1_sec_label, true);
		
		snprintf(str, sizeof(str), "%d" ,Data);
		lv_label_set_text(time1_title, "比较电阻"); //时间1窗口标题变成定时时间
		lv_label_set_text(time1_label, str); 
		lv_obj_align(time1_label, time1frame, LV_ALIGN_CENTER, 2, 10);
		
	}
	else
	{
		lv_obj_set_hidden(time1_label, true);
		
		lv_obj_set_hidden(time1_min_label, false);
		lv_obj_set_hidden(time1_mid_label, false); //分钟和秒中的“:”显示
		lv_obj_set_hidden(time1_sec_label, false);
		
		lv_label_set_text(time1_title, "时间1"); //时间1窗口标题
		
		if(Data/60 < 10)
		{
			snprintf(str, sizeof(str), "0%d" ,Data/60);
		}
		else
		{
			snprintf(str, sizeof(str), "%d" ,Data/60);
		}
		lv_label_set_text(time1_min_label, str);
		if((Data - (Data/60) * 60) < 10)
		{
			snprintf(str, sizeof(str), "0%d" ,Data - (Data/60) * 60);
		}
		else
		{
			snprintf(str, sizeof(str), "%d" ,Data - (Data/60) * 60);
		}
		lv_label_set_text(time1_sec_label, str);
		lv_obj_align(time1_min_label, time1frame, LV_ALIGN_CENTER, -10, 10);
		lv_obj_align(time1_mid_label, time1_min_label, LV_ALIGN_OUT_RIGHT_TOP, 1, 0);
		lv_obj_align(time1_sec_label, time1_mid_label, LV_ALIGN_OUT_RIGHT_TOP, 1, 0);
	}
	lv_obj_align(time1_title, time1frame, LV_ALIGN_IN_TOP_MID, 2, 5);
}
/*
**********************************************************************************************
*                       void mode_option_refresh(MODE which_mode)
* Description: 模式窗口选项刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void mode_option_refresh(u8 mode)
{
	switch(mode)
	{
		case 0x02:
		{
			lv_label_set_text(mode_label, "定时");
			time_turnto_resist(Disp_TimeSS, false); //显示时间1
			if(time1frame->hidden == 1)
			{
				lv_obj_set_hidden(time1frame, false);
			}
				lv_label_set_text(time1_title, "定时时间"); //时间1窗口标题变成定时时间
				lv_obj_align(time1_title, time1frame, LV_ALIGN_IN_TOP_MID, 2, 5);
			if(time2frame->hidden == 0)
			{
				lv_obj_set_hidden(time2frame, true);	//定时模式T2不显示
			}
			if(vlevelframe->hidden == 1)
			{
				lv_obj_set_hidden(vlevelframe, false);	//直流电压模式电压等级显示
			}	
			if(lineScale->hidden == 1)
			{
				lv_obj_set_hidden(lineScale, false);	//刻度盘不显示
			}
		}break;
		case 0x03:
		{
			lv_label_set_text(mode_label, "连续");
			if(time1frame->hidden == 0)
			{
				lv_obj_set_hidden(time1frame, true);	//连续模式T1不显示
			}
			if(time2frame->hidden == 0)
			{
				lv_obj_set_hidden(time2frame, true);	//连续模式T2不显示
			}
		}break;
		case 0x04:
		{
			if(time1frame->hidden == 1)
			{
				lv_obj_set_hidden(time1frame, false);
			}
			if(time2frame->hidden == 0)
			{
				lv_obj_set_hidden(time2frame, true);	//定时模式T2不显示
			}
			lv_label_set_text(mode_label, "比较");
			time_turnto_resist(Disp_CompR, true); //显示比较电阻		
		}break;
		case 0x05:
		{
			if(time1frame->hidden)
			{
				lv_obj_set_hidden(time1frame, false);	//吸收比模式T1显示
			}
			if(time2frame->hidden)
			{
				lv_obj_set_hidden(time2frame, false);	//吸收比模式T2显示
				time2_option_refresh(Disp_XSB_Time2,0);
				time2_option_refresh(Disp_XSB_Time2,1);
			}
			lv_label_set_text(mode_label, "吸收比");
			time_turnto_resist(Disp_XSB_Time1, false); //显示时间1
		}break;
		case 0x06:
		{
			if(time1frame->hidden)
			{
				lv_obj_set_hidden(time1frame, false);	//极化指数模式T1显示
			}
			if(time2frame->hidden)
			{
				lv_obj_set_hidden(time2frame, false);	//极化指数模式T2显示
				time2_option_refresh(Disp_JH_Time2,0);
				time2_option_refresh(Disp_JH_Time2,1);
			}
			if(vlevelframe->hidden == 1)
			{
				lv_obj_set_hidden(vlevelframe, false);	//电压等级显示
			}
			if(lineScale->hidden == 1)
			{
				lv_obj_set_hidden(lineScale, false);	//刻度盘不显示
			}
			lv_label_set_text(mode_label, "极化指数");
			time_turnto_resist(Disp_JH_Time1, false); //显示时间1
		}break;
		case 0x07:
		{
			if(time1frame->hidden == 0)
			{
				lv_obj_set_hidden(time1frame, true);	//直流电压模式T1不显示
			}
			if(time2frame->hidden == 0)
			{
				lv_obj_set_hidden(time2frame, true);	//直流电压模式T2不显示
			}
			if(vlevelframe->hidden == 0)
			{
				lv_obj_set_hidden(vlevelframe, true);	//直流电压模式电压等级不显示
			}	
			if(lineScale->hidden == 0)
			{
				lv_obj_set_hidden(lineScale, true);	//直流电压模式刻度盘不显示
			}
			lv_label_set_text(mode_label, "直流电压");
		}break;
		case 0x08:
		{
			if(time1frame->hidden == 0)
			{
				lv_obj_set_hidden(time1frame, true);	//交流电压模式T1不显示
			}
			if(time2frame->hidden == 0)
			{
				lv_obj_set_hidden(time2frame, true);	//交流电压模式T2不显示
			}
			if(vlevelframe->hidden == 0)
			{
				lv_obj_set_hidden(vlevelframe, true);	//交流电压模式电压等级不显示
			}	
			if(lineScale->hidden == 0)
			{
				lv_obj_set_hidden(lineScale, true);	//交流电压模式刻度盘不显示
			}
			lv_label_set_text(mode_label, "交流电压");
		}break;
		default:
		{
			
		}break;
	}
	lv_obj_align(mode_label, modeframe, LV_ALIGN_CENTER, 2, 10);
}
/*
**********************************************************************************************
*                       void vlevel_option_refresh(VLEVEL level)
* Description: 电压等级窗口选项刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void vlevel_option_refresh(u8 Vlevel)
{
	switch(Vlevel)
	{
		case 0x00:
		{
			lv_label_set_text(vlevel_label, "500V");
		}break;
		case 0x01:
		{
			lv_label_set_text(vlevel_label, "1000V");
		}break;
		case 0x02:
		{
			lv_label_set_text(vlevel_label, "1500V");
		}break;
		case 0x03:
		{
			lv_label_set_text(vlevel_label, "2500V");
		}
		default:
		{
			
		}break;
	}
	lv_obj_align(vlevel_label, vlevelframe, LV_ALIGN_CENTER, 2, 10);
}
/*
**********************************************************************************************
*                       void time1_option_refresh(u32 Data, MODE which_mode, u8 which_switch)
* Description: 时间1窗口选项刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time1_option_refresh(u32 Data, u8 which_mode , u8 which_switch)
{
	char str[10];
	
	if(which_mode == 0x04)
	{
		snprintf(str, sizeof(str), "%d" ,Data);
		lv_label_set_text(time1_label, str);
		lv_obj_align(time1_label, time1frame, LV_ALIGN_CENTER, 2, 10);
	}
	else 
	{
		if(which_switch == 0)
		{
			if(Data/60 < 10)
			{
				snprintf(str, sizeof(str), "0%d" ,Data/60);
			}
			else
			{
				snprintf(str, sizeof(str), "%d" ,Data/60);
			}
			lv_label_set_text(time1_min_label, str);
		}
		else
		{
			if((Data - (Data/60) * 60) < 10)
			{
				snprintf(str, sizeof(str), "0%d" ,Data - (Data/60) * 60);
			}
			else
			{
				snprintf(str, sizeof(str), "%d" ,Data - (Data/60) * 60);
			}
			lv_label_set_text(time1_sec_label, str);
		}
		lv_obj_align(time1_min_label, time1frame, LV_ALIGN_CENTER, -10, 10);
		lv_obj_align(time1_mid_label, time1_min_label, LV_ALIGN_OUT_RIGHT_TOP, 1, 0);
		lv_obj_align(time1_sec_label, time1_mid_label, LV_ALIGN_OUT_RIGHT_TOP, 1, 0);
	}
	 
}

/*
**********************************************************************************************
*                       void time2_option_refresh(u32 Data)
* Description: 时间1窗口选项刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time2_option_refresh(u32 Data, u8 which_switch)
{
	char str[10];

	if(which_switch == 0)
	{
		if(Data/60 < 10)
		{
			snprintf(str, sizeof(str), "0%d" ,Data/60);
		}
		else
		{
			snprintf(str, sizeof(str), "%d" ,Data/60);
		}
		lv_label_set_text(time2_min_label, str);
	}
	else
	{
		if((Data - (Data/60) * 60) < 10)
		{
			snprintf(str, sizeof(str), "0%d" ,Data - (Data/60) * 60);
		}
		else
		{
			snprintf(str, sizeof(str), "%d" ,Data - (Data/60) * 60);
		}
		lv_label_set_text(time2_sec_label, str);
	}
	lv_obj_align(time2_min_label, time2frame, LV_ALIGN_CENTER, -10, 10);
	lv_obj_align(time2_mid_label, time2_min_label, LV_ALIGN_OUT_RIGHT_TOP, 1, 0);
	lv_obj_align(time2_sec_label, time2_mid_label, LV_ALIGN_OUT_RIGHT_TOP, 1, 0);
	 
}		
/*
**********************************************************************************************
*                                   void modeframe_refresh(bool mode) 
* Description: 模式窗口刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void modeframe_refresh(bool mode) 
{
	if(mode) //选中的时候有边框，选项字体颜色也不一样
	{
		modeframe_style.body.border.width = 3;
		modeframe_style.body.border.opa = 200;
		modeframe_style.body.shadow.width = 5;
		modeframe_style.body.shadow.color = LV_COLOR_MAKE(17,28,46);
		modeframe_style.body.border.color = LV_COLOR_MAKE(17,28,46);
		
		mode_label_style.text.color = LV_COLOR_BLUE;
	}
	else
	{
		modeframe_style.body.border.width = 1;
		modeframe_style.body.border.opa = LV_OPA_30;
		modeframe_style.body.shadow.width = 0;
		modeframe_style.body.border.color = LV_COLOR_MAKE(0x40, 0x40, 0x40);
		
		mode_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
	}
	lv_label_set_style(modeframe, &modeframe_style);
}
/*
**********************************************************************************************
*                                   void vlevelframe_refresh(bool mode) 
* Description: 电压等级窗口刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void vlevelframe_refresh(bool mode) 
{
	if(mode) //选中的时候有边框，选项字体颜色也不一样
	{
		vlevelframe_style.body.border.width = 3;
		vlevelframe_style.body.border.opa = 200;
		vlevelframe_style.body.shadow.width = 5;
		vlevelframe_style.body.shadow.color = LV_COLOR_MAKE(17,28,46);
		vlevelframe_style.body.border.color = LV_COLOR_MAKE(17,28,46);
		
		vlevel_label_style.text.color = LV_COLOR_BLUE;
	}
	else
	{
		vlevelframe_style.body.border.width = 1;
		vlevelframe_style.body.border.opa = LV_OPA_30;
		vlevelframe_style.body.shadow.width = 0;
		vlevelframe_style.body.border.color = LV_COLOR_MAKE(0x40, 0x40, 0x40);
		
		vlevel_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
	}
	lv_label_set_style(vlevelframe, &vlevelframe_style);
}
/*
**********************************************************************************************
*                                   void time1frame_refresh(bool mode, MODE which_mode) 
* Description: 时间1窗口刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time1frame_refresh(bool mode, u8 which_mode) 
{
	if(mode) //选中的时候有边框，选项字体颜色也不一样
	{
		time1frame_style.body.border.width = 3;
		time1frame_style.body.border.opa = 200;
		time1frame_style.body.shadow.width = 5;
		time1frame_style.body.shadow.color = LV_COLOR_MAKE(17,28,46);
		time1frame_style.body.border.color = LV_COLOR_MAKE(17,28,46);
		
		if(which_mode == 0x04)
		{
			time1_label_style.text.color = LV_COLOR_BLUE;
		}
		else
		{
			time1_min_label_style.text.color = LV_COLOR_BLUE;
			time1_mid_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
			time1_sec_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
		}
		
		
	}
	else
	{
		time1frame_style.body.border.width = 1;
		time1frame_style.body.border.opa = LV_OPA_30;
		time1frame_style.body.shadow.width = 0;
		time1frame_style.body.border.color = LV_COLOR_MAKE(0x40, 0x40, 0x40);
		
		if(which_mode == 0x04)
		{
			time1_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
		}
		else
		{
			time1_min_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
			time1_mid_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
			time1_sec_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
		}
	}
	lv_label_set_style(time1frame, &time1frame_style);
	lv_label_set_style(time1_label, &time1_label_style);
	lv_label_set_style(time1_min_label, &time1_min_label_style);
	lv_label_set_style(time1_mid_label, &time1_mid_label_style);
	lv_label_set_style(time1_sec_label, &time1_sec_label_style);
}
/*
**********************************************************************************************
*                                   void time2frame_refresh(bool mode) 
* Description: 时间2窗口刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time2frame_refresh(bool mode) 
{
	if(mode)
	{
		time2frame_style.body.border.width = 3;
		time2frame_style.body.border.opa = 200;
		time2frame_style.body.shadow.width = 5;
		time2frame_style.body.shadow.color = LV_COLOR_MAKE(17,28,46);
		time2frame_style.body.border.color = LV_COLOR_MAKE(17,28,46);
		
		time2_min_label_style.text.color = LV_COLOR_BLUE;
		time2_mid_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
		time2_sec_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
	}
	else
	{
		time2frame_style.body.border.width = 1;
		time2frame_style.body.border.opa = LV_OPA_30;
		time2frame_style.body.shadow.width = 0;
		time2frame_style.body.border.color = LV_COLOR_MAKE(0x40, 0x40, 0x40);
		
		time2_min_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
		time2_mid_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
		time2_sec_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
	}
	lv_label_set_style(time2frame, &time2frame_style);
	lv_label_set_style(time2_min_label, &time2_min_label_style);
	lv_label_set_style(time2_mid_label, &time2_mid_label_style);
	lv_label_set_style(time2_sec_label, &time2_sec_label_style);
}
/*
**********************************************************************************************
*                                   void time1_lable_selected_refresh(u8 data) 
* Description: 时间2窗口刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time1_lable_selected_refresh(u8 data)
{
	if(data == MIN)
	{
		time1_min_label_style.text.color = LV_COLOR_BLUE;
		time1_sec_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
	}
	else
	{
		time1_min_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
		time1_sec_label_style.text.color = LV_COLOR_BLUE;
	}
	lv_label_set_style(time1_min_label, &time1_min_label_style);
	lv_label_set_style(time1_sec_label, &time1_sec_label_style);
}
/*
**********************************************************************************************
*                                   void time2_lable_selected_refresh(u8 data) 
* Description: 时间2窗口刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time2_lable_selected_refresh(u8 data)
{
	if(data == MIN)
	{
		time2_min_label_style.text.color = LV_COLOR_BLUE;
		time2_sec_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
	}
	else
	{
		time2_min_label_style.text.color = LV_COLOR_MAKE(0x20, 0x20, 0x20);
		time2_sec_label_style.text.color = LV_COLOR_BLUE;
	}
	lv_label_set_style(time2_min_label, &time2_min_label_style);
	lv_label_set_style(time2_sec_label, &time2_sec_label_style);
}
/*
**********************************************************************************************
*                                   void QRcodeOrLocalRefresh(u8 state)
* Description: 蓝牙二维码/本地框刷新
* Arguments  : state: 0:显示二维码 1：二维码消失 2：本地框显示 3：本地框消失 
* Returns    :
**********************************************************************************************
*/
void QRcodeOrLocalRefresh(u8 state)
{
	switch(state)
	{
		case 0:
		{
			if(QRcode_img->hidden == 1)
			{
				lv_obj_set_hidden(QRcode_img, false);	
			}
		}break;
		case 1:
		{
			if(QRcode_img->hidden == 0)
			{
				lv_obj_set_hidden(QRcode_img, true);	
			}
		}break;
		case 2:
		{
			if(localframe->hidden == 1)
			{
				lv_obj_set_hidden(localframe, false);	
			}
		}break;
		case 3:
		{
			if(localframe->hidden == 0)
			{
				lv_obj_set_hidden(localframe, true);	
			}
		}break;
	}
}
/*
**********************************************************************************************
*                                void cur_win_selected_refresh(win re_win)
* Description: 刷新当前被选中的小窗口
* Arguments  : re_win:需刷新的小窗口
* Returns    :            
**********************************************************************************************
*/
void cur_win_selected_refresh(WIN re_win , bool mode)
{
	switch(re_win)
	{
		case NONE_WIN:
		{
			//没有一个选中的状态	
		}break;
		case MODE_WIN:
		{
			modeframe_refresh(mode);
		}break;
		case VLEVEL_WIN:
		{
			vlevelframe_refresh(mode);
		}break;
		case TIME1_WIN:
		{
			time1frame_refresh(mode, TestMode);
		}break;
		case TIME2_WIN:
		{
			time2frame_refresh(mode);
		}break;
		default:
		{}break;
	}
}
/*
**********************************************************************************************
*                                   void DataRefresh(void) 
* Description: 数据刷新
* Arguments  : None
* Returns    : None           
**********************************************************************************************
*/
void DataRefresh(void)
{
	if(TestState == 1) /* 测试中 */
	{
		Disp_Timeing = Timeing; /* 更新当前时间 */
		Disp_Value = Value1;	/* 更新当前测试值 */
		Disp_BatState = BatState; /* 更新电池电压状态 */
		Disp_BatVol = BatVol;	/* 更新电池电压 */
		Disp_Test_Result = Value2; /* 更新测试结果 */	
	}
	else if(TestState == 3) /* 读记录 */
	{
		Disp_Comm = Communicate;
		Disp_BatState = BatState; /* 更新电池电压状态 */
		Disp_BatVol = BatVol;	/* 更新电池电压 */
		Disp_WhichRecord = WhichRecord;
		Disp_records = Records;
	}
	else if(TestState == 0) /* 空闲状态 */
	{
			Disp_Timeing = Timeing; /* 更新当前时间 */
			Disp_Value = Value1;	/* 更新当前测试值 */
			Disp_BatState = BatState; /* 更新电池电压状态 */
			Disp_BatVol = BatVol;	/* 更新电池电压 */
			Disp_Test_Result = Value2; /* 更新测试结果 */	
			
			Disp_WhichRecord = WhichRecord;
			Disp_records = Records;

			Disp_Mode = TestMode;
			Disp_Comm = Communicate;
			Disp_Vol = TestVol;
			Disp_CompR = CompR;
			Disp_TimeSS = TimeSS;
			Disp_JH_Time1 = JH_Time1;
			Disp_JH_Time2 = JH_Time2;
			Disp_XSB_Time1 = XSB_Time1;
			Disp_XSB_Time2 = XSB_Time2;
	}
}
/*
**********************************************************************************************
*                                   void ReadRecord(u8 which_record) 
* Description: 阅读记录
* Arguments  : which_record:那条记录
* Returns    : None           
**********************************************************************************************
*/
void ReadRecord(u8 which_record) 
{
	u8 record_buff[RECORD_SIZE];
	ReadOneRecord(which_record,record_buff);
	
	Disp_records = RecordData[PARA_ADDR1]; /* 总记录数 */
	Disp_Mode = record_buff[1];		 /* 测试模式 */
	
	switch(Disp_Mode)
	{
		case 0x02:
		{
			Disp_Vol = record_buff[2]; 								/* 电压等级 */
			Disp_TimeSS = Cov_u16(&record_buff[3]);		/* 定时时间 */
			Disp_Timeing = Cov_u16(&record_buff[7]);	/* 运行时间 */
			Disp_Value = Cov_u32(&record_buff[9]);		/* 测试值 */
		}break;
		case 0x03:
		{
			Disp_Vol = record_buff[2]; 								/* 电压等级 */
			Disp_Timeing = Cov_u16(&record_buff[7]);	/* 运行时间 */
			Disp_Value = Cov_u32(&record_buff[9]);		/* 测试值 */
		}break;
		case 0x04:
		{
			Disp_Vol = record_buff[2]; 										/* 电压等级 */
			Disp_CompR = Cov_u32(&record_buff[3]);				/* 比较电阻值 */
			Disp_Timeing = Cov_u16(&record_buff[7]);			/* 运行时间 */
			Disp_Value = Cov_u32(&record_buff[9]);				/* 测试值 */
			Disp_Test_Result = Cov_u16(&record_buff[13]); /* 测试结果 */
		}break;
		case 0x05:
		{
			Disp_Vol = record_buff[2]; 										/* 电压等级 */
			Disp_XSB_Time1 = Cov_u16(&record_buff[3]);		/* 吸收比时间1 */
			Disp_XSB_Time2 = Cov_u16(&record_buff[5]);		/* 吸收比时间2 */
			Disp_Timeing = Cov_u16(&record_buff[7]);			/* 运行时间 */
			Disp_Value = Cov_u32(&record_buff[9]);				/* 测试值 */
			Disp_Test_Result = Cov_u16(&record_buff[13]); /* 测试结果 */
		}break;
		case 0x06:
		{
			Disp_Vol = record_buff[2]; 										/* 电压等级 */
			Disp_JH_Time1 = Cov_u16(&record_buff[3]);			/* 极化指数时间1 */
			Disp_JH_Time2 = Cov_u16(&record_buff[5]);			/* 极化指数时间2 */
			Disp_Timeing = Cov_u16(&record_buff[7]);			/* 运行时间 */
			Disp_Value = Cov_u32(&record_buff[9]);				/* 测试值 */
			Disp_Test_Result = Cov_u16(&record_buff[13]); /* 测试结果 */
		}break;
		case 0x07:
		{
			Disp_Timeing = Cov_u16(&record_buff[7]);			/* 运行时间 */
			Disp_Value = Cov_u32(&record_buff[9]);				/* 测试值 */
		}break;
		case 0x08:
		{
			Disp_Timeing = Cov_u16(&record_buff[7]);			/* 运行时间 */
			Disp_Value = Cov_u32(&record_buff[9]);				/* 测试值 */
		}break;
		default:
		{
		}break;
	}
	if(record_label->hidden == 1)
	{
		lv_obj_set_hidden(record_label, false);
	}
	if(PISI_label->hidden == 1)
	{
		lv_obj_set_hidden(PISI_label, false);
	}
}
/*
**********************************************************************************************
*                                   void ExitReadRecord(void) 
* Description: 退出读记录
* Arguments  : None
* Returns    : None           
**********************************************************************************************
*/
void ExitReadRecord(void)
{
	lv_obj_set_hidden(record_label, true);
	lv_obj_set_hidden(PISI_label, true);	/* 不显示测试结果 */
}
/*
********************************************************************************
*                                   void PgDisp_Tick(void)
* Description:  
* Arguments  : 
* Returns    :            
**********************************************************************************
*/
void PgDisp_Tick(void)
{
	static u32 RefreshTick=0;
	
	/*-----------------------------------------------------------*/
	RefreshTick++;
	
	if(RefreshTick>REFRESH_TIMER)
	{
		RefreshTick=0;
		RefreshEnable=1;               //定时刷新使能
	}
	 
	if(Dis_Dly)
	{
		Dis_Dly--;
	}
}
/*
**********************************************************************************************
*                                   static void RefreshPoll(void)
* Description: 页面刷新逻辑
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void refresh_poll(void)
{
	if(RefreshStep == 0)
	{
		DataRefresh(); //数据刷新
		WholeRefresh(); /* 刷新整个页面 */
		RefreshStep = 1;
	}
	else
	{
		switch(TestState)
		{
			/*----------------------------*/
			case 0: /* 空闲状态 */
			{
				DataRefresh(); //数据刷新
				IdleStatusRefreshDisp();
			}break;
			/*----------------------------*/
			case 1: /* 测试状态 */
			{
				DataRefresh(); //数据刷新
				TestingStatusRefreshDisp();
			}break;
			/*----------------------------*/
			case 2: /*测试结束*/
			{
				/* 数据不再刷新 */
			}break;
			/*----------------------------*/
			case 3: /*阅读记录 */
			{
				DataRefresh();//数据刷新
				RRcordStatusRefreshDisp();
			}break;
			/*----------------------------*/
			default:
			{}break;
		}
	}
}
/*
**********************************************************************************************
*                                   static void IdleStatusRefreshDisp(void)
* Description: 空闲状态的刷新逻辑
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void IdleStatusRefreshDisp(void)
{
	if(TestCompleteRf)
	{
		TestCompleteRf = 0;
		curtime_label_refresh(Disp_Timeing);					/* 刷新运行时间 */
		curtestvalue_label_refresh(Disp_Mode,Disp_Value);		/* 刷新当前值 */
		curvalueunit_label_refresh_lojic(Disp_Mode,Disp_Value);	/* 刷新当前测试值单位 */
		lineScale_refresh_value(Disp_Value);					/* 刷新刻度盘显示 */
	}
	
	if(RecordNeedRefresh)
	{
		ExitReadRecord(); /* 其实就是把读到的记录数据更新到Disp变量里 */
		WholeRefresh(); /* 刷新整个页面 */
		RecordNeedRefresh = 0;
	}
	else
	{
		SettingRefresh();
		MustBeRefresh();
	}
	
	/* 蓝牙/本地处理 */
	BTRefresh();
}
/*
**********************************************************************************************
*                                   static void TestingStatusRefreshDisp(void)
* Description: 测试状态的刷新逻辑
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void TestingStatusRefreshDisp(void)
{
	SettingRefresh();
	MustBeRefresh();
	
	curtime_label_refresh(Disp_Timeing);					/* 刷新运行时间 */
	curtestvalue_label_refresh(Disp_Mode,Disp_Value);		/* 刷新当前值 */
	curvalueunit_label_refresh_lojic(Disp_Mode,Disp_Value);	/* 刷新当前测试值单位 */
	lineScale_refresh_value(Disp_Value);					/* 刷新刻度盘显示 */
	
	if(Disp_Mode == 0x04)
	{
		/* 更新测试结果 */
	}
	else if(Disp_Mode == 0x05)
	{
		if(T2Complete)
		{
			SI_label_refresh(Disp_Test_Result, 1); 
		}
		else
		{
			SI_label_refresh(Disp_Test_Result, 0);
		}
	}
	else if(Disp_Mode == 0x06)
	{
		if(T2Complete)
		{
			PI_label_refresh(Disp_Test_Result, 1);
		}
		else
		{
			PI_label_refresh(Disp_Test_Result, 0);
		}
	}
	
}
/*
**********************************************************************************************
*                                   static void RRcordStatusRefreshDisp(void)
* Description: 读记录状态的刷新逻辑
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void RRcordStatusRefreshDisp(void)
{
	SettingRefresh();
	
	if(RecordNeedRefresh)
	{
		ReadRecord(Disp_WhichRecord); /* 其实就是把读到的记录数据更新到Disp变量里 */
		WholeRefresh(); /* 刷新整个页面 */
		RecordNeedRefresh = 0;
	}
	else
	{
		MustBeRefresh();
	}
	
}
/*
**********************************************************************************************
*                                   static void WholeRefresh(void)
* Description: 全部刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void WholeRefresh(void)
{
	ID_label_refresh(Disp_ID);								/* 刷新ID */
	bugle_label_refresh(Disp_Bugle);						/* 刷新喇叭 */
	battery_icon_refresh(Disp_BatState);					/* 刷新电池 */
	curtime_label_refresh(Disp_Timeing);					/* 刷新运行时间 */
	curtestvalue_label_refresh(Disp_Mode,Disp_Value);		/* 刷新当前值 */
	curvalueunit_label_refresh_lojic(Disp_Mode,Disp_Value);	/* 刷新当前测试值单位 */
	lineScale_refresh_value(Disp_Value);					/* 刷新刻度盘显示 */
	ACorDC_label_refresh_lojic(Disp_Mode);					/* 刷新AC/DC标签 */
	bluetooth_icon_refresh(Disp_Comm);						/* 刷新蓝牙/本地图标 */
	
	if(TestState == 3) /* 读记录 */
	{
		record_label_refresh(Disp_WhichRecord,Disp_records);
	}
	
	lv_obj_set_hidden(PISI_label, true);	/* 不显示测试结果 */
	
	cur_win_selected_refresh(last_win, false);
	cur_win_selected_refresh(cur_win, false);	/* 不选中设置窗口 */
	
	mode_option_refresh(Disp_Mode);	/* 刷新设置模式 */
		 
	vlevel_option_refresh(Disp_Vol); /* 刷新电压等级 */
	cur_win_selected_refresh(last_win, false);
	cur_win_selected_refresh(cur_win, false);	/* 不选中设置窗口 */
	
	/* 刷新时间1/2 */		
	if(Disp_Mode == 0x02) 
	{
		time1_option_refresh(Disp_TimeSS, Disp_Mode, 0);
		time1_option_refresh(Disp_TimeSS, Disp_Mode, 1);
	}
	else if(Disp_Mode == 0x04)
	{
		time1_option_refresh(Disp_CompR, Disp_Mode, cur_switch);
	}
	else if(Disp_Mode == 0x05)
	{
		time1_option_refresh(Disp_XSB_Time1, Disp_Mode, 0);
		time1_option_refresh(Disp_XSB_Time1, Disp_Mode, 1);
		time2_option_refresh(Disp_XSB_Time2, 0);
		time2_option_refresh(Disp_XSB_Time2, 1);
	}
	else if(Disp_Mode == 0x06)
	{
		time1_option_refresh(Disp_JH_Time1, Disp_Mode, 0);
		time1_option_refresh(Disp_JH_Time1, Disp_Mode, 1);
		time2_option_refresh(Disp_JH_Time2, 0);
		time2_option_refresh(Disp_JH_Time2, 1);
	}
}
/*
**********************************************************************************************
*                                   static void MustBeRefresh(void)
* Description: 需不断刷新的部分
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void MustBeRefresh(void)
{
	ID_label_refresh(Disp_ID); /* ID */
	bugle_label_refresh(Disp_Bugle); /* 喇叭 */
	battery_icon_refresh(Disp_BatState); /* 电池容量 */
}
/*
**********************************************************************************************
*                                   static void SettingRefresh(void)
* Description: 设置刷新的部分
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void SettingRefresh(void)
{
	if(SettingFlag)
	{
		SettingFlag = 0;
		cur_win_selected_refresh(last_win, false);
		cur_win_selected_refresh(cur_win, true);
		
		mode_option_refresh(Disp_Mode);
		ACorDC_label_refresh_lojic(Disp_Mode);
		
		/*------------------------------------------------------------------*/
		if(cur_win == TIME1_WIN)
		{
			time1_lable_selected_refresh(cur_switch);
		}
		else if(cur_win == TIME2_WIN)
		{
			time2_lable_selected_refresh(cur_switch);
		}
		/*------------------------------------------------------------------*/
		if((Disp_Mode != 0x07) && (Disp_Mode != 0x08))
		{
			vlevel_option_refresh(Disp_Vol);
			
			if(Disp_Mode == 0x02)
			{
				time1_option_refresh(Disp_TimeSS, Disp_Mode, cur_switch);
			}
			else if(Disp_Mode == 0x04)
			{
				time1_option_refresh(Disp_CompR, Disp_Mode, cur_switch);
			}
			else if(Disp_Mode == 0x05)
			{
				time1_option_refresh(Disp_XSB_Time1, Disp_Mode, cur_switch);
				time2_option_refresh(Disp_XSB_Time2, cur_switch);
			}
			else if(Disp_Mode == 0x06)
			{
				time1_option_refresh(Disp_JH_Time1, Disp_Mode, cur_switch);
				time2_option_refresh(Disp_JH_Time2, cur_switch);
			}
		}
		/*------------------------------------------------------------------*/
	}
}
/*
**********************************************************************************************
*                                   static void BTRefresh(void)
* Description: 蓝牙/本地刷新处理
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
static void BTRefresh(void)
{
	if(Dis_Step == 0)
	{
		if(CommFlag == 1)
		{
			QRcodeOrLocalRefresh(0); /* 请求连接,显示一张二维码图片 */
			Dis_Step = 1;
		}
	}
	else if(Dis_Step == 1)
	{
		if(CommFlag == 2)
		{
			QRcodeOrLocalRefresh(1); /* 连接成功,二维码消失 */
			bluetooth_icon_refresh(Disp_Comm);	/* 刷新蓝牙/本地小图标 */
			Dis_Step = 2;
		}
		else if(CommFlag == 0)
		{
			QRcodeOrLocalRefresh(1); /* 切换到本地,退出连接(还没连上) */
			Dis_Step = 0;
		}
		else
		{
			/* 等待连接 */
		}
	}
	else if(Dis_Step == 2)
	{
		if(CommFlag == 0)
		{
			QRcodeOrLocalRefresh(2); /* 切换到本地,显示本地小窗口,退出连接 */
			bluetooth_icon_refresh(Disp_Comm);	/* 刷新蓝牙/本地小图标 */
			Dis_Dly = 200;
			Dis_Step = 3;
		}	
	}
	else if(Dis_Step == 3)
	{
		if(Dis_Dly == 0)
		{
			QRcodeOrLocalRefresh(3);
			Dis_Step = 0;
		}
	}
}
