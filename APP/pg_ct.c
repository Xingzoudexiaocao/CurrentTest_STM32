/***********************************************************************
文件名称：pg_ct.c
功	  能：页面创建
编写时间：2018.08.20
编 写 人：wuliu
注    意：
***********************************************************************/

#include "main.h"
#include "pg_ct.h"
#include "../lvgl/lvgl.h"

extern lv_img_t sit;
extern lv_img_t QRcode;
/*----------------------------------------------------*/
lv_obj_t * bg;
lv_obj_t * sit_img;
lv_obj_t * mainwin;
lv_obj_t * lineScale;
lv_obj_t * ACorDC_label;
lv_obj_t * battery_label;
lv_obj_t * bugle_label;
lv_obj_t * ID_label;
lv_obj_t * curtime_label;
lv_obj_t * curtestvalue_label;
lv_obj_t * curvalueunit_label;
lv_obj_t * PISI_label;
lv_obj_t * record_label;
lv_obj_t * bluetooth_label;

lv_obj_t * modeframe;		//定义一个"模式"窗口对象
lv_obj_t * mode_title;
lv_obj_t * mode_label;		//定义一个模式选项标签

lv_obj_t * vlevelframe;
lv_obj_t * vlevel_title;
lv_obj_t * vlevel_label;

lv_obj_t * time1frame;
lv_obj_t * time1_title;
lv_obj_t * time1_label;
lv_obj_t * time1_min_label;
lv_obj_t * time1_mid_label;
lv_obj_t * time1_sec_label;

lv_obj_t * time2frame;
lv_obj_t * time2_title;
lv_obj_t * time2_min_label;
lv_obj_t * time2_mid_label;
lv_obj_t * time2_sec_label;

lv_obj_t * communicateframe;
lv_obj_t * communicate_title;

lv_obj_t * QRcode_img;
lv_obj_t * localframe;
lv_obj_t * local_title;

lv_style_t style_bg;
lv_style_t sit_style;
lv_style_t style_mainwin;

lv_style_t ACorDC_style;
lv_style_t battery_style;
lv_style_t bugle_style;
lv_style_t ID_style;
lv_style_t curtime_style;
lv_style_t curtestvalue_style;
lv_style_t curvalueunit_style;
lv_style_t PISI_style;
lv_style_t record_style;
lv_style_t bluetooth_style;

lv_style_t modeframe_style;	//为"模式"窗口对象创建一种风格
lv_style_t mode_label_style;	//为模式选项标签创建一种风格

lv_style_t vlevelframe_style;
lv_style_t vlevel_label_style;

lv_style_t time1frame_style;
lv_style_t time1_label_style;
lv_style_t time1_min_label_style;
lv_style_t time1_mid_label_style;
lv_style_t time1_sec_label_style;

lv_style_t time2frame_style;
lv_style_t time2_min_label_style;
lv_style_t time2_mid_label_style;
lv_style_t time2_sec_label_style;

lv_style_t communicateframe_style;

lv_style_t localframe_style;

const char *Scale_text[10] = {"0","100K","1M","10M","100M","1G","10G","100G","1T","∞"}; //标签

