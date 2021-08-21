#include <logging/log.h>
#include <drivers/flash.h>
#include <storage/flash_map.h>
#include <fs/nvs.h>

#include "config.h"

LOG_MODULE_REGISTER(config);

static struct nvs_fs l_fs;
static float l_values[CONFIG_IDX__LAST] = {
    [CONFIG_IDX_SERVO_LEFT] = 1.0,
    [CONFIG_IDX_SERVO_RIGHT] = 2.0,
};


static int config_read_float(uint16_t addr, float *value)
{
    int rc = nvs_read(&l_fs, addr, value, sizeof(*value));
    return rc != sizeof(*value);
}

static int config_write_float(uint16_t addr, float value)
{
    int rc = nvs_write(&l_fs, addr, &value, sizeof(value));
    if (rc != sizeof(value)) {
        LOG_ERR("Failed to write to NVS");
        return 1;
    }
    return 0;
}

static int config_load(void)
{
    for (int addr = 0; addr < ARRAY_SIZE(l_values); addr++) {
        (void)config_read_float(addr, &l_values[addr]);
    }
    return 0;
}

int config_get_float(enum config_idx index, float *value)
{
    if (index >= ARRAY_SIZE(l_values)) {
        LOG_ERR("Get bad config index");
        return 1;
    }

    *value = l_values[index];
    return 0;
}

int config_set_float(enum config_idx index, float value)
{
    if (index >= ARRAY_SIZE(l_values)) {
        LOG_ERR("Set bad config index");
        return 1;
    }

    l_values[index] = value;
    return config_write_float(index, value);
}

int config_init(void)
{
    struct flash_pages_info page_info = {};
    int rc;

    const struct device *flash = device_get_binding(DT_LABEL(DT_NODELABEL(flash)));
    if (!flash) {
        LOG_ERR("Failed to open flash device");
        return 1;
    }

    const size_t area_size = FLASH_AREA_SIZE(storage);
    l_fs.offset = FLASH_AREA_OFFSET(storage);
    rc = flash_get_page_info_by_offs(flash, l_fs.offset, &page_info);
    if (rc) {
        LOG_ERR("Failed to get page info");
        return 1;
    }
    l_fs.sector_size = page_info.size;
    l_fs.sector_count = area_size / page_info.size;

    LOG_INF("Flash area at 0x%x, page size 0x%x, count %d",
            l_fs.offset, l_fs.sector_size, l_fs.sector_count);

    rc = nvs_init(&l_fs, DT_LABEL(DT_NODELABEL(flash)));
    if (rc) {
        LOG_ERR("Failed to open NVS");
        return 1;
    }

    return config_load();
}
