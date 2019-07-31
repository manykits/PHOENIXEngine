/*
  Example for different sending methods
  
  http://code.google.com/p/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  mySwitch.enableTransmit(11);
}

void loop() {
  int valOpen = digitalRead(8);
  int valClose = digitalRead(9);
  int valStop = digitalRead(10);
  
  if (LOW == valOpen)
  {
    Serial.println("open");
    mySwitch.send(4282155, 24);
    delay(1000);  
  }
  if (LOW == valClose)
  {
    Serial.println("close");
    mySwitch.send(4282304, 24);
    delay(1000); 
  }
  if (LOW == valStop)
  {
    Serial.println("stop");
    mySwitch.send(4282160, 24);
    delay(1000); 
  }
}
