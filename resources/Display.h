#include "mbed.h"

class Display
{
public:
    Display();
    void lcdDraw();
    Thread lcdDrawer;    
};