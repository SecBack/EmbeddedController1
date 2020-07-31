#include "mbed.h"

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

int main()
{
    
    InterruptIn button(D2);
    button.rise(&buttonSwitch);
    AnalogIn rotary(A0);
    float angle;

    while(true) {
        while (buttonStatus == true) {
            angle = rotary.read() * 1024;
            printf ("angle: %d \n", int(angle));
            led = !led;
            ThisThread::sleep_for(angle);
        }
    }
}
