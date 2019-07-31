// PXFArduinoCMD.cpp
#include "PXFArduino.h"

//----------------------------------------------------------------------------
void PXFArduino::OnCMD(String &cmdStr)
{
  int i=0;
  for (; i<NumCMDParams; i++)
  {
    mCmdParams[i] = "";
  }

  String strs = cmdStr;
  mCMDIndexTemp = 0;
  mpCMDParam = strtok((char *)cmdStr.c_str(), " ");
  while (mpCMDParam)
  {
    mCmdParams[mCMDIndexTemp] = String(mpCMDParam);
    mCMDIndexTemp++;
    mpCMDParam = strtok(NULL, " ");

    if (mCMDIndexTemp > NumCMDParams)
      break;
  }

  if (mCMDIndexTemp > 0)
  {
    char cmdCH = atoi(mCmdParams[0].c_str());
    if (sOptTypeStr[OT_TOGET_NETID] == cmdCH)
    {
      _SendNetID();
    }
    else if (sOptTypeStr[OT_PM] == cmdCH)
    {
      int pin = _Str2Pin(mCmdParams[1]);
      int val = _Str2IO(mCmdParams[2]);
      pinMode(pin, val);
      PXFPin pxfPin = _Str2PXFPin(mCmdParams[1]);
     // _PinMode(pxfPin, INPUT == val ? PM_INPUT : PM_OUTPUT);
    }
    else if(sOptTypeStr[OT_DW]==cmdCH)
    {
      int pin = _Str2Pin(mCmdParams[1]);
      bool isHigh = _Str2Bool(mCmdParams[2]);
      digitalWrite(pin, isHigh?HIGH:LOW);
    }
    else if (sOptTypeStr[OT_AW]==cmdCH)
    {
      int pin = _Str2Pin(mCmdParams[1]);
      int val = _Str2Int(mCmdParams[2]);
      analogWrite(pin, val);
    }
    else if (sOptTypeStr[OT_SVR_I]==cmdCH)
    {
#if defined PXF_SERVER
      int index = _Str2Int(mCmdParams[1]);
      int pin = _Str2Pin(mCmdParams[2]);
      if (0 <= index && index < NumMaxServer)
        mServo[index].attach(pin);
#endif
    }
    else if (sOptTypeStr[OT_SVR_W]==cmdCH)
    {
#if defined PXF_SERVER
      int index = _Str2Int(mCmdParams[1]);
      int val = _Str2Int(mCmdParams[2]);
      if (0 <= index && index < NumMaxServer)
        mServo[index].write(val);
#endif
    }
    else if (sOptTypeStr[OT_DST_I]==cmdCH)
    {
      int pinTrigger = _Str2Pin(mCmdParams[1]);
      int pinEcho = _Str2Pin(mCmdParams[2]);
      _DistInit(pinTrigger, pinEcho);
    }
    else if (sOptTypeStr[OT_DST_T]==cmdCH)
    {
      _DistTest();
    }
    else if (sOptTypeStr[OT_MOTO_I]==cmdCH)
    {
      _MotoInit();
    }
    else if (sOptTypeStr[OT_MOTO_I_DRIVER4567] == cmdCH)
    {
      _MotoInit4567();
    }
    else if (sOptTypeStr[OT_MOTO_I_DRIVER298N] == cmdCH)
    {
      int pinL = _Str2Pin(mCmdParams[1]);
      int pinL1 = _Str2Pin(mCmdParams[2]);
      int pinLS = _Str2Pin(mCmdParams[3]);
      int pinR = _Str2Pin(mCmdParams[4]);
      int pinR1 = _Str2Pin(mCmdParams[5]);
      int pinRS = _Str2Pin(mCmdParams[6]);      
      _MotoInit298N(pinL, pinL1, pinLS, pinR, pinR1, pinRS);
    }   
    else if (sOptTypeStr[OT_MOTO_RUN]==cmdCH)
    {
      int motoIndex = _Str2Pin(mCmdParams[1]);
      int dir = _Str2DirType(mCmdParams[2]);
      int spd = _Str2Int(mCmdParams[3]);

      if (0 == dir)
        spd = 0;

      if (0 == motoIndex)
        _LeftRun(dir, spd);
      else if (1 == motoIndex)
        _RightRun(dir, spd);
    }
    else if (sOptTypeStr[OT_MOTO_RUNSIMPLE]==cmdCH)
    {
      int dir = _Str2SimpleDirType(mCmdParams[1]);
      int spd = _Str2Int(mCmdParams[2]);
      if (0 == dir)
      {
        spd = 0;
        _LeftRun(0, spd);
        _RightRun(0, spd);
      }
      else if (1 == dir)
      {
        _LeftRun(1, spd);
        _RightRun(1, spd);
      }
      else if (2 == dir)
      {
        _LeftRun(2, spd);
        _RightRun(2, spd);
      }
      else if (3 == dir)
      {
        _LeftRun(2, spd);
        _RightRun(1, spd);
      }
      else if (4 == dir)
      {
        _LeftRun(1, spd);
        _RightRun(2, spd);
      }
    }
    else if (sOptTypeStr[OT_MOTO_STOP]==cmdCH)
    {
      _LeftRun(0, 0);
      _RightRun(0, 0);
    }
    else if (sOptTypeStr[OT_MP3_INIT]==cmdCH)
    {
      int pinR = _Str2Pin(mCmdParams[1]);
      int pinT = _Str2Pin(mCmdParams[2]);
      //_MP3Init(pinR, pinT);
    }
    else if (sOptTypeStr[OT_MP3_STOP]==cmdCH)
    {
      //_MP3Stop();
    }
    else if (sOptTypeStr[OT_MP3_VOLUME]==cmdCH)
    {
      int volume = _Str2Int(mCmdParams[1]);
      //_MP3Volume(volume);
    }
    else if (sOptTypeStr[OT_MP3_INDEX]==cmdCH)
    {
      int index = _Str2Int(mCmdParams[1]);
      //_MP3Index(index);
    }
    else if (sOptTypeStr[OT_MP3_NEXT]==cmdCH)
    {
      //_MP3Next();
    }
    else if (sOptTypeStr[OT_IR_INIT]==cmdCH)
    {
      int pinR = _Str2Pin(mCmdParams[1]);
      //_IRInit(pinR);      
    }
    else if (sOptTypeStr[OT_IR_SEND]==cmdCH)
    {
      int val = _Str2Int(mCmdParams[1]);
      //_IRSend(val);
    }
    else if (sOptTypeStr[OT_MOTO_I_SPD]==cmdCH)
    {
      int pinLA = _Str2Pin(mCmdParams[1]);
      int pinLB = _Str2Pin(mCmdParams[2]);
      int pinRA = _Str2Pin(mCmdParams[3]);
      int pinRB = _Str2Pin(mCmdParams[4]);
      _MotoSpeedInit(pinLA, pinLB, pinRA, pinRB);
    }
    else if (sOptTypeStr[OT_MC_SEGMENT] == cmdCH)
    {
      int pin = _Str2Int(mCmdParams[1]);
      float val = (float)_Str2Int(mCmdParams[2]);
      mSeg.reset(pin);
      mSeg.display(val);
    }
    else if (sOptTypeStr[OT_MC_MOTO] == cmdCH)
    {
      int pin = _Str2Int(mCmdParams[1]);
      float val = (float)_Str2Int(mCmdParams[2]);
      mMotor.reset(pin);
      mMotor.run(val);
    }
    else if (sOptTypeStr[OT_MC_DISTTEST] == cmdCH)
    {
      int pin = _Str2Int(mCmdParams[1]);
      mUltraSensor.reset(pin);
      mDist = mUltraSensor.distanceCm();
      mIsNewDist = true;
    }
    else if (sOptTypeStr[OT_MB_MOTO] == cmdCH)
    {
      
    }
    else if (sOptTypeStr[OT_MB_SEND] == cmdCH)
    {
    }
    else if (sOptTypeStr[OT_MB_BUZZER] == cmdCH)
    {
    }
  }
}
//----------------------------------------------------------------------------
int PXFArduino::_Str2IO(String &str)
{
  char ch = atoi(str.c_str());
  if (0 == ch)
    return INPUT;
  
  return OUTPUT;
}
//----------------------------------------------------------------------------
int PXFArduino::_Str2Pin(String &str)
{
  int pinVal = atoi(str.c_str());
  if (0 <= pinVal && pinVal <= P_13)
    return pinVal;
  else if (PXFA0 <= pinVal)
    return (pinVal - PXFA0) + A0;

  return 0;
}
//----------------------------------------------------------------------------
PXFPin PXFArduino::_Str2PXFPin(String &str)
{
  int pinVal = atoi(str.c_str());
  if (0 <= pinVal && pinVal <= P_13)
    return P_0 + pinVal;
  else if (PXFA0 <= pinVal)
    return (pinVal - PXFA0) + P_A0;
}
//----------------------------------------------------------------------------
bool PXFArduino::_Str2Bool(String &str)
{
  if (!String("0").compareTo(str))
    return false;

  return true;
}
//----------------------------------------------------------------------------
int PXFArduino::_Str2Int(String &str)
{
  int iVal = atoi(str.c_str());
  return iVal; 
}
//----------------------------------------------------------------------------
int PXFArduino::_Str2DirType(String &str)
{
  return atoi(str.c_str());
}
//----------------------------------------------------------------------------
int PXFArduino::_Str2SimpleDirType(String &str)
{
  return atoi(str.c_str());
}
//----------------------------------------------------------------------------
