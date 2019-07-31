#ifndef _MAPLOCKERINTERFACE_H_
#define _MAPLOCKERINTERFACE_H_

class MapLockerInterface
{
public:
  virtual void lockMap() = 0;
  virtual void unlockMap() = 0;
};

#endif
