#pragma once

#include "include/main.h"

void set_hoek_xpos(int hoek);
void set_hoek_ypos(int hoek);
void stepping_xpos(void);
void stepping_ypos(void);
void homing(void);
bool alarmGaatAf(gptimer_handle_t gptimerHandle, const gptimer_alarm_event_data_t *edata, void *user_ctx);