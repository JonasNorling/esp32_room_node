#pragma once

enum config_idx {
    CONFIG_IDX_THING,
    CONFIG_IDX__LAST
};

int config_init(void);
int config_get_int(enum config_idx index, int *value);
int config_set_int(enum config_idx index, int value);
