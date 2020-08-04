#include "stm32746g_discovery_lcd.h"
#include "Display.h"
#include "GlobalData.h"
#include "mbed.h"
#include "rtos.h"

Display::Display()
    : displayThread(&Display::threadStarter, this, osPriorityNormal, 1024)
{
    //displayThread.signal_set(START_THREAD); 
    displayThread.flags_set(START_THREAD);
}

void Display::threadStarter(void const *p)
{
    Display *instance = (Display*)p;
    instance->lcdDraw();
}

// called on display update, takes care of everything outputted to the display
void Display::lcdDraw()
{
    //displayThread.signal_wait(START_THREAD);
    displayThread.flags_set(6);
    while(true) {
         // preparing the printing
        if (GlobalData::angle < 10) {
            sprintf(GlobalData::angleText, "angle:    %i", (int)GlobalData::angle);
        } else if (GlobalData::angle < 100) {
            sprintf(GlobalData::angleText, "angle:   %i", (int)GlobalData::angle);
        } else if (GlobalData::angle < 1000) {
            sprintf(GlobalData::angleText, "angle:  %i", (int)GlobalData::angle);
        } else {
            sprintf(GlobalData::angleText, "angle: %i", (int)GlobalData::angle);
        }
        sprintf(GlobalData::buttonText, "Status: %i", GlobalData::buttonStatus);
        sprintf(GlobalData::counterText, "counter: %i", GlobalData::counter);
        
        // background color
        BSP_LCD_SetFont(&Font12);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_SetBackColor(LCD_COLOR_BLUE);

        // write variables to display
        BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *) &GlobalData::angleText, LEFT_MODE);
        BSP_LCD_DisplayStringAt(0, LINE(2), (uint8_t *) &GlobalData::buttonText, LEFT_MODE);

        // print counter and change color
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
        BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *) &GlobalData::counterText, RIGHT_MODE);
        BSP_LCD_SetBackColor(LCD_COLOR_BLUE);

        // clearing lines instead of the whole display for speed
        BSP_LCD_ClearStringLine(5);
        BSP_LCD_ClearStringLine(6);

        // display update rate
        ThisThread::sleep_for(250ms);
    }
}