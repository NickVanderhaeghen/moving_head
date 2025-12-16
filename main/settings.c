#include "include/settings.h"

#define RX_BUF_SIZE     2048
#define TX_BUF_SIZE     0
#define QUEUE_SIZE      50
#define PATTERN_TO_DET  0X00

QueueHandle_t uart0_queue;
uart_event_t event;



void init(){
    gpio_config_t gpio_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL<<CONFIG_GPIO_DIP_0) | (1ULL<<CONFIG_GPIO_DIP_1) | (1ULL<<CONFIG_GPIO_DIP_2) | (1ULL<<CONFIG_GPIO_DIP_3) | 
                        (1ULL<<CONFIG_GPIO_DIP_4) | (1ULL<<CONFIG_GPIO_DIP_5) | (1ULL<<CONFIG_GPIO_DIP_6) | (1ULL<<CONFIG_GPIO_DIP_7) | (1ULL<<CONFIG_GPIO_DIP_8),
        .pull_down_en = true,
        .pull_up_en = false,
    };
    gpio_config(&gpio_conf);


    uart_config_t uart_config = {
        .baud_rate = CONFIG_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_2,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
     
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_1, UART_PIN_NO_CHANGE, CONFIG_UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));



    /*
    1 uart byte = 11bit
    1/250000s = 4us per bit
    11 * 4us = 44us per byte
    we willen een timeout na 88us dus 2 bytes
    */
    uart_set_rx_timeout(UART_NUM_1, 2);


    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_1, RX_BUF_SIZE, TX_BUF_SIZE, QUEUE_SIZE, &uart0_queue, 0)); //RX buffer
    //uart_enable_pattern_det_baud_intr(UART_NUM_1, PATTERN_TO_DET, 1, 9, 0, 0); //look for 0x00 byt









    gpio_config_t gpio_motor_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL<<CONFIG_GPIO_COIL_1_X) | (1ULL<<CONFIG_GPIO_COIL_2_X) | (1ULL<<CONFIG_GPIO_COIL_3_X) | (1ULL<<CONFIG_GPIO_COIL_4_X) 
                        | (1ULL<<CONFIG_GPIO_COIL_1_Y) | (1ULL<<CONFIG_GPIO_COIL_2_Y) | (1ULL<<CONFIG_GPIO_COIL_3_Y) | (1ULL<<CONFIG_GPIO_COIL_4_Y),
        .pull_down_en = false,
        .pull_up_en = false,
    };
    gpio_config(&gpio_motor_config);


    gpio_config_t gpio_home_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL<<CONFIG_GPIO_HOME_X) | (1ULL<<CONFIG_GPIO_HOME_Y),
        .pull_down_en = true,
        .pull_up_en = false,
    };
    gpio_config(&gpio_home_config);

    gptimer_config_t gptimer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000, // 1 tick = 1 µs

    };
    gptimer_new_timer(&gptimer_config, &gptimerHandle);


    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,
        .alarm_count = 6000,
        .flags.auto_reload_on_alarm = true,
    };
    gptimer_set_alarm_action(gptimerHandle, &alarm_config);


    gptimer_event_callbacks_t gptimer_callbacks = {
        .on_alarm = alarmGaatAf,
    };
    gptimer_register_event_callbacks(gptimerHandle, &gptimer_callbacks, NULL);

    gptimer_enable(gptimerHandle);
    gptimer_start(gptimerHandle);

}
