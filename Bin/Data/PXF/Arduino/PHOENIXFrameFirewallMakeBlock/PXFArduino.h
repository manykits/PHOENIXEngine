// PXFArduino.h

#ifndef PXFARDUINO_H
#define PXFARDUINO_H

#include <Arduino.h>
#include "PXFTimer.h"

// server
//#define PXF_SERVER 1
#if defined PXF_SERVER
#include <Servo.h>
#endif

#include "Me7SegmentDisplay.h"
#include "MeDCMotor.h"
#include "MeUltrasonicSensor.h"

enum PXFPin
{
    P_0 = 0,
    P_1,
    P_2,
    P_3,
    P_4,
    P_5,
    P_6,
    P_7,
    P_8,
    P_9,
    P_10,
    P_11,
    P_12,
    P_13,
    P_A0,
    P_A1,
    P_A2,
    P_A3,
    P_A4,
    P_A5,
    P_A6,
    P_MAX_TYPE
};
#define PXFA0 30

enum PXFPMode
{
    PM_NONE = 0,
    PM_INPUT,
    PM_OUTPUT,
    PM_MAX_TYPE
};

#define NumMaxCMDs 10
#define GeneralServerMsgID 2

enum PXFMotoType
{
  MT_0,
  MT_1,
  MT_MAX_TYPE,
};

enum PXFDirectionType
{
  DT_NONE,
  DT_FORWORD,
  DT_BACKWORD,
  DT_MAX_TYPE
};

enum PXFSimpleDirectionType
{
  SDT_NONE,
  SDT_FORWORD,
  SDT_BACKWORD,
  SDT_LEFT,
  SDT_RIGHT,
  SDT_MAX_TYPE
};

enum OptionType
{
  OT_TOGET_NETID,
  OT_RETURN_NETID,
  OT_PM,
  OT_DW,
  OT_AW,
  OT_RETURN_DR,
  OT_RETURN_AR,
  OT_SVR_I,
  OT_SVR_W,
  OT_DST_I,
  OT_DST_T,
  OT_RETURN_DIST,
  OT_MOTO_I,
  OT_MOTO_RUN,
  OT_MOTO_RUNSIMPLE,
  OT_MOTO_STOP,
  OT_MOTO_I_SPD,
  OT_RETURN_MOTOSPD,
  OT_MOTO_I_DRIVER4567,
  OT_MOTO_I_DRIVER298N,
  OT_MP3_INIT,
  OT_MP3_PLAY,
  OT_MP3_INDEX,
  OT_MP3_NEXT,
  OT_MP3_STOP,
  OT_MP3_VOLUME,
  OT_IR_INIT,
  OT_IR_SEND,
  OT_RETURN_IR,
  OT_HX711_I,
  OT_HX711_TEST,
  OT_RETURN_HX711,
  OT_MC_INTERNAL_LIGHT,
  OT_MC_LIGHT,
  OT_MC_SEGMENT,
  OT_MC_MOTO,
  OT_MC_DISTTEST,
  OT_MB_MOTO,
  OT_MB_SEND,
  OT_MB_BUZZER,
  OT_MAX_TYPE
};

class PXFArduino
{
public:
  PXFArduino();

  static PXFArduino *pxfarduino;

  static char PinStr[P_MAX_TYPE];
  static char sOptTypeStr[OT_MAX_TYPE];

  // cmds process
  void OnCMDGroup(String &cmdStr);
  void OnCMD(String &cmdStr);

  // to get pin values by timer
  void CalPinVals();

  void _PinMode(PXFPin pin, PXFPMode mode);  
  PXFPMode PinModes[P_MAX_TYPE];

private:
  String I2Str(int val);
  int _Str2IO(String &str);
  int _Str2Pin(String &str);
  PXFPin _Str2PXFPin(String &str);
  bool _Str2Bool(String &str);
  int _Str2Int(String &str);
  int _Str2DirType(String &str);
  int _Str2SimpleDirType(String &str);

  int mIndexCMD;
  char *mpCMD;

#define NumCMDParams 7
  String mCmdParams[NumCMDParams];
  int mCMDIndexTemp;
  char *mpCMDParam;

public:
  void Init(bool isReset=false);
  void Reset();
  void InitPinTickEvent();
  void Tick();

public:
  void _SendNetID();
  void _DistInit(int pinTrig, int pinEcho);
  void _DistTest();
  enum MotoMode
  {
    MM_BOARD,
    MM_298N,
    MM_MAX_TYPE
  };
  void _MotoInit();
  void _MotoInit4567();
  void _MotoInit298N(int pinL, int pinL1, int pinLS, int pinR, int pinR1, int pinRS);
  void _MotoSpeedInit(int encorderLA, int encorderLB, int encorderRA, int encorderRB);
  void _EncoderInit();
  void _LeftRun(int val, int spd);
  void _RightRun(int val, int spd);
  void _Delay(float seconds);
  void _Loop();
  void _SendCMD(String &cmdStr);

public:
  int mPinEncroderLA;
  int mPinEncroderLB;
  byte mEncoderPinLastLA;
  int mDurationL;
  bool mDirectionL;
  int mPinEncroderRA;
  int mPinEncroderRB;
  int mEncoderPinLastRA;
  int mDurationR;
  bool mDirectionR;

  bool mIsNewSpeed;
  int mCurSpeedLeft;
  int mCurSpeedRight;

private:
  int mNetID;

  Timer mTimer;
  int mPinTickEvent;

#if defined PXF_SERVER
  #define NumMaxServer 3
  Servo mServo[NumMaxServer];
#endif

  int mPinDistTrigger;
  int mPinDistEcho;
  float mDist;
  bool mIsNewDist;

  int mPinL0;
  int mPinL1;
  int mPinLSpeed;
  int mPinR0;
  int mPinR1;
  int mPinRSpeed;

  bool mIsUseSpeedEncorder;
  int mEncorderTickEvent;

  MotoMode mMotoMode;

#if defined PXF_SSD1306
  Adafruit_SSD1306 *mDisplay;
#endif

#if defined PXF_MP3
  SoftwareSerial *mMP3Serial;
#endif

#if defined PXF_IR
  IRrecv *mIRrecv;
  IRsend mIRsend;
#endif

  Me7SegmentDisplay mSeg;
  MeDCMotor mMotor;
  MeUltrasonicSensor mUltraSensor;
};

#endif
