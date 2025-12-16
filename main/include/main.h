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
#include "include/motor.h"

extern gptimer_handle_t gptimerHandle;





void readChannel(void);