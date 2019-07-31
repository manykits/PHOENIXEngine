// FBXImporterPlugin.hpp

#ifndef FBXIMPORTERPLUGIN_HPP
#define FBXIMPORTERPLUGIN_HPP

#include "PX2Plugin.hpp"
#include "PX2FBXPre.hpp"

namespace PX2
{

	class FBXIMPORTER_DLL_ITEM FBXImporterPlugin : public Plugin
	{
	public:
		FBXImporterPlugin();
		virtual ~FBXImporterPlugin();

		virtual void OnInstall();
		virtual void OnUninstall();
		virtual void OnUpdate();

	private:
		static Plugin *sPlugin;
	};

}
#endif