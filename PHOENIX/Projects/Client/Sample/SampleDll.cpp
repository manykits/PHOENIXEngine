// SampleDll.cpp

#include "SamplePre.hpp"
#include "PX2PluginManager.hpp"
#include "SamplePlugin.hpp"
#include "PX2InitTerm.hpp"

namespace PX2
{

	Plugin *plugin = 0;

	extern "C" void SAMPLE_DLL_ITEM DLLLoadPlugin(void)
	{
		plugin = new SamplePlugin();
		PluginManager::GetSingleton().InstallPlugin(plugin);
	}

	extern "C" void SAMPLE_DLL_ITEM DLLUnloadPlugin(void)
	{
		PluginManager::GetSingleton().UninstallPlugin(plugin);
	}
}