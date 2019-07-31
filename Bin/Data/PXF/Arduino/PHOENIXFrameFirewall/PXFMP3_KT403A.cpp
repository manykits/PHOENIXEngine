// PXFMP3_KT403A.cpp

#include "PXFMP3_KT403A.h"

#if defined PXF_MP3

volatile uint8_t MP3::MP3_RxPin = 0;
volatile uint8_t MP3::MP3_TxPin = 0;
//----------------------------------------------------------------------------
MP3::MP3()
{
}
//----------------------------------------------------------------------------
MP3::MP3(uint8_t rxPin, uint8_t txpin, bool inverse_logic) : PXFSerial(rxPin, txpin, inverse_logic)
{
  MP3_RxPin = rxPin;
  MP3_TxPin = txpin;
}
//----------------------------------------------------------------------------
void MP3::begin(long speed)
{
  PXFSerial::begin(speed);

  pinMode(MP3_RxPin, INPUT);
  pinMode(MP3_TxPin, OUTPUT);
}
//----------------------------------------------------------------------------
/**************************************************************** 
 * Function Name: SelectPlayerDevice
 * Description: Select the player device, U DISK or SD card.
 * Parameters: 0x01:U DISK;  0x02:SD card
 * Return: none
****************************************************************/ 
void MP3::SelectPlayerDevice(uint8_t device)
{
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    PXFSerial::write(0x09);
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(device);
    PXFSerial::write(0xEF);
    delay(200);
}
/*
void MP3::Specify_FolderMP3_Play(uint16_t index)
{
    // uint8_t hbyte, lbyte;
    // hbyte = index / 256;
    // lbyte = index % 256;
    // PXFSerial::write(0x7E);
    // PXFSerial::write(0xFF);
    // PXFSerial::write(0x06);
    // PXFSerial::write(0x03);
    // PXFSerial::write(uint8_t(0x00));
    // PXFSerial::write(uint8_t(hbyte));
    // PXFSerial::write(uint8_t(lbyte));
    // PXFSerial::write(0xEF);
    // delay(10);
  
    uint8_t hbyte, lbyte;
    hbyte = index / 256;
    lbyte = (index+256) % 256;
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    
    PXFSerial::write(0x12);
    
    PXFSerial::write(0x00);
    
    PXFSerial::write(0x00);
    PXFSerial::write(0x01);
    PXFSerial::write(0xEF);
    delay(10);
}
*/
// 指定为"01"的文件夹，曲目为"001sdafas.mp3"
void MP3::PlayFolderIndex(uint8_t folder, uint8_t index)
{
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    
    PXFSerial::write(0x0F);
    
    PXFSerial::write(uint8_t(0x00));
    
    PXFSerial::write(uint8_t(folder));
    PXFSerial::write(uint8_t(index));
    
    PXFSerial::write(0xEF);
    delay(10);
}
/**************************************************************** 
 * Function Name: PlayPause
 * Description: Pause the MP3 player.
 * Parameters: none
 * Return: none
****************************************************************/ 
void MP3::PlayPause(void)
{
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    PXFSerial::write(0x0E);
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(0xEF);
    delay(20);
}
/**************************************************************** 
 * Function Name: Play
 * Description: Resume the MP3 player.
 * Parameters: none
 * Return: none
****************************************************************/ 
void MP3::Play(void)
{
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    PXFSerial::write(0x0D);
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(0xEF);
    delay(20);
}
//7E FF 06 16 00 00 00 FE E5 EF
void MP3::PlayStop(void)
{
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    PXFSerial::write(0x16);
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(0xEF);
    delay(20);
}
/**************************************************************** 
 * Function Name: PlayNext
 * Description: Play the next song.
 * Parameters: none
 * Return: none
****************************************************************/ 
void MP3::PlayNext(void)
{
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    PXFSerial::write(0x01);
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(0xEF);
    delay(10);
}
/**************************************************************** 
 * Function Name: PlayPrevious
 * Description: Play the previous song.
 * Parameters: none
 * Return: none
****************************************************************/ 
void MP3::PlayPrevious(void)
{
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    PXFSerial::write(0x02);
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(0xEF);
    delay(10);
}

