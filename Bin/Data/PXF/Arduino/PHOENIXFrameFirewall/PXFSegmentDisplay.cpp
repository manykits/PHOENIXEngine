// PXFSegmentDisplay.cpp

#include "PXFSegmentDisplay.h"

#if defined PXF_SEGMENT7
//----------------------------------------------------------------------------
const uint8_t TubeTab[] PROGMEM = 
{
  0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, //0-9
  0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71,                         //'A', 'B', 'C', 'D', 'E', 'F',
  0xbf, 0x86, 0xdb, 0xcf, 0xe6, 0xed, 0xfd, 0x87, 0xff, 0xef, //0.-9.
  0xf7, 0xfc, 0xb9, 0xde, 0xf9, 0xf1,                         //'A.', 'B.', 'C.', 'D.', 'E.', 'F.',
  0, 0x40                                                     //' ','-'
};
//----------------------------------------------------------------------------
SegmentDisplay::SegmentDisplay()
{
}
//----------------------------------------------------------------------------
SegmentDisplay::SegmentDisplay(uint8_t clkPin, uint8_t dataPin)
{
  _dataPin = dataPin;
  _clkPin = clkPin;
  pinMode(_clkPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);
  set();
  clearDisplay();
}
//----------------------------------------------------------------------------
void SegmentDisplay::setpin(uint8_t dataPin, uint8_t clkPin)
{
  _dataPin = dataPin;
  _clkPin = clkPin;
  pinMode(_clkPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);
}
//----------------------------------------------------------------------------
void SegmentDisplay::clearDisplay(void)
{
  uint8_t buf[4] = { ' ', ' ', ' ', ' ' };
  display(buf);
}
//----------------------------------------------------------------------------
void SegmentDisplay::init(void)
{
  clearDisplay();
}
//----------------------------------------------------------------------------
void SegmentDisplay::writeByte(uint8_t wr_data)
{
  uint8_t i;
  uint8_t cnt0;
  for (i = 0; i < 8; i++)  //sent 8bit data
  {
    digitalWrite(_clkPin, LOW);
    if (wr_data & 0x01)
    {
      digitalWrite(_dataPin, HIGH); //LSB first
    }
    else
    {
      digitalWrite(_dataPin, LOW);
    }
    wr_data >>= 1;
    digitalWrite(_clkPin, HIGH);
  }
  digitalWrite(_clkPin, LOW); //wait for ACK
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_clkPin, HIGH);
  pinMode(_dataPin, INPUT);
  while (digitalRead(_dataPin))
  {
    cnt0 += 1;
    if (cnt0 == 200)
    {
      pinMode(_dataPin, OUTPUT);
      digitalWrite(_dataPin, LOW);
      cnt0 = 0;
    }
    //pinMode(_dataPin,INPUT);
  }
  pinMode(_dataPin, OUTPUT);

}
//----------------------------------------------------------------------------
void SegmentDisplay::start(void)
{
  digitalWrite(_clkPin, HIGH); //send start signal to TM1637
  digitalWrite(_dataPin, HIGH);
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clkPin, LOW);
}
//----------------------------------------------------------------------------
void SegmentDisplay::stop(void)
{
  digitalWrite(_clkPin, LOW);
  digitalWrite(_dataPin, LOW);
  digitalWrite(_clkPin, HIGH);
  digitalWrite(_dataPin, HIGH);
}
//----------------------------------------------------------------------------
void SegmentDisplay::write(uint8_t SegData[])
{
  uint8_t i;
  start();    // Start signal sent to TM1637 from MCU.
  writeByte(ADDR_AUTO);
  stop();
  start();
  writeByte(Cmd_SetAddr);
  for (i = 0; i < 4; i++)
  {
    writeByte(SegData[i]);
  }
  stop();
  start();
  writeByte(Cmd_DispCtrl);
  stop();
}
//----------------------------------------------------------------------------
void SegmentDisplay::write(uint8_t BitAddr, uint8_t SegData)
{
  start();    // start signal sent to TM1637 from MCU
  writeByte(ADDR_FIXED);
  stop();
  start();
  writeByte(BitAddr | STARTADDR);
  writeByte(SegData);
  stop();
  start();
  writeByte(Cmd_DispCtrl);
  stop();
}
//----------------------------------------------------------------------------
void SegmentDisplay::display(uint16_t value)
{
  display((int16_t)value);
}
//----------------------------------------------------------------------------
void SegmentDisplay::display(int16_t value)
{
  display((double)value, 0);
}
//----------------------------------------------------------------------------
void SegmentDisplay::display(float value)
{
  uint8_t i=0;
  bool isStart = false;
  uint8_t index = 0;
  uint8_t disp[]={0,0,0,0};
  bool isNeg = false;
  if((float)value<0)
  {
    isNeg = true;
    value = -value;
    disp[0] = 0x21;
    index++;
  }
  for(i=0;i<7;i++)
  {
    int n = checkNum(value,3-i);
    if(n>=1||i==3)
    {
      isStart=true;
    }
    if(isStart)
	{
      if(i==3)
	  {
        disp[index]=n+0x10;
      }
	  else
	  {
        disp[index]=n;
      }
      index++;
    }
    if(index>3)
    {
      break;
    }
  }
  display(disp);
}
//----------------------------------------------------------------------------
void SegmentDisplay::display(long value)
{
  display((double)value, 0);
}
//----------------------------------------------------------------------------
int16_t SegmentDisplay::checkNum(float v,int16_t b)
{
  if(b>=0)
  {
    return floor((v-floor(v/pow(10,b+1))*(pow(10,b+1)))/pow(10,b));
  }
  else
  {
    b=-b;
    int i=0;
    for(i=0;i<b;i++)
    {
      v = v*10;
    }
    return ((int)(v)%10);
  }
}
//----------------------------------------------------------------------------
void SegmentDisplay::display(double value, uint8_t digits)
{
Posotion_1:
  uint8_t buf[4] = { ' ', ' ', ' ', ' ' };
  uint8_t tempBuf[4];
  uint8_t b = 0;
  uint8_t bit_num = 0;
  uint8_t int_num = 0;
  uint8_t isNeg = 0;
  double number = value;
  if (number >= 9999.5 || number <= -999.5)
  {
    buf[0] = ' ';
    buf[1] = ' ';
    buf[2] = ' ';
    buf[3] = 0x0e;
  }
  else
  {
    // Handle negative numbers
    if (number < 0.0)
    {
      number = -number;
      isNeg = 1;
    }
    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    for (uint8_t i = 0; i < digits; ++i)
    {
      rounding /= 10.0;
    }
    number += rounding;

    // Extract the integer part of the number and print it
    uint16_t int_part = (uint16_t)number;
    double remainder = number - (double)int_part;
    do
    {
      uint16_t m = int_part;
      int_part /= 10;
      int8_t c = m - 10 * int_part;
      tempBuf[int_num] = c;
      int_num++;
    }
    while (int_part);

    bit_num = isNeg + int_num + digits;

    if (bit_num > 4)
    {
      bit_num = 4;
      digits = 4 - (isNeg + int_num);
      goto Posotion_1;
    }
    b = 4 - bit_num;
    if (isNeg)
    {
      buf[b++] = 0x21; // '-' display minus sign
    }
    for (uint8_t i = int_num; i > 0; i--)
    {
      buf[b++] = tempBuf[i - 1];
    }
    // Print the decimal point, but only if there are digits beyond
    if (digits > 0)
    {
      buf[b - 1] += 0x10;  // display '.'
      // Extract digits from the remainder one at a time
      while (digits-- > 0)
      {
        remainder *= 10.0;
        int16_t toPrint = int16_t(remainder);
        buf[b++] = toPrint;
        remainder -= toPrint;
      }
    }
  }
  display(buf);
}
//----------------------------------------------------------------------------
void SegmentDisplay::display(uint8_t DispData[])
{
  uint8_t SegData[4];
  uint8_t i;
  for (i = 0; i < 4; i++)
  {
    SegData[i] = DispData[i];
  }
  coding(SegData);
  write(SegData);
}
//----------------------------------------------------------------------------
void SegmentDisplay::display(uint8_t BitAddr, uint8_t DispData)
{
  uint8_t SegData;

  if ((DispData >= 'A' && DispData <= 'F'))
  {
    DispData = DispData - 'A' + 10;
  }
  else if ((DispData >= 'a' && DispData <= 'f'))
  {
    DispData = DispData - 'a' + 10;
  }
  SegData = coding(DispData);
  write(BitAddr, SegData);
}
//----------------------------------------------------------------------------
void SegmentDisplay::display(uint8_t BitAddr, uint8_t DispData, uint8_t point_on)
{
  uint8_t SegData;

  if ((DispData >= 'A' && DispData <= 'F'))
  {
    DispData = DispData - 'A' + 10;
  }
  else if ((DispData >= 'a' && DispData <= 'f'))
  {
    DispData = DispData - 'a' + 10;
  }
  if(point_on == POINT_ON )
  {
    SegData = coding(DispData+0x10);
  }
  else
  {
    SegData = coding(DispData);
  }
  write(BitAddr, SegData);
}
//----------------------------------------------------------------------------
void SegmentDisplay::set(uint8_t brightness, uint8_t SetData, uint8_t SetAddr)
{
  Cmd_SetData = SetData;
  Cmd_SetAddr = SetAddr;
  Cmd_DispCtrl = SEGDIS_ON + brightness;//Set brightness, take effect next display cycle.
}
//----------------------------------------------------------------------------
void SegmentDisplay::setBrightness(uint8_t brightness)
{
  Cmd_DispCtrl = SEGDIS_ON + brightness;
}
//----------------------------------------------------------------------------
void SegmentDisplay::coding(uint8_t DispData[])
{
  for (uint8_t i = 0; i < 4; i++)
  {
    if (DispData[i] >= sizeof(TubeTab) / sizeof(*TubeTab))
    {
      DispData[i] = 32; // Change to ' '(space)
    }
    //DispData[i] = TubeTab[DispData[i]];
    DispData[i] = pgm_read_byte(&TubeTab[DispData[i]]);//+ PointData;
  }
}
//----------------------------------------------------------------------------
uint8_t SegmentDisplay::coding(uint8_t DispData)
{
  if (DispData >= sizeof(TubeTab) / sizeof(*TubeTab))
  {
    DispData = 32; // Change to ' '(space)
  }
  //DispData = TubeTab[DispData];//+ PointData;
  DispData = pgm_read_byte(&TubeTab[DispData]);//+ PointData;
  return DispData;
}
//----------------------------------------------------------------------------

#endif