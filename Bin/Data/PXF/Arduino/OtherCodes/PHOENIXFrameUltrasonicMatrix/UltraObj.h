// UltraObj

#ifndef ULTRAOBJ
#define ULTRAOBJ

class UltraObj
{
public:
  UltraObj();
  ~UltraObj();

  void Init(int pinTrig, int pinEcho);
  void SetRange(float near, float far);
  void Shutdown();
  bool IsInited() const;
    
  void Test();
  void Tick();

  int Index;
  
  int PinTrigger;
  int PinEcho;
  int DistCheckLastTime;

  float Dist;

private:  
  bool mIsInited; 
  float mNear;
  float mFar;
};

#endif
