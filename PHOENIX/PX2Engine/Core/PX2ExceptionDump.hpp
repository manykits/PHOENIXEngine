// PX2ExceptionDump.hpp

#ifndef PX2EXCEPTIONDUMP_HPP
#define PX2EXCEPTIONDUMP_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ExceptionDump
	{
	public:
		ExceptionDump ();
		~ExceptionDump ();

		bool InstallHandler(std::string appName="AppName");
		bool RemoveHandler();

	private:
#if defined WIN32 || defined _WIN32
#if defined _MSC_VER || defined __USE_MINIDUMP__
		static long __stdcall MiniDumpExceptionHandler(struct _EXCEPTION_POINTERS *pExceptionInfo);
		static int msRefCounter;
#elif __GNUC__
		/**/
#endif
#endif

		static std::string mAppName;
		bool mIsInstalled;
	};

}

#endif