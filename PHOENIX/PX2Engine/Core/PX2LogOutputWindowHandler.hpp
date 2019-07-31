// PX2LogOutputWindowHandler.hpp

#ifndef PX2LOGOUTPUTWINDOWHANDLER_HPP
#define PX2LOGOUTPUTWINDOWHANDLER_HPP

#include "PX2Log.hpp"

namespace PX2
{

	/// 输出到调试窗口句柄
	class PX2_ENGINE_ITEM OutputWindowHandler : public LogHandler
	{
	public:
		OutputWindowHandler (unsigned int levels);
		virtual ~OutputWindowHandler ();

		virtual void Handle (const LogBuffer *logBuffer,
			const char *timeStamp);
	};

}

#endif