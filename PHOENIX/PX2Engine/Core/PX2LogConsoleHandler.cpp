// PX2LogConsoleHandler.cpp

#include "PX2LogConsoleHandler.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ConcoseLogHandler::ConcoseLogHandler (unsigned int levels)
	:
LogHandler(levels)
{
}
//----------------------------------------------------------------------------
ConcoseLogHandler::~ConcoseLogHandler ()
{
}
//----------------------------------------------------------------------------
void ConcoseLogHandler::Handle (const LogBuffer *logBuffer,
	const char *timeStamp)
{
	printf("%s", timeStamp);

	if (logBuffer->IsWChar)
	{
		wprintf(L"%s", logBuffer->Buffer);
	}
	else
	{
		printf("%s", (char*)logBuffer->Buffer);
	}

	fflush(stdout);
}
//----------------------------------------------------------------------------