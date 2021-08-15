#include "logging/log.h"
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <drivers/display.h>
#include <drivers/pwm.h>
#include <drivers/counter.h>
#include <sys/byteorder.h>
#include <lvgl.h>
#include <math.h>

LOG_MODULE_REGISTER(main);

const struct device *l_gpio0 = NULL;
const struct device *l_gpio1 = NULL;
const struct device *l_dht22 = NULL;
const struct device *l_display = NULL;
const struct device *l_pwm = NULL;
const struct device *l_counter = NULL;

static bool l_encoder_pressed;
static int l_encoder_position;

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

#define GPIO_PIN_LED 13
#define GPIO_PIN_SERVO 27
#define PWM_CHANNEL GPIO_PIN_SERVO
#define GPIO_PIN_BUTTON (34 - 32)  // "A2"
#define GPIO_PIN_ENC1   (39 - 32)  // "A3"
#define GPIO_PIN_ENC2   (36 - 32)  // "A4"

static int gpio_init()
{
    int rc;

    l_gpio0 = device_get_binding(DT_LABEL(DT_NODELABEL(gpio0)));
    if (!l_gpio0) {
        LOG_ERR("Failed to open GPIO0 device");
        return 1;
    }

    l_gpio1 = device_get_binding(DT_LABEL(DT_NODELABEL(gpio1)));
    if (!l_gpio1) {
        LOG_ERR("Failed to open GPIO1 device");
        return 1;
    }

    rc = gpio_pin_configure(l_gpio0, GPIO_PIN_LED, GPIO_OUTPUT_LOW);
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
    lv_style_set_bg_color(&l_lv_default_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_color(&l_lv_default_style, LV_STATE_FOCUSED, LV_COLOR_RED);
    lv_style_set_bg_color(&l_lv_default_style, LV_STATE_FOCUSED, LV_COLOR_BLUE);
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

static int display_update(int call, float temp, float rh)
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

static int sensor_init()
{
    l_dht22 = device_get_binding(DT_LABEL(DT_INST(0, aosong_dht)));
    if (!l_dht22) {
        LOG_ERR("Failed to open DHT22");
        return 1;
    }

    return 0;
}

static int pwm_init()
{
    l_pwm = device_get_binding(CONFIG_PWM_LED_ESP32_DEV_NAME_0);
    if (!l_pwm) {
        LOG_ERR("Failed to open PWM device");
        return 1;
    }

    uint64_t cycles = 0;
    int rc = pwm_get_cycles_per_sec(l_pwm, PWM_CHANNEL, &cycles);
    if (rc) {
        LOG_ERR("Failed to get PWM cycles");
        return 1;
    }
    LOG_INF("PWM cycles %lld", cycles);

    return 0;
}

static int servo_set(float duty_ms)
{
    float duty = duty_ms * ((float)CONFIG_PWM_LED_ESP32_HS_TIMER0_FREQ / 1000);
    int period = (1 << CONFIG_PWM_LED_ESP32_HS_TIMER0_BIT_NUM) * duty;
    int rc = pwm_pin_set_cycles(l_pwm, PWM_CHANNEL, 0, period, 0);
    if (rc) {
        LOG_ERR("Failed to set PWM");
        return 1;
    }

    return 0;
}

static void input_poll()
{
    static bool last_state[3];
    const bool state[3] = {
        !gpio_pin_get(l_gpio1, GPIO_PIN_BUTTON),
        !gpio_pin_get(l_gpio1, GPIO_PIN_ENC1),
        !gpio_pin_get(l_gpio1, GPIO_PIN_ENC2),
    };

    l_encoder_pressed = state[0];

    if (state[1] && !last_state[1]) {
        //LOG_INF("Knob %s", state[2] ? "-->" : "<--");
        l_encoder_position += state[2] ? -1 : 1;
    }

    memcpy(last_state, state, sizeof(state));
}

static void counter_cb(const struct device *counter_dev,
				       uint8_t chan_id, uint32_t ticks,
				       void *user_data)
{
    const struct counter_alarm_cfg alarm_cfg = {
        .ticks = counter_us_to_ticks(l_counter, 2000),
        .callback = counter_cb,
    };
    int rc = counter_set_channel_alarm(l_counter, 0, &alarm_cfg);
    if (rc) {
        LOG_ERR("Error setting alarm");
    }

    input_poll();
}

static bool encoder_read_cb(lv_indev_drv_t *drv, lv_indev_data_t*data)
{
    int encoder_position = l_encoder_position;
    static int last_encoder_position;

    data->enc_diff = encoder_position - last_encoder_position;
    data->state = l_encoder_pressed ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL;

    if (encoder_position - last_encoder_position) {
        LOG_INF("Encoder: %d", encoder_position - last_encoder_position);
    }

    last_encoder_position = encoder_position;
    return false;
}

static int input_init()
{
    int rc = gpio_pin_configure(l_gpio1, GPIO_PIN_BUTTON, GPIO_INPUT);
    if (rc) {
        LOG_ERR("Error configuring pin");
        return 1;
    }

    rc = gpio_pin_configure(l_gpio1, GPIO_PIN_ENC1, GPIO_INPUT);
    if (rc) {
        LOG_ERR("Error configuring pin");
        return 1;
    }

    rc = gpio_pin_configure(l_gpio1, GPIO_PIN_ENC2, GPIO_INPUT);
    if (rc) {
        LOG_ERR("Error configuring pin");
        return 1;
    }

    l_counter = device_get_binding(DT_LABEL(DT_NODELABEL(timer0)));
    if (!l_counter) {
        LOG_ERR("Failed to open counter device");
        return 1;
    }

    const struct counter_alarm_cfg alarm_cfg = {
        .ticks = counter_us_to_ticks(l_counter, 500000),
        .callback = counter_cb,
    };
    rc = counter_set_channel_alarm(l_counter, 0, &alarm_cfg);
    if (rc) {
        LOG_ERR("Error setting alarm");
        return 1;
    }

    rc = counter_start(l_counter);
    if (rc) {
        LOG_ERR("Error starting counter");
        return 1;
    }

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read_cb;  
    lv_indev_t *indev = lv_indev_drv_register(&indev_drv);
    if (!indev) {
        LOG_ERR("Failed to register input driver");
        return 1;
    }

    lv_indev_set_group(indev, l_lv_group);

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

    if (pwm_init()) {
        return 1;
    }

    if (input_init()) {
        return 1;
    }

    int call = 0;
    float servo_duty = 0.0f;
    while (true) {
        k_sleep(K_MSEC(10));
        if (!(call++ % 128)) {
            gpio_pin_set(l_gpio0, GPIO_PIN_LED, 1);
            if (sensor_sample_fetch(l_dht22)) {
                LOG_ERR("Failed to fetch DHT22 sample");
            }
            gpio_pin_set(l_gpio0, GPIO_PIN_LED, 0);

            struct sensor_value temp;
            struct sensor_value rh;
            if (sensor_channel_get(l_dht22, SENSOR_CHAN_AMBIENT_TEMP, &temp)) {
                LOG_ERR("Failed to read temperature");
            }
            if (sensor_channel_get(l_dht22, SENSOR_CHAN_HUMIDITY, &rh)) {
                LOG_ERR("Failed to read humitidy");
            }

            float temp_f = sensor_value_to_double(&temp);
            float rh_f = sensor_value_to_double(&rh);

            if (display_update(call, temp_f, rh_f)) {
                return 1;
            }
            if (temp_f >= 27.0f) {
                if (servo_duty != 2.0f) {
                    servo_duty = 2.0f;
                    if (servo_set(servo_duty)) {
                        return 1;
                    }
                }
            }
            if (temp_f < 26.0f) {
                if (servo_duty != 1.0f) {
                    servo_duty = 1.0f;
                    if (servo_set(servo_duty)) {
                        return 1;
                    }
                }
            }
        }

        lv_task_handler();
    }

    return 0;
}
