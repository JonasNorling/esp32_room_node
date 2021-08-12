#include "logging/log.h"
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <drivers/display.h>
#include <sys/byteorder.h>

LOG_MODULE_REGISTER(main);

const struct device *l_gpio1 = NULL;
const struct device *l_dht22 = NULL;
const struct device *l_display = NULL;

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

    return 0;
}

static int display_show(int call)
{
    #define PITCH 256
    #define W 160
    #define H 80

    static uint16_t buffer[PITCH * H];
    memset(buffer, 0x00, sizeof(buffer));
    for (int y = 0; y < H; y += 16) {
        for (int x = 0; x < W; x++) {
            buffer[y * PITCH + x] = 0xffff;
        }
    }

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x += 16) {
            buffer[y * PITCH + x] = 0xffff;
        }
    }

    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {
            buffer[y * PITCH + x] = sys_cpu_to_be16(0x001f);
        }
    }

    for (int y = 17; y < 32; y++) {
        for (int x = 0; x < 16; x++) {
            buffer[y * PITCH + x] = sys_cpu_to_be16(0x07e0);
        }
    }

    for (int y = 33; y < 48; y++) {
        for (int x = 0; x < 16; x++) {
            buffer[y * PITCH + x] = sys_cpu_to_be16(0xf800);
        }
    }

    for (int y = 49; y < 64; y++) {
        for (int x = 0; x < 16; x++) {
            buffer[y * PITCH + x] = sys_cpu_to_be16(call);
        }
    }

    for (int y = 0; y < call % H; y++) {
        for (int x = 17; x < 32; x++) {
            buffer[y * PITCH + x] = sys_cpu_to_be16(0x07e0);
        }
    }

    struct display_buffer_descriptor buf_desc = {
        .buf_size = sizeof(buffer),
        .pitch = PITCH,
        .width = W,
        .height = H,
    };

    if (display_write(l_display, 0, 0, &buf_desc, buffer)) {
        LOG_ERR("Display write failed");
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
        if (display_show(call)) {
            return 1;
        }
        call++;

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
    }

    return 0;
}
