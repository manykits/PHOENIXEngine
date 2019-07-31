// PXFArduinoDisplayss1306.cpp
#include "PXFArduino.h"

#if defined PXF_SSD1306

#define OLED_RESET 4
//----------------------------------------------------------------------------
void PXFArduino::_ScreenInit()
{
  mDisplay = new Adafruit_SSD1306(OLED_RESET);
  mDisplay->begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS);
  mDisplay->display();
}
//----------------------------------------------------------------------------

#endif

