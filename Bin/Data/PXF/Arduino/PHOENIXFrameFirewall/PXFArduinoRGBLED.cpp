// PXFArduinoRGBLED.cpp

#include "PXFArduino.h"

#if defined PXF_LEDSTRIP

//----------------------------------------------------------------------------
void PXFArduino::_RGBLEDInit(PXFPin pin, int num)
{
    int pinArduino = PXFPin2Pin(pin);

    mWS2812 = WS2812(num);
    mWS2812.setOutput(pinArduino);
}
//----------------------------------------------------------------------------
void PXFArduino::_RGBLEDSetColor(int index, int r, int g, int b)
{
    cRGB c;
    c.r = r;
    c.g = g;
    c.b = b;
    mWS2812.set_crgb_at(index, c);
    mWS2812.sync();
}
//----------------------------------------------------------------------------

#endif