/*
**********************************************************************************************
*                                   void bg_create(void)
* Description: 创建一个背景
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void bg_create(void)
{
	/*背景风格*/
	lv_style_copy(&style_bg, &lv_style_plain);
	style_bg.body.main_color = LV_COLOR_MAKE(229,228,236);
	style_bg.body.grad_color = LV_COLOR_MAKE(213,213,213);
	style_bg.body.radius = 0;
	style_bg.body.border.width = 0;
	style_bg.body.border.color =  LV_COLOR_MAKE(213,213,213);
	style_bg.body.shadow.color = LV_COLOR_SILVER;
	style_bg.body.opa = LV_OPA_80;

	/*创建一个背景*/
	bg = lv_cont_create(lv_scr_act(), NULL);
	lv_cont_set_style(bg, &style_bg);
	lv_obj_set_size(bg, LV_HOR_RES, LV_VER_RES);
}
/*
**********************************************************************************************
*                                   void sit_img_create(void)
* Description: 创建一个思泰的图标
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void sit_img_create(void)
{
	lv_style_copy(&sit_style, &lv_style_pretty);

	sit_img = lv_img_create(bg, NULL);
	lv_img_set_src(sit_img, &sit);
//	lv_obj_set_size(sit_img,70,39);

	lv_img_set_style(sit_img, &sit_style);
	lv_obj_align(sit_img, bg , LV_ALIGN_IN_TOP_LEFT, 5, 3);
}
/*
**********************************************************************************************
*                                   void ID_label_create(void)
* Description: 创建一个编号的标签
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void ID_label_create(void)
{
	lv_style_copy(&ID_style, &lv_style_pretty);

	ID_label = lv_label_create(bg, NULL);
	lv_obj_align(ID_label, bg , LV_ALIGN_IN_TOP_LEFT, 75, 3);

	lv_label_set_text(ID_label, "ABC-100");
	lv_label_set_style(ID_label, &ID_style);
}
/*
**********************************************************************************************
*                                   void record_label_create(void)
* Description: 创建一个记录的标签
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void record_label_create(void)
{
	lv_style_copy(&record_style, &lv_style_pretty);
	record_style.text.font = &lv_font_myfont_20;

	record_label = lv_label_create(bg, NULL);
	lv_obj_align(record_label, bg , LV_ALIGN_IN_TOP_LEFT, 170, 3);

	lv_label_set_text(record_label, "记录 1/30");
	lv_label_set_style(record_label, &record_style);
	lv_obj_set_hidden(record_label, true);
}
/*
 **********************************************************************************************
 *                                   void bluetooth_icon_create(void)
 * Description: 创建一个蓝牙/本地的图标(SYMBOL_BATTERY_FULL)
 * Arguments  :
 * Returns    :
 **********************************************************************************************
 */
 void bluetooth_icon_create(void)
 {
 	lv_style_copy(&bluetooth_style, &lv_style_pretty);

 	bluetooth_label = lv_label_create(bg, NULL);

 	lv_label_set_text(bluetooth_label, SYMBOL_BLUETOOTH);
 	lv_label_set_style(bluetooth_label, &bluetooth_style);
	lv_obj_align(bluetooth_label, bg , LV_ALIGN_IN_TOP_RIGHT, -72, 3);
 }
 /*
**********************************************************************************************
*                                   void bugle_icon_create(void)
* Description: 创建一个喇叭的图标(SYMBOL_VOLUME_MAX)
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void bugle_icon_create(void)
{
	lv_style_copy(&bugle_style, &lv_style_pretty);

	bugle_label = lv_label_create(bg, NULL);

	lv_label_set_text(bugle_label, SYMBOL_VOLUME_MAX);
	lv_label_set_style(bugle_label, &bugle_style);
	lv_obj_align(bugle_label, bg , LV_ALIGN_IN_TOP_RIGHT, -40, 3);
}
/*
**********************************************************************************************
*                                   void battery_icon_create(void)
* Description: 创建一个电池的图标(SYMBOL_BATTERY_FULL)
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void battery_icon_create(void)
{
	lv_style_copy(&battery_style, &lv_style_pretty);

	battery_label = lv_label_create(bg, NULL);

	lv_label_set_text(battery_label, SYMBOL_BATTERY_1);
	lv_label_set_style(battery_label, &battery_style);
	lv_obj_align(battery_label, bg , LV_ALIGN_IN_TOP_RIGHT, -5, 3);
}
/*
**********************************************************************************************
*                                   void mainwin_create(void)
* Description: 创建一个主窗口
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void mainwin_create(void)
{
	/*主窗口的风格*/
	lv_style_copy(&style_mainwin, &lv_style_plain);
	style_mainwin.body.main_color = LV_COLOR_MAKE(229,228,236);
	style_mainwin.body.grad_color = LV_COLOR_MAKE(229,228,236);//LV_COLOR_MAKE(213,213,213);
	style_mainwin.body.radius = 12;
	style_mainwin.body.border.width = 6;
