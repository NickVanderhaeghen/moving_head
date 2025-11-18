#include "include/ws2812b.h"


static led_strip_handle_t led_strip;


void config_led(){
    led_strip_config_t strip_config = {
        .strip_gpio_num = CONFIG_WS2812B_GPIO,
        .max_leds = CONFIG_WS2812B_LED_COUNT
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false,
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));

    led_strip_clear(led_strip);
}


void setColor(uint8_t rValue, uint8_t gValue, uint8_t bValue, int index){
    led_strip_set_pixel(led_strip, index, rValue, gValue, bValue);
    led_strip_refresh(led_strip);
}