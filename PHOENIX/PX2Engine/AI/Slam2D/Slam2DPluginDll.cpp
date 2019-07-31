// Slam2DPluginDll.cpp

#include "Slam2DPlugin_Pre.hpp"
#include "PX2PluginManager.hpp"
#include "PX2InitTerm.hpp"
#include "Slam2DPlugin.hpp"

namespace PX2
{

	Plugin *plugin = 0;

	extern "C" void SLAM2D_DLL_ITEM DLLLoadPlugin(void)
	{
		plugin = new Slam2DPlugin();
		PluginManager::GetSingleton().InstallPlugin(plugin);
	}

	extern "C" void SLAM2D_DLL_ITEM DLLUnloadPlugin(void)
	{
		PluginManager::GetSingleton().UninstallPlugin(plugin);
	}
}