/***********************************************************************
文件名称：pg_rf.h
功	  能：页面刷新
编写时间：2018.08.20
编 写 人：wuliu
注    意：
***********************************************************************/
#ifndef _PG_RF_H_
#define _PG_RF_H_

#include "main.h"

typedef enum
{
	NONE_WIN,	
	MODE_WIN,		//模式
	VLEVEL_WIN,	//电压等级
	TIME1_WIN,	//时间1
	TIME2_WIN,	//时间2
}WIN;

typedef enum
{
	TIMING = 0,					//定时
	CONTINUOUS,					//连续
	COMPARE,						//比较
	ABSORPTANCE,				//吸收比
	POLARIZATION_INDEX,	//极化指数
	DIRECT_VOLTAGE,			//直流电压
	ALTERNATING_VOLTAGE,//交流电压
}MODE;

typedef enum
{
	LEVEL_500,
	LEVEL_1000,
	LEVEL_1500,
	LEVEL_2500,
}VLEVEL;


typedef enum
{
	AC,
	DC,
	OTHER,
}AC_DC;

typedef enum
{
	BAT_EMPTY,	//没电了
	BAT_1,			//1/4电量
	BAT_2,			//1/2电量
	BAT_3,			//3/4电量
	BAT_FULL,		//满电状态
}BAT;

/*
********************************************************************************
*                                   void PgDisp_Tick(void)
* Description:  
* Arguments  : 
* Returns    :            
**********************************************************************************
*/
void PgDisp_Tick(void);
/*
**********************************************************************************************
*                                   void refresh_poll(void) 
* Description: 刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void refresh_poll(void);
/*
**********************************************************************************************
*                       void mode_option_refresh(u8 mode)
* Description: 模式窗口选项刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void mode_option_refresh(u8 mode);
/*
**********************************************************************************************
*                       void vlevel_option_refresh(u8 Vlevel)
* Description: 电压等级窗口选项刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void vlevel_option_refresh(u8 Vlevel);
/*
**********************************************************************************************
*                     void time1_option_refresh(u32 Data, u8 which_mode , u8 which_switch)
* Description: 时间1窗口选项刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time1_option_refresh(u32 Data, u8 which_mode , u8 which_switch);
/*
**********************************************************************************************
*                       void time2_option_refresh(u32 Data, u8 which_switch)
* Description: 时间1窗口选项刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time2_option_refresh(u32 Data, u8 which_switch);
/*
**********************************************************************************************
*                                   void modeframe_refresh(bool mode) 
* Description: 模式窗口刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void modeframe_refresh(bool mode);
/*
**********************************************************************************************
*                                   void vlevelframe_refresh(bool mode) 
* Description: 电压等级窗口刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void vlevelframe_refresh(bool mode);
/*
**********************************************************************************************
*                             void time1frame_refresh(bool mode, u8 which_mode) 
* Description: 时间1窗口刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time1frame_refresh(bool mode, u8 which_mode);
/*
**********************************************************************************************
*                                   void time2frame_refresh(bool mode) 
* Description: 时间2窗口刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time2frame_refresh(bool mode);
/*
**********************************************************************************************
*                                   void time1_lable_selected_refresh(u8 data) 
* Description: 时间2窗口刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time1_lable_selected_refresh(u8 data);
/*
**********************************************************************************************
*                                   void time2_lable_selected_refresh(u8 data) 
* Description: 时间2窗口刷新
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time2_lable_selected_refresh(u8 data);

#endif


