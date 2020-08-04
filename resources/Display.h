#include "mbed.h"
#define START_THREAD 1

class Display
{
public:
    Display();
    static void threadStarter(void const *p);
    void lcdDraw();
    Thread displayThread;
};