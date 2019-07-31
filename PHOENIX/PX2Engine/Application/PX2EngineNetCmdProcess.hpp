// PX2EngineNetCmdProcess.hpp

#ifndef PX2ENGINENETCMDPROCESS_HPP
#define PX2ENGINENETCMDPROCESS_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class EngineNetCmdProcess
	{
	public:
		EngineNetCmdProcess();
		~EngineNetCmdProcess();

		static void OnCmd(const std::string &fromip, const std::string &cmdStr,
			const std::string &paramStr0, const std::string &paramStr1);
	};

}

#endif