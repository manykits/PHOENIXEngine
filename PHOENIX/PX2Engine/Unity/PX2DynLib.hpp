// PX2DynLib.hpp

#ifndef PX2DYNLIB_HPP
#define PX2DYNLIB_HPP

#include "PX2UnityPre.hpp"
#if defined(__ANDROID__) || defined(__LINUX__)
#include <dlfcn.h>
#endif

#if defined(_WIN32) || defined(WIN32)
#    define PLUGIN_HANDLE hInstance
#    define PLUGIN_LOAD( a ) LoadLibrary( a )
#    define PLUGIN_GETSYM( a, b ) GetProcAddress( a, b )
#    define PLUGIN_UNLOAD( a ) !FreeLibrary( a )

struct HINSTANCE__;
typedef struct HINSTANCE__* hInstance;

#elif defined (__LINUX__) || defined(__ANDROID__)
#define PLUGIN_HANDLE void*
#define PLUGIN_LOAD( a ) dlopen( a, RTLD_LAZY | RTLD_GLOBAL)
#define PLUGIN_GETSYM( a, b ) dlsym( a, b )
#define PLUGIN_UNLOAD( a ) dlclose( a )

#elif defined (__APPLE__) && !defined(__IOS__)
#define PLUGIN_HANDLE CFBundleRef
#define PLUGIN_LOAD( a ) mac_loadExeBundle( a )
#define PLUGIN_GETSYM( a, b ) mac_getBundleSym( a, b )
#define PLUGIN_UNLOAD( a ) mac_unloadExeBundle( a )
#else
#define PLUGIN_HANDLE void*
#define PLUGIN_LOAD( a ) void( a )
#define PLUGIN_GETSYM( a, b ) void( a, b )
#define PLUGIN_UNLOAD( a ) void( a )
#endif

namespace PX2
{

	class PX2_ENGINE_ITEM DynLib
	{
	public:
		DynLib(const std::string &name);
		virtual ~DynLib();

		const std::string &GetName();

		void Load();
		void Unload();

		void* GetSymbol(const std::string &strName);

	protected:
		std::string DynlibError();

		std::string mName;
		PLUGIN_HANDLE mhInst;
	};

}

#endif
