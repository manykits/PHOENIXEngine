// PX2Thread.hpp

#ifndef PX2THREAD_HPP
#define PX2THREAD_HPP

#include "PX2CorePre.hpp"
#include "PX2ThreadType.hpp"
#include "PX2Runnable.hpp"
#include "PX2Mutex.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{
	class ThreadLocalStorage;

	class PX2_ENGINE_ITEM Thread
	{
	public:
		Thread ();
		Thread (const std::string& name);
		~Thread ();

		int GetID ();
		void SetName(const std::string& name);
		std::string GetName ();

		enum Priority
		{
			PRIO_LOWEST,
			PRIO_LOW,
			PRIO_NORMAL,
			PRIO_HIGH,
			PRIO_HIGHEST
		};
		void SetPriority(Priority prio);
		Priority GetPriority() const;

		void SetStackSize(int size);
		int GetStackSize() const;

		void Start (Runnable& runable);
		typedef void (*Callback)(void*);
		void Start (Callback callback, void *data = 0);

		void Join();
		bool Join(long milliseconds);

		bool IsRunning() const;
		ThreadType GetThread ();
		unsigned int GetThreadID ();

		static void DoYield();

public_internal:
		Runnable *mRunable;
		Callback mCallback;
		void* mUserData;

	protected:
		std::string MakeName();
		static int UniqueID();

		ThreadLocalStorage& TLS();
		void ClearTLS();

	private:
		void ThreadCleanUp ();

		int mID;
		std::string mName;
		Priority mPriority;
		ThreadType mThread;
		unsigned int mThreadID;
		unsigned int mStackSize;

		ThreadLocalStorage* mTLS;
		Mutex *mMutex;
		friend class ThreadLocalStorage;
	};

#include "PX2Thread.inl"
	typedef Pointer0<Thread> ThreadPtr;

}

#endif
