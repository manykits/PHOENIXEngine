// FBXImporterDll.cpp

#include "FBXImporter_Pre.hpp"
#include "PX2PluginManager.hpp"
#include "FBXImporterPlugin.hpp"
#include "PX2InitTerm.hpp"

namespace PX2
{

	Plugin *plugin = 0;

	extern "C" void FBXIMPORTER_DLL_ITEM DLLLoadPlugin(void)
	{
		plugin = new FBXImporterPlugin();
		PluginManager::GetSingleton().InstallPlugin(plugin);
	}

	extern "C" void FBXIMPORTER_DLL_ITEM DLLUnloadPlugin(void)
	{
		PluginManager::GetSingleton().UninstallPlugin(plugin);
	}
}