#include "include/readArray.h"
#include "include/ws2812b.h"
#include "include/settings.h"
#include "include/main.h"


uint8_t rValue, gValue, bValue = 0;
int posX, posY = 0;
uint8_t array[513] = {0};


void readArray(int *channel) {
    int len = uart_read_bytes(UART_NUM_1, &array, 513, 1 / portTICK_PERIOD_MS);
    if(len > 0){
        printf("Received %d bytes from UART\n", len);
        for(int i = 0; i < len; i++){
            printf("Byte %d: %d\n", i, array[i]);
        }

        updateLightAndPosition(*channel);
        printf("Channel: %d, R: %d, G: %d, B: %d, X: %d, Y: %d\n", *channel, rValue, gValue, bValue, ((posX*360)/255), posY);
    }
}

void updateLightAndPosition(int channel){
        if(channel > 507){
            rValue = 0;
            gValue = 0;
            bValue = 0;
            posX = 0;
            posY = 0;
        }
        else{
            rValue = array[channel+1]; //we beginnen bij channel+1 omdat de eerste byte een startbyte is.
            gValue = array[channel+2];
            bValue = array[channel+3];
            posX = array[channel+4];
            posY = array[channel+5];
        }


        for (int i = 0; i < CONFIG_WS2812B_LED_COUNT; i++){
            setColor(rValue, gValue, bValue, i);
        }
        set_hoek_xpos((posX*360)/255);
        set_hoek_ypos(posY);
}
