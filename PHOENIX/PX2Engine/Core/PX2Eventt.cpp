// PX2Signal.cpp

#include "PX2Eventt.hpp"
#include "PX2Assert.hpp"
using namespace PX2;

#if (defined(_WIN32) || defined(WIN32))
#include <Windows.h>
//----------------------------------------------------------------------------
Eventt::Eventt (bool autoReset)
	:
mIsAutoReset(autoReset),
mEvent(0)
{
	mEvent = CreateEvent(NULL, autoReset? FALSE:TRUE, FALSE, NULL);
	assertion(0!=mEvent, "CreateEvent failed.\n");
}
//----------------------------------------------------------------------------
Eventt::~Eventt ()
{
	if (mEvent)
		CloseHandle(mEvent);
}
//----------------------------------------------------------------------------
void Eventt::Set ()
{
	if (!SetEvent(mEvent))
	{
		assertion(false, "SetEvent failed.\n");
	}
}
//----------------------------------------------------------------------------
void Eventt::Wait ()
{
	switch (WaitForSingleObject(mEvent, INFINITE))
	{
	case WAIT_OBJECT_0:
		return;
	default:
		assertion(false, "wait for event failed");
		break;
	}
}
//----------------------------------------------------------------------------
bool Eventt::Wait (long milliseconds)
{
	switch (WaitForSingleObject(mEvent, milliseconds + 1))
	{
	case WAIT_TIMEOUT:
		return false;
	case WAIT_OBJECT_0:
		return true;
	default:
		assertion(false, "wait for event failed");
		break;
	}

	return false;
}
//----------------------------------------------------------------------------
void Eventt::Reset ()
{
	if (!ResetEvent(mEvent))
	{
		assertion(false, "ResetEvent failed.\n");
	}
}
//----------------------------------------------------------------------------
#else
#include <sys/time.h>
//----------------------------------------------------------------------------
Eventt::Eventt (bool autoReset)
	:
mIsAutoReset(autoReset),
mState(false)
{
	if (pthread_mutex_init(&mMutex, 0))
		assertion(false, "cannot create event (mutex)");

	if (pthread_cond_init(&mCond, 0))
		assertion(false, "cannot create event (condition)");
}
//----------------------------------------------------------------------------
Eventt::~Eventt ()
{
	pthread_cond_destroy(&mCond);
	pthread_mutex_destroy(&mMutex);
}
//----------------------------------------------------------------------------
void Eventt::Set ()
{
	if (pthread_mutex_lock(&mMutex))
	{
		assertion(false, "cannot signal event (lock)");
	}

	mState = true;

	if (pthread_cond_broadcast(&mCond))
	{
		pthread_mutex_unlock(&mMutex);
		assertion(false, "cannot signal event");
	}

	pthread_mutex_unlock(&mMutex);
}
//----------------------------------------------------------------------------
void Eventt::Wait ()
{
	if (pthread_mutex_lock(&mMutex))
	{
		assertion(false, "wait for event failed (lock)");
	}

	while (!mState) 
	{
		if (pthread_cond_wait(&mCond, &mMutex))
		{
			pthread_mutex_unlock(&mMutex);
			assertion(false, "wait for event failed");
		}
	}
	if (mIsAutoReset)
		mState = false;

	pthread_mutex_unlock(&mMutex);
}
//----------------------------------------------------------------------------
bool Eventt::Wait (long milliseconds)
{
	int rc = 0;
	struct timespec abstime;

	struct timeval tv;
	gettimeofday(&tv, 0);
	abstime.tv_sec  = tv.tv_sec + milliseconds / 1000;
	abstime.tv_nsec = tv.tv_usec*1000 + (milliseconds % 1000)*1000000;
	if (abstime.tv_nsec >= 1000000000)
	{
		abstime.tv_nsec -= 1000000000;
		abstime.tv_sec++;
	}

	if (pthread_mutex_lock(&mMutex) != 0)
	{
		assertion(false, "wait for event failed (lock)");
	}

	while (!mState) 
	{
		if ((rc = pthread_cond_timedwait(&mCond, &mMutex, &abstime)))
		{
			pthread_mutex_unlock(&mMutex);

			assertion(false, "cannot wait for event");
		}
	}
	if (rc == 0 && mIsAutoReset) 
		mState = false;

	pthread_mutex_unlock(&mMutex);

	return 0==rc;
}
//----------------------------------------------------------------------------
void Eventt::Reset ()
{
	if (pthread_mutex_lock(&mMutex))
	{
		assertion(false, "cannot reset event.");
	}

	mState = false;
	pthread_mutex_unlock(&mMutex);
}
//----------------------------------------------------------------------------
#endif