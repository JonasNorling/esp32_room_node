#include <logging/log.h>
#include <drivers/display.h>
#include <lvgl.h>
#include <sys/byteorder.h>

#include "gui.h"
#include "config.h"

LOG_MODULE_REGISTER(gui);

static const struct device *l_display = NULL;
static lv_indev_t *l_lv_encoder;
static lv_obj_t *l_lv_page;
static lv_obj_t *l_lv_temp;
static lv_obj_t *l_lv_rh;
static lv_obj_t *l_lv_button1;
static lv_obj_t *l_lv_button2;
static lv_obj_t *l_lv_checkbox;
static lv_obj_t *l_lv_slider_servo_left;
static lv_obj_t *l_lv_slider_label_servo_left;
static lv_obj_t *l_lv_slider_servo_right;
static lv_obj_t *l_lv_slider_label_servo_right;
static lv_style_t l_lv_bg_style;
static lv_style_t l_lv_default_style;
static lv_style_t l_lv_slider_indic_style;
static lv_style_t l_lv_tiny_style;
static lv_style_t l_lv_value_style;
static lv_group_t *l_lv_main_group;

static void init_gauge_page(lv_obj_t *parent);
static void init_config_page(lv_obj_t *parent);


static void event_cb(lv_obj_t * obj, lv_event_t event)
{
    switch (event) {
    case LV_EVENT_FOCUSED:
        // Scroll focused object into view
        lv_page_focus(l_lv_page, obj, LV_ANIM_ON);
        break;
    case LV_EVENT_VALUE_CHANGED:
        if (obj == l_lv_button2) {
            int checked = lv_checkbox_get_state(obj) == LV_BTN_STATE_CHECKED_RELEASED;
            (void)config_set_int(CONFIG_IDX_THING, checked);
        }
        else if (obj == l_lv_slider_servo_left) {
            char buffer[32];
            int value = lv_slider_get_value(obj);
            float value_ms = value / 100.0;
            snprintf(buffer, sizeof(buffer), "Servo left: %.2fms", value_ms);
            lv_label_set_text(l_lv_slider_label_servo_left, buffer);
            (void)config_set_int(CONFIG_IDX_SERVO_LEFT, value * 10);
        }
        else if (obj == l_lv_slider_servo_right) {
            char buffer[32];
            int value = lv_slider_get_value(obj);
            float value_ms = value / 100.0;
            snprintf(buffer, sizeof(buffer), "Servo right: %.2fms", value_ms);
            lv_label_set_text(l_lv_slider_label_servo_right, buffer);
            (void)config_set_int(CONFIG_IDX_SERVO_RIGHT, value * 10);
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

static void update_with_config()
{
    int value = 0;
    (void)config_get_int(CONFIG_IDX_SERVO_LEFT, &value);
    lv_slider_set_value(l_lv_slider_servo_left, value / 10, LV_ANIM_OFF);
    event_cb(l_lv_slider_servo_left, LV_EVENT_VALUE_CHANGED);
    (void)config_get_int(CONFIG_IDX_SERVO_RIGHT, &value);
    lv_slider_set_value(l_lv_slider_servo_right, value / 10, LV_ANIM_OFF);
    event_cb(l_lv_slider_servo_right, LV_EVENT_VALUE_CHANGED);
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
    lv_style_set_pad_inner(&l_lv_bg_style, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_top(&l_lv_bg_style, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&l_lv_bg_style, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_left(&l_lv_bg_style, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&l_lv_bg_style, LV_STATE_DEFAULT, 0);
    lv_obj_add_style(lv_scr_act(), LV_OBJ_PART_MAIN, &l_lv_bg_style);

    lv_style_init(&l_lv_default_style);
    lv_style_set_text_color(&l_lv_default_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_color(&l_lv_default_style, LV_STATE_FOCUSED, LV_COLOR_YELLOW);
    lv_style_set_bg_color(&l_lv_default_style, LV_STATE_DEFAULT, LV_COLOR_GRAY);

    lv_style_init(&l_lv_slider_indic_style);
    lv_style_set_bg_color(&l_lv_slider_indic_style, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_style_set_bg_color(&l_lv_slider_indic_style, LV_STATE_FOCUSED, LV_COLOR_YELLOW);
    lv_style_set_bg_color(&l_lv_slider_indic_style, LV_STATE_EDITED, LV_COLOR_RED);

    lv_style_init(&l_lv_tiny_style);
    lv_style_set_text_color(&l_lv_tiny_style, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_style_set_text_font(&l_lv_tiny_style, 0, &lv_font_montserrat_16);

    lv_style_init(&l_lv_value_style);
    lv_style_set_text_color(&l_lv_value_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&l_lv_value_style, 0, &lv_font_montserrat_32);

    l_lv_main_group = lv_group_create();
    lv_indev_set_group(l_lv_encoder, l_lv_main_group);

    l_lv_page = lv_page_create(lv_scr_act(), NULL);
    lv_obj_add_style(l_lv_page, LV_OBJ_PART_MAIN, &l_lv_bg_style);
    lv_obj_set_size(l_lv_page, LV_HOR_RES, LV_VER_RES);
    init_gauge_page(l_lv_page);

    lv_obj_t *container = lv_cont_create(l_lv_page, NULL);
    lv_obj_add_style(container, LV_OBJ_PART_MAIN, &l_lv_bg_style);
    lv_cont_set_fit(container, LV_FIT_TIGHT);
    lv_cont_set_layout(container, LV_LAYOUT_COLUMN_LEFT);
    lv_obj_set_pos(container, 0, LV_VER_RES);
    init_config_page(container);

    update_with_config();

    return 0;
}

static void init_gauge_page(lv_obj_t *parent)
{
    l_lv_temp = lv_label_create(parent, NULL);   
    lv_obj_add_style(l_lv_temp, LV_OBJ_PART_MAIN, &l_lv_value_style);
    lv_obj_set_pos(l_lv_temp, 0, 0);

    l_lv_rh = lv_label_create(parent, NULL);
    lv_obj_add_style(l_lv_rh, LV_OBJ_PART_MAIN, &l_lv_value_style);
    lv_obj_set_pos(l_lv_rh, 0, LV_VER_RES - 30);

    lv_obj_t *lv_temp_label = lv_label_create(parent, NULL);   
    lv_obj_add_style(lv_temp_label, LV_OBJ_PART_MAIN, &l_lv_tiny_style);
    lv_label_set_text(lv_temp_label, "T");
    lv_obj_align(lv_temp_label, l_lv_temp, LV_ALIGN_OUT_BOTTOM_LEFT, 0, -8);

    lv_obj_t *lv_rh_label = lv_label_create(parent, NULL);
    lv_obj_add_style(lv_rh_label, LV_OBJ_PART_MAIN, &l_lv_tiny_style);
    lv_label_set_text(lv_rh_label, "RH%");
    lv_obj_align(lv_rh_label, l_lv_rh, LV_ALIGN_OUT_TOP_LEFT, 0, 8);   

    // Make the temp display focusable
    lv_group_add_obj(l_lv_main_group, l_lv_temp);
    lv_obj_set_event_cb(l_lv_temp, event_cb);
}

static void init_config_page(lv_obj_t *parent)
{
    l_lv_button1 = lv_btn_create(parent, NULL);
    lv_obj_t *label = lv_label_create(l_lv_button1, NULL);
    lv_label_set_text(label, "knapp");
    lv_obj_add_style(l_lv_button1, LV_OBJ_PART_MAIN, &l_lv_default_style);
    lv_group_add_obj(l_lv_main_group, l_lv_button1);
    lv_obj_set_event_cb(l_lv_button1, event_cb);

    l_lv_button2 = lv_checkbox_create(parent, NULL);
    lv_checkbox_set_text(l_lv_button2, "checkbox");
    int checked = 0;
    (void)config_get_int(CONFIG_IDX_THING, &checked);
    lv_checkbox_set_checked(l_lv_button2, checked);
    lv_obj_add_style(l_lv_button2, LV_OBJ_PART_MAIN, &l_lv_default_style);
    lv_group_add_obj(l_lv_main_group, l_lv_button2);
    lv_obj_set_event_cb(l_lv_button2, event_cb);

    l_lv_checkbox = lv_checkbox_create(parent, NULL);
    lv_checkbox_set_text(l_lv_checkbox, "checkbox");
    lv_obj_add_style(l_lv_checkbox, LV_OBJ_PART_MAIN, &l_lv_default_style);
    lv_group_add_obj(l_lv_main_group, l_lv_checkbox);
    lv_obj_set_event_cb(l_lv_checkbox, event_cb);

    l_lv_slider_label_servo_left = lv_label_create(parent, NULL);
    lv_label_set_text(l_lv_slider_label_servo_left, "--");

    l_lv_slider_servo_left = lv_slider_create(parent, NULL);
    lv_slider_set_range(l_lv_slider_servo_left, 0, 300);
    lv_obj_set_size(l_lv_slider_servo_left, 0.7 * LV_HOR_RES, 6);
    lv_obj_add_style(l_lv_slider_servo_left, LV_SLIDER_PART_INDIC, &l_lv_slider_indic_style);
    lv_group_add_obj(l_lv_main_group, l_lv_slider_servo_left);
    lv_obj_set_event_cb(l_lv_slider_servo_left, event_cb);

    l_lv_slider_label_servo_right = lv_label_create(parent, NULL);
    lv_label_set_text(l_lv_slider_label_servo_right, "--");

    l_lv_slider_servo_right = lv_slider_create(parent, NULL);
    lv_slider_set_range(l_lv_slider_servo_right, 0, 300);
    lv_obj_set_size(l_lv_slider_servo_right, 0.7 * LV_HOR_RES, 6);
    lv_obj_add_style(l_lv_slider_servo_right, LV_SLIDER_PART_INDIC, &l_lv_slider_indic_style);
    lv_group_add_obj(l_lv_main_group, l_lv_slider_servo_right);
    lv_obj_set_event_cb(l_lv_slider_servo_right, event_cb);
}

int gui_update(int call, float temp, float rh)
{
    char buffer_t[8] = "";
    char buffer_rh[8] = "";
    snprintf(buffer_t, sizeof(buffer_t), "%.1fC", temp);
    snprintf(buffer_rh, sizeof(buffer_rh), "%.1f%%", rh);

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
