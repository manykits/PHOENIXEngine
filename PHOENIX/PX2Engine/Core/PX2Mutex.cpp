// PX2Mutex.cpp

#include "PX2Mutex.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

#if (defined(_WIN32) || defined(WIN32)) && !defined(PX2_USE_PTHREAD)

// system mutex非常慢，Critical sections相比较而言快很多。
#define PX2_USE_CRITICAL_SECTIONS

//----------------------------------------------------------------------------
#ifdef PX2_USING_VC80
// See the comments at
// http://msdn.microsoft.com/en-us/library/ms683476(VS.85).aspx
// about why these #defines expose InitializeCriticalSectionAndSpinCount.
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500
#endif
#include <windows.h>
//----------------------------------------------------------------------------
Mutex::Mutex (bool isRecursive)
	:
mIsRecursive(isRecursive)
{
#ifdef PX2_USE_CRITICAL_SECTIONS
	// The MSVS2010 documentation mentioned that the heap manager spin lock
	// was set to approximately 4000.  Let's see how 4096 does.
	mMutex = new CRITICAL_SECTION;
	BOOL success = InitializeCriticalSectionAndSpinCount(
		(CRITICAL_SECTION*)mMutex, 4096);
	assertion(success == TRUE, "Failed to initialize critical section.\n");
	PX2_UNUSED(success);
#else
	mMutex = CreateMutex(NULL, FALSE, NULL);
	assertion(mMutex != 0, "Failed to create mutex\n");
#endif
}
//----------------------------------------------------------------------------
Mutex::~Mutex ()
{
#ifdef PX2_USE_CRITICAL_SECTIONS
	DeleteCriticalSection((CRITICAL_SECTION*)mMutex);
	delete mMutex;
#else
	BOOL closed = CloseHandle((HANDLE)mMutex);
	assertion(closed == TRUE, "Failed to destroy mutex\n");
	PX2_UNUSED(closed);
#endif
}
//----------------------------------------------------------------------------
void Mutex::Enter ()
{
#ifdef PX2_USE_CRITICAL_SECTIONS
	EnterCriticalSection((CRITICAL_SECTION*)mMutex);
#else
	DWORD result = WaitForSingleObject((HANDLE)mMutex, INFINITE);
	PX2_UNUSED(result);
	// result:
	//   WAIT_ABANDONED (0x00000080)
	//   WAIT_OBJECT_0  (0x00000000), signaled
	//   WAIT_TIMEOUT   (0x00000102), [not possible with INFINITE]
	//   WAIT_FAILED    (0xFFFFFFFF), not signaled
#endif
}
//----------------------------------------------------------------------------
void Mutex::Leave ()
{
#ifdef PX2_USE_CRITICAL_SECTIONS
	LeaveCriticalSection((CRITICAL_SECTION*)mMutex);
#else
	BOOL released = ReleaseMutex((HANDLE)mMutex);
	PX2_UNUSED(released);
#endif
}
//----------------------------------------------------------------------------
#elif defined(PX2_USE_PTHREAD)
//----------------------------------------------------------------------------
Mutex::Mutex (bool isRecursive)
	:
mIsRecursive(isRecursive)
{
	pthread_mutexattr_init(&mMutex.Attribute);
	
	if (mIsRecursive)
	{
		pthread_mutexattr_settype(&mMutex.Attribute, PTHREAD_MUTEX_RECURSIVE);
	}

	pthread_mutex_init(&mMutex.Mutex, &mMutex.Attribute);
}
//----------------------------------------------------------------------------
Mutex::~Mutex ()
{
	pthread_mutex_destroy(&mMutex.Mutex);
	pthread_mutexattr_destroy(&mMutex.Attribute);
}
//----------------------------------------------------------------------------
void Mutex::Enter ()
{
	int result = pthread_mutex_lock(&mMutex.Mutex);
	assertion(0==result, "pthread_mutex_lock failed.\n");
	PX2_UNUSED(result);
}
//----------------------------------------------------------------------------
void Mutex::Leave ()
{
	int result = pthread_mutex_unlock(&mMutex.Mutex);
	assertion(0==result, "pthread_mutex_unlock failed.\n");
	PX2_UNUSED(result);
}
//----------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------
