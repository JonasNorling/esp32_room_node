#include <logging/log.h>
#include <drivers/display.h>
#include <lvgl.h>

#include "gui.h"

LOG_MODULE_REGISTER(gui);

static const struct device *l_display = NULL;
static lv_obj_t *l_lv_hello_world;
static lv_obj_t *l_lv_temp;
static lv_obj_t *l_lv_rh;
static lv_obj_t *l_lv_temp_label;
static lv_obj_t *l_lv_rh_label;
static lv_obj_t *l_lv_button1;
static lv_obj_t *l_lv_button2;
static lv_style_t l_lv_default_style;
static lv_style_t l_lv_tiny_style;
static lv_style_t l_lv_value_style;
static lv_group_t *l_lv_group;


int gui_init()
{
    l_display = device_get_binding(DT_LABEL(DT_INST(0, sitronix_st7735r)));

    if (!l_display) {
        LOG_ERR("Failed to open display");
        return 1;
    }

    if (display_blanking_off(l_display)) {
        LOG_ERR("Display unblank failed");
        return 1;
    }

    struct display_capabilities caps;
    display_get_capabilities(l_display, &caps);
    LOG_INF("Pixel format: %d", caps.current_pixel_format);

    lv_style_init(&l_lv_default_style);
    lv_style_set_bg_color(&l_lv_default_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_color(&l_lv_default_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_color(&l_lv_default_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_color(&l_lv_default_style, LV_STATE_FOCUSED, LV_COLOR_RED);
    lv_style_set_bg_color(&l_lv_default_style, LV_STATE_FOCUSED, LV_COLOR_BLUE);
    lv_style_set_radius(&l_lv_default_style, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_top(&l_lv_default_style, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&l_lv_default_style, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_inner(&l_lv_default_style, LV_STATE_DEFAULT, 0);
    lv_style_set_text_font(&l_lv_default_style, 0, lv_theme_get_font_small());

    lv_style_init(&l_lv_tiny_style);
    lv_style_set_text_color(&l_lv_tiny_style, LV_STATE_DEFAULT, LV_COLOR_RED);
    lv_style_set_text_font(&l_lv_tiny_style, 0, &lv_font_montserrat_16);

    lv_style_init(&l_lv_value_style);
    lv_style_set_text_color(&l_lv_value_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&l_lv_value_style, 0, lv_theme_get_font_normal());

    lv_obj_add_style(lv_scr_act(), LV_OBJ_PART_MAIN, &l_lv_default_style);

    l_lv_hello_world = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(l_lv_hello_world, "Hello world!");
	lv_obj_align(l_lv_hello_world, NULL, LV_ALIGN_CENTER, 0, 0);

    l_lv_temp = lv_label_create(lv_scr_act(), NULL);   
    lv_obj_add_style(l_lv_temp, LV_OBJ_PART_MAIN, &l_lv_value_style);
    lv_obj_align(l_lv_temp, NULL, LV_ALIGN_IN_TOP_LEFT, 0, -5);

    l_lv_rh = lv_label_create(lv_scr_act(), NULL);
    lv_obj_add_style(l_lv_rh, LV_OBJ_PART_MAIN, &l_lv_value_style);
    lv_obj_align(l_lv_rh, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 5);

    l_lv_temp_label = lv_label_create(lv_scr_act(), NULL);   
    lv_obj_add_style(l_lv_temp_label, LV_OBJ_PART_MAIN, &l_lv_tiny_style);
    lv_label_set_text(l_lv_temp_label, "T");
    lv_obj_align(l_lv_temp_label, l_lv_temp, LV_ALIGN_OUT_BOTTOM_LEFT, 0, -8);

    l_lv_rh_label = lv_label_create(lv_scr_act(), NULL);
    lv_obj_add_style(l_lv_rh_label, LV_OBJ_PART_MAIN, &l_lv_tiny_style);
    lv_label_set_text(l_lv_rh_label, "RH%");
    lv_obj_align(l_lv_rh_label, l_lv_rh, LV_ALIGN_OUT_TOP_LEFT, 0, 8);

    l_lv_group = lv_group_create();
    l_lv_button1 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_align(l_lv_button1, NULL, LV_ALIGN_IN_TOP_RIGHT, 0, 0);
    lv_label_set_text(lv_label_create(l_lv_button1, NULL), "knapp");
    lv_obj_add_style(l_lv_button1, LV_OBJ_PART_MAIN, &l_lv_default_style);
    lv_group_add_obj(l_lv_group, l_lv_button1);

    l_lv_button2 = lv_checkbox_create(lv_scr_act(), NULL);
    lv_obj_align(l_lv_button2, l_lv_button1, LV_ALIGN_OUT_BOTTOM_RIGHT, 0, 0);
    lv_checkbox_set_text(l_lv_button2, "text");
    lv_obj_add_style(l_lv_button2, LV_OBJ_PART_MAIN, &l_lv_default_style);
    lv_group_add_obj(l_lv_group, l_lv_button2);

    return 0;
}

int gui_update(int call, float temp, float rh)
{
    char buffer_count[16] = "";
    char buffer_t[8] = "";
    char buffer_rh[8] = "";
    snprintf(buffer_count, sizeof(buffer_count), "Uptime %d", call);
    snprintf(buffer_t, sizeof(buffer_t), "%.1fC", temp);
    snprintf(buffer_rh, sizeof(buffer_rh), "%.1f%%", rh);

    lv_label_set_text(l_lv_hello_world, buffer_count);
    lv_label_set_text(l_lv_temp, buffer_t);
    lv_label_set_text(l_lv_rh, buffer_rh);

    LOG_INF("DHT22 temp: %s, RH: %s",
            log_strdup(buffer_t), log_strdup(buffer_rh));

    return 0;
}

lv_group_t *gui_get_lv_group()
{
    return l_lv_group;
}

int gui_do(void)
{
    lv_task_handler();
    return 0;
}
