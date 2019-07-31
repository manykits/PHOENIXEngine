// PhoenixFrameFirewall.ino

#include "PXFArduino.h"

PXFArduino manykit;

void setup() 
{ 
  // Serial
  Serial.begin(9600);

  manykit.Init(true);             

#if defined PXF_AXIS
  manykit._InitAxis();
#endif

#if defined PXF_SSD1306
  manykit._ScreenInit();
#endif
}

String recvStr;
void loop()
{   
  while (Serial.available())
  {
    char c = Serial.read();
    
    if ('\n' == c)
    {
      if (recvStr.length() > 0)
      {
        // to compatile with PHOENIXEngine
        // 2 for length,2 for id
        String cmdStr = recvStr.substring(4);
        manykit.OnCMD(cmdStr);
      }
      recvStr = "";
    }
    else
    {
      recvStr += c;
    }
  }

  manykit.Tick(); 
}
