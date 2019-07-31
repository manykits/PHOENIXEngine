// PXFLEDMatrix.cpp

#include "PXFLEDMatrix.h"
#include "PXFLEDMatrixData.h"

#if defined PXF_LEDMATRIX

//----------------------------------------------------------------------------
LEDMatrix::LEDMatrix()
{
}
//----------------------------------------------------------------------------
LEDMatrix::LEDMatrix(uint8_t SCK_Pin, uint8_t DIN_Pin)
{
	u8_SCKPin = SCK_Pin; 
	u8_DINPin = DIN_Pin;

	pinMode(u8_SCKPin, OUTPUT);
	pinMode(u8_DINPin, OUTPUT);
	digitalWrite(u8_SCKPin,HIGH);
	digitalWrite(u8_DINPin,HIGH);

    writeByte(Mode_Address_Auto_Add_1);
    setBrightness(Brightness_5);
    clearScreen();
}
//----------------------------------------------------------------------------
void LEDMatrix::writeByte(uint8_t data)
{
    //Start
    digitalWrite(u8_SCKPin, HIGH);
    digitalWrite(u8_DINPin, LOW);

    for(char i=0;i<8;i++)
    {
        digitalWrite(u8_SCKPin, LOW);
        digitalWrite(u8_DINPin, (data & 0x01));
        digitalWrite(u8_SCKPin, HIGH);
        data = data >> 1;
    }

    //End
    digitalWrite(u8_SCKPin, LOW);
    digitalWrite(u8_DINPin, LOW);
    digitalWrite(u8_SCKPin, HIGH);
    digitalWrite(u8_DINPin, HIGH);
    // delayMicroseconds(1);
}
//----------------------------------------------------------------------------
void LEDMatrix::writeBytesToAddress(uint8_t Address, const uint8_t *P_data, 
uint8_t count_of_data)
{
    uint8_t T_data;

    if(Address > 15 || count_of_data==0)
        return;

    Address = ADDRESS(Address);

    //Start
    digitalWrite(u8_SCKPin, HIGH);
    digitalWrite(u8_DINPin, LOW);

    //write Address
    for(char i=0;i<8;i++)
    {
        digitalWrite(u8_SCKPin, LOW);
        digitalWrite(u8_DINPin, (Address & 0x01));
        digitalWrite(u8_SCKPin, HIGH);
        Address = Address >> 1;
    }

    //write data
    for(uint8_t k=0; k<count_of_data; k++)
    {
        T_data = *(P_data + k);

        for(char i=0;i<8;i++)
        {
            digitalWrite(u8_SCKPin, LOW);
            digitalWrite(u8_DINPin, (T_data & 0x80));
            digitalWrite(u8_SCKPin, HIGH);
            T_data = T_data << 1;
        }
    }

    //End
    digitalWrite(u8_SCKPin, LOW);
    digitalWrite(u8_DINPin, LOW);
    digitalWrite(u8_SCKPin, HIGH);
    digitalWrite(u8_DINPin, HIGH);
    // delayMicroseconds(1);
}
//----------------------------------------------------------------------------
void LEDMatrix::clearScreen()
{
    for(uint8_t i=0;i<LED_BUFFER_SIZE;i++)
    {
        u8_Display_Buffer[i] = 0x00;
    }

    i_Color_Index = 1;
    b_Draw_Str_Flag = 0;

    writeBytesToAddress(0,u8_Display_Buffer,LED_BUFFER_SIZE);
}
//----------------------------------------------------------------------------
void LEDMatrix::setBrightness(uint8_t Bright)
{
    if((uint8_t)Bright>8)
    {
        Bright = Brightness_8;
    }

    if((uint8_t)Bright != 0)
    {
        Bright = (LED_Matrix_Brightness_TypeDef)((uint8_t)(Bright-1)|0x08);
        
    }
    writeByte(0x80 | (uint8_t)Bright);

}
//----------------------------------------------------------------------------
void LEDMatrix::setColorIndex(int Color_Number)
{
    i_Color_Index = Color_Number;
}
//----------------------------------------------------------------------------
void LEDMatrix::drawBitmap(int8_t x, int8_t y, uint8_t Bitmap_Width,
 uint8_t *Bitmap)
{
    if(x>15 || y>7 || Bitmap_Width==0)
        return;

    if(i_Color_Index == 1)
    {
        for(uint8_t k=0;k<Bitmap_Width;k++)
        {
          if(x+k>=0){
            u8_Display_Buffer[x+k] =
             (u8_Display_Buffer[x+k] & (0xff << (8-y))) | (y>0?(Bitmap[k] >> y):(Bitmap[k] << (-y)));
          }
        }
    }
    else if(i_Color_Index == 0)
    {
        for(uint8_t k=0;k<Bitmap_Width;k++)
        {
            if(x+k>=0){
              u8_Display_Buffer[x+k] =
               (u8_Display_Buffer[x+k] & (0xff << (8-y))) | (y>0?(~Bitmap[k] >> y):(~Bitmap[k] << (-y)));
            }
        }
    }

    writeBytesToAddress(0,u8_Display_Buffer,LED_BUFFER_SIZE);
}
//----------------------------------------------------------------------------
void LEDMatrix::drawStr(int16_t X_position, int8_t Y_position, const char *str)
{
    b_Draw_Str_Flag = 1;

    for(i16_Number_of_Character_of_Str = 0; str[i16_Number_of_Character_of_Str] != '\0'; i16_Number_of_Character_of_Str++)
    {
        if(i16_Number_of_Character_of_Str < STRING_DISPLAY_BUFFER_SIZE - 1)
        {
            i8_Str_Display_Buffer[i16_Number_of_Character_of_Str] = str[i16_Number_of_Character_of_Str];
        }
        else
        {
            break;
        }
    }
    i8_Str_Display_Buffer[i16_Number_of_Character_of_Str] = '\0';


    if(X_position < -(i16_Number_of_Character_of_Str * 6))
    {
        X_position = -(i16_Number_of_Character_of_Str * 6);
    }
    else if(X_position > 16)
    {
        X_position = 16;
    }
    i16_Str_Display_X_Position = X_position;


    if(Y_position < -1)
    {
        Y_position = -1;
    }
    else if(Y_position >15)
    {
        Y_position = 15;
    }
    i8_Str_Display_Y_Position = Y_position;

    showStr();

}
//----------------------------------------------------------------------------
void LEDMatrix::showStr()
{
    uint8_t display_buffer_label = 0;

    if(i16_Str_Display_X_Position > 0)
    {
        for(display_buffer_label = 0; display_buffer_label < i16_Str_Display_X_Position && display_buffer_label < LED_BUFFER_SIZE; display_buffer_label++)
        {
            u8_Display_Buffer[display_buffer_label] = 0x00;
        }

        if(display_buffer_label < LED_BUFFER_SIZE)
        {
            uint8_t num;

            for(uint8_t k=0;display_buffer_label < LED_BUFFER_SIZE && k < i16_Number_of_Character_of_Str;k++)
            {
                for(num=0; pgm_read_byte(&Character_font_6x8[num].Character[0]) != '@'; num++)
                {
                    if(i8_Str_Display_Buffer[k] == pgm_read_byte(&Character_font_6x8[num].Character[0]))
                    {
                        for(uint8_t j=0;j<6;j++)
                        {
                            u8_Display_Buffer[display_buffer_label] = pgm_read_byte(&Character_font_6x8[num].data[j]);
                            display_buffer_label++;

                            if(display_buffer_label >= LED_BUFFER_SIZE)
                            {
                                break;
                            }
                        }
                        break;
                    }
                }

                if(pgm_read_byte(&Character_font_6x8[num].Character[0]) == '@')
                {
                    i8_Str_Display_Buffer[k] = ' ';
                    k--;
                }
            }

            if(display_buffer_label < LED_BUFFER_SIZE)
            {
                for(display_buffer_label = display_buffer_label; display_buffer_label < LED_BUFFER_SIZE; display_buffer_label++)
                {
                    u8_Display_Buffer[display_buffer_label] = 0x00;
                }
            }
        }
    }

    else if(i16_Str_Display_X_Position <= 0)
    {
        if(i16_Str_Display_X_Position == -(i16_Number_of_Character_of_Str * 6))
        {
            for(; display_buffer_label < LED_BUFFER_SIZE; display_buffer_label++)
            {
                u8_Display_Buffer[display_buffer_label] = 0x00;
            }
        }
        else
        {
            int8_t j = (-i16_Str_Display_X_Position) % 6;
            uint8_t num;

            i16_Str_Display_X_Position = -i16_Str_Display_X_Position;

            for(int16_t k=i16_Str_Display_X_Position/6; display_buffer_label < LED_BUFFER_SIZE && k < i16_Number_of_Character_of_Str;k++)
            {
                for(num=0; pgm_read_byte(&Character_font_6x8[num].Character[0]) != '@'; num++)
                {
                    if(i8_Str_Display_Buffer[k] == pgm_read_byte(&Character_font_6x8[num].Character[0]))
                    {
                        for(;j<6;j++)
                        {
                            u8_Display_Buffer[display_buffer_label] = pgm_read_byte(&Character_font_6x8[num].data[j]);
                            display_buffer_label++;

                            if(display_buffer_label >= LED_BUFFER_SIZE)
                            {
                                break;
                            }
                        }
                        j=0;
                        break;
                    }
                }

                if(pgm_read_byte(&Character_font_6x8[num].Character[0]) == '@')
                {
                    i8_Str_Display_Buffer[k] = ' ';
                    k--;
                }

            }

            if(display_buffer_label < LED_BUFFER_SIZE)
            {
                for(; display_buffer_label < LED_BUFFER_SIZE; display_buffer_label++)
                {
                    u8_Display_Buffer[display_buffer_label] = 0x00;
                }
            }

            i16_Str_Display_X_Position = -i16_Str_Display_X_Position;
        }
    }


    if(7 - i8_Str_Display_Y_Position >= 0)
    {
        for(uint8_t k=0; k<LED_BUFFER_SIZE; k++)
        {
            u8_Display_Buffer[k] = u8_Display_Buffer[k] << (7 - i8_Str_Display_Y_Position);
        }
    }
    else
    {
        for(uint8_t k=0; k<LED_BUFFER_SIZE; k++)
        {
            u8_Display_Buffer[k] = u8_Display_Buffer[k] >> (i8_Str_Display_Y_Position - 7);
        }
    }

    
    if(i_Color_Index == 0)
    {
        for(uint8_t k=0; k<LED_BUFFER_SIZE; k++)
        {
            u8_Display_Buffer[k] = ~u8_Display_Buffer[k];
        }
    }

    writeBytesToAddress(0,u8_Display_Buffer,LED_BUFFER_SIZE);

}
//----------------------------------------------------------------------------
void LEDMatrix::showClock(uint8_t hour, uint8_t minute, bool point_flag)
{
    u8_Display_Buffer[0]  = pgm_read_byte(&Clock_Number_font_3x8[hour/10].data[0]);
    u8_Display_Buffer[1]  = pgm_read_byte(&Clock_Number_font_3x8[hour/10].data[1]);
    u8_Display_Buffer[2]  = pgm_read_byte(&Clock_Number_font_3x8[hour/10].data[2]);
 
    u8_Display_Buffer[3]  = 0x00;
 
    u8_Display_Buffer[4]  = pgm_read_byte(&Clock_Number_font_3x8[hour%10].data[0]);
    u8_Display_Buffer[5]  = pgm_read_byte(&Clock_Number_font_3x8[hour%10].data[1]);
    u8_Display_Buffer[6]  = pgm_read_byte(&Clock_Number_font_3x8[hour%10].data[2]);
 
    u8_Display_Buffer[9]  = pgm_read_byte(&Clock_Number_font_3x8[minute/10].data[0]);
    u8_Display_Buffer[10] = pgm_read_byte(&Clock_Number_font_3x8[minute/10].data[1]);
    u8_Display_Buffer[11] = pgm_read_byte(&Clock_Number_font_3x8[minute/10].data[2]);

    u8_Display_Buffer[12] = 0x00;

    u8_Display_Buffer[13] = pgm_read_byte(&Clock_Number_font_3x8[minute%10].data[0]);
    u8_Display_Buffer[14] = pgm_read_byte(&Clock_Number_font_3x8[minute%10].data[1]);
    u8_Display_Buffer[15] = pgm_read_byte(&Clock_Number_font_3x8[minute%10].data[2]);


    if(point_flag == PointOn)
    {
        u8_Display_Buffer[7] = 0x28;
        u8_Display_Buffer[8] = 0x28;
    }
    else
    {
        u8_Display_Buffer[7] = 0x00;
        u8_Display_Buffer[8] = 0x00;
    }

    if(i_Color_Index == 0)
    {
        for(uint8_t k=0; k<LED_BUFFER_SIZE; k++)
        {
            u8_Display_Buffer[k] = ~u8_Display_Buffer[k];
        }
    }

    writeBytesToAddress(0,u8_Display_Buffer,LED_BUFFER_SIZE);
}
//----------------------------------------------------------------------------
void LEDMatrix::showNum(float value,uint8_t digits)
{
Posotion_1:
  uint8_t buf[4] = { 0x0c, 0x0c, 0x0c, 0x0c};
  uint8_t tempBuf[4];
  uint8_t b = 0;
  uint8_t bit_num = 0;
  uint8_t int_num = 0;
  uint8_t isNeg = 0;
  double number = value;
  if (number >= 9999.5)
  {
    buf[0] = 9;
    buf[1] = 9;
    buf[2] = 9;
    buf[3] = 9;
  }
  else if(number <= -999.5)
  {
    buf[0] = 0x0b;
    buf[1] = 9;
    buf[2] = 9;
    buf[3] = 9;  
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
      buf[b++] = 0x0b; // '-' display minus sign
    }
    for (uint8_t i = int_num; i > 0; i--)
    {
      buf[b++] = tempBuf[i - 1];
    }
    // Print the decimal point, but only if there are digits beyond
    if (digits > 0)
    {
      if((b == 3) && (int16_t(remainder*10) == 0))
      {
        buf[3] = 0x0c;
	  }
      else if((b == 2) && (int16_t(remainder*100) == 0))
      {
        buf[2] = 0x0c;
        buf[3] = 0x0c;
	  }
	  else if((b == 1) && (int16_t(remainder*1000) == 0))
      {
        buf[1] = 0x0c;
        buf[2] = 0x0c;
        buf[3] = 0x0c;
	  }
	  else
	  {
        buf[b++] = 0x0a;  // display '.'
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
  }

  u8_Display_Buffer[0]  = pgm_read_byte(&Clock_Number_font_3x8[buf[0]].data[0]);
  u8_Display_Buffer[1]  = pgm_read_byte(&Clock_Number_font_3x8[buf[0]].data[1]);
  u8_Display_Buffer[2]  = pgm_read_byte(&Clock_Number_font_3x8[buf[0]].data[2]);
  
  u8_Display_Buffer[3]  = 0x00;
  
  u8_Display_Buffer[4]  = pgm_read_byte(&Clock_Number_font_3x8[buf[1]].data[0]);
  u8_Display_Buffer[5]  = pgm_read_byte(&Clock_Number_font_3x8[buf[1]].data[1]);
  u8_Display_Buffer[6]  = pgm_read_byte(&Clock_Number_font_3x8[buf[1]].data[2]);
  
  u8_Display_Buffer[7]  = 0x00;
  
  u8_Display_Buffer[8]  = pgm_read_byte(&Clock_Number_font_3x8[buf[2]].data[0]);
  u8_Display_Buffer[9]  = pgm_read_byte(&Clock_Number_font_3x8[buf[2]].data[1]);
  u8_Display_Buffer[10]  = pgm_read_byte(&Clock_Number_font_3x8[buf[2]].data[2]);
  
  u8_Display_Buffer[11]  = 0x00;
  
  u8_Display_Buffer[12]  = pgm_read_byte(&Clock_Number_font_3x8[buf[3]].data[0]);
  u8_Display_Buffer[13]  = pgm_read_byte(&Clock_Number_font_3x8[buf[3]].data[1]);
  u8_Display_Buffer[14]  = pgm_read_byte(&Clock_Number_font_3x8[buf[3]].data[2]);

  u8_Display_Buffer[15]  = 0x00;

  if(i_Color_Index == 0)
  {
    for(uint8_t k=0; k<LED_BUFFER_SIZE; k++)
    {
      u8_Display_Buffer[k] = ~u8_Display_Buffer[k];
    }
  }

  writeBytesToAddress(0,u8_Display_Buffer,LED_BUFFER_SIZE);
}
//----------------------------------------------------------------------------

#endif