
#include "include/main.h"


TaskHandle_t xt_array;
TaskHandle_t xt_readChannel;
gptimer_handle_t gptimerHandle = NULL;

int channel = 0;
uint8_t dmx_values[CONFIG_DMX_CHANNELS];

uint8_t rValue, gValue, bValue = 0;
int posX, posY = 0;


void readChannel(void){
/*
We hebben 9 dipswitches om 512 kanalen te representeren
We hebben hier 3 opeenvolgende kanalen nodig voor R G B geteld vanaf het ingestelde kanaal
We moeten dus 8 pinnen van de esp32 verbinden met de dipswitches en uitlezen via de pinnen
*/
        channel = 0;
        if(gpio_get_level(CONFIG_GPIO_DIP_0)){
            channel += 1;
        }
        if(gpio_get_level(CONFIG_GPIO_DIP_1)){
            channel += 2;
        }
        if(gpio_get_level(CONFIG_GPIO_DIP_2)){
            channel += 4;
        }
        if(gpio_get_level(CONFIG_GPIO_DIP_3)){
            channel += 8;
        }
        if(gpio_get_level(CONFIG_GPIO_DIP_4)){
            channel += 16;
        }
        if(gpio_get_level(CONFIG_GPIO_DIP_5)){
            channel += 32;
        }
        if(gpio_get_level(CONFIG_GPIO_DIP_6)){
            channel += 64;
        }
        if(gpio_get_level(CONFIG_GPIO_DIP_7)){
            channel += 128;
        }
        if(gpio_get_level(CONFIG_GPIO_DIP_8)){
            channel += 256;
        }
    
}



void updateLightAndPosition(){
    if(channel > (512-CONFIG_DMX_CHANNELS)){
        rValue = 0;
        gValue = 0;
        bValue = 0;
        posX = 0;
        posY = 0;
    }
    else{
        rValue = dmx_values[channel+1]; //we beginnen bij channel+1 omdat de eerste byte een startbyte is.
        gValue = dmx_values[channel+2];
        bValue = dmx_values[channel+3];
        posX = dmx_values[channel+4];
        posY = dmx_values[channel+5];
    }


    for (int i = 0; i < CONFIG_WS2812B_LED_COUNT; i++){
        setColor(rValue, gValue, bValue, i);
    }
    set_hoek_xpos((posX*360)/255);
    set_hoek_ypos(posY);
}


void app_main(void)
{
    init();
    homing();
    readChannel();
    printf("Starting on channel: %d\n", channel);

    while (1)
    {
        readArray(&channel, dmx_values);

        updateLightAndPosition();

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}