// PX2System.hpp

#ifndef PX2SYSTEM_HPP
#define PX2SYSTEM_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM System
	{
	public:
		static void SleepSeconds (float seconds);
		static int GetNumCPUs ();
		static int GetCurrentThreadID ();
		static std::string GetUniqueID ();

		static std::string CallGetOutPut(const std::string &cmdStr);
		static bool CallGetStatus(const std::string &cmdStr);
	};

}

#endif