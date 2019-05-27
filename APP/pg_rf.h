/***********************************************************************
�ļ����ƣ�pg_rf.h
��	  �ܣ�ҳ��ˢ��
��дʱ�䣺2018.08.20
�� д �ˣ�wuliu
ע    �⣺
***********************************************************************/
#ifndef _PG_RF_H_
#define _PG_RF_H_

#include "main.h"

typedef enum
{
	NONE_WIN,	
	MODE_WIN,		//ģʽ
	VLEVEL_WIN,	//��ѹ�ȼ�
	TIME1_WIN,	//ʱ��1
	TIME2_WIN,	//ʱ��2
}WIN;

typedef enum
{
	TIMING = 0,					//��ʱ
	CONTINUOUS,					//����
	COMPARE,						//�Ƚ�
	ABSORPTANCE,				//���ձ�
	POLARIZATION_INDEX,	//����ָ��
	DIRECT_VOLTAGE,			//ֱ����ѹ
	ALTERNATING_VOLTAGE,//������ѹ
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
	BAT_EMPTY,	//û����
	BAT_1,			//1/4����
	BAT_2,			//1/2����
	BAT_3,			//3/4����
	BAT_FULL,		//����״̬
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
* Description: ˢ��
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void refresh_poll(void);
/*
**********************************************************************************************
*                       void mode_option_refresh(u8 mode)
* Description: ģʽ����ѡ��ˢ��
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void mode_option_refresh(u8 mode);
/*
**********************************************************************************************
*                       void vlevel_option_refresh(u8 Vlevel)
* Description: ��ѹ�ȼ�����ѡ��ˢ��
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void vlevel_option_refresh(u8 Vlevel);
/*
**********************************************************************************************
*                     void time1_option_refresh(u32 Data, u8 which_mode , u8 which_switch)
* Description: ʱ��1����ѡ��ˢ��
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time1_option_refresh(u32 Data, u8 which_mode , u8 which_switch);
/*
**********************************************************************************************
*                       void time2_option_refresh(u32 Data, u8 which_switch)
* Description: ʱ��1����ѡ��ˢ��
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time2_option_refresh(u32 Data, u8 which_switch);
/*
**********************************************************************************************
*                                   void modeframe_refresh(bool mode) 
* Description: ģʽ����ˢ��
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void modeframe_refresh(bool mode);
/*
**********************************************************************************************
*                                   void vlevelframe_refresh(bool mode) 
* Description: ��ѹ�ȼ�����ˢ��
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void vlevelframe_refresh(bool mode);
/*
**********************************************************************************************
*                             void time1frame_refresh(bool mode, u8 which_mode) 
* Description: ʱ��1����ˢ��
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time1frame_refresh(bool mode, u8 which_mode);
/*
**********************************************************************************************
*                                   void time2frame_refresh(bool mode) 
* Description: ʱ��2����ˢ��
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time2frame_refresh(bool mode);
/*
**********************************************************************************************
*                                   void time1_lable_selected_refresh(u8 data) 
* Description: ʱ��2����ˢ��
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time1_lable_selected_refresh(u8 data);
/*
**********************************************************************************************
*                                   void time2_lable_selected_refresh(u8 data) 
* Description: ʱ��2����ˢ��
* Arguments  : 
* Returns    :            
**********************************************************************************************
*/
void time2_lable_selected_refresh(u8 data);

#endif


