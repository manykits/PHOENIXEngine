// PX2DynLib.cpp

#include "PX2DynLib.hpp"
#include "PX2Log.hpp"
using namespace PX2;

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#endif

#if defined __APPLE__ && !defined __IOS__
#include "macPlugins.h"
#endif

//----------------------------------------------------------------------------
DynLib::DynLib(const std::string &name)
{
	mName = name;
	mhInst = 0;
}
//----------------------------------------------------------------------------
DynLib::~DynLib()
{
}
//----------------------------------------------------------------------------
const std::string &DynLib::GetName()
{
	return mName;
}
//----------------------------------------------------------------------------
void DynLib::Load()
{
	PX2_LOG_INFO("Loading library: %s", mName.c_str());

#if !defined __IOS__
	mhInst = (PLUGIN_HANDLE)PLUGIN_LOAD(mName.c_str());

	if (!mhInst)
	{
		assertion(false, "Could not load dynamic library %s . System Error: %s",
			mName.c_str(), DynlibError().c_str());
		PX2_LOG_ERROR("Could not load dynamic library %s . System Error: %s",
			mName.c_str(), DynlibError().c_str());
	}
#endif
}
//----------------------------------------------------------------------------
void DynLib::Unload()
{
	PX2_LOG_INFO("Unload library %s", mName.c_str());

#if !defined __IOS__
	if (PLUGIN_UNLOAD(mhInst))
	{
		PX2_LOG_ERROR("Could not unload dynamic library %s . System Error: %s", 
			mName.c_str(), DynlibError().c_str());
	}
#endif
}
//----------------------------------------------------------------------------
void* DynLib::GetSymbol(const std::string &strName)
{
#if !defined __IOS__
	return (void*)PLUGIN_GETSYM(mhInst, strName.c_str());
#else
	return 0;
#endif
}
//----------------------------------------------------------------------------
std::string DynLib::DynlibError()
{
#if defined(_WIN32) || defined(WIN32)
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
		);
	std::string ret = (char*)lpMsgBuf;
	LocalFree(lpMsgBuf);
	return ret;
#elif defined (__LINUX__) || defined(__ANDROID__)
	return std::string(dlerror());
#elif defined (__APPLE__) && !defined(__IOS__)
	return std::string(mac_errorBundle());
#else
	return std::string("");
#endif
}
//----------------------------------------------------------------------------