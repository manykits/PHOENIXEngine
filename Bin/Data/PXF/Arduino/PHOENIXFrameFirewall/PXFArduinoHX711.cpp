// PXFArduinoHX711.cpp

#include "PXFArduino.h"

//----------------------------------------------------------------------------
void PXFArduino::_HX711Init(int index, int pinOut, int pinClk)
{
#if defined PXF_XH711
    if (0 == index)
    {
        mXH711_0 = HX711(pinOut, pinClk);
    }
    else if (1 == index)
    {
        mXH711_1 = HX711(pinOut, pinClk);
    }
    else if (2 == index)
    {
        mXH711_2 = HX711(pinOut, pinClk);
    }
    else if (3 == index)
    {
        mXH711_3 = HX711(pinOut, pinClk);       
    }
#endif
}
//----------------------------------------------------------------------------
float PXFArduino::_ReadHX711(int index)
{
    float val = 0.0;
    
    #if defined PXF_XH711
    if (0 == index)
    {
        val = mXH711_0.read_average(4); 
    }
    else if (1 == index)
    {
        val = mXH711_1.read_average(4); 
    }
    else if (2 == index)
    {
        val = mXH711_2.read_average(4); 
    }
    else if (3 == index)
    {
         val = mXH711_3.read_average(4);      
    }
    #endif
    
    return val;
}
//----------------------------------------------------------------------------
void PXFArduino::_HXSend(int index, float val)
{
#if defined PXF_XH711
    unsigned char cmdCh = sOptTypeVal[OT_RETURN_HX711];
    char strCMDCh[32];
    memset(strCMDCh, 0, 32);
    itoa(cmdCh, strCMDCh, 10);
    
    char strI[32];
    memset(strI, 0, 32);
    itoa(index, strI, 10); // 10 is decimal

    char strF[32];
    memset(strF, 0, 32);
    itoa((int)val, strF, 10); // 10 is decimal

    //char strF[] = "";  
    //dtostrf(val, 2, 1, strF);

    String sendStr = String(strCMDCh) + " " + String(strI) + " " + String(strF);
    _SendCMD(sendStr);
#endif
}
//----------------------------------------------------------------------------
