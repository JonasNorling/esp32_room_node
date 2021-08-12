#include "logging/log.h"
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <drivers/display.h>
#include <sys/byteorder.h>
#include <lvgl.h>
#include <math.h>

LOG_MODULE_REGISTER(main);

const struct device *l_gpio1 = NULL;
const struct device *l_dht22 = NULL;
const struct device *l_display = NULL;

static lv_obj_t *l_lv_hello_world;
static lv_obj_t *l_lv_temp;
static lv_obj_t *l_lv_rh;
static lv_obj_t *l_lv_temp_label;
static lv_obj_t *l_lv_rh_label;
static lv_style_t l_lv_default_style;
static lv_style_t l_lv_tiny_style;
static lv_style_t l_lv_value_style;

#define GPIO_PIN_LED 13

static int gpio_init()
{
    int rc;

    l_gpio1 = device_get_binding(DT_LABEL(DT_NODELABEL(gpio0)));
    if (!l_gpio1) {
        LOG_ERR("Failed to open GPIO device");
        return 1;
    }

    rc = gpio_pin_configure(l_gpio1, GPIO_PIN_LED, GPIO_OUTPUT_LOW);
    if (rc) {
        LOG_ERR("Error configuring pin");
        return 1;
    }

    return 0;
}

static int display_init()
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


    return 0;
}

static int display_update(int call, float temp, float rh)
{
    char buffer[32] = "";
    snprintf(buffer, sizeof(buffer), "Hello %d", call);
    lv_label_set_text(l_lv_hello_world, buffer);

    snprintf(buffer, sizeof(buffer), "%dC", (int)(temp + 0.5));
    lv_label_set_text(l_lv_temp, buffer);

    snprintf(buffer, sizeof(buffer), "%d%%", (int)(rh + 0.5));
    lv_label_set_text(l_lv_rh, buffer);

    lv_task_handler();
    return 0;
}

static int sensor_init()
{
    l_dht22 = device_get_binding(DT_LABEL(DT_INST(0, aosong_dht)));
    if (!l_dht22) {
        LOG_ERR("Failed to open DHT22");
        return 1;
    }

    return 0;
}

int main(int argc, char **argv)
{
    LOG_INF("Hello there");

    if (gpio_init()) {
        return 1;
    }

    if (sensor_init()) {
        return 1;
    }

    if (display_init()) {
        return 1;
    }

    int call = 0;
    while (true) {
        k_sleep(K_MSEC(1000));
        gpio_pin_set(l_gpio1, GPIO_PIN_LED, 1);
        k_sleep(K_MSEC(100));
        gpio_pin_set(l_gpio1, GPIO_PIN_LED, 0);

        if (sensor_sample_fetch(l_dht22)) {
            LOG_ERR("Failed to fetch DHT22 sample");
        }

        struct sensor_value temp;
        struct sensor_value rh;
        if (sensor_channel_get(l_dht22, SENSOR_CHAN_AMBIENT_TEMP, &temp)) {
            LOG_ERR("Failed to read temperature");
        }
        if (sensor_channel_get(l_dht22, SENSOR_CHAN_HUMIDITY, &rh)) {
            LOG_ERR("Failed to read humitidy");
        }

        LOG_INF("DHT22 temp: %d.%06d, RH: %d.%06d",
                temp.val1, temp.val2, rh.val1, rh.val2);

        if (display_update(call,
                           sensor_value_to_double(&temp),
                           sensor_value_to_double(&rh))) {
            return 1;
        }
        call++;
    }

    return 0;
}
