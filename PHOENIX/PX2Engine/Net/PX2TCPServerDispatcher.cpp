// PX2TCPServerDispatcher.cpp

#include "PX2TCPServerDispatcher.hpp"
#include "PX2Mutex.hpp"
#include "PX2ScopedCS.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
class TCPConnectionNotification : public Notification
{
public:
	TCPConnectionNotification(const StreamSocket& socket) :
		mSocket(socket)
	{
	}

	~TCPConnectionNotification()
	{
	}

	const StreamSocket& GetSocket() const
	{
		return mSocket;
	}

private:
	StreamSocket mSocket;
};
//----------------------------------------------------------------------------
TCPServerDispatcher::TCPServerDispatcher(TCPServerConnectionFactory *factory,
	ThreadPool* threadPool, TCPServerParams *params) :
_rc(1),
mParams(params),
mCurrentThreads(0),
mNumTotalConnections(0),
mNumCurrentConnections(0),
mNumMaxConcurrentConnections(0),
mNumRefusedConnections(0),mIsStopped(false),
mConnectionFactory(factory),
mThreadPool(threadPool)
{
	if (!mParams)
		mParams = new0 TCPServerParams;

	if (mParams->GetNumMaxThreads() == 0)
		mParams->SetMaxThreads(threadPool->Capacity());
}
//----------------------------------------------------------------------------
TCPServerDispatcher::~TCPServerDispatcher()
{
	mParams = 0;
	mThreadPool = 0;
}
//----------------------------------------------------------------------------
void TCPServerDispatcher::Run()
{
	int idleTime = (int)mParams->GetThreadIdleTime().TotalMilliseconds();

	for (;;)
	{
		NotificationPtr notif = mQueue.WaitDequeueNotification(idleTime);
		if (notif)
		{
			TCPConnectionNotification* tcpNotif =
				(TCPConnectionNotification*)((Notification*)notif);
			if (tcpNotif)
			{
				TCPServerConnectionPtr connection = 
					mConnectionFactory->CreateConnection(tcpNotif->GetSocket());

				BeginConnection();

				connection->Start();

				EndConnection();
			}
		}

		ScopedCS lock(&mMutex);
		if (mIsStopped || (mCurrentThreads > 1 && mQueue.IsEmpty()))
		{
			--mCurrentThreads;
			break;
		}

		std::cout << "4" << std::endl;
	}
}
//----------------------------------------------------------------------------
namespace
{
	static const std::string threadName("TCPServerConnection");
}
//----------------------------------------------------------------------------
void TCPServerDispatcher::Enqueue(const StreamSocket& socket)
{
	ScopedCS lock(&mMutex);

	if (mQueue.GetSize() < mParams->GetMaxQueued())
	{
		mQueue.EnqueueNotification(new0 TCPConnectionNotification(socket));
		if (!mQueue.HasIdleThreads() && mCurrentThreads < mParams->GetNumMaxThreads())
		{
			mThreadPool->StartWithPriority(mParams->GetThreadPriority(), *this, threadName);
			++mCurrentThreads;
		}
	}
	else
	{
		++mNumRefusedConnections;
	}
}
//----------------------------------------------------------------------------
void TCPServerDispatcher::Stop()
{
	mIsStopped = true;
	mQueue.Clear();
	mQueue.WakeUpAll();
}
//----------------------------------------------------------------------------
int TCPServerDispatcher::CurrentThreads() const
{
	ScopedCS lock(&mMutex);

	return mCurrentThreads;
}
//----------------------------------------------------------------------------
int TCPServerDispatcher::GetNumMaxThreads() const
{
	ScopedCS lock(&mMutex);

	return mThreadPool->Capacity();
}
//----------------------------------------------------------------------------
ThreadPool *TCPServerDispatcher::GetThreadPool()
{
	return mThreadPool;
}
//----------------------------------------------------------------------------
int TCPServerDispatcher::GetNumTotalConnections() const
{
	ScopedCS lock(&mMutex);

	return mNumTotalConnections;
}
//----------------------------------------------------------------------------
int TCPServerDispatcher::GetNumCurrentConnections() const
{
	ScopedCS lock(&mMutex);

	return mNumCurrentConnections;
}
//----------------------------------------------------------------------------
int TCPServerDispatcher::GetNumMaxConcurrentConnections() const
{
	ScopedCS lock(&mMutex);

	return mNumMaxConcurrentConnections;
}
//----------------------------------------------------------------------------
int TCPServerDispatcher::GetNumQueuedConnections() const
{
	return mQueue.GetSize();
}
//----------------------------------------------------------------------------
int TCPServerDispatcher::GetNumRefusedConnections() const
{
	ScopedCS lock(&mMutex);

	return mNumRefusedConnections;
}
//----------------------------------------------------------------------------
void TCPServerDispatcher::BeginConnection()
{
	ScopedCS lock(&mMutex);

	++mNumTotalConnections;
	++mNumCurrentConnections;
	if (mNumCurrentConnections > mNumMaxConcurrentConnections)
		mNumMaxConcurrentConnections = mNumCurrentConnections;
}
//----------------------------------------------------------------------------
void TCPServerDispatcher::EndConnection()
{
	ScopedCS lock(&mMutex);

	--mNumCurrentConnections;
}
//----------------------------------------------------------------------------
const TCPServerParams *TCPServerDispatcher::params() const
{
	return mParams;
}
//----------------------------------------------------------------------------