// PX2Timer.hpp

#ifndef PX2TIMER_HPP
#define PX2TIMER_HPP

#include "PX2CorePre.hpp"
#include "PX2Timestamp.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{
	
	class Object;

	class PX2_ENGINE_ITEM Timer
	{
	public:
		Timer ();
		virtual ~Timer ();

		// time_t 是日历时间
		virtual bool OnTimer (float appTime) = 0;
		virtual void OnRemove();

	public_internal:
		std::string Name;
		int mTimerID;
		float StartTime;
		double mInterval; // 秒
		void *Action;
		void *UserData;
	};
	typedef Pointer0<Timer> TimerPtr;

}

#endif