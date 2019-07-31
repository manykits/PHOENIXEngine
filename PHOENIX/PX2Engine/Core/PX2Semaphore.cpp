// PX2Semaphore.cpp

#include "PX2Semaphore.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
//----------------------------------------------------------------------------
Semaphore::Semaphore(int initNum):
mInitNum(initNum),
mMax(initNum)
{
	assertion(initNum>0, "n>0.\n");

	mHandle = CreateSemaphoreW(NULL, mInitNum, mMax, NULL);
	assertion(0!=mHandle, "mHandle should create failed.\n");
}
//----------------------------------------------------------------------------
Semaphore::Semaphore(int initNum, int max) :
mInitNum(initNum),
mMax(max),
mHandle(0)
{
	assertion(initNum >= 0 && max>0 && initNum <= max,
		"initNum>0 && max>0 && initNum<=max.\n");

	mHandle = CreateSemaphoreW(NULL, mInitNum, mMax, NULL);
	assertion(0!=mHandle, "mHandle should create failed.\n");
}
//----------------------------------------------------------------------------
Semaphore::~Semaphore ()
{
	CloseHandle(mHandle);
}
//----------------------------------------------------------------------------
void Semaphore::Set()
{
	if (!ReleaseSemaphore(mHandle, 1, NULL))
	{
		assertion(false, "wait for semaphore failed");
	}
}
////----------------------------------------------------------------------------
//void Semaphore::Set(int num)
//{
//	if (!ReleaseSemaphore(mHandle, num, NULL))
//	{
//		assertion(false, "wait for semaphore failed");
//	}
//}
//----------------------------------------------------------------------------
void Semaphore::Wait()
{
	switch (WaitForSingleObject(mHandle, INFINITE))
	{
	case WAIT_OBJECT_0:
		return;
	default:
		assertion(false, "wait for semaphore failed");
		break;
	}
}
//----------------------------------------------------------------------------
#elif defined(__LINUX__) || defined(__APPLE__) || defined(__ANDROID__)

#ifdef __APPLE__
#include <sys/time.h>
#else
#include <sys/timeb.h>
#endif
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

Semaphore::Semaphore (int initNum) :
mInitNum(initNum),
mMax(initNum)
{
	sem_init(&mSem, 0, initNum);
}
//----------------------------------------------------------------------------
Semaphore::Semaphore (int initNum, int max):
mInitNum(initNum),
mMax(max)
{
	assertion(initNum >= 0 && max > 0 && initNum <= max,
		"initNum >= 0 && max > 0 && initNum <= max");

	sem_init(&mSem, 0, initNum);
}
//----------------------------------------------------------------------------
Semaphore::~Semaphore ()
{
	sem_destroy(&mSem);
}
//----------------------------------------------------------------------------
void Semaphore::Set()
{
	sem_post(&mSem);
}
//----------------------------------------------------------------------------
void Semaphore::Wait()
{
	sem_wait(&mSem);
}
#endif
//----------------------------------------------------------------------------