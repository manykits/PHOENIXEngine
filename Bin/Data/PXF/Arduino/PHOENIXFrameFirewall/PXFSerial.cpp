// PXFSerial.cpp

#include "PXFSerial.h"

#if defined PXF_MP3

#include "PXFSerial.h"
#include <stdio.h>
#include <stdarg.h>

//----------------------------------------------------------------------------
PXFSerial::PXFSerial(void):
SoftwareSerial(0, 0)
{
}
//----------------------------------------------------------------------------
PXFSerial::PXFSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic):
 SoftwareSerial(receivePin, transmitPin, inverse_logic)
{
  _scratch = false;
  _hard = false;
  _polling = false;
  _RxPin = receivePin;
  _TxPin = transmitPin;
#if defined(__AVR_ATmega32U4__)
   _polling = false;
  if((receivePin == 0) && (transmitPin == 1))
  {
    _hard = true;
  }
  else
  {
    _hard = false;
  }
#else
  if((receivePin == 0) && (transmitPin == 1))
  {
    _hard = true;
  }
  else
  {
    _hard = false;
  }
#endif
}
//----------------------------------------------------------------------------
void PXFSerial::setHardware(bool mode)
{
  _hard = mode;
}
//----------------------------------------------------------------------------
void PXFSerial::begin(long baudrate)
{
  _bitPeriod = 1000000 / baudrate;
  if (_hard)
  {
#if defined(__AVR_ATmega32U4__)
    _scratch ? Serial.begin(baudrate) : Serial1.begin(baudrate);
#elif defined(__AVR_ATmega2560__)
    if (getPort() == PORT_15)
    {
      Serial3.begin(baudrate);
    }
    else if (getPort() == PORT_16)
    {
      Serial.begin(baudrate);
    }
    else
    {
      Serial2.begin(baudrate);
    }
#else
    Serial.begin(baudrate);
#endif
  }
  else
  {
    SoftwareSerial::begin(baudrate);
  }
}
//----------------------------------------------------------------------------
void PXFSerial::end(void)
{
  if (_hard)
  {
#if defined(__AVR_ATmega32U4__)
    Serial1.end();
#elif defined(__AVR_ATmega2560__)
    if (getPort() == PORT_15)
    {
      Serial3.end();
    }
    else if (getPort() == PORT_16)
    {
      Serial.end();
    }
    else
    {
      Serial2.end();
    }
#else
    Serial.end();
#endif
  }
  else
  {
    SoftwareSerial::end();
  }
}
//----------------------------------------------------------------------------
size_t PXFSerial::write(uint8_t byte)
{
  if (_hard)
  {
#if defined(__AVR_ATmega32U4__)
    return (_scratch ? Serial.write(byte) : Serial1.write(byte) );
#elif defined(__AVR_ATmega2560__)
    if (getPort() == PORT_15)
    {
      return (Serial3.write(byte) );
    }
    else if (getPort() == PORT_16)
    {
      return (Serial.write(byte) );
    }
    else
    {
      return (Serial2.write(byte) );
    }
#else
    return (Serial.write(byte) );
#endif
  }
  else
  {
    return (SoftwareSerial::write(byte) );
  }
}
//----------------------------------------------------------------------------
int16_t PXFSerial::read(void)
{
  if (_polling)
  {
    int16_t temp = _byte;
    _byte = -1;
    return (temp > -1 ? temp : poll() );
  }
  if (_hard)
  {
#if defined(__AVR_ATmega32U4__)
    return (_scratch ? Serial.read() : Serial1.read() );
#elif defined(__AVR_ATmega2560__)
    if (getPort() == PORT_15)
    {
      return (Serial3.read() );
    }
    else if(getPort() == PORT_16)
    {
      return (Serial.read() );
    }
    else
    {
      return (Serial2.read() );
    }
#else
    return (Serial.read() );
#endif
  }
  else
  {
    return (SoftwareSerial::read() );
  }
}
//----------------------------------------------------------------------------
int16_t PXFSerial::available(void)
{
  if (_polling)
  {
    _byte = poll();
    return (_byte > -1 ? 1 : 0);
  }
  if (_hard)
  {
#if defined(__AVR_ATmega32U4__)
    return (_scratch ? Serial.available() : Serial1.available() );
#elif defined(__AVR_ATmega2560__)
    if (getPort() == PORT_15)
    {
      return (Serial3.available() );
    }
    else if(getPort() == PORT_16)
    {
      return (Serial.available() );
    }
    else
    {
      return (Serial2.available() );
    }
#else
    return (Serial.available() );
#endif
  }
  else
  {
    return (SoftwareSerial::available() );
  }
}
//----------------------------------------------------------------------------
bool PXFSerial::listen(void)
{
  if (_hard)
  {
    return (true);
  }
  else
  {
    return (SoftwareSerial::listen() );
  }
}
//----------------------------------------------------------------------------
bool PXFSerial::isListening(void)
{
  if (_hard)
  {
    return (true);
  }
  else
  {
    return (SoftwareSerial::isListening() );
  }
}
//----------------------------------------------------------------------------
int16_t PXFSerial::poll(void)
{
  int16_t val = 0;
  int16_t bitDelay = _bitPeriod - clockCyclesToMicroseconds(50);
  if (digitalRead(_RxPin) == LOW)
  {
    for (int16_t offset = 0; offset < 8; offset++)
    {
      delayMicroseconds(bitDelay);
      val |= digitalRead(_RxPin) << offset;
    }
    delayMicroseconds(bitDelay);
    return (val & 0xff);
  }
  return (-1);
}
//----------------------------------------------------------------------------
void PXFSerial::sendString(char *str)
{
  while(*str)
  {
    write(*str++);
  }
}
//----------------------------------------------------------------------------
void PXFSerial::printf(char *fmt,...)
{
  va_list ap;
  char string[128];
  va_start(ap,fmt);
  vsprintf(string,fmt,ap);
  sendString(string);
  va_end(ap);
}
//----------------------------------------------------------------------------
boolean PXFSerial::dataLineAvailable(void)
{
  if(available())
  {
    char c = read();
    if(c=='\n')
    {
      buffer[bufferIndex] = 0;
      return true;
    }
    else
    {
      buffer[bufferIndex]=c;
      bufferIndex++;
    }
  }
  return false;
}
//----------------------------------------------------------------------------
String PXFSerial::readDataLine(void)
{
  if(bufferIndex>0)
  {
    lastLine = buffer;
  }
  bufferIndex = 0;
  memset(buffer,0,64);
  return lastLine;
}
//----------------------------------------------------------------------------
float PXFSerial::getValue(String key)
{
  String s = readDataLine();
  if(stringLength(s)>2)
  {
    char * tmp;
    char * str;
    str = strtok_r((char*)s.c_str(), "=", &tmp);
    if(str!=NULL && strcmp(str,key.c_str())==0)
    {
      float v = atof(tmp);
      return v;
    }
  }
  return 0;
}
//----------------------------------------------------------------------------
boolean PXFSerial::equalString(String s1,String s2)
{
  return s1.equals(s2);
}
//----------------------------------------------------------------------------
String PXFSerial::concatenateWith(String s1,String s2)
{
  return s1+s2;
}
//----------------------------------------------------------------------------
char PXFSerial::letterOf(int i,String s)
{
  return s.charAt(i);
}
//----------------------------------------------------------------------------
int PXFSerial::stringLength(String s)
{
  return s.length();
}
//----------------------------------------------------------------------------

#endif
