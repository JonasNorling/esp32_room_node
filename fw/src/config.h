#pragma once

enum config_idx {
    CONFIG_IDX_TRIGGER_TEMP,
    CONFIG_IDX_HYSTERESIS,
    CONFIG_IDX_SERVO_LEFT,   // ms
    CONFIG_IDX_SERVO_RIGHT,  // ms
    CONFIG_IDX__LAST
};

int config_init(void);
int config_get_float(enum config_idx index, float *value);
int config_set_float(enum config_idx index, float value);
