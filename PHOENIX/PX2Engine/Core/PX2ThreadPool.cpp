// PX2ThreadPool.cpp

#include "PX2ThreadPool.hpp"
#include "PX2Eventt.hpp"
#include "PX2Assert.hpp"
#include "PX2ScopedCS.hpp"
#include "PX2Memory.hpp"
#include "PX2ThreadLocal.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PooledThread::PooledThread(const std::string& name, int stackSize)
	: 
mIsIdle(true), 
mIdleTime(0), 
mTarget(0), 
mName(name), 
mThread(name),
mTargetCompleted(false)
{
	assertion(stackSize>=0, "stackSize must larger than 0.\n");
	mMutex = new0 Mutex();
	mThread.SetStackSize(stackSize);
	mIdleTime = std::time(0);
}
//----------------------------------------------------------------------------
PooledThread::~PooledThread()
{
	delete0(mMutex);
}
//----------------------------------------------------------------------------
void PooledThread::Start()
{
	mThread.Start(*this);
	mStarted.Wait();
}
//----------------------------------------------------------------------------
void PooledThread::Start(Thread::Priority priority, Runnable& target)
{
	ScopedCS cs(mMutex);

	assertion(mTarget == 0, "mTarget must not inited.\n");

	mTarget = &target;
	mThread.SetPriority(priority);
	mTargetReady.Set();
}
//----------------------------------------------------------------------------
void PooledThread::Start(Thread::Priority priority, Runnable& target,
	const std::string& name)
{
	ScopedCS cs(mMutex);

	std::string fullName(name);
	if (name.empty())
	{
		fullName = mName;
	}
	else
	{
		fullName.append(" (");
		fullName.append(mName);
		fullName.append(")");
	}
	mThread.SetName(fullName);
	mThread.SetPriority(priority);

	assertion(mTarget==0, "mTarget must be 0.\n");

	mTarget = &target;
	mTargetReady.Set();
}
//----------------------------------------------------------------------------
inline bool PooledThread::IsIdle()
{
	return mIsIdle;
}
//----------------------------------------------------------------------------
int PooledThread::IdleTime()
{
	ScopedCS cs(mMutex);

	return (int) (time(0) - mIdleTime);
}
//----------------------------------------------------------------------------
void PooledThread::Join()
{
	mMutex->Enter();
	Runnable* target = mTarget;
	mMutex->Leave();
	if (target)
		mTargetCompleted.Wait();
}
//----------------------------------------------------------------------------
void PooledThread::Activate()
{
	ScopedCS cs(mMutex);

	assertion(mIsIdle, "mIsIdle must not be 0.\n");
	mIsIdle = false;
	mTargetCompleted.Reset();
}
//----------------------------------------------------------------------------
void PooledThread::Release()
{
	const long JOIN_TIMEOUT = 10000;

	mMutex->Enter();
	mTarget = 0;
	mMutex->Leave();

	if (mThread.IsRunning())
		mTargetReady.Set();

	mThread.Join(JOIN_TIMEOUT);
}
//----------------------------------------------------------------------------
void PooledThread::Run()
{
	mStarted.Set();
	for (;;)
	{
		mTargetReady.Wait();
		mMutex->Enter();
		if (mTarget) // a NULL target means kill yourself
		{
			mMutex->Leave();

			mTarget->Run();

			ScopedCS cs(mMutex);
			mTarget  = 0;

			mIdleTime = time(NULL);

			mIsIdle     = true;
			mTargetCompleted.Set();
			ThreadLocalStorage::Clear();
			mThread.SetName(mName);
			mThread.SetPriority(Thread::PRIO_NORMAL);
		}
		else
		{
			mMutex->Leave();
			break;
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ThreadPool
//----------------------------------------------------------------------------
ThreadPool::ThreadPool(int minCapacity,	int maxCapacity, int idleTime,
	int stackSize)
	: 
mMinCapacity(minCapacity), 
mMaxCapacity(maxCapacity), 
mIdleTime(idleTime),
mSerial(0),
mAge(0),
mStackSize(stackSize)
{
	assertion(minCapacity >= 1 && maxCapacity >= minCapacity && idleTime > 0,
		"");

	for (int i = 0; i < mMinCapacity; i++)
	{
		PooledThread* thread = _CreateThread();
		mThreads.push_back(thread);
		thread->Start();
	}

	mMutex = new0 Mutex();
}
//----------------------------------------------------------------------------
ThreadPool::ThreadPool(const std::string& name,	int minCapacity,
	int maxCapacity, int idleTime, int stackSize)
	:
mName(name),
	mMinCapacity(minCapacity), 
	mMaxCapacity(maxCapacity), 
	mIdleTime(idleTime),
	mSerial(0),
	mAge(0),
	mStackSize(stackSize)
{
	assertion(minCapacity >= 1 && maxCapacity >= minCapacity && idleTime > 0,
		"");

	for (int i = 0; i < mMinCapacity; i++)
	{
		PooledThread* thread = _CreateThread();
		mThreads.push_back(thread);
		thread->Start();
	}

	mMutex = new0 Mutex();
}
//----------------------------------------------------------------------------
ThreadPool::~ThreadPool()
{
	StopAll();

	if (mMutex)
	{
		delete0(mMutex);
		mMutex = 0;
	}

	mThreads.clear();
}
//----------------------------------------------------------------------------
Mutex *ThreadPool::GetMutex()
{
	return mMutex;
}
//----------------------------------------------------------------------------
void ThreadPool::AddCapacity(int n)
{
	ScopedCS cs(mMutex);

	assertion(mMaxCapacity + n >= mMinCapacity, "");
	mMaxCapacity += n;
	Housekeep();
}
//----------------------------------------------------------------------------
int ThreadPool::Capacity() const
{
	ScopedCS cs(mMutex);

	return mMaxCapacity;
}
//----------------------------------------------------------------------------
int ThreadPool::Available() const
{
	ScopedCS cs(mMutex);

	int count = 0;
	for (ThreadVec::const_iterator it = mThreads.begin(); it != mThreads.end();
		++it)
	{
		if ((*it)->IsIdle())
			++count;
	}
	return (int) (count + mMaxCapacity - mThreads.size());
}
//----------------------------------------------------------------------------
int ThreadPool::Used() const
{
	ScopedCS cs(mMutex);

	int count = 0;
	for (ThreadVec::const_iterator it = mThreads.begin(); it != mThreads.end();
		++it)
	{
		if (!(*it)->IsIdle())
			++count;
	}
	return count;
}
//----------------------------------------------------------------------------
int ThreadPool::Allocated() const
{
	ScopedCS cs(mMutex);

	return int(mThreads.size());
}
//----------------------------------------------------------------------------
void ThreadPool::Start(Runnable& target)
{
	GetThread()->Start(Thread::PRIO_NORMAL, target);
}
//----------------------------------------------------------------------------
void ThreadPool::Start(Runnable& target, const std::string& name)
{
	GetThread()->Start(Thread::PRIO_NORMAL, target, name);
}
//----------------------------------------------------------------------------
void ThreadPool::StartWithPriority(Thread::Priority priority, Runnable& target)
{
	GetThread()->Start(priority, target);
}
//----------------------------------------------------------------------------
void ThreadPool::StartWithPriority(Thread::Priority priority, Runnable& target, const std::string& name)
{
	GetThread()->Start(priority, target, name);
}
//----------------------------------------------------------------------------
void ThreadPool::StopAll()
{
	ScopedCS cs(mMutex);

	for (ThreadVec::iterator it = mThreads.begin(); it != mThreads.end(); ++it)
	{
		(*it)->Release();
	}
	mThreads.clear();
}
//----------------------------------------------------------------------------
void ThreadPool::JoinAll()
{
	ScopedCS cs(mMutex);

	for (ThreadVec::iterator it = mThreads.begin(); it != mThreads.end(); ++it)
	{
		(*it)->Join();
	}
	Housekeep();
}
//----------------------------------------------------------------------------
void ThreadPool::Collect()
{
	ScopedCS cs(mMutex);
	Housekeep();
}
//----------------------------------------------------------------------------
void ThreadPool::Housekeep()
{
	mAge = 0;
	if ((int)mThreads.size() <= mMinCapacity)
		return;

	ThreadVec idleThreads;
	ThreadVec expiredThreads;
	ThreadVec activeThreads;
	idleThreads.reserve(mThreads.size());
	activeThreads.reserve(mThreads.size());

	for (ThreadVec::iterator it = mThreads.begin(); it != mThreads.end(); ++it)
	{
		if ((*it)->IsIdle())
		{
			if ((*it)->IdleTime() < mIdleTime)
				idleThreads.push_back(*it);
			else 
				expiredThreads.push_back(*it);	
		}
		else activeThreads.push_back(*it);
	}
	int n = (int) activeThreads.size();
	int limit = (int) idleThreads.size() + n;
	if (limit < mMinCapacity) 
		limit = mMinCapacity;
	idleThreads.insert(idleThreads.end(), expiredThreads.begin(), expiredThreads.end());
	mThreads.clear();
	for (ThreadVec::iterator it = idleThreads.begin(); it != idleThreads.end(); ++it)
	{
		if (n < limit)
		{
			mThreads.push_back(*it);
			++n;
		}
		else
			(*it)->Release();
	}
	mThreads.insert(mThreads.end(), activeThreads.begin(), activeThreads.end());
}
//----------------------------------------------------------------------------
PooledThread* ThreadPool::GetThread()
{
	ScopedCS cs(mMutex);

	if (++mAge == 32)
		Housekeep();

	PooledThread* thread = 0;
	for (ThreadVec::iterator it = mThreads.begin(); !thread && it != mThreads.end(); ++it)
	{
		if ((*it)->IsIdle())
			thread = *it;
	}
	if (!thread)
	{
		if ((int)mThreads.size() < mMaxCapacity)
		{
			thread = _CreateThread();
			thread->Start();
			mThreads.push_back(thread);
		}
		else
		{
			assertion(false, "No thread available.\n");
		}
	}
	thread->Activate();
	return thread;
}
//----------------------------------------------------------------------------
PooledThread* ThreadPool::_CreateThread()
{
	std::ostringstream name;
	name << mName << "[#" << ++mSerial << "]";
	return new0 PooledThread(name.str(), mStackSize);
}
//----------------------------------------------------------------------------