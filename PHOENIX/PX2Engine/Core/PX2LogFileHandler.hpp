// PX2LogFileHandler.hpp

#ifndef PX2LOGFILEHANDLER_HPP
#define PX2LOGFILEHANDLER_HPP

#include "PX2Log.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM FileLogHandler : public LogHandler
	{
	public:
		FileLogHandler (unsigned int levels, const char *filename);
		virtual ~FileLogHandler ();

		virtual void Handle (const LogBuffer *logBuffer,
			const char *timeStamp);

	private:
		FILE *mFile;
	};

}

#endif