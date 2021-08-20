#pragma once

int gui_init(void);
int gui_update(int call, float temp, float rh);
int gui_do(void);
lv_group_t *gui_get_lv_group();
