// PXFArduinoLEDMatrix.cpp

#include "PXFArduino.h"

#if defined PXF_LEDMATRIX

void PXFArduino::_LEDMatrixInit(int sckPin, int dinPin)
{
    mLEDMatrix = LEDMatrix(sckPin, dinPin);
    mLEDMatrix.setColorIndex(1);
}
//----------------------------------------------------------------------------
void PXFArduino::_LEDMatrixSetBrightness(int brightness)
{
  mLEDMatrix.setBrightness(brightness);
}
//----------------------------------------------------------------------------
void PXFArduino::_LEDMatrixClearScreen()
{
    mLEDMatrix.clearScreen();
}
//----------------------------------------------------------------------------
void PXFArduino::_LEDMatrixSetColorIndex(int iColor_Number)
{
    mLEDMatrix.setColorIndex(iColor_Number);
}
//----------------------------------------------------------------------------
void PXFArduino::_LEDMatrixDrawBitmap(int8_t x, int8_t y, uint8_t bitmap_Width, 
    uint8_t *bitmap)
{
   mLEDMatrix.drawBitmap(x, y, bitmap_Width, bitmap);
}
//----------------------------------------------------------------------------
void PXFArduino::_LEDMatrixLightPos(int8_t x, int8_t y, int width, bool onOff)
{
    if (width > 16)
        width = 16;
        
    if (onOff)
    {
        unsigned char buf[16]={128,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
        for (int i=0; i<width; i++)
        {
            buf[i] = 128;
        }

        mLEDMatrix.drawBitmap(x,y,16,buf);
    }
    else
    {
        unsigned char buf[16]={0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
        for (int i=0; i<width; i++)
        {
            buf[i] = 0;
        }
        mLEDMatrix.drawBitmap(x,y,16,buf);
    }
}
//----------------------------------------------------------------------------
void PXFArduino::_LEDMatrixDrawStr(int16_t x_position, int8_t y_position,
     const char *str)
{
    mLEDMatrix.drawStr(x_position, y_position, str);
}
//----------------------------------------------------------------------------
void PXFArduino::_LEDMatrixShowClock(uint8_t hour, uint8_t minute,
 bool isPointOn)
{
    mLEDMatrix.showClock(hour, minute, isPointOn);
}
//----------------------------------------------------------------------------
void PXFArduino::_LEDMatrixShowNum(float value,uint8_t val)
{
    mLEDMatrix.showNum(value, val);
}
//----------------------------------------------------------------------------

#endif
