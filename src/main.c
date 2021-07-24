#include "logging/log.h"
#include <drivers/gpio.h>

LOG_MODULE_REGISTER(main);

const struct device *l_gpio1 = NULL;

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

int main(int argc, char **argv)
{
    LOG_INF("Hello there");

    if (gpio_init()) {
        return 1;
    }

    while (true) {
        k_sleep(K_MSEC(100));
        gpio_pin_set(l_gpio1, GPIO_PIN_LED, 1);
        k_sleep(K_MSEC(100));
        gpio_pin_set(l_gpio1, GPIO_PIN_LED, 0);
    }

    return 0;
}
