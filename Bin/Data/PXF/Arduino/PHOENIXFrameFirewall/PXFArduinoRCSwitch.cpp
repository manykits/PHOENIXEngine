// PXFArduinoRCSwitch.cpp

#include "PXFArduino.h"

#if defined PXF_RCSWITCH

//----------------------------------------------------------------------------
void PXFArduino::_InitRCSwitchReceive(int pinTimerIndex)
{
    mRCSwitch.enableReceive(pinTimerIndex); 
}
//----------------------------------------------------------------------------
void PXFArduino::_RCInit(int pin)
{
  mRCSwitch.enableTransmit(pin);
}
//----------------------------------------------------------------------------
void PXFArduino::_RCSend(long val)
{
    mRCSwitch.send(val, 24);
    delay(100);
    mRCSwitch.send(val, 24);
    delay(100);
    mRCSwitch.send(val, 24);
}
//----------------------------------------------------------------------------

#endif
