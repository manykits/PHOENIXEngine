/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class PXFSerial
 * \brief   Driver for serial.
 * @file    PXFSerial.h
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2015/01/20
 * @brief   Header for for PXFSerial.cpp module
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2016, MakeBlock. Use is subject to license \n
 * conditions. The main licensing options available are GPL V2 or Commercial: \n
 *
 * \par Open Source Licensing GPL V2
 * This is the appropriate option if you want to share the source code of your \n
 * application with everyone you distribute it to, and you also want to give them \n
 * the right to share who uses it. If you wish to use this software under Open \n
 * Source Licensing, you must contribute all your source code to the open source \n
 * community in accordance with the GPL Version 2 when your application is \n
 * distributed. See http://www.gnu.org/copyleft/gpl.html
 *
 * \par Description
 * This file is a drive for serial, It support hardware and software serial
 *
 * \par Method List:
 *
 *    1. void PXFSerial::setHardware(bool mode)
 *    2. void PXFSerial::begin(long baudrate)
 *    3. void PXFSerial::end(void)
 *    4. size_t PXFSerial::write(uint8_t byte)
 *    5. int16_t PXFSerial::read(void)
 *    6. int16_t PXFSerial::available(void)
 *    7. bool PXFSerial::listen(void)
 *    8. bool PXFSerial::isListening(void)
 *    9. int16_t PXFSerial::poll(void)
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Mark Yan         2015/09/08     1.0.0            Rebuild the old lib.
 * Mark Yan         2016/01/20     1.0.1            support arduino pin-setting.
 * </pre>
 */
#ifndef MeSerial_H
#define MeSerial_H

#include "PXFArduinoConfig.h"

#if defined PXF_MP3

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

class PXFSerial : public SoftwareSerial
{
public:
  PXFSerial(void);
  PXFSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false);

  void setHardware(bool mode);

  void begin(long baudrate);

  size_t write(uint8_t byte);

  int read();

  int available();

  int16_t poll(void);

  void end(void);

  bool listen(void);

  bool isListening(void);

  void sendString(char *str);

  void printf(char *fmt,...);

  boolean dataLineAvailable(void);
  String readDataLine(void);
  String concatenateWith(String s1,String s2);
  char letterOf(int i,String s);
  int stringLength(String s);
  boolean equalString(String s1,String s2);
  float getValue(String key);

protected:
  bool _hard;
  bool _polling;
  bool _scratch;
  int16_t _bitPeriod;
  int16_t _byte;
  long _lastTime;
  char buffer[64];
  String lastLine;
  int bufferIndex;

private:
  volatile uint8_t _RxPin;
  volatile uint8_t _TxPin; 
};

#endif

#endif

