// SlamGMappingPlugin.cpp

#include "SlamGMappingPlugin.hpp"
#include "PX2SlamGMappingManager.hpp"
#include "PX2LuaPlusContext.hpp"
#include "PX2Time.hpp"
#include "PX2Application.hpp"
#include "PX2Log.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Plugin *SlamGMappingPlugin::sPlugin = 0;
SlamGMappingManager *sManager = 0;
//----------------------------------------------------------------------------
SlamGMappingPlugin::SlamGMappingPlugin()
{
	mName = "SlamGMappingPlugin";
}
//----------------------------------------------------------------------------
SlamGMappingPlugin::~SlamGMappingPlugin()
{
}
//----------------------------------------------------------------------------
void SlamGMappingPlugin::OnInstall()
{
	PX2_LOG_INFO("Slam2DPlugin::OnInstall()");

	sManager = new0 SlamGMappingManager();
	sManager->Initlize();
}
//----------------------------------------------------------------------------
void SlamGMappingPlugin::OnUninstall()
{
	PX2_LOG_INFO("Slam2DPlugin::OnUninstall()");

	if (sManager)
	{
		sManager->Terminate();
		delete0(sManager);
	}

	SlamGMappingManager::Set(0);
}
//----------------------------------------------------------------------------
void SlamGMappingPlugin::OnUpdate()
{
	float time = Time::GetTimeInSeconds();
	float elapseTime = PX2_APP.GetElapsedTime();
	if (sManager)
	{
		sManager->Update(time, elapseTime);
	}
}
//----------------------------------------------------------------------------