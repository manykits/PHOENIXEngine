// PX2PluginManager.hpp

#ifndef PX2PLUGINMANAGER_HPP
#define PX2PLUGINMANAGER_HPP

#include "PX2UnityPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2Plugin.hpp"
#include "PX2DynLib.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM PluginManager : public Singleton<PluginManager>
	{
	public:
		PluginManager();
		virtual ~PluginManager();

		void Load(const std::string &filename);

		void Unload(const std::string &filename);
		void UnloadPlugins();

		void Update();

	public_internal:
		void InstallPlugin(Plugin* plugin);
		void UninstallPlugin(Plugin* plugin);

	protected:
		std::vector<Plugin *> mPlugins;
		std::vector<DynLib *> mPluginLibs;
	};

#define PX2_PLUGINMAN PluginManager::GetSingleton()

}

#endif