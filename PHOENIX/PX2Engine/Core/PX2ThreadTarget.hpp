// PX2ThreadTarget.hpp

#ifndef PX2THREADTARGET_HPP
#define PX2THREADTARGET_HPP

#include "PX2Runnable.hpp"

namespace PX2
{

	/**
	*	class MyObject
	*	{
	*		static void DoSomething() {}
	*	};
	*	ThreadTarget ra(&MyObject::DoSomething);
	*	Thread thr;
	*	thr.start(ra);
	*	or
	*	void DoSomething() {}
	*	ThreadTarget ra(DoSomething);
	*	Thread thr;
	*	thr.start(ra);
	*/
	class PX2_ENGINE_ITEM ThreadTarget : public Runnable
	{
	public:
		typedef void (*Callback)();

		ThreadTarget (Callback callback);
		ThreadTarget (const ThreadTarget& te);
		~ThreadTarget ();

		ThreadTarget& operator = (const ThreadTarget &te);

		virtual void Run ();

	private:
		ThreadTarget();

		Callback mCallback;
	};

}

#endif