//	style_mainwin.body.opa = LV_OPA_100;
	style_mainwin.body.border.color = LV_COLOR_GREEN;//LV_COLOR_MAKE(17,28,46);
	style_mainwin.body.shadow.width = 0;//5;
	style_mainwin.body.shadow.color = LV_COLOR_GREEN;//LV_COLOR_MAKE(17,28,46);
	
//	style_mainwin.body.main_color = LV_COLOR_HEX(0xf0f0f0);
//	style_mainwin.body.grad_color = LV_COLOR_HEX(0xf0f0f0);
//	style_mainwin.body.radius = 2;
//	style_mainwin.body.border.width = 0;
//	style_mainwin.body.opa = LV_OPA_100;
//	style_mainwin.body.border.color = LV_COLOR_GREEN;
//	style_mainwin.body.border.width = 2;

	/*创建一个背景*/
	mainwin = lv_cont_create(bg, NULL);
	lv_cont_set_style(mainwin, &style_mainwin);
	lv_obj_set_size(mainwin, MAINWIN_WIDTH, MAINWIN_HIGHT);
	lv_obj_set_pos(mainwin, MAINWIN_POSX, MAINWIN_POSY);
}
/*
**********************************************************************************************
*                                   void ACorDC_label_creat(void)
* Description: 创建一个AC/DC的标签
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void ACorDC_label_creat(void)
{
	lv_style_copy(&ACorDC_style, &lv_style_pretty);

	ACorDC_label = lv_label_create(mainwin, NULL);
	lv_obj_align(ACorDC_label, mainwin , LV_ALIGN_IN_TOP_LEFT, 20, 10);
	lv_label_set_text(ACorDC_label, "AC");
	lv_label_set_style(ACorDC_label, &ACorDC_style);

}
/*
**********************************************************************************************
*                                   void curtime_label_create(void)
* Description: 创建一个当前时间的标签
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void curtime_label_create(void)
{
	lv_style_copy(&curtime_style, &lv_style_pretty);

	curtime_label = lv_label_create(mainwin, NULL);
	lv_obj_align(curtime_label, mainwin , LV_ALIGN_IN_TOP_LEFT, 20, 40);

	lv_label_set_text(curtime_label, "17:56");
	lv_label_set_style(curtime_label, &curtime_style);
}
/*
**********************************************************************************************
*                                   void curtestvalue_label_creat(void)
* Description: 创建一个当前测试值的标签
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void curtestvalue_label_create(void)
{
	lv_style_copy(&curtestvalue_style, &lv_style_pretty);
	curtestvalue_style.text.font = &lv_font_myfont_86;
	curtestvalue_label = lv_label_create(mainwin, NULL);

	lv_label_set_text(curtestvalue_label, "100.56");
	lv_label_set_style(curtestvalue_label, &curtestvalue_style);
	lv_obj_align(curtestvalue_label, mainwin , LV_ALIGN_IN_TOP_MID, 5, 10);
}
/*
**********************************************************************************************
*                                   void curvalueunit_label_create(void)
* Description: 创建一个当前测试值单位的标签
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void curvalueunit_label_create(void)
{
	lv_style_copy(&curvalueunit_style, &lv_style_pretty);
	curvalueunit_style.text.font = &lv_font_myfont_20;

	curvalueunit_label = lv_label_create(mainwin, NULL);

	lv_label_set_text(curvalueunit_label, "MΩ");

	lv_label_set_style(curvalueunit_label, &curvalueunit_style);
	lv_obj_align(curvalueunit_label, mainwin , LV_ALIGN_IN_TOP_RIGHT, -10, 75);
}
/*
**********************************************************************************************
*                                   void PISI_label_create(void)
* Description: 创建一个当前PI/SI的标签
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void PISI_label_create(void)
{
	lv_style_copy(&PISI_style, &lv_style_pretty);

	PISI_label = lv_label_create(mainwin, NULL);

	lv_label_set_text(PISI_label, "PI: 11.25");
	lv_label_set_style(PISI_label, &PISI_style);
	lv_obj_align(PISI_label, mainwin , LV_ALIGN_IN_TOP_MID,  -10, 100);
}
/*
**********************************************************************************************
*      bool lineScale_designed(lv_obj_t *lineScale,const lv_areat *mask,lv_design_mode_t mode)
* Description: 绘制直线刻度盘函数
* Arguments  :
* Returns    :
**********************************************************************************************
*/
bool lineScale_designed(lv_obj_t *lineScale,const lv_area_t *mask,lv_design_mode_t mode)
{
	if(mode == LV_DESIGN_COVER_CHK)
	{
		return false;
	}
	else if(mode == LV_DESIGN_DRAW_MAIN)
	{
		uint8_t i;

		uint8_t line_cnt = 46;

		uint32_t min;
		uint32_t max;
		int16_t level;

		uint32_t *ext = lv_obj_get_ext_attr(lineScale);
		uint32_t temp = *ext;

		if(temp < 10) //0-100K
		{
			min = 0;
			max = 10;

			level = (temp - min) * 6 / (max - min);
		}
		else if(temp < 100) // 100K-1M
		{
			min = 10;
			max = 100;

			level = 6 + (temp - min) * 6 / (max - min);
		}
		else if(temp < 1000) //1M-10M
		{
			min = 100;
			max = 1000;

			level = 11 + (temp - min) * 6 / (max - min);
		}
		else if(temp < 10000) //10M-100M
		{
			min = 1000;
			max = 10000;

			level = 16 + (temp - min) * 6 / (max - min);
		}
		else if(temp < 100000) //100M-1G
		{
			min = 10000;
			max = 100000;

			level = 21 + (temp - min) * 6 / (max - min);
		}
		else if(temp < 1000000) //1G-10G
		{
			min = 100000;
			max = 1000000;

			level = 26 + (temp - min) * 6 / (max - min);
		}
		else if(temp <= 10000000) //10G-100G
		{
			min = 1000000;
			max = 10000000;

			level = 31 + (temp - min) * 6 / (max - min);
		}
		else
		{
			level = line_cnt;
		}

		/*创建一种线的风格*/
		lv_style_t line_style;
		lv_style_copy(&line_style,&lv_style_plain);
		line_style.body.main_color = LV_COLOR_GREEN;
		line_style.body.grad_color = LV_COLOR_ORANGE;
		line_style.line.color = LV_COLOR_MAKE(162,162,162);;
		line_style.line.width = 4;

		/*定义线的另种风格*/
		lv_style_t linetemp_style;
		lv_style_copy(&linetemp_style,&line_style);

		/*创建一种标签的风格*/
		lv_style_t lable_style;
		lv_style_copy(&lable_style,&lv_style_plain);
//		lable_style.text.font = &lv_font_myfont_20;

		/*定义线的两个点*/
		lv_point_t p1;
		lv_point_t p2;

		/*定义标签的面积及右下角点*/
		lv_area_t  label_cord;
		lv_point_t lable_size;


		/*画线*/
		for(i = 0; i < line_cnt; i++)
		{
			if(i == 0)
			{
				p1.x = lineScale->coords.x1;
				p2.x = p1.x;
			}
			else
			{
				p1.x +=  lv_obj_get_width(lineScale)/line_cnt;
				p2.x = p1.x;
			}

			if(i%5)
			{
				p1.y = lineScale->coords.y1 + 15 + 9;
				p2.y = lineScale->coords.y2;
			}
			else
			{
				p1.y = lineScale->coords.y1 + 15;
				p2.y = lineScale->coords.y2;
			}

			if(i >= level)
			{
				linetemp_style.line.color = line_style.line.color;
			}
			else
			{
				linetemp_style.line.color = lv_color_mix(line_style.body.grad_color, line_style.body.main_color, (255 * i) / line_cnt);
			}

			lv_draw_line(&p1, &p2, mask, &linetemp_style);
		}
		/*绘制标签*/
		for(i = 0; i < 10; i++)
		{

			label_cord.y1 = lineScale->coords.y1;
			label_cord.y2 = label_cord.y1 + 30;
			
			if(i == 9)
			{
				lable_style.text.font = &lv_font_myfont_20;
			}
			else
			{
				lable_style.text.font = &lv_font_myfont_16;
			}

			lv_txt_get_size(&lable_size , Scale_text[i], lable_style.text.font,
			lable_style.text.letter_space, lable_style.text.line_space, LV_COORD_MAX, LV_TXT_FLAG_NONE);
		

			if(i == 0)
			{
				label_cord.x1 = lineScale->coords.x1;
				label_cord.x2 = label_cord.x1 + 30;

			}
			else
			{
				label_cord.x1 = lineScale->coords.x1 + (lv_obj_get_width(lineScale)/46 * 5 * i - (lable_size.x/2) + 3);
				label_cord.x2 = label_cord.x1 + 60;
			}

			lv_draw_label(&label_cord, mask, &lable_style, Scale_text[i], LV_TXT_FLAG_NONE, NULL);
		}
	}

	return true;
}
/*
**********************************************************************************************
*                            void lineScale_create(void)
* Description: 创建一个直线刻度盘对象,设置其显示的位置及其大小(宽度和高度),并增加了个扩展数据
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void lineScale_create(void)
{
	lineScale = lv_obj_create(mainwin, NULL);
	uint32_t *ext = lv_obj_allocate_ext_attr(lineScale, sizeof(uint32_t));
	*ext = 10000;
	lv_obj_set_size(lineScale, LINESCALE_WIDTH, LINESCALE_HIGHT);
	lv_obj_set_design_func(lineScale, lineScale_designed);
	lv_obj_align(lineScale, mainwin , LV_ALIGN_IN_BOTTOM_LEFT, 15, -15);
}
/*
**********************************************************************************************
*                                   void mode_label_create(void)
* Description: 创建一个模式选项标签
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void mode_label_create(void)
{
	lv_style_copy(&mode_label_style ,&lv_style_pretty);
	mode_label_style.text.font = &lv_font_myfont_20;

	mode_label = lv_label_create(modeframe, NULL);
	lv_label_set_text(mode_label, "连续");
	lv_obj_align(mode_label, modeframe, LV_ALIGN_CENTER, 0, 10);
	lv_label_set_style(mode_label, &mode_label_style);
}
/*
**********************************************************************************************
*                                   void modeframe_creat(void)
* Description: 创建一个“模式”框图
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void modeframe_creat(void)
{
	lv_style_copy(&modeframe_style ,&lv_style_pretty);
	modeframe_style.body.main_color = LV_COLOR_HEX(0xf0f0f0);
	modeframe_style.body.grad_color = LV_COLOR_HEX(0xf0f0f0);
	modeframe_style.text.font = &lv_font_myfont_20;
	modeframe_style.body.radius = 2;
	modeframe_style.body.border.width = 1;
//	modeframe_style.body.border.color = LV_COLOR_GREEN;
//	modeframe_style.body.border.opa = 200;
	modeframe_style.text.font = &lv_font_myfont_20;

	modeframe = lv_cont_create(bg, NULL);
	lv_obj_set_pos(modeframe, MODEFRAME_POSX, MODEFRAME_POSY);
	lv_obj_set_size(modeframe, MODEFRAME_WIDTH, MODEFRAME_HIGHT);
	lv_cont_set_style(modeframe, &modeframe_style);

	mode_title = lv_label_create(modeframe, NULL);
	lv_label_set_text(mode_title, "模式");
	lv_label_set_style(mode_title, &modeframe_style);
	lv_obj_align(mode_title, modeframe, LV_ALIGN_IN_TOP_MID, 2, 5);

	mode_label_create();
}
/*
**********************************************************************************************
*                                   void vlevel_label_create(void)
* Description: 创建一个电压等级选项标签
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void vlevel_label_create(void)
{
	lv_style_copy(&vlevel_label_style ,&lv_style_pretty);
//	vlevel_label_style.text.font = &lv_font_hanzi_20;
//	vlevel_label_style.text.color = LV_COLOR_BLUE;

	vlevel_label = lv_label_create(vlevelframe, NULL);
	lv_label_set_text(vlevel_label, "1500V");
	lv_obj_align(vlevel_label, vlevelframe, LV_ALIGN_CENTER, 2, 10);
	lv_label_set_style(vlevel_label, &vlevel_label_style);
}
/*
**********************************************************************************************
*                                   void vlevelframe_creat(void)
* Description: 创建一个“电压等级”框图
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void vlevelframe_creat(void)
{
	lv_style_copy(&vlevelframe_style ,&lv_style_pretty);
	vlevelframe_style.body.main_color = LV_COLOR_HEX(0xf0f0f0);
	vlevelframe_style.body.grad_color = LV_COLOR_HEX(0xf0f0f0);
	vlevelframe_style.text.font = &lv_font_myfont_20;
	vlevelframe_style.text.color = LV_COLOR_BLACK;
	vlevelframe_style.body.radius = 2;

	vlevelframe = lv_cont_create(bg, NULL);
	lv_obj_set_pos(vlevelframe, VLEVELFRAME_POSX, VLEVELFRAME_POSY);
	lv_obj_set_size(vlevelframe, VLEVELFRAME_WIDTH, VLEVELFRAME_HIGHT);
	lv_cont_set_style(vlevelframe, &vlevelframe_style);

	vlevel_title = lv_label_create(vlevelframe, NULL);
	lv_label_set_text(vlevel_title, "电压等级");
	lv_label_set_style(vlevel_title, &vlevelframe_style);
	lv_obj_align(vlevel_title, vlevelframe, LV_ALIGN_IN_TOP_MID, 2, 5);

	vlevel_label_create();
}
/*
**********************************************************************************************
*                                   void time1_label_create(void)
* Description: 创建一个时间1选项标签
* Arguments  :
* Returns    :
**********************************************************************************************
*/

