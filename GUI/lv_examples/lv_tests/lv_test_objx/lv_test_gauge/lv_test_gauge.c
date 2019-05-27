/**
 * @file lv_test_gauge.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_test_gauge.h"

#if USE_LV_GAUGE && USE_LV_TESTS

/*********************
 *      DEFINES
 *********************/
#define GAUGEWIDTH	250
#define GAUGEHEIGHT 250
#define MYDESIGN	
/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Create gauges to test their functionalities
 */
void lv_test_gauge_1(void)
{
    /* Create a default object*/
    lv_obj_t *gauge1 = lv_gauge_create(lv_scr_act(), NULL);
    lv_obj_set_pos(gauge1, 10, 10);
    lv_gauge_set_value(gauge1, 0, 75);

    /*Copy the previous gauge and set smaller size for it*/
    lv_obj_t *gauge2 = lv_gauge_create(lv_scr_act(), gauge1);
    lv_obj_set_size(gauge2, 2 * lv_obj_get_width(gauge1) / 3,  2 * lv_obj_get_height(gauge1) / 3);
    lv_obj_align(gauge2, gauge1, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    /*Copy the first gauge add more needles and set new style*/
    lv_color_t needle_colors[3] = {LV_COLOR_BLUE, LV_COLOR_PURPLE, LV_COLOR_TEAL};

    /*Create a styled gauge*/
    static lv_style_t style3;
    lv_style_copy(&style3, &lv_style_pretty);
    style3.body.main_color = LV_COLOR_GREEN;
    style3.body.grad_color = LV_COLOR_RED;
    style3.body.padding.hor = 10;
    style3.body.padding.inner = 10;
    style3.body.padding.ver = 8;
    style3.body.border.color= LV_COLOR_GRAY;
    style3.line.width = 2;

    lv_obj_t *gauge3 = lv_gauge_create(lv_scr_act(), gauge1);
    lv_obj_align(gauge3, gauge1, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    lv_obj_set_style(gauge3, &style3);
    lv_gauge_set_scale(gauge3, 100, 20, 5);
    lv_gauge_set_needle_count(gauge3, 1, needle_colors);
    lv_gauge_set_value(gauge3, 0, 30);
//    lv_gauge_set_value(gauge3, 1, 40);
//    lv_gauge_set_value(gauge3, 2, 60);
		
		/*Create a label to gauge*/
		lv_obj_t * label = lv_label_create(gauge3,NULL);
		lv_obj_align(label,gauge3,LV_ALIGN_CENTER,0,0);
		
    /*Copy the modified 'gauge3' and set a smaller size for it*/
    lv_obj_t *gauge4 = lv_gauge_create(lv_scr_act(), gauge3);
    lv_obj_set_size(gauge4, GAUGEWIDTH, GAUGEHEIGHT);
    lv_obj_align(gauge4, gauge3, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
		
		
		static lv_style_t h_style;
    lv_style_copy(&h_style, &lv_style_plain);
		h_style.body.main_color = LV_COLOR_WHITE;
		h_style.body.grad_color = LV_COLOR_BLUE;
		h_style.body.border.color = LV_COLOR_GRAY;
		h_style.body.border.width = 2;
		h_style.body.border.opa = LV_OPA_50;
		h_style.text.color = LV_COLOR_WHITE;
		
		lv_obj_t *h = lv_cont_create(gauge4, NULL);
		lv_obj_set_size(h,50,30);
		lv_obj_align(h,gauge4,LV_ALIGN_CENTER,0,-(GAUGEHEIGHT/4));
    lv_obj_set_style(h, &h_style);
		
		/*Create a label to gauge*/
		lv_obj_t * label2 = lv_label_create(h,NULL);
		lv_obj_align(label2,h,LV_ALIGN_CENTER,0,0);
		
		static lv_style_t my_style;
    lv_style_copy(&my_style, &lv_style_plain);
		my_style.body.main_color = LV_COLOR_RED;
    my_style.body.grad_color = LV_COLOR_RED;
    my_style.body.padding.hor = 6;
    my_style.body.padding.inner = 10;
    my_style.body.padding.ver = 8;
    my_style.body.border.color= LV_COLOR_RED;
    my_style.line.width = 2;
		
		
		lv_obj_t * lmeter = lv_lmeter_create(lv_scr_act(),NULL);
		lv_obj_set_size(lmeter,GAUGEWIDTH,GAUGEHEIGHT);
		lv_obj_align(lmeter,gauge4,LV_ALIGN_CENTER,0,+4);
		lv_obj_set_style(lmeter, &my_style);
		lv_lmeter_set_scale(lmeter, 100, 48);
		lv_lmeter_set_range(lmeter, 0, 100);
    lv_lmeter_set_value(lmeter, 30);
		
		lv_lmeter_ext_t * ext = lv_obj_get_ext_attr(lmeter);
		
		ext->enplay = 108;
		

}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#endif /*USE_LV_GAUGE && USE_LV_TESTS*/

