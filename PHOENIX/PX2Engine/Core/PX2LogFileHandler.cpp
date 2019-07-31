// PX2LogFileHandler.cpp

#include "PX2LogFileHandler.hpp"
using namespace PX2;

FileLogHandler::FileLogHandler (unsigned int levels, const char *filename)
:
LogHandler(levels)
{
	mFile = fopen(filename, "wt");
}
//----------------------------------------------------------------------------
FileLogHandler::~FileLogHandler ()
{
	if (mFile)
	{
		fclose(mFile);
		mFile = 0;
	}
}
//----------------------------------------------------------------------------
void FileLogHandler::Handle (const LogBuffer *logBuffer, const char *timeStamp)
{
	fprintf(mFile, "%s", timeStamp);

	if (logBuffer->IsWChar)
	{
		fwprintf(mFile, L"%s", logBuffer->Buffer);
	}
	else
	{
		fprintf(mFile, "%s", (char*)logBuffer->Buffer);
	}

	fflush(mFile);
}
//----------------------------------------------------------------------------