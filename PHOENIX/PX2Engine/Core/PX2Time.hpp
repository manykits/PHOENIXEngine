// PX2Time.hpp

#ifndef PX2TIME_H
#define PX2TIME_H

#include "PX2CorePre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Time
	{
	public:
		Time();
		~Time();

		static double GetTimeInMicroseconds();
		static double GetTimeInSeconds();
		static void ResetTime();

		static double FrameElapsedSeconds;
		static double FrameRunnedSeconds;
	};

}

#endif
