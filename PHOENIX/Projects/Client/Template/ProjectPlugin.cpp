// ProjectPlugin.cpp

#include "ProjectPlugin.hpp"
#include "PX2Log.hpp"
#include "PX2LuaPlusContext.hpp"
#include "PX2ScriptManager.hpp"
#include "ProjectToLua.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
ProjectPlugin::ProjectPlugin()
{
	mName = "ProjectPlugin";
}
//----------------------------------------------------------------------------
ProjectPlugin::~ProjectPlugin()
{
}
//----------------------------------------------------------------------------
void ProjectPlugin::OnInstall()
{
	PX2_LOG_INFO("ProjectPlugin::OnInstall()");

	PX2_EW.ComeIn(this);
}
//----------------------------------------------------------------------------
void ProjectPlugin::OnUninstall()
{
	PX2_LOG_INFO("ProjectPlugin::OnUninstall()");

	PX2_EW.GoOut(this);

	// lua open
	LuaPlusContext *luaContext = (LuaPlusContext*)PX2_SC_LUA;
	tolua_Project_open((lua_State*)luaContext->GetLuaState());
}
//----------------------------------------------------------------------------
void ProjectPlugin::OnUpdate()
{
}
//----------------------------------------------------------------------------
void ProjectPlugin::OnEvent(Event *event)
{
	PX2_UNUSED(event);
}
//----------------------------------------------------------------------------