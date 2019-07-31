// UltraMat

#include "UltraMat.h"

UltraMat::UltraMat()
{
  for (int i=0; i<NumMaxUltra; i++)
  {
    Objs[i].Index = i;
  }

  mpCMD = 0;
}

void UltraMat::Init(int i, int pinTrigger, int pinEcho)
{
  if (0<=i && i<NumMaxUltra)
  {
    Objs[i].Init(pinTrigger, pinEcho);
  }
}

void UltraMat::Test(int i)
{
   if (0<=i && i<NumMaxUltra)
  {
    Objs[i].Test();
  }
}

float UltraMat::GetDistance(int i)
{
   if (0<=i && i<NumMaxUltra)
  {
    return Objs[i].Dist;
  }
  return 0.0f;
}

void UltraMat::SetRange(int i, float near, float far)
{
  if (0<=i && i<NumMaxUltra)
  {
    Objs[i].SetRange(near, far);
  }
}

void UltraMat::Shutdown(int index)
{
  if (0<=index && index<NumMaxUltra)
  {
    Objs[index].Shutdown();
  }
}

void UltraMat::OnCMD(String &cmdStr)
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
    if (OT_DSTMAT_I ==cmdCH)
    {
      int index = _Str2Pin(mCmdParams[1]);
      int pinTrigger = _Str2Pin(mCmdParams[2]);
      int pinEcho = _Str2Pin(mCmdParams[3]);
      Init(index, pinTrigger, pinEcho);
    }
  }
}

int UltraMat::_Str2Pin(String &str)
{
  int pinVal = atoi(str.c_str());
  if (0 <= pinVal && pinVal <= P_13)
    return pinVal;
  else if (PXFA0 <= pinVal)
    return (pinVal - PXFA0) + A0;

  return 0;
}

void UltraMat::Tick()
{
  for (int i=0; i<NumMaxUltra; i++)
  {
    if (Objs[i].IsInited())
    {
      Objs[i].Tick();
    }
  }
}
