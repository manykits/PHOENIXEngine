// ProjectDll.cpp

#include "ProjectPre.hpp"
#include "PX2PluginManager.hpp"
#include "ProjectPlugin.hpp"
#include "PX2InitTerm.hpp"

namespace PX2
{

	Plugin *plugin = 0;

	extern "C" void PROJECT_DLL_ITEM DLLLoadPlugin(void)
	{
		plugin = new ProjectPlugin();
		PluginManager::GetSingleton().InstallPlugin(plugin);
	}

	extern "C" void PROJECT_DLL_ITEM DLLUnloadPlugin(void)
	{
		PluginManager::GetSingleton().UninstallPlugin(plugin);
	}
}