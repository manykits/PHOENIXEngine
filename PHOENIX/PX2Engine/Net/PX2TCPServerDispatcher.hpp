// PX2TCPServerDispatcher.hpp

#ifndef PX2TCPSERVERDISPATCHER_HPP
#define PX2TCPSERVERDISPATCHER_HPP

#include "PX2NetPre.hpp"
#include "PX2Runnable.hpp"
#include "PX2ThreadPool.hpp"
#include "PX2TCPServerParams.hpp"
#include "PX2TCPServerConnectionFactory.hpp"
#include "PX2Mutex.hpp"
#include "PX2NotificationQueue.hpp"

namespace PX2
{
	
	class PX2_ENGINE_ITEM TCPServerDispatcher : public Runnable
	{
	public:
		TCPServerDispatcher();
		TCPServerDispatcher(TCPServerConnectionFactory *factory, 
			ThreadPool* threadPool, TCPServerParams *params);
		virtual ~TCPServerDispatcher();

		virtual void Run();

		void Enqueue(const StreamSocket& socket);

		void Stop();

		int CurrentThreads() const;
		int GetNumMaxThreads() const;
		ThreadPool *GetThreadPool();

		int GetNumTotalConnections() const;
		int GetNumCurrentConnections() const;
		int GetNumMaxConcurrentConnections() const;
		int GetNumQueuedConnections() const;
		int GetNumRefusedConnections() const;

		const TCPServerParams *params() const;

	protected:
		void BeginConnection();
		void EndConnection();

	private:
		TCPServerDispatcher(const TCPServerDispatcher&);
		TCPServerDispatcher& operator = (const TCPServerDispatcher&);

		int _rc;
		TCPServerParamsPtr  mParams;
		int mCurrentThreads;
		int mNumTotalConnections;
		int mNumCurrentConnections;
		int mNumMaxConcurrentConnections;
		int mNumRefusedConnections;
		bool mIsStopped;
		NotificationQueue mQueue;
		TCPServerConnectionFactoryPtr mConnectionFactory;
		ThreadPoolPtr mThreadPool;
		mutable Mutex mMutex;
	};
	typedef Pointer0<TCPServerDispatcher> TCPServerDispatcherPtr;

}

#endif