// PX2Thread.cpp

#include "PX2Thread.hpp"
#include "PX2Mutex.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2Assert.hpp"
#include "PX2ThreadLocal.hpp"
#include "PX2Memory.hpp"
#include "PX2ScopedCS.hpp"
using namespace PX2;

#if (defined(_WIN32) || defined(WIN32)) && !defined(PX2_USE_PTHREAD)
#include <windows.h>
#else
#endif
//----------------------------------------------------------------------------
Thread::Thread () :
mRunable(0),
mCallback(0),
mUserData(0),
mStackSize(0),
mPriority(PRIO_NORMAL),
mTLS(0)
{
	mID = UniqueID();
	mName = MakeName();
	mMutex = new0 Mutex();
}
//----------------------------------------------------------------------------
Thread::Thread (const std::string& name) :
mRunable(0),
mCallback(0),
mUserData(0),
mStackSize(0),
mTLS(0)
{
	mID = UniqueID();
	mName = name;
	mMutex = new0 Mutex();
}
//----------------------------------------------------------------------------
Thread::~Thread ()
{
	if (mMutex)
		delete0(mMutex);

	if (mTLS)
		delete0(mTLS);
}
//----------------------------------------------------------------------------
void Thread::SetName(const std::string& name)
{
	ScopedCS cs(mMutex);
	mName = name;
}
//----------------------------------------------------------------------------
std::string Thread::MakeName()
{
	std::ostringstream name;
	name << '#' << mID;
	return name.str();
}
//----------------------------------------------------------------------------
int Thread::UniqueID()
{
	static unsigned count = 0;
	++count;
	return count;
}
//----------------------------------------------------------------------------
ThreadLocalStorage& Thread::TLS()
{
	if (!mTLS)
		mTLS = new0 ThreadLocalStorage();

	return *mTLS;
}
//----------------------------------------------------------------------------
void Thread::ClearTLS()
{
	if (mTLS)
	{
		delete0(mTLS);
		mTLS = 0;
	}
}
//----------------------------------------------------------------------------
#if defined(WIN32) && !defined(PX2_USE_PTHREAD)
DWORD WINAPI RunnableEntry(LPVOID t)
#else
static void* RunnableEntry(void* t)
#endif
{
	Thread *thread = (Thread*)(t);

	if (thread->mRunable)
	{
		thread->mRunable->Run();
		thread->mRunable = 0;
	}
	else
	{
		thread->mCallback(thread->mUserData);
	}

	return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
#if (defined(_WIN32) || defined(WIN32)) && !defined(PX2_USE_PTHREAD)
//----------------------------------------------------------------------------
void Thread::SetPriority(Priority prio)
{
	if (mPriority == prio)
		return;

	mPriority = prio;

	if (mThread)
	{
		if (0 == SetThreadPriority(mThread, mPriority))
		{
			assertion(false, "cannot set thread priority");
		}
	}
}
//----------------------------------------------------------------------------
void Thread::SetStackSize(int size)
{
	mStackSize = size;
}
//----------------------------------------------------------------------------
void Thread::Start (Runnable& runable)
{
	if (IsRunning())
		return;

	mRunable = &runable;

	mThread = CreateThread(NULL, mStackSize, RunnableEntry, (LPVOID)this, 0, 
		(LPDWORD)&mThreadID);
}
//----------------------------------------------------------------------------
void Thread::Start(Callback callback, void *data)
{
	if (IsRunning())
		return;

	mCallback = callback;
	mUserData = data;

	mThread = CreateThread(NULL, mStackSize, RunnableEntry, (LPVOID)this, 0, 
		(LPDWORD)&mThreadID);
}
//----------------------------------------------------------------------------
void Thread::Join ()
{
	if (!mThread) 
		return;

	switch (WaitForSingleObject(mThread, INFINITE))
	{
	case WAIT_OBJECT_0:
		ThreadCleanUp();
		break;
	default:
		break;
	}
}
//----------------------------------------------------------------------------
bool Thread::Join(long milliseconds)
{
	if (!mThread) 
		return true;

	switch (WaitForSingleObject(mThread, milliseconds + 1))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		ThreadCleanUp();
		return true;
	default:
		break;
	}

	return true;
}
//----------------------------------------------------------------------------
bool Thread::IsRunning() const
{
	if (mThread)
	{
		DWORD ec = 0;
		return GetExitCodeThread(mThread, &ec) && ec == STILL_ACTIVE;
	}

	return false;
}
//----------------------------------------------------------------------------
void Thread::DoYield()
{
	Sleep(0);
}
//----------------------------------------------------------------------------
void Thread::ThreadCleanUp ()
{
	if (!mThread) 
		return;

	if (CloseHandle(mThread))
		mThread = 0;
}
//----------------------------------------------------------------------------
#elif defined(PX2_USE_PTHREAD)
//----------------------------------------------------------------------------
void Thread::SetPriority(Priority prio)
{
	if (mPriority == prio)
		return;

	mPriority = prio;

	if (IsRunning())
	{
	}
}
//----------------------------------------------------------------------------
void Thread::SetStackSize(int size)
{
	mStackSize = size;
}
//----------------------------------------------------------------------------
void Thread::Start (Runnable& target)
{
	if (IsRunning())
		return;

	pthread_attr_t attributes;
	pthread_attr_init(&attributes);

	if (mStackSize != 0)
	{
		if (0 != pthread_attr_setstacksize(&attributes,mStackSize))
		{
			pthread_attr_destroy(&attributes);
			assertion(false, "can not set thread stack size.\n");
			return;
		}
	}

	mRunable = &target;

	if (pthread_create(&mThread, &attributes, RunnableEntry, this))
	{
		mRunable = 0;
		pthread_attr_destroy(&attributes);
		assertion(false, "cannot start thread");
		return;
	}
	pthread_attr_destroy(&attributes);
}
//----------------------------------------------------------------------------
void Thread::Start (Callback callback, void *data)
{
	if (IsRunning())
		return;

	pthread_attr_t attributes;
	pthread_attr_init(&attributes);

	if (mStackSize != 0)
	{
		if (0 != pthread_attr_setstacksize(&attributes, mStackSize))
		{
			assertion(false, "can not set thread stack size\n");
			return;
		}
	}

	mCallback = callback;
	mUserData = data;

	if (pthread_create(&mThread, &attributes, RunnableEntry, this))
	{
		mCallback = 0;
		mUserData = 0;
		assertion(false, "cannot start thread.cpp\n");
	}
}
//----------------------------------------------------------------------------
void Thread::Join ()
{
	void* ret;
	pthread_join(mThread, &ret);
}
//----------------------------------------------------------------------------
bool Thread::Join(long milliseconds)
{
	void* ret;
	pthread_join(mThread, &ret);
	
	return true;
}
//----------------------------------------------------------------------------
bool Thread::IsRunning() const
{
	return (mRunable!= 0) || (mCallback!=0);
}
//----------------------------------------------------------------------------
void Thread::DoYield()
{
	sched_yield();
};
//----------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------