void MP3::PlayRandom()
{
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    PXFSerial::write(0x18);
    PXFSerial::write(0x00);
    PXFSerial::write(0x00);
    PXFSerial::write(0x00);
    PXFSerial::write(0xFE);
    delay(10);
}

/**************************************************************** 
 * Function Name: PlayLoop
 * Description: single song loop
 * Parameters: none
 * Return: none
****************************************************************/ 
void MP3::PlaySingleLoopOpen()//7E FF 06 19 00 00 00 FE E2 EF
{
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    PXFSerial::write(0x19);
    PXFSerial::write(0x00);
    PXFSerial::write(0x00);
    PXFSerial::write(0x00);
    PXFSerial::write(0xFE);
    PXFSerial::write(0xE2);
    PXFSerial::write(0xEF);
    delay(10);
}
/**************************************************************** 
 * Function Name: PlayLoop
 * Description: single song stop loop
 * Parameters: none
 * Return: none
****************************************************************/ 
void MP3::PlaySingleLoopClose()//7E FF 06 19 00 00 01 FE E1 EF
{
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    PXFSerial::write(0x19);
    PXFSerial::write(0x00);
    PXFSerial::write(0x00);
    PXFSerial::write(0x01);
    PXFSerial::write(0xFE);
    PXFSerial::write(0xE1);
    PXFSerial::write(0xEF);
    delay(10);
}
/**************************************************************** 
 * Function Name: PlayLoop
 * Description: Play all the songs and loop.
 * Parameters: none
 * Return: none
****************************************************************/ 
void MP3::PlayAllLoopOpen(void)//7E FF 06 11 00 00 01 EF
{
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    PXFSerial::write(0x11);
    PXFSerial::write(0x00);
    PXFSerial::write(0x00);
    PXFSerial::write(0x01);
    PXFSerial::write(0xEF);
    delay(10);
}
/**************************************************************** 
 * Function Name: PlayLoop
 * Description: Play loop for all the songs.
 * Parameters: none
 * Return: none
****************************************************************/ 
void MP3::PlayAllLoopClose(void)//7E FF 06 11 00 00 00 EF
{
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    PXFSerial::write(0x11);
    PXFSerial::write(0x00);
    PXFSerial::write(0x00);
    PXFSerial::write(0x00);
    PXFSerial::write(0xEF);
    delay(10);
}
void MP3::PlayFolderLoopOpen(uint8_t folder)
{
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    PXFSerial::write(0x17);
    PXFSerial::write(0x00);
    PXFSerial::write(0x00);
    
    PXFSerial::write((uint8_t)folder);

    PXFSerial::write(0xEF);
    delay(10);
}
/**************************************************************** 
 * Function Name: SetVolume
 * Description: Set the volume, the range is 0x00 to 0x1E.
 * Parameters: volume: the range is 0x00 to 0x1E.
 * Return: none
****************************************************************/ 
void MP3::SetVolume(uint8_t volume)
{
    if(volume>30)
        {volume=30;}
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    PXFSerial::write(0x06);
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(volume);
    PXFSerial::write(0xEF);
    delay(10);
}
/**************************************************************** 
 * Function Name: IncreaseVolume
 * Description: Increase the volume.
 * Parameters: none
 * Return: none
****************************************************************/ 
void MP3::IncreaseVolume(void)
{
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    PXFSerial::write(0x04);
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(0xEF);
    delay(10);
}
/**************************************************************** 
 * Function Name: DecreaseVolume
 * Description: Decrease the volume.
 * Parameters: none
 * Return: none
****************************************************************/ 
void MP3::DecreaseVolume(void)
{
    PXFSerial::write(0x7E);
    PXFSerial::write(0xFF);
    PXFSerial::write(0x06);
    PXFSerial::write(0x05);
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(uint8_t(0x00));
    PXFSerial::write(0xEF);
    delay(10);
}

#endif
