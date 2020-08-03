#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

int buttonStatus = 0;
int angle;

InterruptIn button(D2);
DigitalOut led(D3); // Initialise the digital pin LED1 as an output
AnalogIn rotary(A0);



void buttonSwitch() {
    if (buttonStatus == 0) {
        buttonStatus = 1;
    } else {
        buttonStatus = 0;
    }
}

void lcdStart()
{
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);

    BSP_LCD_SetFont(&Font12);
    BSP_LCD_Clear(uint32_t LCD_COLOR_GREEN);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_GREEN);

    BSP_LCD_DisplayStringAt(0, LINE(3), (uint8_t *)"Advanced Case", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, LINE(4), (uint8_t *)"31/7/2020", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"By Albert", CENTER_MODE);
    HAL_Delay(3000);
    BSP_LCD_Clear(uint32_t LCD_COLOR_BLUE);
}

void lcdDraw()
{
    char text[30];
    if (angle < 10) {
        sprintf(text, "angle:    %i", (int)angle);
    } else if (angle < 100) {
        sprintf(text, "angle:   %i", (int)angle);
    } else if (angle < 1000) {
        sprintf(text, "angle:  %i", (int)angle);
    } else {
        sprintf(text, "angle: %i", (int)angle);
    }

    char buttonText[30];
    sprintf(buttonText, "Status: %i", buttonStatus);
    
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *) &text, CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, LINE(6), (uint8_t *) &buttonText, CENTER_MODE);
    HAL_Delay(50);
    BSP_LCD_ClearStringLine(5);
    BSP_LCD_ClearStringLine(6);
}

void blink() 
{
    while(true){
        if (buttonStatus == true) {
            led = !led;
            ThisThread::sleep_for(std::chrono::milliseconds(angle));
            printf ("angle: %d \n", angle);
        }
    }
}

int main()
{
    lcdStart();
    button.rise(&buttonSwitch);

    Thread ledController;
    ledController.start(&blink);

    while(true) {
        angle = (int)(rotary.read() * 1024);

        if (buttonStatus == 0) {
            led.write(0);
        }
            
        Thread LCD_control;
        LCD_control.start(&lcdDraw);
        ThisThread::sleep_for(15ms);
    }
}
