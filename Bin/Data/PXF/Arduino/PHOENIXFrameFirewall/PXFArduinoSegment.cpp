// PXFArduinoSegment.cpp

#include "PXFArduino.h"

#if defined PXF_SEGMENT7
//----------------------------------------------------------------------------
void PXFArduino::_SegmentInit(int clkPin, int dataPin)
{
    mSegmentDisplay = SegmentDisplay(clkPin, dataPin);
}
//----------------------------------------------------------------------------
void PXFArduino::_SegmentSetBrightness(int brightness)
{
    mSegmentDisplay.setBrightness((uint8_t)brightness);
}
//----------------------------------------------------------------------------
void PXFArduino::_SegmentClear()
{
    mSegmentDisplay.clearDisplay();
}
//----------------------------------------------------------------------------
void PXFArduino::_SegmentDisplayInt(int val)
{
    mSegmentDisplay.display((int16_t)val);
}
//----------------------------------------------------------------------------
void PXFArduino::_SegmentDisplayFloat(float val)
{
    mSegmentDisplay.display(val);
}
//----------------------------------------------------------------------------
#endif