// PXFArduino.cpp

#include "PXFArduino.h"

//----------------------------------------------------------------------------
double Kp=0.6, Ki=5, Kd=0; 
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
unsigned char PXFArduino::sOptTypeVal[OT_MAX_TYPE] =
{
    100, //OT_TOGET_NETID
    101, //OT_RETRUN_NETID
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
    19, //OT_MP3_DO
    20, //OT_MP3_PLAYFOLDER
    21, //OT_MP3_SETVOLUME
    24, //OT_IR_INIT
    25, //OT_IR_SEND
    26, //OT_RETURN_IR
    27, //OT_HX711_I
    28, //OT_HX711_TEST
    29, //OT_RETURN_HX711
    30, //OT_DSTMAT_I
    31, //OT_RETURN_DSTMAT
    32, //OT_AXIS_I
    33, //OT_RETURN_AXIS
    34, //OT_SET_TIME,
    35, //OT_RC_INIT
    36, //OT_RC_SEND
    37, //OT_RETRUN_RC
    38, //OT_DHT_I
    39, //OT_RETURN_DHTTEMP
    40, //OT_RETURN_DHTHUMI
    41, //OT_LEDSTRIP_I
    42, //OT_LEDSTRIP_SET
    43, //OT_SEGMENT_I
    44, //OT_SEGMENT_BRIGHTNESS
    45, //OT_SEGMENT_CLEAR
    46, //OT_SEGMENT_DISPLAY
    47, //OT_LEDMATRIX_I
    48, //OT_LEDMATRIX_BRIGHTNESS
    49, //OT_LEDMATRIX_CLEARSCREEN
    50, //OT_LEDMATRIX_LIGHTAT
    150, //OT_MC_INTERNAL_LIGHT
    151, //OT_MC_LIGHT
    152, //OT_MC_SEGMENT
    153, //OT_MC_MOTO
    154, //OT_MC_DISTTEST
    161, //OT_MB_MOTO
    162, //OT_MB_SEND
    163, //OT_MB_BUZZER
    200 //OT_VERSION
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
  mSettedTimeMe = 0;
  mLastSendVersionTime = 0;

  digitalWrite(13, LOW);

  for (int i = P_0; i <= P_MAX_TYPE; i++)
  {
    PinModes[i] = 10;
  }

  mpCMD = NULL;

  mCMDIndexTemp = 0;
  mpCMD = 0;

  mPinDistTrigger = 0;
  mPinDistEcho = 0;
  mDist = 0.0f;
  mDistCheckLastTime = 0;

  mPinL0 = 0;
  mPinL1 = 0;
  mPinLSpeed = 0;
  mPinR0 = 0;
  mPinR1 = 0;
  mPinRSpeed = 0;

  mIsUseSpeedEncorder = false;

  mMotoMode = MM_BOARD;
  mPinEncroderLA = 2;
  mPinEncroderLB = 10;
  mPinEncroderRA = 3;
  mPinEncroderRB = 11;

#if defined PXF_PID
  mEncoder0PinALastL = 0;
  mEncoder0PinALastR = 0;
  mDurationL = 0;
  mDurationR = 0;
  mAbsDurationL = 0;
  mAbsDurationR = 0;
  mIsStopL = true;
  mIsStopR = true;
  mValOutputL = 0;
  mValOutputR = 0;
  mDirectionL = true;
  mDirectionR = true;

  mPID = 0;
  mPID1 = 0;
#endif

#if defined PXF_SSD1306
  mDisplay = 0;                                                     
#endif

#if defined PXF_DFMP3
  if (isReset && mMP3DFSerial)
  {
    delete mMP3DFSerial;
  }
  mMP3DFSerial = 0;
#endif

#if defined PXF_IR
  if (isReset && mIRrecv)
  {
    delete mIRrecv;
  }
  mIRrecv = 0;
#endif

#if defined PXF_AXIS
  mPitch = 0.0f;
  mRoll = 0.0f;
  mYaw = 0.0f;
  mAxisTickEvent = -1;
  mAxisLastTick = 0;
#endif

#if defined PXF_PID
  if (mPID)
  {
    delete(mPID);
  }
  mPID = 0;

  if (mPID1)
  {
    delete(mPID1);
  }
  mPID1 = 0;
#endif

#if defined PXF_DHT
  mIsInitedDHT = false;
#endif
}
//----------------------------------------------------------------------------
void PXFArduino::_PinMode(PXFPin pin, PXFPMode mode)
{
    PinModes[(int)pin] = mode;
    int p = PXFPin2Pin(pin);
    pinMode(p, mode==PM_OUTPUT?OUTPUT:INPUT);
}
//----------------------------------------------------------------------------
void PXFArduino::_DigitalWrite(PXFPin pin, bool isHigh)
{
  int p = PXFPin2Pin(pin);
  digitalWrite(p, isHigh?HIGH:LOW);
}
//----------------------------------------------------------------------------
void PXFArduino::_PwmWrite(PXFPin pin, int val)
{
  int p = PXFPin2Pin(pin);
  analogWrite(p, val);
}
//----------------------------------------------------------------------------
void PXFArduino::_AnalogWrite(PXFPin pin, int val)
{
  int p = PXFPin2Pin(pin);
  analogWrite(p, val);
}
//----------------------------------------------------------------------------
int PXFArduino::_AnalogRead(PXFPin pin)
{
  int p = PXFPin2Pin(pin);
  return analogRead(p);
}
//----------------------------------------------------------------------------
int PXFArduino::_DigitalRead(PXFPin pin)
{
  int p = PXFPin2Pin(pin);
  return digitalRead(p);
}
//----------------------------------------------------------------------------
void PXFArduino::_ServerInit(int index, PXFPin pin)
{
#if defined PXF_SERVO
  int p = PXFPin2Pin(pin);
  if (0 <= index && index < NumMaxServer)
    mServo[index].attach(p);
#endif
}
//----------------------------------------------------------------------------
void PXFArduino::_ServerWrite(int index, int val)
{
#if defined PXF_SERVO
  if (0 <= index && index < NumMaxServer)
    mServo[index].write(val);
#endif
}
//----------------------------------------------------------------------------
void PXFArduino::_DistInit(PXFPin trigger, PXFPin echo)
{
  int pinT = PXFPin2Pin(trigger);
  int pinE = PXFPin2Pin(echo);
  _DistInit_(pinT, pinE);
}
//----------------------------------------------------------------------------
float PXFArduino::_GetDist()
{
  return mDist;
}
//----------------------------------------------------------------------------
void PXFArduino::_VehicleSpeedEncorderInit(PXFPin encorderLA, PXFPin encorderLB, PXFPin encorderRA, PXFPin encorderRB)
{
  int pinLA = PXFPin2Pin(encorderLA);
  int pinLB = PXFPin2Pin(encorderLB);
  int pinRA = PXFPin2Pin(encorderRA);
  int pinRB = PXFPin2Pin(encorderRB);
  _MotoSpeedInit(pinLA, pinLB, pinRA, pinRB);
}
//----------------------------------------------------------------------------
void PXFArduino::_VehicleRun(int index, PXFDirectionType dir, int speed)
{
  if (0==dir)
    speed = 0;

  if (0 == index)
    _LeftRun((int)dir, speed);
  else
    _RightRun((int)dir, speed);
}
//----------------------------------------------------------------------------
void PXFArduino::_VehicleSimpleRun(PXFSimpleDirectionType dir, int spd)
{
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
//----------------------------------------------------------------------------
void PXFArduino::_VehicleStop()
{
  _LeftRun(0, 0);
  _RightRun(0, 0);
}
//----------------------------------------------------------------------------
void PXFArduino::_WeightInit(int index, PXFPin pinR, PXFPin pinT)
{
}
//----------------------------------------------------------------------------
void PXFArduino::_WeightTest(int index)
{ 
}
//----------------------------------------------------------------------------
float PXFArduino::_GetWeight(int index)
{
  return 0.0f;
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
  unsigned char cmdCh = sOptTypeVal[OT_RETURN_DR];
  char strCMDCh[32];
  memset(strCMDCh, 0, 32);
  itoa(cmdCh, strCMDCh, 10);
    
  for (int i=P_0; i<=P_13; i++)
  {
    if (PM_INPUT == PinModes[i])
    {
      int val = digitalRead(i);
      Serial.print("0000");
      Serial.print(String(strCMDCh)); 
      Serial.print(" ");
      Serial.print(i);
      Serial.print(" ");
      Serial.println(val);
    }
  }
  
  unsigned char cmdCh1 = sOptTypeVal[OT_RETURN_AR];
  char strCMDCh1[32];
  memset(strCMDCh1, 0, 32);
  itoa(cmdCh1, strCMDCh1, 10);
  
  for (int i=P_A0; i<=P_A5; i++)
  {
    int pin = A0 + (i - P_A0);
    if (PM_INPUT == PinModes[i])
    {
      int val = analogRead(pin);
      Serial.print("0000");
      Serial.print(String(strCMDCh1)); 
      Serial.print(" ");
      Serial.print(i);
      Serial.print(" ");
      Serial.println(val);
    }
  }
}
//----------------------------------------------------------------------------
void PXFArduino::_SendNetID()
{
    char str[32];
    memset(str, 0, 32);
    itoa(mNetID, str, 10); // 10 is decimal

    unsigned char cmdCh = sOptTypeVal[OT_RETURN_NETID];
    char strCMDCh[32];
    memset(strCMDCh, 0, 32);
    itoa(cmdCh, strCMDCh, 10);

    String sendStr = String(strCMDCh) + " " + str;
    _SendCMD(sendStr);
}
//----------------------------------------------------------------------------
void PXFArduino::_SendVersion()
{
    unsigned char cmdCh = sOptTypeVal[OT_VERSION];
    char strCMDCh[32];
    memset(strCMDCh, 0, 32);
    itoa(cmdCh, strCMDCh, 10);
        
    Serial.print("0000");
    Serial.println(String(strCMDCh)); 
}
//----------------------------------------------------------------------------
boolean result;
boolean resultR;
//----------------------------------------------------------------------------
void PXFArduino::Tick()
{
#if defined PXF_PID
  if (mIsUseSpeedEncorder && mPID && mPID1)
  {
    if (!mIsStopL)
      analogWrite(mPinLSpeed, (int)mValOutputL);
    if (!mIsStopR)
      analogWrite(mPinRSpeed, (int)mValOutputR);
    
    mAbsDurationL = abs(mDurationL);
    mAbsDurationR = abs(mDurationR);
    
    result = mPID->Compute();
    resultR = mPID1->Compute();
    if(result)   
      mDurationL = 0;
    if(resultR)
      mDurationR = 0;
  }
#endif

  if (millis()  - mLastSendVersionTime >= 1000)
  {
      mLastSendVersionTime = millis();
      _SendVersion();
  }

  mTimer.update();

#if defined PXF_IR
  decode_results iresultes;
  if (mIRrecv && mIRrecv->decode(&iresultes))
  {
    int val = iresultes.value;
    mIRrecv->resume();
    _IRRecv(val);
  }
#endif

#if defined PXF_RCSWITCH
 if (mRCSwitch.available()) 
 {   
    int recvVal = mRCSwitch.getReceivedValue();

    unsigned char cmdCh = sOptTypeVal[OT_RETRUN_RC];
    char strCMDCh[32];
    memset(strCMDCh, 0, 32);
    itoa(cmdCh, strCMDCh, 10);
    
    Serial.print("0000");
    Serial.print(String(strCMDCh)); 
    Serial.print(" ");
    Serial.println(recvVal);

    mRCSwitch.resetAvailable();
  }
#endif

#if defined PXF_DHT
  _DHTSendTemperatureHumidity();
#endif
}
void sCalPinValue()
{
  PXFArduino::pxfarduino->CalPinVals();
}
//----------------------------------------------------------------------------
void PXFArduino::_DistInit_(int pinTrig, int pinEcho)
{
  mPinDistTrigger = pinTrig;
  mPinDistEcho = pinEcho;
  pinMode(mPinDistTrigger, OUTPUT); // 定义超声波输出脚
  pinMode(mPinDistEcho, INPUT);     // 定义超声波输入脚

  mDist = 0.0f;
  mDistCheckLastTime = 0;
}
//----------------------------------------------------------------------------
void PXFArduino::_DistTest()
{
  digitalWrite(mPinDistTrigger, LOW); //低高低电平发一个短时间脉冲去TrigPin
  delayMicroseconds(2);
  digitalWrite(mPinDistTrigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(mPinDistTrigger, LOW);

  float dist = pulseIn(mPinDistEcho, HIGH) / 58.0; //将回波时间换算成cm
  dist = (int(dist * 100.0)) / 100.0;       //保留两位小数
  if (2 <= dist && dist <= 400)
  {
    mDist = dist;
  }
}
//----------------------------------------------------------------------------
void PXFArduino::_MotoInit10111213()
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

#if defined PXF_AXIS
  mPitch = 0.0f;
  mRoll = 0.0f;
  mYaw = 0.0f;
#endif

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

#if defined PXF_AXIS
  mPitch = 0.0f;
  mRoll = 0.0f;
  mYaw = 0.0f;
#endif

 //_MotoSpeedInit(2, 10, 3, 11);
}
//----------------------------------------------------------------------------
#if defined PXF_PID
void wheelSpeed()
{
  int lstate = digitalRead(PXFArduino::pxfarduino->mPinEncroderLA);
  if((PXFArduino::pxfarduino->mEncoder0PinALastL == LOW) && lstate==HIGH)
  {
    int val = digitalRead(PXFArduino::pxfarduino->mPinEncroderLB);
    if(val == LOW && PXFArduino::pxfarduino->mDirectionL)
    {
      PXFArduino::pxfarduino->mDirectionL = false; //Reverse
    }
    else if(val == HIGH && !PXFArduino::pxfarduino->mDirectionL)
    {
      PXFArduino::pxfarduino->mDirectionL = true;  //Forward
    }
  }
  PXFArduino::pxfarduino->mEncoder0PinALastL = lstate;
 
  if(!PXFArduino::pxfarduino->mDirectionL) 
    PXFArduino::pxfarduino->mDurationL++;
  else  
    PXFArduino::pxfarduino->mDurationL--;
}
void wheelSpeedR()
{
  int lstate = digitalRead(PXFArduino::pxfarduino->mPinEncroderRA);
  if((PXFArduino::pxfarduino->mEncoder0PinALastR == LOW) && lstate==HIGH)
  {
    int val = digitalRead(PXFArduino::pxfarduino->mPinEncroderRB);
    if(val == LOW && !PXFArduino::pxfarduino->mDirectionR)
    {
      PXFArduino::pxfarduino->mDirectionR = true; //Reverse
    }
    else if(val == HIGH && PXFArduino::pxfarduino->mDirectionR)
    {
      PXFArduino::pxfarduino->mDirectionR = false;  //Forward
    }
  }
  PXFArduino::pxfarduino->mEncoder0PinALastR = lstate;
 
  if(!PXFArduino::pxfarduino->mDirectionR)
    PXFArduino::pxfarduino->mDurationR++;
  else 
   PXFArduino::pxfarduino->mDurationR--;
}
#endif 
//----------------------------------------------------------------------------
void PXFArduino::_MotoSpeedInit(int encorderLA, int encorderLB,
  int encorderRA, int encorderRB)
{
  mPinEncroderLA = encorderLA;
  mPinEncroderLB = encorderLB;
  mPinEncroderRA = encorderRA;
  mPinEncroderRB = encorderRB;

  if (encorderLA == encorderLB && encorderRA==encorderRB)
  {
    mIsUseSpeedEncorder = false;

#if defined PXF_PID
    if (mPID)
    {
      delete(mPID);
    }
    mPID = 0;
    if (mPID1)
    {
      delete(mPID1);
    }
    mPID1 = 0;
    mEncoder0PinALastL = 0;
    mEncoder0PinALastR = 0;
    mDurationL = 0;
    mDurationR = 0;
    mAbsDurationL = 0;
    mAbsDurationR = 0;
    mValOutputL = 0;
    mValOutputR = 0;
    mDirectionL = true;
    mDirectionR = true;
#endif
  }
  else
  {
    pinMode(mPinEncroderLB, INPUT);
    pinMode(mPinEncroderRB, INPUT);

    mIsUseSpeedEncorder = true;

#if defined PXF_PID
    mEncoder0PinALastL = 0;
    mEncoder0PinALastR = 0;
    mDurationL = 0;
    mDurationR = 0;
    mAbsDurationL = 0;
    mAbsDurationR = 0;
    mValOutputL = 0;
    mValOutputR = 0;
    mDirectionL = true;
    mDirectionR = true;

    if (mPID)
    {
      delete(mPID);
    }
    mPID = new PID(&mAbsDurationL, &mValOutputL, &mSetPoint_L, Kp, Ki, Kd, DIRECT);
  
    if (mPID1)
    {
      delete(mPID1);
    }
    mPID1 = new PID(&mAbsDurationR, &mValOutputR, &mSetPoint_R, Kp, Ki, Kd, DIRECT);
    
    mPID->SetMode(AUTOMATIC);
    mPID->SetSampleTime(100);
    mPID1->SetMode(AUTOMATIC);
    mPID1->SetSampleTime(100);
    
    mDirectionL = true;
    pinMode(mPinEncroderLB, INPUT);  
    attachInterrupt(0, wheelSpeed, CHANGE);
  
    mDirectionR = true;
    pinMode(mPinEncroderRB, INPUT);  
    attachInterrupt(1, wheelSpeedR, CHANGE);
#endif 
  }
}
//----------------------------------------------------------------------------
float PXFArduino::_VehicleGetSpeed(int index)
{
  return 0.0f;
}
//----------------------------------------------------------------------------
void PXFArduino::_LeftRun(int val, int spd)
{
  if (MM_BOARD == mMotoMode)
  {
    if (mIsUseSpeedEncorder)
    {

      mSetPoint_L = spd;

      if (0 == val)
      {
        analogWrite(mPinLSpeed, 0);
        mSetPoint_L = 0;
        #if defined PXF_PID
        mIsStopL=true;
        #endif
      }
      else if (1 == val)
      {
        digitalWrite(mPinL0, HIGH);
        #if defined PXF_PID
        mIsStopL=false; 
        #endif
      }
      else if (2 == val)
      {
        digitalWrite(mPinL0, LOW);
        #if defined PXF_PID
        mIsStopL=false;
        #endif
      }
    }
    else
    {
      mSetPoint_L = spd;
      analogWrite(mPinLSpeed, (int)mSetPoint_L);

      if (0 == val)
      {
        #if defined PXF_PID
        mIsStopL=true;
        #endif
      }
      if (1 == val)
      {
        digitalWrite(mPinL0, HIGH);
        #if defined PXF_PID
        mIsStopL=false; 
        #endif
      }
      else if (2 == val)
      {
        digitalWrite(mPinL0, LOW); 
        #if defined PXF_PID
        mIsStopL=false; 
        #endif
      }
    }
  }
  else if (MM_298N == mMotoMode)
  {
    if (mIsUseSpeedEncorder)
    {
      mSetPoint_L = spd;
      
      if (0 == val)
      {
        digitalWrite(mPinL0, LOW);
        digitalWrite(mPinL1, LOW);
        analogWrite(mPinLSpeed, 0);
        mSetPoint_L = 0;
#if defined PXF_PID
        mIsStopL=true;
#endif
      }
      else if (1 == val)
      {
        digitalWrite(mPinL0, HIGH);
        digitalWrite(mPinL1, LOW);
#if defined PXF_PID
        mIsStopL=false;
#endif
      }
      else if (2 == val)
      {
        digitalWrite(mPinL0, LOW);
        digitalWrite(mPinL1, HIGH);
#if defined PXF_PID
        mIsStopL=false; 
#endif
      }
    }
    else
    {
      mSetPoint_L = spd;
      
      if (0 == val)
      {
        digitalWrite(mPinL0, LOW);
        digitalWrite(mPinL1, LOW);
        mSetPoint_L = 0;

#if defined PXF_PID
        mIsStopL=true; 
#endif
      }
      else if (1 == val)
      {
        digitalWrite(mPinL0, HIGH);
        digitalWrite(mPinL1, LOW);

#if defined PXF_PID
        mIsStopL=false; 
#endif
      }
      else if (2 == val)
      {
        digitalWrite(mPinL0, LOW);
        digitalWrite(mPinL1, HIGH);

#if defined PXF_PID
        mIsStopL=false; 
#endif
      }

      analogWrite(mPinLSpeed, (int)mSetPoint_L);
    }
  }
}
//----------------------------------------------------------------------------
void PXFArduino::_RightRun(int val, int spd)
{
  if (MM_BOARD == mMotoMode)
  {
    if (mIsUseSpeedEncorder)
    {
      mSetPoint_R = spd;

      if (0 == val)
      {
        analogWrite(mPinRSpeed, 0); 
#if defined PXF_PID
        mIsStopR=true;
#endif
        mSetPoint_R = 0;
      }
      else if (1 == val)
      {
        digitalWrite(mPinR0, HIGH);
#if defined PXF_PID
       mIsStopR=false; 
#endif
      }
      else if (2 == val)
      {
        digitalWrite(mPinR0, LOW); 
#if defined PXF_PID
        mIsStopR=false; 
#endif
      }
    }
    else
    {
      mSetPoint_R = spd;
      analogWrite(mPinRSpeed, (int)mSetPoint_R);

      if (0 == val)
      {
#if defined PXF_PID
        mIsStopR=true;
#endif
      }
      if (1 == val)
      {
        digitalWrite(mPinR0, HIGH);
#if defined PXF_PID
        mIsStopR=false; 
#endif
      }
      else if (2 == val)
      {
        digitalWrite(mPinR0, LOW); 
#if defined PXF_PID
        mIsStopR=false; 
#endif
      }
    }
  }
  else if (MM_298N == mMotoMode)
  {
    if (mIsUseSpeedEncorder)
    { 
      mSetPoint_R = spd;       
          
      if (0 == val)
      {
        digitalWrite(mPinR0, LOW);
        digitalWrite(mPinR1, LOW);
        analogWrite(mPinRSpeed, 0); 
        mSetPoint_R = 0;
#if defined PXF_PID
        mIsStopR=true;
#endif
      }
      else if (1 == val)
      {
        digitalWrite(mPinR0, LOW);
        digitalWrite(mPinR1, HIGH);
#if defined PXF_PID
        mIsStopR=false;
#endif
      }
      else if (2 == val)
      {
        digitalWrite(mPinR0, HIGH);
        digitalWrite(mPinR1, LOW);

#if defined PXF_PID
        mIsStopR=false;
#endif
      }
    }
    else
    {
      mSetPoint_R = spd;

      if (0 == val)
      {
        digitalWrite(mPinR0, LOW);
        digitalWrite(mPinR1, LOW);
        mSetPoint_R = 0;
        
#if defined PXF_PID
        mIsStopR=true;
#endif
      }
      else if (1 == val)
      {
        digitalWrite(mPinR0, LOW);
        digitalWrite(mPinR1, HIGH);

#if defined PXF_PID
        mIsStopR=false;
#endif
      }
      else if (2 == val)
      {
        digitalWrite(mPinR0, HIGH);
        digitalWrite(mPinR1, LOW);
        
#if defined PXF_PID
        mIsStopR=false;
#endif
      }

      analogWrite(mPinRSpeed, (int)mSetPoint_R);
    }
  }
}
//----------------------------------------------------------------------------
void sAxisTickTimer()
{
#if defined PXF_AXIS
  if (0==PXFArduino::pxfarduino->mSettedTimeMe)
    return;

  Vector norm = PXFArduino::pxfarduino->mMPU.readNormalizeGyro();
  unsigned long curTimer = millis();
  unsigned long timeStep = curTimer - PXFArduino::pxfarduino->mAxisLastTick;
  PXFArduino::pxfarduino->mAxisLastTick = curTimer;
  float fTime = timeStep*0.001;

  PXFArduino::pxfarduino->mPitch = PXFArduino::pxfarduino->mPitch + norm.YAxis * fTime;
  PXFArduino::pxfarduino->mRoll = PXFArduino::pxfarduino->mRoll + norm.XAxis * fTime;
  PXFArduino::pxfarduino->mYaw = PXFArduino::pxfarduino->mYaw + norm.ZAxis * fTime;

  char cmdCh = PXFArduino::pxfarduino->sOptTypeVal[OT_RETURN_AXIS]; 
  char strCMDCh[32];
  memset(strCMDCh, 0, 32);
  itoa(cmdCh, strCMDCh, 10);

  unsigned long sendTime = curTimer - PXFArduino::pxfarduino->mSettedTimeMe;
  Serial.print("0000");
  Serial.print(String(strCMDCh));
  Serial.print(" ");
  Serial.print(sendTime);
  Serial.print(" ");
  Serial.print(norm.YAxis);
  Serial.print(" ");
  Serial.print(norm.XAxis);
  Serial.print(" ");
  Serial.print(norm.ZAxis);
  Serial.print(" ");
  Serial.print(PXFArduino::pxfarduino->mPitch);
  Serial.print(" ");
  Serial.print(PXFArduino::pxfarduino->mRoll);  
  Serial.print(" ");
  Serial.println(PXFArduino::pxfarduino->mYaw);
#endif
}
//----------------------------------------------------------------------------
void PXFArduino::_SetTime()
{
  mSettedTimeMe = millis();
  if (0.0==mSettedTimeMe)
    mSettedTimeMe = 0.001;
}
//----------------------------------------------------------------------------
void PXFArduino::_InitAxis()
{
#if defined PXF_AXIS
  mMPU.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G);
  while(!mMPU.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    delay(1000);
  }

  mMPU.calibrateGyro();
  mMPU.setThreshold(3);

  mPitch = 0.0f;
  mRoll = 0.0f;
  mYaw = 0.0f;

  mAxisLastTick = 0;
  if (-1 != mAxisTickEvent)
    mTimer.stop(mAxisTickEvent);
  mAxisTickEvent = -1;
  mAxisTickEvent = mTimer.every(50, sAxisTickTimer);

  mAxisLastTick = millis();
#endif
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
  //int allLength = 2 + cmdStr.length();
  //*(unsigned short *)(&lastCmdStr[0]) = (unsigned short)allLength; // length
  //*(unsigned short *)(&lastCmdStr[2]) = (unsigned short)GeneralServerMsgID; // id
  Serial.println(lastCmdStr);
}
//----------------------------------------------------------------------------
