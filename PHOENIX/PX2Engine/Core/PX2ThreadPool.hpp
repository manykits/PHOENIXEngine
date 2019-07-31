// PX2ThreadPool.hpp

#ifndef PX2THREADPOOL_HPP
#define PX2THREADPOOL_HPP

#include "PX2Thread.hpp"
#include "PX2Mutex.hpp"
#include "PX2Runnable.hpp"
#include "PX2Thread.hpp"
#include "PX2Eventt.hpp"
#include "PX2Singleton_NeedNew.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM PooledThread : public Runnable
	{
	public:
		PooledThread(const std::string& name, int stackSize = 0);
		~PooledThread();

		void Start();
		void Start(Thread::Priority priority, Runnable& target);
		void Start(Thread::Priority priority, Runnable& target, const std::string& name);
		bool IsIdle();
		int IdleTime();
		void Join();
		void Activate();
		void Release();
		void Run();

	private:
		volatile bool mIsIdle;
		volatile std::time_t mIdleTime;
		Runnable *mTarget;
		std::string mName;
		Thread mThread;
		Eventt mTargetReady;
		Eventt mTargetCompleted;
		Eventt mStarted;
		Mutex *mMutex;
	};
	typedef Pointer0<PooledThread> PooledThreadPtr;
	
	class ThreadPool
	{
	public:
		ThreadPool(int minCapacity = 2,	int maxCapacity = 16,
			int idleTime = 60, int stackSize = 0);
		ThreadPool(const std::string& name, int minCapacity = 2,
			int maxCapacity = 16, int idleTime = 60,
			int stackSize = 0);
		~ThreadPool();

		Mutex *GetMutex();

		void AddCapacity(int n);
		int Capacity() const;
		void SetStackSize(int stackSize);
		int GetStackSize() const;
		int Used() const;
		int Allocated() const;
		int Available() const;

		void Start(Runnable& target);
		void Start(Runnable& target, const std::string& name);
		void StartWithPriority(Thread::Priority priority, Runnable& target);
		void StartWithPriority(Thread::Priority priority, Runnable& target, const std::string& name);
		void StopAll();
		void JoinAll();
		void Collect();
		const std::string& name() const;

	protected:
		PooledThread* GetThread();
		PooledThread* _CreateThread();

		void Housekeep();

	private:
		ThreadPool(const ThreadPool& pool);
		ThreadPool& operator = (const ThreadPool& pool);

		typedef std::vector<PooledThreadPtr > ThreadVec;

		std::string mName;
		int mMinCapacity;
		int mMaxCapacity;
		int mIdleTime;
		int mSerial;
		int mAge;
		int mStackSize;
		ThreadVec mThreads;
		mutable Mutex *mMutex;
	};
	typedef Pointer0<ThreadPool> ThreadPoolPtr;

}

#endif