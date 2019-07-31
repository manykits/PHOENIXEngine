// PX2TCPServerParams.cpp

#include "PX2TCPServerParams.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
TCPServerParams::TCPServerParams() :
mThreadIdleTime(10000000),
mMaxThreads(0),
mMaxQueued(64),
mThreadPriority(Thread::PRIO_NORMAL)
{
}
//----------------------------------------------------------------------------
TCPServerParams::~TCPServerParams()
{
}
//----------------------------------------------------------------------------
void TCPServerParams::SetThreadIdleTime(const Timespan& milliseconds)
{
	mThreadIdleTime = milliseconds;
}
//----------------------------------------------------------------------------
void TCPServerParams::SetMaxThreads(int count)
{
	assertion(count > 0, "");

	mMaxThreads = count;
}
//----------------------------------------------------------------------------
void TCPServerParams::SetMaxQueued(int count)
{
	assertion(count >= 0, "");

	mMaxQueued = count;
}
//----------------------------------------------------------------------------
void TCPServerParams::SetThreadPriority(Thread::Priority prio)
{
	mThreadPriority = prio;
}
//----------------------------------------------------------------------------