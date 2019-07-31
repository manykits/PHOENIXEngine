// UltraMat

#ifndef ULTRAMAT
#define ULTRAMAT

#include "UltraObj.h"
#include "Arduino.h"

const int NumMaxUltra = 6;
const int OT_DSTMAT_I = 30;
const int OT_RETURN_DSTMAT = 31;

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

class UltraMat
{
public:
  UltraMat();

  void Init(int index, int pinTrigger, int pinEcho);
  void Test(int index);
  float GetDistance(int index);
  void SetRange(int index, float near, float far);
  void Shutdown(int index);  
  void Tick();

  void OnCMD(String &cmdStr);

  UltraObj Objs[NumMaxUltra];

private:
  int _Str2Pin(String &str);
  char *mpCMD;

#define NumCMDParams 7
  String mCmdParams[NumCMDParams];
  int mCMDIndexTemp;
  char *mpCMDParam;
};

#endif
