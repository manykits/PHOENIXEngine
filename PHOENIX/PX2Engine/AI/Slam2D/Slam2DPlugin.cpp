// Slam2DPlugin.cpp

#include "Slam2DPlugin.hpp"
#include "PX2LuaPlusContext.hpp"
#include "PX2Log.hpp"
#include "PX2Slam2DManager.hpp"
#include "PX2Application.hpp"
#include "SampleToLua.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Plugin *Slam2DPlugin::sPlugin = 0;
Slam2DManager *sManager = 0;
//----------------------------------------------------------------------------
Slam2DPlugin::Slam2DPlugin()
{
	mName = "Slam2DPlugin";
}
//----------------------------------------------------------------------------
Slam2DPlugin::~Slam2DPlugin()
{
}
//----------------------------------------------------------------------------
void Slam2DPlugin::OnInstall()
{
	PX2_LOG_INFO("Slam2DPlugin::OnInstall()");

	// lua open
	LuaPlusContext *luaContext = (LuaPlusContext*)PX2_SC_LUA;
	tolua_Sample_open((lua_State*)luaContext->GetLuaState());

	sManager = new0 Slam2DManager();
	sManager->Initlize();
}
//----------------------------------------------------------------------------
void Slam2DPlugin::OnUninstall()
{
	PX2_LOG_INFO("Slam2DPlugin::OnUninstall()");

	if (sManager)
	{
		sManager->Terminate();
		delete0(sManager);
	}

	Slam2DManager::Set(0);
}
//----------------------------------------------------------------------------
void Slam2DPlugin::OnUpdate()
{
	float time = Time::GetTimeInSeconds();
	float elapseTime = PX2_APP.GetElapsedTime();
	if (sManager)
	{
		sManager->Update(time, elapseTime);
	}
}
//----------------------------------------------------------------------------