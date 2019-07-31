// PX2Plugin.hpp

#ifndef PX2PLUGIN_HPP
#define PX2PLUGIN_HPP

#include "PX2UnityPre.hpp"
#include "PX2EditEventType.hpp"

namespace PX2
{

	PX2_DECLARE_EVENT_BEGIN(PX2_ENGINE_ITEM PluginES)
	PX2_EVENT(Install)
	PX2_EVENT(UnInstall)
	PX2_DECLARE_EVENT_END(PluginES)

	class PX2_ENGINE_ITEM Plugin
	{
	public:
		Plugin();
		virtual ~Plugin();

		std::string GetName();

		virtual void OnInstall();
		virtual void OnUninstall();
		virtual void OnUpdate();

	protected:
		std::string mName;
	};

}

#endif