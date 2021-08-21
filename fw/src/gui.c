#include <logging/log.h>
#include <drivers/display.h>
#include <lvgl.h>
#include <sys/byteorder.h>

#include "gui.h"
#include "config.h"

LOG_MODULE_REGISTER(gui);

static const struct device *l_display = NULL;
static lv_indev_t *l_lv_encoder;
static lv_obj_t *l_lv_tileview;
static lv_obj_t *l_lv_page;
static lv_obj_t *l_lv_hello_world;
static lv_obj_t *l_lv_temp;
static lv_obj_t *l_lv_rh;
static lv_obj_t *l_lv_temp_label;
static lv_obj_t *l_lv_rh_label;
static lv_obj_t *l_lv_button1;
static lv_obj_t *l_lv_button2;
static lv_obj_t *l_lv_checkbox;
static lv_style_t l_lv_bg_style;
static lv_style_t l_lv_default_style;
static lv_style_t l_lv_tiny_style;
static lv_style_t l_lv_value_style;
static lv_group_t *l_lv_main_group;
static lv_group_t *l_lv_config_group;

static void init_gauge_page(lv_obj_t *parent);
static void init_config_page(lv_obj_t *parent);


static void tile1_cb(lv_obj_t * obj, lv_event_t event)
{
    LOG_INF("Tile 1 event on %p: %u", obj, event);
    if (event == LV_EVENT_FOCUSED) {
        lv_tileview_set_tile_act(l_lv_tileview, 0, 0, LV_ANIM_ON);
    }
}

static void tile2_cb(lv_obj_t * obj, lv_event_t event)
{
    LOG_INF("Tile 2 event on %p: %u", obj, event);
    if (event == LV_EVENT_FOCUSED) {
        lv_tileview_set_tile_act(l_lv_tileview, 0, 1, LV_ANIM_ON);
        lv_indev_set_group(l_lv_encoder, l_lv_config_group);
    }
}

static void config_cb(lv_obj_t * obj, lv_event_t event)
{
    LOG_INF("Event on %p: %u", obj, event);
    switch (event) {
    case LV_EVENT_FOCUSED:
        lv_page_focus(l_lv_page, obj, LV_ANIM_ON);
        break;
    case LV_EVENT_VALUE_CHANGED:
        if (obj == l_lv_button2) {
            int checked = lv_checkbox_get_state(obj) == LV_BTN_STATE_CHECKED_RELEASED;
            (void)config_set_int(CONFIG_IDX_THING, checked);
        }
        break;
    }
}

