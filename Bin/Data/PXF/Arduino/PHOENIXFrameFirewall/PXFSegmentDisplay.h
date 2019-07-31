// PXFSegmentDisplay.h

#ifndef PXFSEGMENTDISPLAY_H
#define PXFSEGMENTDISPLAY_H

#include "PXFArduinoConfig.h"

#if defined PXF_SEGMENT7

//************definitions for TM1637*********************
#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>

/* Exported constants --------------------------------------------------------*/
/******************definitions for TM1637**********************/
const uint8_t   ADDR_AUTO = 0x40;   //Automatic address increment mode
const uint8_t   ADDR_FIXED = 0x44;   //Fixed address mode
const uint8_t   STARTADDR = 0xc0;   //start address of display register
const uint8_t   SEGDIS_ON = 0x88;   //diplay on
const uint8_t   SEGDIS_OFF = 0x80;   //diplay off
/**** definitions for the clock point of the digit tube *******/
const uint8_t POINT_ON = 1;
const uint8_t POINT_OFF = 0;
/**************definitions for brightness***********************/
const uint8_t BRIGHTNESS_0 = 0;
const uint8_t BRIGHTNESS_1 = 1;
const uint8_t BRIGHTNESS_2 = 2;
const uint8_t BRIGHTNESS_3 = 3;
const uint8_t BRIGHTNESS_4 = 4;
const uint8_t BRIGHTNESS_5 = 5;
const uint8_t BRIGHTNESS_6 = 6;
const uint8_t BRIGHTNESS_7 = 7;
///@brief Class for numeric display module

class SegmentDisplay
{
public:
  SegmentDisplay();
  SegmentDisplay(uint8_t clkPin, uint8_t dataPin);

  void init(void); // Clear display
  void set(uint8_t = BRIGHTNESS_2, uint8_t = ADDR_AUTO, uint8_t = STARTADDR);// Take effect next display cycle.
  void setpin(uint8_t dataPin, uint8_t clkPin);
  void write(uint8_t SegData[]);
  void write(uint8_t BitAddr, uint8_t SegData);
  void display(uint16_t value);
  void display(int16_t value);
  void display(float value);
  void display(long value);
  int16_t checkNum(float v,int16_t b);
  void display(double value, uint8_t = 1);
  void display(uint8_t DispData[]);
  void display(uint8_t BitAddr, uint8_t DispData);
  void display(uint8_t BitAddr, uint8_t DispData, uint8_t point_on);
  void clearDisplay(void);
  void setBrightness(uint8_t brightness);
  void coding(uint8_t DispData[]);
  uint8_t coding(uint8_t DispData);

private:
  uint8_t Cmd_SetData;
  uint8_t Cmd_SetAddr;
  uint8_t Cmd_DispCtrl;
  bool _PointFlag; //_PointFlag=1:the clock point on

  void writeByte(uint8_t wr_data);// Write 8 bits data to tm1637.
  void start(void);// Send start bits
  void point(bool PointFlag);// Whether to light the clock point ":". Take effect next display cycle.
  void stop(void); // Send stop bits.
  
  uint8_t _clkPin;
  uint8_t _dataPin;
};

#endif

#endif