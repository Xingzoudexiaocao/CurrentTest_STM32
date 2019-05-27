/***********************************************************************
�ļ����ƣ�pg_ct.h
��	  �ܣ�ҳ�洴��
��дʱ�䣺2018.08.20
�� д �ˣ�wuliu
ע    �⣺
***********************************************************************/
#ifndef _PG_CT_H_
#define _PG_CT_H_

#include "../lvgl/lvgl.h"

#define MAINWIN_WIDTH	440		//�����ڵĿ��
#define MAINWIN_HIGHT	210		//�����ڵĸ߶�
#define MAINWIN_POSX	20		//�����ڵ�X����
#define MAINWIN_POSY	35		//�����ڵ�Y����

#define LINESCALE_WIDTH	440		//ֱ�߿̶��̿��
#define LINESCALE_HIGHT	50		//ֱ�߿̶��̸߶�

#define MODEFRAME_WIDTH		95		//"ģʽ"���ڵĿ��
#define MODEFRAME_HIGHT		50		//"ģʽ"���ڵĸ߶�
#define MODEFRAME_POSX		20		//"ģʽ"���ڵ�X����
#define MODEFRAME_POSY		260		//"ģʽ"���ڵ�Y����

#define VLEVELFRAME_WIDTH	MODEFRAME_WIDTH
#define VLEVELFRAME_HIGHT	MODEFRAME_HIGHT
#define VLEVELFRAME_POSX	MODEFRAME_POSX+MODEFRAME_WIDTH+20
#define VLEVELFRAME_POSY	MODEFRAME_POSY

#define TIME1FRAME_WIDTH	VLEVELFRAME_WIDTH
#define TIME1FRAME_HIGHT	VLEVELFRAME_HIGHT
#define TIME1FRAME_POSX		VLEVELFRAME_POSX+VLEVELFRAME_WIDTH+20
#define TIME1FRAME_POSY		VLEVELFRAME_POSY

#define TIME2FRAME_WIDTH	TIME1FRAME_WIDTH
#define TIME2FRAME_HIGHT	TIME1FRAME_HIGHT
#define TIME2FRAME_POSX		TIME1FRAME_POSX+TIME1FRAME_WIDTH+20
#define TIME2FRAME_POSY		TIME1FRAME_POSY

#define COMMFRAME_WIDTH		TIME2FRAME_WIDTH
#define COMMFRAME_HIGHT		TIME2FRAME_HIGHT
#define COMMFRAME_POSX		TIME2FRAME_POSX+TIME2FRAME_WIDTH+15
#define COMMFRAME_POSY		TIME2FRAME_POSY

#define LOCALFRAME_WIDTH		100
#define LOCALFRAME_HIGHT		100
#define LOCALFRAME_POSX			(480 - 100)/2
#define LOCALFRAME_POSY			(320 - 100)/2

extern lv_obj_t * bg;
extern lv_obj_t * sit_img;
extern lv_obj_t * mainwin;
extern lv_obj_t * lineScale;
extern lv_obj_t * ACorDC_label;
extern lv_obj_t * battery_label;
extern lv_obj_t * bugle_label;
extern lv_obj_t * ID_label;
extern lv_obj_t * curtime_label;
extern lv_obj_t * curtestvalue_label;
extern lv_obj_t * curvalueunit_label;
extern lv_obj_t * PISI_label;
extern lv_obj_t * record_label;
extern lv_obj_t * bluetooth_label;

extern lv_obj_t * modeframe;		
extern lv_obj_t * mode_title;
extern lv_obj_t * mode_label;		

extern lv_obj_t * vlevelframe;
extern lv_obj_t * vlevel_title;
extern lv_obj_t * vlevel_label;

extern lv_obj_t * time1frame;
extern lv_obj_t * time1_title;
extern lv_obj_t * time1_label;
extern lv_obj_t * time1_min_label;
extern lv_obj_t * time1_mid_label;
extern lv_obj_t * time1_sec_label;

extern lv_obj_t * time2frame;
extern lv_obj_t * time2_title;
extern lv_obj_t * time2_min_label;
extern lv_obj_t * time2_mid_label;
extern lv_obj_t * time2_sec_label;

extern lv_obj_t * communicateframe;
extern lv_obj_t * communicate_title;

extern lv_style_t style_bg;
extern lv_style_t sit_style;
extern lv_style_t style_mainwin;

extern lv_style_t ACorDC_style;
extern lv_style_t battery_style;
extern lv_style_t bugle_style;
extern lv_style_t ID_style;
extern lv_style_t curtime_style;
extern lv_style_t curtestvalue_style;
extern lv_style_t curvalueunit_style;
extern lv_style_t PISI_style;
extern lv_style_t record_style;
extern lv_style_t bluetooth_style;

extern lv_style_t modeframe_style;	
extern lv_style_t mode_label_style;	

extern lv_style_t vlevelframe_style;
extern lv_style_t vlevel_label_style;

extern lv_style_t time1frame_style;
extern lv_style_t time1_label_style;
extern lv_style_t time1_min_label_style;
extern lv_style_t time1_mid_label_style;
extern lv_style_t time1_sec_label_style;

extern lv_style_t time2frame_style;
extern lv_style_t time2_min_label_style;
extern lv_style_t time2_mid_label_style;
extern lv_style_t time2_sec_label_style;

extern lv_style_t communicateframe_style;

extern lv_obj_t * QRcode_img;
extern lv_obj_t * localframe;
extern lv_obj_t * local_title;

extern lv_style_t localframe_style;

/*
**********************************************************************************************
*                                   void page_create(void)
* Description: ����һ��ҳ��
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void page_create(void);

#endif
