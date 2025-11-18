#pragma once

#include <stdio.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/ledc.h"
#include "hal/ledc_types.h"
#include "esp_err.h"


#include "include/readArray.h"
#include "include/ws2812b.h"
#include "include/settings.h"

extern gptimer_handle_t gptimerHandle;

void set_hoek_xpos(int hoek);
void set_hoek_ypos(int hoek);
void stepping_xpos(void);
void stepping_ypos(void);
void back_to_home(void);
bool alarmGaatAf(gptimer_handle_t gptimerHandle, const gptimer_alarm_event_data_t *edata, void *user_ctx);
void readChannel(void);