#include "include/motor.h"

//MOTOR ST5709S1208-B
#define GEAR_RATIO_XPOS             1                                               //GEAR RATIO
#define STAP_ANGLE_INTERNAL_XPOS    0.9                                             //INTERNAL STEP ANGLE PER STEP IN DEGREES
#define STAP_ANGLE_EXTERNAL_XPOS    (STAP_ANGLE_INTERNAL_XPOS / GEAR_RATIO_XPOS)    //EXTERNAL STEP ANGLE PER STEP IN DEGREES
#define AMOUNT_STEPS_XPOS           (360/STAP_ANGLE_EXTERNAL_XPOS)                  //NUMBER OF STEPS FOR 360 DEGREES
#define MOTOR_XPOS_HOME_PIN         CONFIG_GPIO_HOME_X                                     //HOME PIN
#define MOTOR_XPOS_MAX_ROTATION     1                                               //MAX ROTATION
#define MOTOR_XPOS_MAX_STEPS        AMOUNT_STEPS_XPOS*MOTOR_XPOS_MAX_ROTATION       //MAX STEPS

//MOTOR ST28
#define GEAR_RATIO_YPOS             64                                              //GEAR RATIO
#define STAP_ANGLE_INTERNAL_YPOS    11.25                                           //INTERNAL STEP ANGLE PER STEP IN DEGREES
#define STAP_ANGLE_EXTERNAL_YPOS    (STAP_ANGLE_INTERNAL_YPOS / GEAR_RATIO_YPOS)    //EXTERNAL STEP ANGLE PER STEP IN DEGREES
#define AMOUNT_STEPS_YPOS           (360/STAP_ANGLE_EXTERNAL_YPOS)                  //NUMBER OF STEPS FOR 360 DEGREES
#define MOTOR_YPOS_HOME_PIN         CONFIG_GPIO_HOME_Y                                     //HOME PIN
#define MOTOR_YPOS_MAX_ROTATION     0.5                                             //MAX ROTATION
#define MOTOR_YPOS_MAX_STEPS        AMOUNT_STEPS_YPOS*MOTOR_YPOS_MAX_ROTATION       //MAX STEPS



int steps_xpos = 0;
int steps_ypos = 0;
int angle_x = 0;
int angle_y = 0;
bool direction_xpos = true; //true = clockwise, false = counterclockwise
bool direction_ypos = true; //true = clockwise, false = counterclockwise
bool hoekxhome = false;
bool hoekyhome = false;

int xpos_pins[4] = {CONFIG_GPIO_COIL_1_X, CONFIG_GPIO_COIL_2_X, CONFIG_GPIO_COIL_3_X, CONFIG_GPIO_COIL_4_X};

int ypos_pins[4] = {CONFIG_GPIO_COIL_1_Y, CONFIG_GPIO_COIL_2_Y, CONFIG_GPIO_COIL_3_Y, CONFIG_GPIO_COIL_4_Y};

const int8_t stappen [4][4] = {
    {1,0,1,0},
    {0,1,1,0},
    {0,1,0,1},
    {1,0,0,1}
};

const int8_t stappeny [4][4] = {
    {1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1}
};

//DOET DE OMREKENING VAN HOEK NAAR AANTAL STAPPEN EN BEPAALT DE RICHTING
void set_hoek_xpos(int hoek){
    if(hoek == 0){
        hoekxhome = true;
    }
    if(hoek <= 360){        //deze if zorgt ervoor dat er niet meer dan 1 volledige rotatie gemaakt wordt
        if(angle_x < hoek){
            direction_xpos = false;
            steps_xpos = ((hoek - angle_x) * AMOUNT_STEPS_XPOS) / 360;
        }
        else{
            direction_xpos = true;
            steps_xpos = ((angle_x - hoek) * AMOUNT_STEPS_XPOS) / 360;
        }
            angle_x = hoek;
    }   
}     
//DOET DE OMREKENING VAN HOEK NAAR AANTAL STAPPEN EN BEPAALT DE RICHTING
void set_hoek_ypos(int hoek){  
    if(hoek == 0){
        hoekyhome = true;
    }
    if(hoek <= 180){    //deze if zorgt ervoor dat er niet meer dan 0.5 volledige rotatie gemaakt wordt
        if(angle_y < hoek){
            direction_ypos = false;
            steps_ypos = ((hoek - angle_y) * AMOUNT_STEPS_YPOS) / 360;
        }
            
        else{
            direction_ypos = true;
            steps_ypos = ((angle_y - hoek) * AMOUNT_STEPS_YPOS) / 360;
        }
            angle_y = hoek;
    }
}
//GAAT EEN STAPJE DOEN DOOR DE MOTOR TE AANSTUREN
void stepping_xpos(void)
{
    if(steps_xpos>0){
        steps_xpos--;
        static int step = 0;
        step++;
        step = step % 4;
        
        if(direction_xpos){
            for(int i = 0; i < 4; i++){
                gpio_set_level(xpos_pins[i], stappen[step][i]);
            }
        }
        else{
            for(int i = 0; i < 4; i++){
                gpio_set_level(xpos_pins[i], stappen[(4 - step) % 4][i]);
            }
        }        
    }
    if (gpio_get_level(MOTOR_XPOS_HOME_PIN) == 1 && hoekxhome == true){
        steps_xpos = 0;
        hoekxhome = false;
    }
}
//GAAT EEN STAPJE DOEN DOOR DE MOTOR TE AANSTUREN
void stepping_ypos(void)
{
    if(steps_ypos>0){
        steps_ypos--;
        static int step = 0;
        step++;
        step = step % 4;
        
        if(direction_ypos){
            for(int i = 0; i < 4; i++){
                gpio_set_level(ypos_pins[i], stappeny[step][i]);
            }
        }
        else{
            for(int i = 0; i < 4; i++){
                gpio_set_level(ypos_pins[i], stappeny[(4 - step) % 4][i]);
            }
        }
    }
    if (gpio_get_level(MOTOR_YPOS_HOME_PIN) == 1 && hoekyhome == true){
        steps_ypos = 0;
        hoekyhome = false;
    }
}

//ZORGT VOOR HET HOMEN VAN DE MOTOREN, GAAT DUS TERUG NAAR HET BEGINPUNT
void homing(void){
    gptimer_stop(gptimerHandle);
    bool xhome = false;
    bool yhome = false;
    while (xhome == false)
    {
        steps_xpos = 1;
        stepping_xpos();
        if(gpio_get_level(MOTOR_XPOS_HOME_PIN) == 1){
            xhome = true;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
    while (yhome == false)
    {
        steps_ypos = 1;
        stepping_ypos();
        if(gpio_get_level(MOTOR_YPOS_HOME_PIN) == 1){
            yhome = true;
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
    gptimer_start(gptimerHandle);

}

/*
ZORGT ERVOOR DAT ER BIJ ELKE ALARM INTERRUPT EEN STAPJE WORDT GEZET
ZO KUNNEN WE DE TIMING VAN DE MOTOREN CONTROLEREN
WE KUNNEN GEEN DELAY GEBRUIKEN WANT ANDERS BLIJFT ONS PROGRAMMA HANGEN
*/
bool alarmGaatAf(gptimer_handle_t gptimerHandle, const gptimer_alarm_event_data_t *edata, void *user_ctx){
    stepping_xpos();
    stepping_ypos();

    return false;
}
