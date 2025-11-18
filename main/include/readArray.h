#pragma once

#include <include/ws2812b.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <driver/uart.h>
#include <esp_log.h>
#include <driver/gpio.h>
#include <esp_timer.h>
#include <string.h>



void readArray(int *channel);

void updateLightAndPosition(int channel);