#pragma once

#include "math.h"
#include <led_strip.h>
#include <led_strip_rmt.h>
#include "esp_log.h"
#include "esp_err.h"




void config_led();
//config de ledstrip


void setColor(uint8_t rValue, uint8_t gValue, uint8_t bValue, int index);