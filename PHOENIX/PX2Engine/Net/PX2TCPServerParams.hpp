// PX2TCPServerParams.hpp

#ifndef PX2TCPSERVERPARAMS_HPP
#define PX2TCPSERVERPARAMS_HPP

#include "PX2NetPre.hpp"
#include "PX2Timespan.hpp"
#include "PX2Thread.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM TCPServerParams
	{
	public:
		TCPServerParams();
		virtual ~TCPServerParams();

		void SetThreadIdleTime(const Timespan& idleTime);
		const Timespan& GetThreadIdleTime() const;

		void SetMaxQueued(int count);
		int GetMaxQueued() const;

		void SetMaxThreads(int count);
		int GetNumMaxThreads() const;

		void SetThreadPriority(Thread::Priority prio);
		Thread::Priority GetThreadPriority() const;

	private:
		Timespan mThreadIdleTime;
		int mMaxThreads;
		int mMaxQueued;
		Thread::Priority mThreadPriority;
	};

#include "PX2TCPServerParams.inl"
	typedef Pointer0<TCPServerParams> TCPServerParamsPtr;

}

#endif