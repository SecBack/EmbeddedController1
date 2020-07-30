#include "mbed.h"


// Blinking rate in milliseconds
#define BLINKING_RATE 500ms


int main()
{
    DigitalOut led(LED1); // Initialise the digital pin LED1 as an output
    DigitalIn button(D8); // Configure P1_14 pin as input

    int buttonStatus = 0; // TODO implement button like a switch

    while (true) {
        if (button.read() == 1) {
            led.write(1);
        }
        if (button.read() == 0) {
            led.write(0);
        }
    }
}
