// SlamGMappingPluginDll.cpp

#include "SlamGMappingPlugin_Pre.hpp"
#include "PX2PluginManager.hpp"
#include "PX2InitTerm.hpp"
#include "SlamGMappingPlugin.hpp"

namespace PX2
{

	Plugin *plugin = 0;

	extern "C" void SLAMGMAPPING_DLL_ITEM DLLLoadPlugin(void)
	{
		plugin = new SlamGMappingPlugin();
		PluginManager::GetSingleton().InstallPlugin(plugin);
	}

	extern "C" void SLAMGMAPPING_DLL_ITEM DLLUnloadPlugin(void)
	{
		PluginManager::GetSingleton().UninstallPlugin(plugin);
	}
}