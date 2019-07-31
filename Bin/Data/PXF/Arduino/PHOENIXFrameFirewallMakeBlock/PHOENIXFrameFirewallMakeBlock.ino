// PhoenixFrameFirewall.ino

#include "PXFArduino.h"

#define PXF_SEG
#if defined PXF_SEG
#include "MeOrion.h"
#endif

PXFArduino pxa;

void setup() 
{  
  // Serial
  Serial.begin(9600);

  pxa.Init(true);

#if defined PXF_SSD1306
  pxa._ScreenInit();
#endif

  pxa.InitPinTickEvent();
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
        pxa.OnCMD(cmdStr);
      }
      recvStr = "";
    }
    else
    {
      recvStr += c;
    }
  }

  pxa.Tick(); 
}
