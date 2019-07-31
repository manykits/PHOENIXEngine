// PXFMP3_KT403A.h

#ifndef __MP3PLAYER_KT403A_H__
#define __MP3PLAYER_KT403A_H__

#include "PXFArduinoConfig.h"

#if defined PXF_MP3

#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include "PXFSerial.h"

struct checksum{
    unsigned char chk_sum_h;
    unsigned char chk_sum_l;
};

class MP3 : public PXFSerial
{
 public:
    MP3();
    MP3(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic);

    void begin(long rate= 9600);
    void SelectPlayerDevice(uint8_t device);
    //void Specify_FolderMP3_Play(uint16_t index);
    
    void PlayFolderIndex(uint8_t folder, uint8_t index);
    
    void Play();
    void PlayPause();
    void PlayStop();
    
    void PlayNext();
    void PlayPrevious();

    void PlayRandom();

    void PlaySingleLoopOpen();
    void PlaySingleLoopClose();
    void PlayAllLoopOpen();
    void PlayAllLoopClose();
    void PlayFolderLoopOpen(uint8_t folder);

    void SetVolume(uint8_t volume);
    void IncreaseVolume();
    void DecreaseVolume();
    
private:
  static volatile uint8_t MP3_RxPin;
  static volatile uint8_t MP3_TxPin;
};

#endif

#endif
