// PXFArduino.cpp

#include "PXFArduino.h"

//----------------------------------------------------------------------------
PXFArduino *PXFArduino::pxfarduino = NULL;
//----------------------------------------------------------------------------
char PXFArduino::PinStr[P_MAX_TYPE] =
{
  0,
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  9,
  10,
  11,
  12,
  13,
  30,
  31,
  32,
  33,
  34,
  35,
  36
};
//----------------------------------------------------------------------------
char PXFArduino::sOptTypeStr[OT_MAX_TYPE] =
{
    50, //OT_TOGET_NETID
    51, //OT_RETRUN_NETID
    0,  //OT_PM
    1,  //OT_DW
    2,  //OT_AW
    3,  //OT_RETURN_DR
    4,  //OT_RETURN_AR
    5,  //OT_SVR_I
    6,  //OT_SVR_W
    7,  //OT_DST_I
    8,  //OT_DST_T
    9,  //OT_RETURN_DIST
    10, //OT_MOTO_I
    11, //OT_MOTO_RUN
    12, //OT_MOTO_RUNSIMPLE
    13, //OT_MOTO_STOP
    14, //OT_MOTO_I_SPD
    15, //OT_RETURN_MOTOSPD
    16, //OT_MOTO_I_DRIVER4567
    17, //OT_MOTO_I_DRIVER298N
    18, //OT_MP3_INIT
    19, //OT_MP3_PLAY
    20, //OT_MP3_INDEX
    21, //OT_MP3_NEXT
    22, //OT_MP3_STOP
    23, //OT_MP3_VOLUME
    24, //OT_IR_INIT
    25, //OT_IR_SEND
    26, //OT_RETURN_IR
    27, //OT_HX711_I
    28, //OT_HX711_TEST
    29,  //OT_RETURN_HX711
    500, //OT_MC_INTERNAL_LIGHT
    501, //OT_MC_LIGHT
    502, //OT_MC_SEGMENT
    503, //OT_MC_MOTO,
    504, //OT_MC_DISTTEST
    
};
//----------------------------------------------------------------------------
PXFArduino::PXFArduino()
{
  pxfarduino = this;

  mNetID = 12345678;
  Init(false);
}
//----------------------------------------------------------------------------
void PXFArduino::Init(bool isReset)
{
  digitalWrite(13, LOW);

  for (int i = P_0; i <= P_MAX_TYPE; i++)
  {
    PinModes[i] = 0;
  }

  mIndexCMD = 0;
  mpCMD = NULL;

  mCMDIndexTemp = 0;
  mpCMD = 0;

  mPinTickEvent = -1;
  mPinDistTrigger = 0;
  mPinDistEcho = 0;
  mDist = 0.0f;

  mPinL0 = 0;
  mPinL1 = 0;
  mPinLSpeed = 0;
  mPinR0 = 0;
  mPinR1 = 0;
  mPinRSpeed = 0;
  mIsNewSpeed = false;

  mIsUseSpeedEncorder = false;
  mEncorderTickEvent = -1;

  mPinEncroderLA = 2;
  mPinEncroderLB = 12;
  mEncoderPinLastLA = 0;
  mDurationL = 0;
  mDirectionL = 0;

  mPinEncroderRA = 3;
  mPinEncroderRB = 13;
  mEncoderPinLastRA = 0;
  mDurationR = 0;
  mDirectionR = 0;

  mIsNewSpeed = false;
  mCurSpeedLeft = 0;
  mCurSpeedRight = 0;

#if defined PXF_SSD1306
  mDisplay = 0;                                                     
#endif

#if defined PXF_MP3
  if (isReset && mMP3Serial)
  {
    delete mMP3Serial;
  }
  mMP3Serial = 0;
#endif

#if defined PXF_IR
  if (isReset && mIRrecv)
  {
    delete mIRrecv;
  }
  mIRrecv = 0;
#endif
}
//----------------------------------------------------------------------------
void PXFArduino::_PinMode(PXFPin pin, PXFPMode mode)
{
    PinModes[(int)pin] = mode;
}
//----------------------------------------------------------------------------
String PXFArduino::I2Str(int val)
{
  char str[25];
  itoa(val, str, 10); // 10 is decimal
  return str;
}
//----------------------------------------------------------------------------
void PXFArduino::CalPinVals()
{
  for (int i=P_0; i<=P_13; i++)
  {
    if (PM_OUTPUT == PinModes[i])
    {
      int val = digitalRead(i);
      String str = String("dr ") + String(i) + " " + I2Str(val);
      Serial.println(str);
    }
  }
  for (int i=P_A0; i<=P_A5; i++)
  {
    int pin = A0 + (i - P_A5);
    if (PM_OUTPUT == PinModes[i])
    {
      int val = analogRead(pin);
      String str = String("ar ") + String(i) + " " + I2Str(val);
      Serial.println(str);
    }
  }
}
//----------------------------------------------------------------------------
void PXFArduino::_SendNetID()
{
    char str[32];
    memset(str, 0, 32);
    itoa(mNetID, str, 10); // 10 is decimal

    char cmdCh = sOptTypeStr[OT_RETURN_NETID];
    char strCMDCh[32];
    memset(strCMDCh, 0, 32);
    itoa(cmdCh, strCMDCh, 10);

    String sendStr = String(strCMDCh) + " " + str;
    _SendCMD(sendStr);
}
//----------------------------------------------------------------------------
void PXFArduino::Tick()
{
  mTimer.update();

  if (mIsNewDist)
  {
    char str[32];
    memset(str, 0, 32);
    itoa(mDist, str, 10); // 10 is decimal

    char cmdCh = sOptTypeStr[OT_RETURN_DIST];
    char strCMDCh[32];
    memset(strCMDCh, 0, 32);
    itoa(cmdCh, strCMDCh, 10);

    String sendStr = String(strCMDCh) + " " + str;
    _SendCMD(sendStr);

    mIsNewDist = false;
  }
  
#if defined PXF_IR
  decode_results iresultes;
  if (mIRrecv && mIRrecv->decode(&iresultes))
  {
    int val = iresultes.value;
    _IRRecv(val);
    mIRrecv->resume();
  }
#endif
}
//----------------------------------------------------------------------------
void sCalPinValue()
{
  //PXFArduino::pxfarduino->CalPinVals();

  if (PXFArduino::pxfarduino->mIsNewSpeed)
  {
    char stringL[32];
    memset(stringL, 0, 32);
    itoa(PXFArduino::pxfarduino->mCurSpeedLeft, stringL, 10);
    char stringR[32];
    memset(stringR, 0, 32);
    itoa(PXFArduino::pxfarduino->mCurSpeedRight, stringR, 10);

    int cmdCh = (int)PXFArduino::sOptTypeStr[OT_RETURN_MOTOSPD];
    char strCMDCh[32];
    memset(strCMDCh, 0, 32);
    itoa(cmdCh, strCMDCh, 10);

    String sendStr = String(strCMDCh) + String(" ") + String(stringL) + String(" ") + String(stringR);
    PXFArduino::pxfarduino->_SendCMD(sendStr);

    PXFArduino::pxfarduino->mIsNewSpeed = false;
  }
}
//----------------------------------------------------------------------------
void PXFArduino::InitPinTickEvent()
{
  if (-1 != mPinTickEvent)
    mTimer.stop(mPinTickEvent);
  mPinTickEvent = -1;
  mPinTickEvent = mTimer.every(00, sCalPinValue);

  mIsNewDist = false;
}
//----------------------------------------------------------------------------
void PXFArduino::_DistInit(int pinTrig, int pinEcho)
{
  mPinDistTrigger = pinTrig;
  mPinDistEcho = pinEcho;
  pinMode(mPinDistTrigger, OUTPUT); // 定义超声波输出脚
  pinMode(mPinDistEcho, INPUT);     // 定义超声波输入脚

  mIsNewDist = false;
}
//----------------------------------------------------------------------------
void PXFArduino::_DistTest()
{
  digitalWrite(mPinDistTrigger, LOW); //低高低电平发一个短时间脉冲去TrigPin
  delayMicroseconds(2);
  digitalWrite(mPinDistTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(mPinDistTrigger, LOW);

  mDist = pulseIn(mPinDistEcho, HIGH) / 58.0; //将回波时间换算成cm
  mDist = (int(mDist * 100.0)) / 100.0;       //保留两位小数
  if (mDist < 6) mDist = 6;
  if (mDist > 600) mDist = 600;

  mIsNewDist = true;
}
//----------------------------------------------------------------------------
void PXFArduino::_MotoInit()
{
  mPinL0 = 12;
  mPinL1 = 12;
  mPinR0 = 13;
  mPinR1 = 13;
  mPinLSpeed = 10;
  mPinRSpeed = 11;

  pinMode(mPinL0, OUTPUT);
  pinMode(mPinR0, OUTPUT);
  pinMode(mPinLSpeed, OUTPUT);
  pinMode(mPinRSpeed, OUTPUT);

  mMotoMode = MM_BOARD;
  mIsUseSpeedEncorder = false;
}
//----------------------------------------------------------------------------
void PXFArduino::_MotoInit4567()
{
  mPinL0 = 4;
  mPinL1 = 4;
  mPinR0 = 7;
  mPinR1 = 7;
  mPinLSpeed = 5;
  mPinRSpeed = 6;

  pinMode(mPinL0, OUTPUT);
  pinMode(mPinR0, OUTPUT);
  pinMode(mPinLSpeed, OUTPUT);
  pinMode(mPinRSpeed, OUTPUT);

  mMotoMode = MM_BOARD;
  mIsUseSpeedEncorder = false;
}
//----------------------------------------------------------------------------
void PXFArduino::_MotoInit298N(int pinL, int pinL1, int pinLS, int pinR, 
  int pinR1, int pinRS)
{
  mPinL0 = pinL;
  mPinL1 = pinL1;
  mPinLSpeed = pinLS;
  
  mPinR0 = pinR;
  mPinR1 = pinR1;
  mPinRSpeed = pinRS;

  pinMode(mPinL0, OUTPUT);
  pinMode(mPinL1, OUTPUT);
  pinMode(mPinLSpeed, OUTPUT);
    
  pinMode(mPinR0, OUTPUT);
  pinMode(mPinR1, OUTPUT);
  pinMode(mPinRSpeed, OUTPUT);

  mMotoMode = MM_298N;
  mIsUseSpeedEncorder = false;
}
//----------------------------------------------------------------------------
void PXFArduino::_MotoSpeedInit(int encorderLA, int encorderLB,
  int encorderRA, int encorderRB)
{
  mPinEncroderLA = encorderLA;
  mPinEncroderLB = encorderLB;
  mPinEncroderRA = encorderRA;
  mPinEncroderRB = encorderRB;
  _EncoderInit();
}
//----------------------------------------------------------------------------
void sUpdateSpeed()
{
  PXFArduino::pxfarduino->mIsNewSpeed = true;
  PXFArduino::pxfarduino->mCurSpeedLeft = PXFArduino::pxfarduino->mDurationL;
  PXFArduino::pxfarduino->mCurSpeedRight = PXFArduino::pxfarduino->mDurationR;
  PXFArduino::pxfarduino->mDurationL = 0;
  PXFArduino::pxfarduino->mDurationR = 0;
}
void sWheelSpeedL()
{
  int lstate = digitalRead(PXFArduino::pxfarduino->mPinEncroderLA);
  if ((PXFArduino::pxfarduino->mEncoderPinLastLA == LOW) && lstate == HIGH)
  {
    int val = digitalRead(PXFArduino::pxfarduino->mPinEncroderLB);
    if (val == LOW)
    {
      PXFArduino::pxfarduino->mDirectionL = false; //Reverse
    }
    else if (val == HIGH)
    {
      PXFArduino::pxfarduino->mDirectionL = true; //Forward
    }
  }
  PXFArduino::pxfarduino->mEncoderPinLastLA = lstate;

  if (!PXFArduino::pxfarduino->mDirectionL)
    PXFArduino::pxfarduino->mDurationL++;
  else
    PXFArduino::pxfarduino->mDurationL--;
}
void sWheelSpeedR()
{
  int rstate = digitalRead(PXFArduino::pxfarduino->mPinEncroderRA);
  if ((PXFArduino::pxfarduino->mEncoderPinLastRA == LOW) && rstate == HIGH)
  {
    int val = digitalRead(PXFArduino::pxfarduino->mPinEncroderRB);
    if (val == LOW)
    {
      PXFArduino::pxfarduino->mDirectionR = false; //Reverse
    }
    else if (val == HIGH)
    {
      PXFArduino::pxfarduino->mDirectionR = true; //Forward
    }
  }
  PXFArduino::pxfarduino->mEncoderPinLastRA = rstate;

  if (!PXFArduino::pxfarduino->mDirectionR)
    PXFArduino::pxfarduino->mDurationR++;
  else
    PXFArduino::pxfarduino->mDurationR--;
}
//----------------------------------------------------------------------------
void PXFArduino::_EncoderInit()
{
  mIsUseSpeedEncorder = true;

  attachInterrupt(0, sWheelSpeedL, CHANGE); // 0 is mPinEncroderLA D2
  pinMode(mPinEncroderLB, INPUT);
  mDirectionL = true;

  attachInterrupt(1, sWheelSpeedR, CHANGE); // 1 is mPinEncroderRA D3
  pinMode(mPinEncroderRB, INPUT);
  mDirectionR = true;

  if (-1 != mEncorderTickEvent)
    mTimer.stop(mEncorderTickEvent);
  mEncorderTickEvent = -1;
  mEncorderTickEvent = mTimer.every(100, sUpdateSpeed);
}
//----------------------------------------------------------------------------
void PXFArduino::_LeftRun(int val, int spd)
{
  if (MM_BOARD == mMotoMode)
  {
    analogWrite(mPinLSpeed, spd);

    if (1 == val)
      digitalWrite(mPinL0, HIGH);
    else if (2 == val)
      digitalWrite(mPinL0, LOW);
  }
  else if (MM_298N == mMotoMode)
  {
    if (0 == val)
    {
      digitalWrite(mPinL0, LOW);
      digitalWrite(mPinL1, LOW);
    }
    else if (1 == val)
    {
      digitalWrite(mPinL0, HIGH);
      digitalWrite(mPinL1, LOW);
    }
    else if (2 == val)
    {
      digitalWrite(mPinL0, LOW);
      digitalWrite(mPinL1, HIGH);
    }
    
    analogWrite(mPinLSpeed, spd);
  }
}
//----------------------------------------------------------------------------
void PXFArduino::_RightRun(int val, int spd)
{
  if (MM_BOARD == mMotoMode)
  {
    analogWrite(mPinRSpeed, spd);

    if (1 == val)
      digitalWrite(mPinR0, HIGH);
    else if (2 == val)
      digitalWrite(mPinR0, LOW);
  }
  else if (MM_298N == mMotoMode)
  {
    if (0 == val)
    {
      digitalWrite(mPinR0, LOW);
      digitalWrite(mPinR1, LOW);
    }
    else if (1 == val)
    {
      digitalWrite(mPinR0, LOW);
      digitalWrite(mPinR1, HIGH);
    }
    else if (2 == val)
    {
      digitalWrite(mPinR0, HIGH);
      digitalWrite(mPinR1, LOW);
    }
    
    analogWrite(mPinRSpeed, spd);
  }
}
//----------------------------------------------------------------------------
void PXFArduino::_Delay(float seconds)
{
    long endTime = millis() + seconds * 1000;
    while(millis() < endTime)
      _Loop();
}
//----------------------------------------------------------------------------
void PXFArduino::_Loop()
{
}
//----------------------------------------------------------------------------
void PXFArduino::_SendCMD(String &cmdStr)
{
  String lastCmdStr = "0000" + cmdStr;
  int allLength = 2 + cmdStr.length();
  *(unsigned short *)(&lastCmdStr[0]) = (unsigned short)allLength; // length
  *(unsigned short *)(&lastCmdStr[2]) = (unsigned short)GeneralServerMsgID; // id
  Serial.println(lastCmdStr);
}
//----------------------------------------------------------------------------
