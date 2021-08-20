#pragma once

int gui_init(void);
int gui_update(int call, float temp, float rh);
int gui_do(void);
int gui_encoder_init(bool (read_cb)(lv_indev_drv_t*, lv_indev_data_t*));