int gui_encoder_init(bool (read_cb)(lv_indev_drv_t*, lv_indev_data_t*))
{
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = read_cb;
    l_lv_encoder = lv_indev_drv_register(&indev_drv);
    if (!l_lv_encoder) {
        LOG_ERR("Failed to register input driver");
        return 1;
    }
    return 0;
}

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

    lv_style_init(&l_lv_bg_style);
    lv_style_set_bg_color(&l_lv_bg_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_border_width(&l_lv_bg_style, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(lv_scr_act(), LV_OBJ_PART_MAIN, &l_lv_bg_style);

    lv_style_init(&l_lv_default_style);
    lv_style_set_text_color(&l_lv_default_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_color(&l_lv_default_style, LV_STATE_FOCUSED, LV_COLOR_YELLOW);
    lv_style_set_bg_color(&l_lv_default_style, LV_STATE_DEFAULT, LV_COLOR_GRAY);

    lv_style_init(&l_lv_tiny_style);
    lv_style_set_text_color(&l_lv_tiny_style, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_style_set_text_font(&l_lv_tiny_style, 0, &lv_font_montserrat_16);

    lv_style_init(&l_lv_value_style);
    lv_style_set_text_color(&l_lv_value_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&l_lv_value_style, 0, &lv_font_montserrat_32);

    l_lv_tileview = lv_tileview_create(lv_scr_act(), NULL);
    static lv_point_t valid_pos[] = {{0,0}, {0, 1}};
    lv_tileview_set_valid_positions(l_lv_tileview, valid_pos, 2);
    lv_obj_t *tile1 = lv_obj_create(l_lv_tileview, NULL);
    lv_obj_add_style(tile1, LV_OBJ_PART_MAIN, &l_lv_bg_style);
    lv_obj_set_size(tile1, LV_HOR_RES, LV_VER_RES);
    lv_tileview_add_element(l_lv_tileview, tile1);
    init_gauge_page(tile1);

    lv_obj_t *tile2 = lv_obj_create(l_lv_tileview, NULL);
    lv_obj_add_style(tile2, LV_OBJ_PART_MAIN, &l_lv_bg_style);
    lv_obj_set_size(tile2, LV_HOR_RES, LV_VER_RES);
    lv_tileview_add_element(l_lv_tileview, tile2);
    lv_obj_set_pos(tile2, 0, LV_VER_RES);
    init_config_page(tile2);

    l_lv_main_group = lv_group_create();
    lv_group_add_obj(l_lv_main_group, tile1);
    lv_group_add_obj(l_lv_main_group, tile2);

    lv_obj_set_event_cb(tile1, tile1_cb);
    lv_obj_set_event_cb(tile2, tile2_cb);

    lv_indev_set_group(l_lv_encoder, l_lv_main_group);
    return 0;
}

static void init_gauge_page(lv_obj_t *parent)
{
    l_lv_temp = lv_label_create(parent, NULL);   
    lv_obj_add_style(l_lv_temp, LV_OBJ_PART_MAIN, &l_lv_value_style);
    lv_obj_align(l_lv_temp, NULL, LV_ALIGN_IN_TOP_LEFT, 0, -5);

    l_lv_rh = lv_label_create(parent, NULL);
    lv_obj_add_style(l_lv_rh, LV_OBJ_PART_MAIN, &l_lv_value_style);
    lv_obj_align(l_lv_rh, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 5);

    l_lv_temp_label = lv_label_create(parent, NULL);   
    lv_obj_add_style(l_lv_temp_label, LV_OBJ_PART_MAIN, &l_lv_tiny_style);
    lv_label_set_text(l_lv_temp_label, "T");
    lv_obj_align(l_lv_temp_label, l_lv_temp, LV_ALIGN_OUT_BOTTOM_LEFT, 0, -8);

    l_lv_rh_label = lv_label_create(parent, NULL);
    lv_obj_add_style(l_lv_rh_label, LV_OBJ_PART_MAIN, &l_lv_tiny_style);
    lv_label_set_text(l_lv_rh_label, "RH%");
    lv_obj_align(l_lv_rh_label, l_lv_rh, LV_ALIGN_OUT_TOP_LEFT, 0, 8);   
}

static void init_config_page(lv_obj_t *parent)
{
    l_lv_config_group = lv_group_create();

    l_lv_page = lv_page_create(parent, NULL);
    lv_obj_set_size(l_lv_page, 160, 80);
    lv_obj_align(l_lv_page, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(l_lv_page, LV_OBJ_PART_MAIN, &l_lv_bg_style);

    l_lv_hello_world = lv_label_create(l_lv_page, NULL);
    lv_label_set_text(l_lv_hello_world, "Hello world!");
	lv_obj_align(l_lv_hello_world, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_group_add_obj(l_lv_config_group, l_lv_hello_world);
    lv_obj_set_event_cb(l_lv_hello_world, config_cb);

    l_lv_button1 = lv_btn_create(l_lv_page, NULL);
    lv_obj_align(l_lv_button1, l_lv_hello_world, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_obj_t *label = lv_label_create(l_lv_button1, NULL);
    lv_label_set_text(label, "knapp");
    lv_obj_add_style(l_lv_button1, LV_OBJ_PART_MAIN, &l_lv_default_style);
    lv_group_add_obj(l_lv_config_group, l_lv_button1);
    lv_obj_set_event_cb(l_lv_button1, config_cb);

    l_lv_button2 = lv_checkbox_create(l_lv_page, NULL);
    lv_obj_align(l_lv_button2, l_lv_button1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_checkbox_set_text(l_lv_button2, "checkbox");
    int checked = 0;
    (void)config_get_int(CONFIG_IDX_THING, &checked);
    lv_checkbox_set_checked(l_lv_button2, checked);
    lv_obj_add_style(l_lv_button2, LV_OBJ_PART_MAIN, &l_lv_default_style);
    lv_group_add_obj(l_lv_config_group, l_lv_button2);
    lv_obj_set_event_cb(l_lv_button2, config_cb);

    l_lv_checkbox = lv_checkbox_create(l_lv_page, NULL);
    lv_obj_align(l_lv_checkbox, l_lv_button2, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 0);
    lv_checkbox_set_text(l_lv_checkbox, "checkbox");
    lv_obj_add_style(l_lv_checkbox, LV_OBJ_PART_MAIN, &l_lv_default_style);
    lv_group_add_obj(l_lv_config_group, l_lv_checkbox);
    lv_obj_set_event_cb(l_lv_checkbox, config_cb);
}

int gui_update(int call, float temp, float rh)
{
    char buffer_count[16] = "";
    char buffer_t[8] = "";
    char buffer_rh[8] = "";
    snprintf(buffer_count, sizeof(buffer_count), "Uptime %d", k_uptime_get_32());
    snprintf(buffer_t, sizeof(buffer_t), "%.1fC", temp);
    snprintf(buffer_rh, sizeof(buffer_rh), "%.1f%%", rh);

    lv_label_set_text(l_lv_hello_world, buffer_count);
    lv_label_set_text(l_lv_temp, buffer_t);
    lv_label_set_text(l_lv_rh, buffer_rh);

    LOG_INF("DHT22 temp: %s, RH: %s",
            log_strdup(buffer_t), log_strdup(buffer_rh));

    return 0;
}

int gui_do(void)
{
    lv_task_handler();
    return 0;
}
