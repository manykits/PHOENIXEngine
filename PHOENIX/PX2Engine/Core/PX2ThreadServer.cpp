// PX2ThreadServer.cpp

#include "PX2ThreadServer.hpp"
#include "PX2Memory.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ThreadServer::ThreadServer ()
{
}
//----------------------------------------------------------------------------
ThreadServer::~ThreadServer ()
{
}
//----------------------------------------------------------------------------
bool ThreadServer::StartThreads (int threadsNum)
{
	if (0 == threadsNum)
		return false;

	for (int i=0; i<threadsNum; i++)
	{
		Thread *thread = new0 Thread();
		thread->Start(*this);
		mThreads.push_back(thread);
	}

	return true;
}
//----------------------------------------------------------------------------
void ThreadServer::WaitThreadsEnd ()
{
	for (int i=0; i<(int)mThreads.size(); i++)
	{
		mThreads[i]->Join();
	}

	for (int i=0; i<(int)mThreads.size(); i++)
	{
		delete0(mThreads[i]);
	}

	mThreads.clear();
}
//----------------------------------------------------------------------------