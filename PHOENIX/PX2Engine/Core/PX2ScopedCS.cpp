// PX2ScopedCS.cpp

#include "PX2ScopedCS.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ScopedCS::ScopedCS (Mutex* mutex)
    :
    mMutex(mutex)
{
    mMutex->Enter();
}
//----------------------------------------------------------------------------
ScopedCS::~ScopedCS ()
{
    mMutex->Leave();
}
//----------------------------------------------------------------------------