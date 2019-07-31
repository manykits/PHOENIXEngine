// PXFArduinoDHT.cpp

#include "PXFArduino.h"

#if defined PXF_DHT

//----------------------------------------------------------------------------
void PXFArduino::_DHTInit(PXFPin pin)
{
  int pinArduino = PXFPin2Pin(pin);
  mDHT = PXFDHT(pinArduino, PXF_DHTTYPE);
  mDHT.begin();
  mIsInitedDHT = true;
}
//----------------------------------------------------------------------------
void PXFArduino::_DHTSendTemperatureHumidity()
{
  float temp = mDHT.readTemperature();
  float humi = mDHT.readHumidity();
  
  if (!isnan(humi) && !isnan(temp)) 
  {
    unsigned char cmdCh = sOptTypeVal[OT_RETURN_DHTTEMP];
    char strCMDCh[32];
    memset(strCMDCh, 0, 32);
    itoa(cmdCh, strCMDCh, 10);
  
    Serial.print("0000");
    Serial.print(String(strCMDCh)); 
    Serial.print(" ");
    Serial.print(temp);
    Serial.print(" ");
    Serial.println(humi);
  }
}
//----------------------------------------------------------------------------

#endif
