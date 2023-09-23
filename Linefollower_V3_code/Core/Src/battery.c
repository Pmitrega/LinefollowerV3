#include "main.h"
#include "battery.h"
#include <stdio.h>

#define THRE_BATT_VOLT (uint16_t)(4096/7.5/3.3*2.34 * 7.2)



#define LONG_BATT_LED_BLINKING()        htim4.Instance->PSC = 2000; \
                                        htim4.Instance->CCR2 = 5000

#define SHORT_BATT_LED_BLINKING()       htim4.Instance->PSC = 3000; \
                                        htim4.Instance->CCR2 = 300

#define BATT_RAW_TO_FLOAT_COEFF  (7.50f/2902.f)

/*extern variables declaration*/
extern uint16_t adc_readings[];
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim8;



BatteryState battery_state = BatteryNotInit;

void disable_motors(){
      /*disable motors*/
      htim8.Instance->CCR3 = 0;
      htim8.Instance->CCR4 = 0;
      htim8.Instance->CNT = 0;
      HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_3);
      HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_4);
}



void set_batt_led(BatteryState batt_state){
    if(batt_state == BatteryNotInit){
        htim4.Instance->PSC = 9000;
        htim4.Instance->CCR2 = 120;
    }
    else if(batt_state == BatteryLow){
        HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, 1);
        LONG_BATT_LED_BLINKING();
        TURN_OFF_SENSORS();
    }
    else if (batt_state == BatteryHigh){
        SHORT_BATT_LED_BLINKING();
    }
    
}

/******Public Functions Start******/
void battery_monitor(){
    if(battery_state == BatteryNotInit){
        set_batt_led(battery_state);
    }
    if(adc_readings[0] < THRE_BATT_VOLT){
        if(battery_state != BatteryLow){
        battery_state = BatteryLow;
        set_batt_led(battery_state);
        disable_motors();
        }
    }


    else if(adc_readings[0] >= THRE_BATT_VOLT + 15){
        if(battery_state != BatteryHigh){
        battery_state = BatteryHigh;
        set_batt_led(battery_state);
        }
    }
}

int get_battery_voltage_string(char* buffer){
    return sprintf(buffer, "B:%d\n", (int)((float)adc_readings[0]*BATT_RAW_TO_FLOAT_COEFF * 1000));
}

float get_battV_float(){
    return (float)adc_readings[0]*BATT_RAW_TO_FLOAT_COEFF;
}
/******Public Functions End******/