// ProjectPlugin.hpp

#ifndef PLUGINPLUGIN_HPP
#define PLUGINPLUGIN_HPP

#include "PX2Plugin.hpp"
#include "ProjectPre.hpp"

namespace PX2
{

	class PROJECT_DLL_ITEM ProjectPlugin : public Plugin, public EventHandler, public Singleton<ProjectPlugin>
	{
	public:
		ProjectPlugin();
		virtual ~ProjectPlugin();

		virtual void OnInstall();
		virtual void OnUninstall();
		virtual void OnUpdate();

		virtual void OnEvent(Event *ent);
	};

}
#endif