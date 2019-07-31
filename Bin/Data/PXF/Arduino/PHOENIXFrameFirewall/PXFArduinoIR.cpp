// PXFArduinoIR.cpp

#include "PXFArduino.h"

//----------------------------------------------------------------------------
void PXFArduino::_IRInit_(int pin)
{
#if defined PXF_IR
    if (mIRrecv)
    {
        delete mIRrecv;
        mIRrecv = 0;
    }

    pinMode(pin, INPUT);
    mIRrecv = new IRrecv(pin);
    mIRrecv->enableIRIn();
#endif
}
//----------------------------------------------------------------------------
void PXFArduino::_IRSend(int val)
{ 
#if defined PXF_IR
    mIRsend.sendSony(val, 32);
    if (mIRrecv)
      mIRrecv->enableIRIn();
#endif
}
//----------------------------------------------------------------------------
void PXFArduino::_IRRecv(int val)
{
#if defined PXF_IR
    unsigned char cmdCh = sOptTypeVal[OT_RETURN_IR];
    char strCMDCh[32];
    memset(strCMDCh, 0, 32);
    itoa(cmdCh, strCMDCh, 10);

    Serial.print("0000");
    Serial.print(String(strCMDCh));
    Serial.print(" ");
    Serial.println(val);

    // if (7611 == val)
    // {
    //     _LeftRun(1,255);
    //     _RightRun(1, 255);
    // }
    // else if (-4645 == val)
    // {
    //     _LeftRun(2, 255);
    //     _RightRun(2, 255);
    // }
    // else if (-11233 == val)
    // {
    //     _LeftRun(2, 255);
    //     _RightRun(1, 255);
    // }
    // else if (19899 == val)
    // {
    //     _LeftRun(1, 255);
    //     _RightRun(2, 255);
    // }
    // else if (19227 == val)
    // {
    //     _LeftRun(0, 0);
    //     _RightRun(0, 0);
    // }
#endif
}
//----------------------------------------------------------------------------
