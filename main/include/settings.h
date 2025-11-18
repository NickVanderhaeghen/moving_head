#pragma once

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <driver/gptimer.h>

#include <driver/uart.h>

#include <esp_log.h>

#include "include/main.h"

extern QueueHandle_t uart0_queue;
extern uart_event_t event;

void init();
