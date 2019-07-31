// PX2LP_Plugin.hpp

#ifndef PX2LP_PLUGIN_HPP
#define PX2LP_PLUGIN_HPP

#include "PX2Plugin.hpp"
#include "PX2LP_Pre.hpp"
#include "PX2LP_Plugin.hpp"

namespace PX2
{

	class PX2LP_DLL_ITEM LP_Plugin : public Plugin
	{
	public:
		LP_Plugin();
		virtual ~LP_Plugin();

		virtual std::string GetName();

		virtual void OnInstall();
		virtual void OnUninstall();
		virtual void OnUpdate();
	};

}

#endif