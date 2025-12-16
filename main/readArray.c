#include "include/readArray.h"
#include "include/ws2812b.h"
#include "include/settings.h"
#include "include/main.h"


uint8_t array[513] = {0};


void readArray(int *channel, uint8_t *dmx_values){
    int len = uart_read_bytes(UART_NUM_1, &array, 513, 1 / portTICK_PERIOD_MS);
    if(len > 0){
        printf("Received %d bytes from UART\n", len);
        for(int i = 0; i < len; i++){
            printf("Byte %d: %d\n", i, array[i]);
            int pos = *channel+i;
            dmx_values[i] = array[pos];
        }
    }
}