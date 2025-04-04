/******************************************************************************
 *
 * Module: Application
 *
 * File Name: App.c
 *
 * Description: Source file for Application Tasks.
 *
 * Author: Mohamed Tarek
 ******************************************************************************/

#include "App.h"
#include "Button.h"
#include "Led.h"
#include "Dio.h"
#include "Mcu.h"
#include "Port.h"

#define LED_PORT        DioConf_LED1_PORT_NUM
#define LED_PIN         DioConf_LED1_CHANNEL_NUM
#define BUTTON_PORT     DioConf_SW1_PORT_NUM
#define BUTTON_PIN      DioConf_SW1_CHANNEL_NUM
#define BUTTON_PRESSED  STD_LOW
#define BUTTON_RELEASED STD_HIGH

void Init_Task(void)
{
    /* Initialize Mcu Driver */
    Mcu_Init();

    Dio_Init(&Dio_Configuration);

    /* Configure LED Pin as Output */
    Port_ConfigType led_config = {LED_PORT, LED_PIN, OUTPUT, OFF, STD_LOW};
    Port_SetupGpioPin(&led_config);

    /* Configure Button Pin as Input with Pull-Up Resistor */
    Port_ConfigType button_config = {BUTTON_PORT, BUTTON_PIN, INPUT, PULL_UP, BUTTON_RELEASED};
    Port_SetupGpioPin(&button_config);
}

/* Description: Task executes every 20 Mili-seconds to check the button state */
void Button_Task(void)
{
    Button_RefreshState();
}

/* Description: Task executes every 40 Mili-seconds to refresh the LED */
void Led_Task(void)
{
    Led_RefreshOutput();
}

/* Description: Task executes every 60 Mili-seconds to get the button status and toggle the led */
void App_Task(void)
{
    static uint8 button_previous_state = BUTTON_RELEASED;
    static uint8 button_current_state  = BUTTON_RELEASED;

    button_current_state = Button_GetState(); /* Read the button state */

    /* Only Toggle the led in case the current state of the switch is pressed
     * and the previous state is released */ 
    if((button_current_state == BUTTON_PRESSED) && (button_previous_state == BUTTON_RELEASED))
    {
        Led_Toggle();
    }

    button_previous_state = button_current_state;
}
