// PX2ScopedCS.cpp

#include "PX2ScopedCSUN.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ScopedCSUN::ScopedCSUN (Mutex* mutex, bool unlockNow)
	:
mMutex(mutex)
{
	if (unlockNow)
		mMutex->Leave();
}
//----------------------------------------------------------------------------
ScopedCSUN::~ScopedCSUN ()
{
	mMutex->Enter();
}
//----------------------------------------------------------------------------