// PX2LP_Dll.cpp

#include "PX2PluginManager.hpp"
#include "PX2LP_Plugin.hpp"

namespace PX2
{

	LP_Plugin *plugin = 0;

	extern "C" void PX2LP_DLL_ITEM DLLLoadPlugin(void)
	{
		plugin = new LP_Plugin();
		PluginManager::GetSingleton().InstallPlugin(plugin);
	}

	extern "C" void PX2LP_DLL_ITEM DLLUnloadPlugin(void)
	{
		PluginManager::GetSingleton().UninstallPlugin(plugin);
	}

}