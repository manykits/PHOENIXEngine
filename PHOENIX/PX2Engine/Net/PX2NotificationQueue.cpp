// PX2NotificationQueue.cpp

#include "PX2NotificationQueue.hpp"
#include "PX2ScopedCS.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
NotificationQueue::NotificationQueue()
{
}
//----------------------------------------------------------------------------
NotificationQueue::~NotificationQueue()
{
	try
	{
		Clear();
	}
	catch (...)
	{
	}
}
//----------------------------------------------------------------------------
void NotificationQueue::EnqueueNotification(Notification *notification)
{
	ScopedCS cs(&mMutex);

	if (mWaitQueue.empty())
	{
		mNFQueue.push_back(notification);
	}
	else
	{
		WaitInfo* waitInfo = mWaitQueue.front();
		mWaitQueue.pop_front();
		waitInfo->Notif = notification;
		waitInfo->EventAvailable.Set();
	}
}
//----------------------------------------------------------------------------
void NotificationQueue::EnqueueUrgentNotification(Notification *notification)
{
	ScopedCS cs(&mMutex);

	if (mWaitQueue.empty())
	{
		mNFQueue.push_front(notification);
	}
	else
	{
		WaitInfo* waitInfo = mWaitQueue.front();
		mWaitQueue.pop_front();
		waitInfo->Notif = notification;
		waitInfo->EventAvailable.Set();
	}
}
//----------------------------------------------------------------------------
NotificationPtr NotificationQueue::DequeueNotification()
{
	ScopedCS cs(&mMutex);

	return DequeueOne();
}
//----------------------------------------------------------------------------
NotificationPtr NotificationQueue::WaitDequeueNotification()
{
	NotificationPtr notif;

	WaitInfo* waitInfo = 0;
	{
		ScopedCS cs(&mMutex);

		notif = DequeueOne();
		if (notif)
			return notif;

		waitInfo = new0 WaitInfo;
		mWaitQueue.push_back(waitInfo);
	}

	waitInfo->EventAvailable.Wait();
	notif = waitInfo->Notif;
	delete0(waitInfo);

	return notif;
}
//----------------------------------------------------------------------------
NotificationPtr NotificationQueue::WaitDequeueNotification(long milliseconds)
{
	NotificationPtr notif;
	WaitInfo* waitInfo = 0;
	{
		ScopedCS cs(&mMutex);
		notif = DequeueOne();
		if (notif)
			return notif;

		waitInfo = new0 WaitInfo;
		mWaitQueue.push_back(waitInfo);
	}
	if (waitInfo->EventAvailable.Wait(milliseconds))
	{
		notif = waitInfo->Notif;
	}
	else
	{
		ScopedCS cs(&mMutex);

		notif = waitInfo->Notif;
		for (WaitQueue::iterator it = mWaitQueue.begin(); it != mWaitQueue.end(); ++it)
		{
			if (*it == waitInfo)
			{
				mWaitQueue.erase(it);
				break;
			}
		}
	}
	delete0(waitInfo);

	return notif;
}
//----------------------------------------------------------------------------
void NotificationQueue::WakeUpAll()
{
	ScopedCS cs(&mMutex);
	for (WaitQueue::iterator it = mWaitQueue.begin(); it != mWaitQueue.end(); ++it)
	{
		(*it)->EventAvailable.Set();
	}
	mWaitQueue.clear();
}
//----------------------------------------------------------------------------
bool NotificationQueue::IsEmpty() const
{
	ScopedCS cs(&mMutex);
	return mNFQueue.empty();
}
//----------------------------------------------------------------------------
int NotificationQueue::GetSize() const
{
	ScopedCS cs(&mMutex);

	return static_cast<int>(mNFQueue.size());
}
//----------------------------------------------------------------------------
void NotificationQueue::Clear()
{
	ScopedCS cs(&mMutex);
	mNFQueue.clear();
}
//----------------------------------------------------------------------------
bool NotificationQueue::HasIdleThreads() const
{
	ScopedCS cs(&mMutex);
	return !mWaitQueue.empty();
}
//----------------------------------------------------------------------------
NotificationPtr NotificationQueue::DequeueOne()
{
	NotificationPtr notif = 0;
	if (!mNFQueue.empty())
	{
		notif = mNFQueue.front();
		mNFQueue.pop_front();
	}
	return notif;
}
//----------------------------------------------------------------------------