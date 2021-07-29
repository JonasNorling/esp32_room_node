#include "logging/log.h"
#include <drivers/gpio.h>
#include <drivers/sensor.h>

LOG_MODULE_REGISTER(main);

const struct device *l_gpio1 = NULL;
const struct device *l_dht22 = NULL;

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

    while (true) {
        k_sleep(K_MSEC(2000));
        gpio_pin_set(l_gpio1, GPIO_PIN_LED, 1);
        k_sleep(K_MSEC(100));
        gpio_pin_set(l_gpio1, GPIO_PIN_LED, 0);

        if (sensor_sample_fetch(l_dht22)) {
            LOG_ERR("Failed to fetch sample");
        }

        struct sensor_value temp;
        struct sensor_value rh;
        if (sensor_channel_get(l_dht22, SENSOR_CHAN_AMBIENT_TEMP, &temp)) {
            LOG_ERR("Failed to read temperature");
        }
        if (sensor_channel_get(l_dht22, SENSOR_CHAN_HUMIDITY, &rh)) {
            LOG_ERR("Failed to read humitidy");
        }

        LOG_INF("Temp: %d.%06d, RH: %d.%06d",
                temp.val1, temp.val2, rh.val1, rh.val2);
    }

    return 0;
}
