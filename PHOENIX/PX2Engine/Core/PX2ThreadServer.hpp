// PX2ThreadServer.hpp

#ifndef PX2THREADSERVER_HPP
#define PX2THREADSERVER_HPP

#include "PX2CorePre.hpp"
#include "PX2Runnable.hpp"
#include "PX2Thread.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ThreadServer : public Runnable
	{
	public:
		ThreadServer ();
		virtual ~ThreadServer ();

		bool StartThreads (int threadsNum);
		void WaitThreadsEnd ();

	private:
		std::vector<Thread*> mThreads;
	};

}

#endif