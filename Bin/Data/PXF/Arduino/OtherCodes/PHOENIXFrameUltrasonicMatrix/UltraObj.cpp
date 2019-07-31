// UtraObj.cpp

#include "UltraObj.h"
#include "UltraMat.h"
#include <Arduino.h>

void _SendCMD(String &cmdStr)
{
  String lastCmdStr = "0000" + cmdStr;
  //int allLength = 2 + cmdStr.length();
  //*(unsigned short *)(&lastCmdStr[0]) = (unsigned short)allLength; // length
  //*(unsigned short *)(&lastCmdStr[2]) = (unsigned short)GeneralServerMsgID; // id
  Serial.println(lastCmdStr);
}

UltraObj::UltraObj()
{
  Index = 0;
  
  PinTrigger = 0;
  PinEcho = 0;
  DistCheckLastTime = 0;

  Dist = 0.0f;

  mNear = 6.0f;
  mFar = 600.0f;

  mIsInited = false;
}

UltraObj::~UltraObj()
{
  
}

void UltraObj::Init(int pinTrig, int pinEcho)
{
  PinTrigger = pinTrig;
  PinEcho = pinEcho;
  pinMode(PinTrigger, OUTPUT); // 定义超声波输出脚
  pinMode(PinEcho, INPUT);     // 定义超声波输入脚
  
  DistCheckLastTime = 0;

  Dist = 0.0f;
  
  mNear = 6.0f;
  mFar = 800.0f;

  mIsInited = true;
}

void UltraObj::SetRange(float near, float far)
{
  mNear = near;
  mFar = far;
}

void UltraObj::Shutdown()
{
  mIsInited = false;
}

bool UltraObj::IsInited() const
{
  return mIsInited;
}

void UltraObj::Tick()
{
  long duration;
  if(millis() - DistCheckLastTime > 23)
  {
     DistCheckLastTime = millis();
     Test(); 
  }
}

void UltraObj::Test()
{
  if (!mIsInited)
    return;
  digitalWrite(PinTrigger, LOW); //低高低电平发一个短时间脉冲去TrigPin
  delayMicroseconds(2);
  digitalWrite(PinTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(PinTrigger, LOW);

  Dist = pulseIn(PinEcho, HIGH) / 58.0; //将回波时间换算成cm
  Dist = (int(Dist * 100.0)) / 100.0;       //保留两位小数
  if (Dist < mNear) Dist = mNear;
  if (Dist > mFar) Dist = mFar;

/*
  char str[32];
  memset(str, 0, 32);
  itoa(Dist, str, 10); // 10 is decimal

  char strIndex[32];
  memset(strIndex, 0, 32);
  itoa(Index, strIndex, 10); // 10 is decimal

  char cmdCh =(char) OT_RETURN_DSTMAT;
  char strCMDCh[32];
  memset(strCMDCh, 0, 32);
  itoa(cmdCh, strCMDCh, 10);

  String sendStr = String(strCMDCh) + " " + String(strIndex) + " " +  str;
  _SendCMD(sendStr);
  */
}

