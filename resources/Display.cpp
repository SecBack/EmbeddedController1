#include "stm32746g_discovery_lcd.h"
#include "Display.h"
#include "GlobalData.h"
#include "mbed.h"

// when this object is created create thread
Display::Display()
{
    // run lcdDraw async
    lcdDrawer.start(callback(this, &Display::lcdDraw));
}

// called on display update, takes care of everything outputted to the display
void Display::lcdDraw()
{
    //displayThread.signal_wait(START_THREAD);
    while(true) {
         // preparing the printing
        if (angle < 10) {
            sprintf(angleText, "angle:    %i", (int)angle);
        } else if (angle < 100) {
            sprintf(angleText, "angle:   %i", (int)angle);
        } else if (angle < 1000) {
            sprintf(angleText, "angle:  %i", (int)angle);
        } else {
            sprintf(angleText, "angle: %i", (int)angle);
        }
        sprintf(buttonText, "Status: %i", buttonStatus);
        sprintf(counterText, "counter: %i", counter);
        
        // background color
        BSP_LCD_SetFont(&Font12);
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_SetBackColor(LCD_COLOR_BLUE);

        // write variables to display
        BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *) &angleText, LEFT_MODE);
        BSP_LCD_DisplayStringAt(0, LINE(2), (uint8_t *) &buttonText, LEFT_MODE);

        // print counter and change color
        BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
        BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *) &counterText, RIGHT_MODE);
        BSP_LCD_SetBackColor(LCD_COLOR_BLUE);

        // clearing lines instead of the whole display for speed
        BSP_LCD_ClearStringLine(5);
        BSP_LCD_ClearStringLine(6);

        // display update rate
        ThisThread::sleep_for(250ms);
    }
}