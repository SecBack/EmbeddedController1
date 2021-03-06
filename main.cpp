#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"

// global variables 
int buttonStatus = 0;
int angle;
int counter = 9000;

char angleText[30];
char buttonText[30];
char counterText[30];

InterruptIn button(D2);
DigitalOut led(D3);
AnalogIn rotary(A0);

// turn button into a switch
void buttonSwitch() {
    if (buttonStatus == 0) {
        buttonStatus = 1;
    } else {
        buttonStatus = 0;
    }
}

// start screen for display
void lcdStart()
{
    // init display
    BSP_LCD_Init();
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);

    // font, text and background color
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_Clear(uint32_t LCD_COLOR_GREEN);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_GREEN);

    // by line
    BSP_LCD_DisplayStringAt(0, LINE(3), (uint8_t *)"Advanced Case", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, LINE(4), (uint8_t *)"31/7/2020", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"By Albert", CENTER_MODE);
    HAL_Delay(3000);
    BSP_LCD_Clear(uint32_t LCD_COLOR_BLUE);
}

// called on display update, takes care of everything outputted to the display
void lcdDraw()
{   
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

// increments the counter 
void counterIncrement()
{
    counter++;
    if (counter >= 9999) {
        counter = 0;
    }
}

// when the switch is on make the led blink
void blink() 
{
    while(true) {
        // print to the serial, to demoenstrate async
        printf ("angle: %d \n", angle);
        if (buttonStatus == true) {
            led = !led;
            // blink rate is dependant on the angle on the rotary angle sensor
            ThisThread::sleep_for(std::chrono::milliseconds(angle));
        }
    }
}

int main()
{
    // draw the start screen
    lcdStart();
    // async callback to buttonswitch() when button is on the way down
    button.rise(&buttonSwitch);

    Thread ledController;
    // blick async
    ledController.start(&blink);

    Thread LCD_control;
    // draw to the display async
    LCD_control.start(&lcdDraw);

    // the code run below is run as fast as possible, meaning that although some of the values
    // are updated as often as possible, they might be showen at a different rate on the display
    while(true) {
        // take input from rotary angle sonsor as often as possible
        angle = (int)(rotary.read() * 1024);
        // count as fast as possible
        counterIncrement();

        if (buttonStatus == 0) {
            led.write(0);
        }
    }
}
