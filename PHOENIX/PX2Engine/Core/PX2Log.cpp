// PX2Log.cpp

#include "PX2Log.hpp"
#include "PX2Assert.hpp"
#include "PX2Memory.hpp"
#include "PX2LogFileHandler.hpp"
#include "PX2LogConsoleHandler.hpp"
#include "PX2LogOutputWindowHandler.hpp"
#include "PX2Time.hpp"
#include "PX2System.hpp"
#include "PX2LocalDateTime.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
LogBuffer::LogBuffer (int isWChar) :
Level(0),
IsWChar(isWChar)
{
	memset(Buffer, 0, sizeof(Buffer));
}
//----------------------------------------------------------------------------
LogBuffer::~LogBuffer ()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// LogHandler
//----------------------------------------------------------------------------
LogHandler::LogHandler (unsigned int levels)
:
mLevelFlags(levels)
{
}
//----------------------------------------------------------------------------
LogHandler::~LogHandler ()
{
}
//----------------------------------------------------------------------------
unsigned int LogHandler::GetLevels ()
{
	return mLevelFlags;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Logger
//----------------------------------------------------------------------------
Logger::Logger ()
	:
mDoQuit(false),
mIsBlock(true),
mIsLogFileInfo(true)
{
	memset(mTimeBuff, 0, sizeof(mTimeBuff));
}
//----------------------------------------------------------------------------
Logger::~Logger ()
{
	CloseLogger();
}
//----------------------------------------------------------------------------
void Logger::SetBlock (bool isBlock)
{
	mIsBlock = isBlock;
}
//----------------------------------------------------------------------------
bool Logger::IsBlock ()
{
	return mIsBlock;
}
//----------------------------------------------------------------------------
void Logger::SetLogFileInfo(bool logFileInfo)
{
	mIsLogFileInfo = logFileInfo;
}
//----------------------------------------------------------------------------
bool Logger::IsLogFileInfo() const
{
	return mIsLogFileInfo;
}
//----------------------------------------------------------------------------
bool Logger::AddHandler (LogHandler *handler)
{
	if (!handler)
		return false;

	for (int i=0; i<(int)mHandlers.size(); i++)
	{
		if (handler == mHandlers[i])
			return false;
	}

	mHandlers.push_back(handler);

	return true;
}
//----------------------------------------------------------------------------
void Logger::RemoveHandler (LogHandler *handler)
{
	std::vector<LogHandler*>::iterator it = mHandlers.begin();
	for (; it!=mHandlers.end(); it++)
	{
		if (*it == handler)
		{
			mHandlers.erase(it);
			return;
		}
	}
}
//----------------------------------------------------------------------------
bool Logger::AddFileHandler (const char *filename, unsigned int levels)
{
	FileLogHandler *handler = new0 FileLogHandler(levels, filename);
	return AddHandler(handler);
}
//----------------------------------------------------------------------------
bool Logger::AddConsoleHandler (unsigned int levels)
{
	ConcoseLogHandler *handler = new0 ConcoseLogHandler(levels);
	return AddHandler(handler);
}
//----------------------------------------------------------------------------
bool Logger::AddOutputWindowHandler (unsigned int levels)
{
	OutputWindowHandler *handler = new0 OutputWindowHandler(levels);
	return AddHandler(handler);
}
//----------------------------------------------------------------------------
void Logger::StartLogger ()
{
	if (!mIsBlock)
	{
		mDoQuit = false;
		StartThreads(1);
	}
}
//----------------------------------------------------------------------------
void Logger::CloseLogger ()
{
	if (!mIsBlock)
	{
		mDoQuit = true;
		WaitThreadsEnd();
	}

	for (int i=0; i<(int)mHandlers.size(); i++)
	{
		delete0(mHandlers[i]);
	}

	mHandlers.clear();
}
//----------------------------------------------------------------------------
void Logger::LogMessage (int level, long line, const char* fileName, 
	const char* format, ...)
{
	if (mHandlers.empty())
		return;

	if (mDoQuit)
		return;

	char infoBuf[LogBuffer::MaxLogLength+1] = {0};

	va_list argptr;
	va_start(argptr, format);
	vsprintf(infoBuf, format, argptr);
	va_end(argptr);

	std::string errorFlag;
	switch (level)
	{
	case LT_INFO:
		errorFlag = " [INFO] ";
		break;
	case LT_ERROR:
		errorFlag = " [ERROR] ";
		break;
	case LT_USER:
		errorFlag = " [USER] ";
		break;
	default:
		errorFlag = " [UNKNOWN] ";
		break;
	}

	std::string strFilename = _GetFileName(fileName);

	LogBuffer *logBuffer = new LogBuffer();
	logBuffer->Level = level;

	if (mIsLogFileInfo)
	{
		sprintf((char*)logBuffer->Buffer, "%s%s [%s:%d]\n", errorFlag.c_str(), infoBuf,
			strFilename.c_str(), (int)line);
	}
	else
	{
		sprintf((char*)logBuffer->Buffer, "%s%s\n", errorFlag.c_str(), infoBuf);
	}

	if (mIsBlock)
	{
		CalcTimeStamp();

		for (int i=0; i<(int)mHandlers.size(); ++i)
		{
			if (0 != (mHandlers[i]->GetLevels()&level))
				mHandlers[i]->Handle(logBuffer, mTimeBuff);
		}
		
		delete logBuffer;
	}
	else
	{
		mLogQueue.AddMsg(logBuffer);
	}
}
//----------------------------------------------------------------------------
void Logger::LogInfo (const std::string &tag, const std::string &info)
{
	PX2_LOG_INFO("%s:%s", tag.c_str(), info.c_str());
}
//----------------------------------------------------------------------------
void Logger::LogError (const std::string &tag, const std::string &info)
{
	PX2_LOG_ERROR("%s:%s", tag.c_str(), info.c_str());
}
//----------------------------------------------------------------------------
void Logger::LogUser (const std::string &tag, const std::string &info)
{
	PX2_LOG_USER("%s:%s", tag.c_str(), info.c_str());
}
//----------------------------------------------------------------------------
void Logger::Run ()
{
	if (mIsBlock)
		return;

	while (!mDoQuit)
	{
		LogBuffer *logBuffer = mLogQueue.PopMsg();
		
		if (logBuffer)
		{
			CalcTimeStamp();

			for (int i=0; i<(int)mHandlers.size(); ++i)
			{
				if (0 != (mHandlers[i]->GetLevels()&&logBuffer->Level))
					mHandlers[i]->Handle(logBuffer, mTimeBuff);
			}

			delete logBuffer;

			System::SleepSeconds(0.01f);
		}
		else
		{
			System::SleepSeconds(0.1f);
		}
	}
}
//----------------------------------------------------------------------------
void Logger::CalcTimeStamp ()
{
	LocalDateTime time;
	sprintf(mTimeBuff, "[%d:%d:%d]", time.Hour(), time.Minute(), time.Second());
}
//----------------------------------------------------------------------------
std::string Logger::_GetFileName (const std::string &path)
{
	size_t ind = path.find_last_of("\\");
	return path.substr(ind + 1);
}
//----------------------------------------------------------------------------
std::wstring Logger::_GetFileName (const std::wstring &path)
{
	size_t ind = path.find_last_of(L"\\");
	return path.substr(ind + 1);
}
//----------------------------------------------------------------------------