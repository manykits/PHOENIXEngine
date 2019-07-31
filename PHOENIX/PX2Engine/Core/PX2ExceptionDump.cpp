// PX2Exception.cpp

#include "PX2ExceptionDump.hpp"
#include "PX2System.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
std::string ExceptionDump::mAppName;
//----------------------------------------------------------------------------
#if defined WIN32 || defined _WIN32
#include <windows.h>
#if defined _MSC_VER || defined __USE_MINIDUMP__
#include "dbghelp.h"
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);
int ExceptionDump::msRefCounter = 0;
#elif __GNUC__
/**/
#endif
#else //Unix/Linux
/**/
#endif

#ifndef COMPILER_STRING
#ifdef __GNUC__
/**/
#else
#define COMPILER_STRING  ""
#endif
#endif

#define COMPILATION_DATE  __DATE__ " " __TIME__
//----------------------------------------------------------------------------
ExceptionDump::ExceptionDump():
mIsInstalled(false)
{
}
//----------------------------------------------------------------------------
ExceptionDump::~ExceptionDump()
{
	if (mIsInstalled)
	{
		RemoveHandler();
	}
}
//----------------------------------------------------------------------------
bool ExceptionDump::InstallHandler(std::string appName)
{
#if defined WIN32 || defined __WINDOWS__
#if defined _MSC_VER || defined __USE_MINIDUMP__
	++msRefCounter;
	if (1 == msRefCounter)
	{
		mAppName = appName;
		::SetUnhandledExceptionFilter(ExceptionDump::MiniDumpExceptionHandler);
	}

#elif __GNUC__
	/**/
#endif
#else // Unix/Linux
	/**/
#endif

	mIsInstalled = true;
	return true;
}
//----------------------------------------------------------------------------
bool ExceptionDump::RemoveHandler()
{
	if (!mIsInstalled)
	{
		return false;
	}

#if defined WIN32 || defined __WINDOWS__
#if defined _MSC_VER || defined __USE_MINIDUMP__
	--msRefCounter;
	if (msRefCounter == 0)
	{
		::SetUnhandledExceptionFilter(0);
	}
#elif __GNUC__
	/**/
#endif	
#else //Unix/Linux
	/**/
#endif

	mIsInstalled = false;
	return true;
}
//----------------------------------------------------------------------------
#if defined WIN32 || defined __WINDOWS__
#if defined _MSC_VER || defined __USE_MINIDUMP__
long ExceptionDump::MiniDumpExceptionHandler(struct _EXCEPTION_POINTERS *pExceptionInfo)
{
	HMODULE hDll = NULL;
	char szAppPath[_MAX_PATH];
	std::string strAppDirectory;

	GetModuleFileName(NULL, szAppPath, _MAX_PATH);
	strAppDirectory = szAppPath;
	strAppDirectory = strAppDirectory.substr(0, strAppDirectory.rfind("\\"));
	if(strAppDirectory.rfind('\\') != strAppDirectory.size())
	{
		strAppDirectory += '\\';
	}

	std::string strFileNameDbgHelp = strAppDirectory + "DBGHELP.DLL";
	hDll = ::LoadLibrary(strFileNameDbgHelp.c_str());

	if (!hDll)
	{
		hDll = ::LoadLibrary("DBGHELP.DLL");
	}

	if (!hDll)
	{
		std::cout << "Could not generate report - DBGHELP.DLL could not be found." << std::endl;
		return EXCEPTION_CONTINUE_SEARCH;
	}

	MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDll, "MiniDumpWriteDump");
	if(!pDump)
	{
		std::cout << "Could not generate report - DBGHELP.DLL is to old." << std::endl;
		return EXCEPTION_CONTINUE_SEARCH;
	}

	SYSTEMTIME stLocalTime;
	GetLocalTime(&stLocalTime);

	char dumpfile[250] = {'\0'};
	sprintf(dumpfile, "%s-%04d-%02d-%02d_%02d%02d%02d_%d.dmp",
		mAppName.c_str(),
		stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
		stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,
		System::GetCurrentThreadID());

	std::string strFileNameDump = strAppDirectory + dumpfile;

	HANDLE hFile = ::CreateFile(strFileNameDump.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL );

	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "Could not create memory dump file." << std::endl;
		return EXCEPTION_EXECUTE_HANDLER;
	}

	_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

	ExInfo.ThreadId = ::GetCurrentThreadId();
	ExInfo.ExceptionPointers = pExceptionInfo;
	ExInfo.ClientPointers = NULL;

	std::cout << "Generating minidump file... " << dumpfile << std::endl;

	BOOL bOK = pDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL );
	if(!bOK)
	{
		std::cout << "Could not dump memory to file." << std::endl;
		::CloseHandle(hFile);
		return EXCEPTION_CONTINUE_SEARCH;
	}

	::CloseHandle(hFile);
	return EXCEPTION_EXECUTE_HANDLER;
}
#elif __GNUC__
/**/
#endif
//----------------------------------------------------------------------------
#else //Unix/Linux
/**/
#endif