void time1_label_create(void)
{
	/*比较电阻值的风格*/
	lv_style_copy(&time1_label_style ,&lv_style_pretty);
//	time1_label_style.text.font = &lv_font_hanzi_20;
//	time1_label_style.text.color = LV_COLOR_BLUE;
	
	/*时间分钟的风格*/
	lv_style_copy(&time1_min_label_style ,&lv_style_pretty);
//	time1_min_label_style.text.font = &lv_font_hanzi_20;
//	time1_min_label_style.text.color = LV_COLOR_BLUE;
	
	/*时间“:”的风格*/
	lv_style_copy(&time1_mid_label_style ,&lv_style_pretty);
//	time1_mid_label_style.text.font = &lv_font_hanzi_20;
//	time1_mid_label_style.text.color = LV_COLOR_BLUE;
	
	/*时间秒的风格*/
	lv_style_copy(&time1_sec_label_style ,&lv_style_pretty);
//	time1_sec_label_style.text.font = &lv_font_hanzi_20;
//	time1_sec_label_style.text.color = LV_COLOR_BLUE;

	/*比较电阻值标签*/
	time1_label = lv_label_create(time1frame, NULL);
	
	/*时间值的标签*/
	time1_min_label = lv_label_create(time1frame, NULL);	
	time1_mid_label = lv_label_create(time1frame, NULL);
	time1_sec_label = lv_label_create(time1frame, NULL);
	
	/*比较电阻值*/
	lv_label_set_text(time1_label, "300");
	lv_label_set_style(time1_label, &time1_label_style);
	lv_obj_align(time1_label, time1frame, LV_ALIGN_CENTER, 2, 10);
	
	lv_obj_set_hidden(time1_label, true);
	
	/*时间*/
	lv_label_set_text(time1_min_label, "17");
	lv_label_set_style(time1_min_label, &time1_min_label_style);
	lv_obj_align(time1_min_label, time1frame, LV_ALIGN_CENTER, -14, 10);
	
	lv_label_set_text(time1_mid_label, ":");
	lv_label_set_style(time1_mid_label, &time1_mid_label_style);
	lv_obj_align(time1_mid_label, time1_min_label, LV_ALIGN_OUT_RIGHT_TOP, 1, 0);
	
	lv_label_set_text(time1_sec_label, "26");
	lv_label_set_style(time1_sec_label, &time1_sec_label_style);
	lv_obj_align(time1_sec_label, time1_mid_label, LV_ALIGN_OUT_RIGHT_TOP, 1, 0);
		
}
/*
**********************************************************************************************
*                                   void timeframe_create(void)
* Description: 创建一个“时间1”框图
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void time1frame_create(void)
{
	lv_style_copy(&time1frame_style ,&lv_style_pretty);
	time1frame_style.body.main_color = LV_COLOR_HEX(0xf0f0f0);
	time1frame_style.body.grad_color = LV_COLOR_HEX(0xf0f0f0);
	time1frame_style.text.font = &lv_font_myfont_20;
	time1frame_style.text.color = LV_COLOR_BLACK;
	time1frame_style.body.radius = 2;

	time1frame = lv_cont_create(bg, NULL);
	lv_obj_set_pos(time1frame, TIME1FRAME_POSX, TIME1FRAME_POSY);
	lv_obj_set_size(time1frame, TIME1FRAME_WIDTH, TIME1FRAME_HIGHT);
	lv_cont_set_style(time1frame, &time1frame_style);

	time1_title = lv_label_create(time1frame, NULL);
	lv_label_set_text(time1_title, "时间1");
	lv_label_set_style(time1_title, &time1frame_style);
	lv_obj_align(time1_title, time1frame, LV_ALIGN_IN_TOP_MID, 2, 5);

	time1_label_create();
}
/*
**********************************************************************************************
*                                   void vlevel_label_create(void)
* Description: 创建一个时间2选项标签
* Arguments  :
* Returns    :
**********************************************************************************************
*/

