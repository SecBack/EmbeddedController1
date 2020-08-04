#include "mbed.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include "Display.h"
#include "GlobalData.h"

int buttonStatus = 0;
bool touchStatus = false;
int counterStatus = 0;
int counter = 9000;
int angle;

char angleText[30];
char buttonText[30];
char counterText[30];

InterruptIn button(D2);
DigitalOut led(D3);
InterruptIn touch(D4);
AnalogIn rotary(A0);

// turn button into a switch
void buttonSwitch()
{
    if (buttonStatus == 0) {
        buttonStatus = 1;
    } else {
        buttonStatus = 0;
    }
}

void touchSwitch()
{
    touchStatus = !touchStatus;
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

void clearAndHomeSerial()
{
//Serial Terminal
    printf("%c",char(27)); // ESC
    printf("[2J"); // clear screen
    printf("%c",char(27)); // ESC
    printf("[H"); // cursor to home
}

// increments the counter 
void counterIncrement()
{
    // primitive debugging to serial
    //printf("touchStatus %d \n", touchStatus);
    //clearAndHomeSerial();

    if (touchStatus == true) {
        counter++;        
    }
    if (counter >= 9999) {
        counter = 0;
    }
    // TODO reset counter on display touch
}

// when the switch is on make the led blink
void blink() 
{
    while(true) {
        // print to the serial, to demoenstrate async
        //printf ("angle: %d \n", angle);
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
    touch.rise(&touchSwitch);


    Thread ledController;
    // blick async
    ledController.start(&blink);

    // draw to the display async
    Display display;
    

    BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
    TS_StateTypeDef touchState;

    // the code run below is run as fast as possible, meaning that although some of the values
    // are updated as often as possible, they might be showen at a different rate on the display
    while(true) {
        BSP_TS_GetState(&touchState);

        // take input from rotary angle sonsor as often as possible
        angle = (int)(rotary.read() * 1024);
        // count as fast as possible
        counterIncrement();

        if (buttonStatus == 0) {
            led.write(0);
        }

        if(touchState.touchDetected) {
            printf("touch detected");
        }
    }
}
