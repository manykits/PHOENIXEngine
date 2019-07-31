// PXFArduinoMP3.cpp

#include "PXFArduino.h"

//----------------------------------------------------------------------------
void PXFArduino::_MP3Init_(int pinR, int pinT)
{
#if defined PXF_DFMP3
  if (mMP3DFSerial)
  {
    delete mMP3DFSerial;
    mMP3DFSerial = 0;
  }
  mMP3DFSerial = new SoftwareSerial(pinR, pinT);
  mMP3DFSerial->begin(9600);
  mp3_set_serial (*mMP3DFSerial); //set Serial for DFPlayer-mini mp3 module 
#endif

#if defined PXF_MP3
  mMP3 = MP3(pinR, pinT, false);
  mMP3.begin(9600);
  mMP3.SelectPlayerDevice(0x02);
  mMP3.SetVolume(15);
#endif
}
//----------------------------------------------------------------------------
void PXFArduino::_MP3Do(Mp3PlayType type)
{
  if (type == MPT_PLAY)
  {
#if defined PXF_MP3
    mMP3.Play();
#endif
  }
  else if (type == MPT_PAUSE)
  {
#if defined PXF_MP3
    mMP3.PlayPause();
#endif
  }
  else if (type == MPT_STOP)
  {
#if defined PXF_MP3
    mMP3.PlayStop();
#endif
  }
  else if (type == MPT_NEXT)
  {
#if defined PXF_MP3
    mMP3.PlayNext();
#endif
  }
  else if (type == MPT_BEFORE)
  {
#if defined PXF_MP3
    mMP3.PlayPrevious();
#endif
  }
  else if (type == MPT_RANDOM)
  {
#if defined PXF_MP3
    mMP3.PlayRandom();
#endif
  }
  else if (type == MPT_LOOP_SINGLE)
  {
#if defined PXF_MP3
    mMP3.PlaySingleLoopOpen();
#endif
  }
  else if (type == MPT_LOOP_SINGLE_CLOSE)
  {
#if defined PXF_MP3
    mMP3.PlaySingleLoopClose();
#endif
  }
  else if (type == MPT_LOOP_ALL)
  {
#if defined PXF_MP3
    mMP3.PlayAllLoopOpen();
#endif
  }
  else if (type == MPT_LOOP_ALL_CLOSE)
  {
#if defined PXF_MP3
    mMP3.PlayAllLoopClose();
#endif
  }
  else if (type == MPT_VOLUME_INCREASE)
  {
#if defined PXF_MP3
    mMP3.IncreaseVolume();
#endif
  }
  else if (type == MPT_VOLUME_DECREASE)
  {
#if defined PXF_MP3
    mMP3.DecreaseVolume();
#endif 
  }
}
//----------------------------------------------------------------------------
  void PXFArduino::_MP3FolderPlay(int folder, int index)
  {
#if defined PXF_MP3
    mMP3.PlayFolderIndex(folder, index);
#endif
  }
  //----------------------------------------------------------------------------
  void PXFArduino::_MP3SetVolime(int val)
  {
#if defined PXF_MP3
    mMP3.SetVolume(val);
#endif
  }
//----------------------------------------------------------------------------