void time2_label_create(void)
{
	/*时间分钟的风格*/
	lv_style_copy(&time2_min_label_style ,&lv_style_pretty);
//	time2_min_label_style.text.font = &lv_font_hanzi_20;
//	time2_min_label_style.text.color = LV_COLOR_BLUE;
	
	/*时间“:”的风格*/
	lv_style_copy(&time2_mid_label_style ,&lv_style_pretty);
//	time2_mid_label_style.text.font = &lv_font_hanzi_20;
//	time2_mid_label_style.text.color = LV_COLOR_BLUE;
	
	/*时间秒的风格*/
	lv_style_copy(&time2_sec_label_style ,&lv_style_pretty);
//	time2_sec_label_style.text.font = &lv_font_hanzi_20;
//	time2_sec_label_style.text.color = LV_COLOR_BLUE;
	
	/*时间值的标签*/
	time2_min_label = lv_label_create(time2frame, NULL);	
	time2_mid_label = lv_label_create(time2frame, NULL);
	time2_sec_label = lv_label_create(time2frame, NULL);
	
	/*时间*/
	lv_label_set_text(time2_min_label, "17");
	lv_label_set_style(time2_min_label, &time2_min_label_style);
	lv_obj_align(time2_min_label, time2frame, LV_ALIGN_CENTER, -14, 10);
	
	lv_label_set_text(time2_mid_label, ":");
	lv_label_set_style(time2_mid_label, &time2_mid_label_style);
	lv_obj_align(time2_mid_label, time2_min_label, LV_ALIGN_OUT_RIGHT_TOP, 1, 0);
	
	lv_label_set_text(time2_sec_label, "26");
	lv_label_set_style(time2_sec_label, &time2_sec_label_style);
	lv_obj_align(time2_sec_label, time2_mid_label, LV_ALIGN_OUT_RIGHT_TOP, 1, 0);
}
/*
**********************************************************************************************
*                                   void time2frame_create(void)
* Description: 创建一个“时间2”框图
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void time2frame_create(void)
{
	lv_style_copy(&time2frame_style ,&lv_style_pretty);
	time2frame_style.body.main_color = LV_COLOR_HEX(0xf0f0f0);
	time2frame_style.body.grad_color = LV_COLOR_HEX(0xf0f0f0);
	time2frame_style.text.font = &lv_font_myfont_20;
	time2frame_style.text.color = LV_COLOR_BLACK;
	time2frame_style.body.radius = 3;

	time2frame = lv_cont_create(bg, NULL);
	lv_obj_set_pos(time2frame, TIME2FRAME_POSX, TIME2FRAME_POSY);
	lv_obj_set_size(time2frame, TIME2FRAME_WIDTH, TIME2FRAME_HIGHT);
	lv_cont_set_style(time2frame, &time2frame_style);

	time2_title = lv_label_create(time2frame, NULL);
	lv_label_set_text(time2_title, "时间2");
	lv_label_set_style(time2_title, &time2frame_style);
	lv_obj_align(time2_title, time2frame, LV_ALIGN_IN_TOP_MID, 2, 5);

	time2_label_create();
}
/*
**********************************************************************************************
*                                   void QRcode_img_create(void)
* Description: 创建一个蓝牙二维码
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void QRcode_img_create(void)
{
	QRcode_img = lv_img_create(bg, NULL);
	lv_img_set_src(QRcode_img, &QRcode);
//	lv_obj_set_size(QRcode_img,100,100);

	lv_img_set_style(QRcode_img, &sit_style);
	lv_obj_set_pos(QRcode_img, LOCALFRAME_POSX, LOCALFRAME_POSY);
//	lv_obj_align(QRcode_img, bg , LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_hidden(QRcode_img, true);
}
/*
**********************************************************************************************
*                                   void localframe_create(void)
* Description: 创建一个本地框
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void localframe_create(void)
{
	/*主窗口的风格*/
	lv_style_copy(&localframe_style, &lv_style_plain);
	localframe_style.body.main_color = LV_COLOR_MAKE(229,228,236);
	localframe_style.body.grad_color = LV_COLOR_MAKE(229,228,236);
	localframe_style.body.radius = 12;
	localframe_style.body.border.width = 6;
