#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

// Blinking rate in milliseconds
#define BLINKING_RATE 500ms
bool buttonStatus = false;

DigitalOut led(D3); // Initialise the digital pin LED1 as an output

void buttonSwitch() {
    if (buttonStatus == false) {
        led.write(1);
        buttonStatus = true;
    } else {
        led.write(0);
        buttonStatus = false;
    }
}

void lcdStart()
{
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);

    BSP_LCD_SetFont(&Font12);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_GREEN);

    BSP_LCD_DisplayStringAt(0, LINE(3), (uint8_t *)"Advanced Case", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, LINE(4), (uint8_t *)"31/7/2020", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"By Albert", CENTER_MODE);
    HAL_Delay(4000);
    BSP_LCD_cLEAR
}

void lcdDraw()
{
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);

    BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"insert info here", CENTER_MODE);    
}

int main()
{
    Thread LCD_control;
    lcdStart();

    InterruptIn button(D2);
    button.rise(&buttonSwitch);
    AnalogIn rotary(A0);
    float angle;

    while(true) {
        while (buttonStatus == true) {
            angle = rotary.read() * 1024;
            printf ("angle: %d \n", int(angle));
            led = !led;
            wait_us(angle*1000);
        }
        LCD_control.start(&lcdDraw);
    }
}
