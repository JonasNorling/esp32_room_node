#include <logging/log.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <drivers/pwm.h>
#include <drivers/counter.h>
#include <sys/byteorder.h>
#include <lvgl.h>
#include <math.h>

#include "gui.h"

LOG_MODULE_REGISTER(main);

static const struct device *l_gpio0 = NULL;
static const struct device *l_gpio1 = NULL;
static const struct device *l_dht22 = NULL;
static const struct device *l_pwm = NULL;

static bool l_encoder_pressed;
static int l_encoder_position;

#define GPIO_PIN_LED 13
#define GPIO_PIN_SERVO 27
#define PWM_CHANNEL GPIO_PIN_SERVO
#define GPIO_PIN_BUTTON (34 - 32)  // "A2"
#define GPIO_PIN_ENC1   (39 - 32)  // "A3"
#define GPIO_PIN_ENC2   (36 - 32)  // "A4"

static void input_timer_cb(struct k_timer *timer_id);
K_TIMER_DEFINE(l_input_timer, input_timer_cb, NULL);


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

static void input_timer_cb(struct k_timer *timer_id)
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

    k_timer_start(&l_input_timer, K_MSEC(5), K_MSEC(5));

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

    if (gui_encoder_init(encoder_read_cb)) {
        return 1;
    }

    if (gui_init()) {
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

            if (gui_update(call, temp_f, rh_f)) {
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

        gui_do();
    }

    return 0;
}
