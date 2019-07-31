// SlamGMappingPlugin.hpp

#ifndef SLAMGMAPPINGPLUGIN_HPP
#define SLAMGMAPPINGPLUGIN_HPP

#include "SlamGMappingPlugin_Pre.hpp"
#include "PX2Plugin.hpp"

namespace PX2
{

	class SLAMGMAPPING_DLL_ITEM SlamGMappingPlugin : public Plugin
	{
	public:
		SlamGMappingPlugin();
		virtual ~SlamGMappingPlugin();

		virtual void OnInstall();
		virtual void OnUninstall();
		virtual void OnUpdate();

	private:
		static Plugin *sPlugin;
	};

}

#endif