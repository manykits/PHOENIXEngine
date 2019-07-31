#ifndef hectormapmutex_h__
#define hectormapmutex_h__

#include "MapLockerInterface.h"
#include "PX2Mutex.hpp"

class HectorMapMutex : public MapLockerInterface
{
public:
  virtual void lockMap()
  {
    mapModifyMutex_.Enter();
  }

  virtual void unlockMap()
  {
    mapModifyMutex_.Leave();

  }

  PX2::Mutex mapModifyMutex_;
};

#endif
