// Slam2DPlugin.hpp

#ifndef SLAM2DPLUGIN_HPP
#define SLAM2DPLUGIN_HPP

#include "Slam2DPlugin_Pre.hpp"
#include "PX2Plugin.hpp"

namespace PX2
{

	class SLAM2D_DLL_ITEM Slam2DPlugin : public Plugin
	{
	public:
		Slam2DPlugin();
		virtual ~Slam2DPlugin();

		virtual void OnInstall();
		virtual void OnUninstall();
		virtual void OnUpdate();

	private:
		static Plugin *sPlugin;
	};

}

#endif