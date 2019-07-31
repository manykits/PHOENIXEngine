// PX2ThreadTarget.cpp

#include "PX2ThreadTarget.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ThreadTarget::ThreadTarget (Callback callback)
	:
mCallback(callback)
{
}
//----------------------------------------------------------------------------
ThreadTarget::ThreadTarget (const ThreadTarget& te)
	:
mCallback(te.mCallback)
{
}
//----------------------------------------------------------------------------
ThreadTarget::~ThreadTarget ()
{
}
//----------------------------------------------------------------------------
ThreadTarget& ThreadTarget::operator = (const ThreadTarget &te)
{
	mCallback  = te.mCallback;
	return *this;
}
//----------------------------------------------------------------------------
void ThreadTarget::Run ()
{
	mCallback();
}
//----------------------------------------------------------------------------