//	localframe_style.body.opa = LV_OPA_100;
	localframe_style.body.border.color = LV_COLOR_GREEN;
	localframe_style.body.shadow.width = 0;//5;
	localframe_style.body.shadow.color = LV_COLOR_GREEN;

	/*创建一个背景*/
	localframe = lv_cont_create(bg, NULL);
	lv_cont_set_style(localframe, &localframe_style);
	lv_obj_set_size(localframe, LOCALFRAME_WIDTH, LOCALFRAME_HIGHT);
	lv_obj_set_pos(localframe, LOCALFRAME_POSX, LOCALFRAME_POSY);
	
	local_title = lv_label_create(localframe, NULL);
	lv_label_set_text(local_title, "本地");
	lv_label_set_style(local_title, &localframe_style);
	lv_obj_align(local_title, modeframe, LV_ALIGN_IN_TOP_MID, 2, 5);
	lv_obj_set_hidden(localframe, true);
}

/*
**********************************************************************************************
*                                   void page_create(void)
* Description: 创建一个页面
* Arguments  :
* Returns    :
**********************************************************************************************
*/
void page_create(void)
{
	bg_create();
	sit_img_create();
	ID_label_create();
	record_label_create();
	bluetooth_icon_create();
	battery_icon_create();
	bugle_icon_create();
	mainwin_create();
	ACorDC_label_creat();
	curtime_label_create();
	curtestvalue_label_create();
	curvalueunit_label_create();
	PISI_label_create();
	lineScale_create();
	modeframe_creat();
	vlevelframe_creat();
	time1frame_create();
	time2frame_create();
	QRcode_img_create();
	localframe_create();
}
