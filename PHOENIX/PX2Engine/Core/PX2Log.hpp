// PX2Log.hpp

#ifndef PX2LOG_HPP
#define PX2LOG_HPP

#include "PX2CorePre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2ThreadServer.hpp"
#include "PX2FixMemoryObject.hpp"
#include "PX2MsgQueue.hpp"

namespace PX2
{

	/// 日志类型
	enum LogType
	{
		LT_INFO = 1,
		LT_ERROR = 2,
		LT_USER = 4
	};

	class PX2_ENGINE_ITEM LogBuffer : public FixMemoryObject<LogBuffer>
	{
	public:
		LogBuffer (int isWChar=0);
		virtual ~LogBuffer ();

		static const int MaxLogLength = 512;
		int Level;
		int IsWChar;
		char Buffer[2*MaxLogLength];
	};

	class PX2_ENGINE_ITEM LogHandler
	{
	public:
		LogHandler (unsigned int levels);
		virtual ~LogHandler ();

		virtual void Handle (const LogBuffer *logBuffer,
			const char *timeStamp) = 0;

		unsigned int GetLevels ();

	protected:
		unsigned int mLevelFlags;
	};

	class PX2_ENGINE_ITEM Logger : public ThreadServer, public Singleton<Logger>
	{
	public:
		Logger ();
		virtual ~Logger ();

		void SetBlock (bool isBlock);
		bool IsBlock ();

		void SetLogFileInfo(bool logFileInfo);
		bool IsLogFileInfo() const;

		// LogHandler 需要被new0 分配，在CloseLogger会对所有的handler进行释放
		bool AddHandler (LogHandler *handler);
		void RemoveHandler (LogHandler *handler);
		bool AddFileHandler (const char *filename, unsigned int levels);
		bool AddConsoleHandler (unsigned int levels);
		bool AddOutputWindowHandler (unsigned int levels);

		void StartLogger ();
		void CloseLogger();

		void LogMessage (int level, long line, const char* fileName, const char* format, ...);

		// for script
		void LogInfo (const std::string &tag, const std::string &info);
		void LogError (const std::string &tag, const std::string &info);
		void LogUser (const std::string &tag, const std::string &info);

		virtual void Run ();

	private:
		void CalcTimeStamp ();
		std::string _GetFileName (const std::string &path);
		std::wstring _GetFileName (const std::wstring &path);

		bool mIsBlock;
		bool mIsLogFileInfo;
		
		bool mDoQuit;
		char mTimeBuff[64];

		std::vector<LogHandler*> mHandlers;
		MsgQueue<LogBuffer> mLogQueue;
	};

#define PX2_LOG_INFO(...)			Logger::GetSingleton().LogMessage(PX2::LT_INFO, __LINE__, __FILE__, ##__VA_ARGS__)
#define PX2_LOG_ERROR(...)			Logger::GetSingleton().LogMessage(PX2::LT_ERROR, __LINE__, __FILE__, ##__VA_ARGS__)
#define PX2_LOG_USER(...)			Logger::GetSingleton().LogMessage(PX2::LT_USER, __LINE__, __FILE__, ##__VA_ARGS__)

}

#endif
