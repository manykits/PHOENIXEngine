// FBXImporterPlugin.cpp

#include "FBXImporterPlugin.hpp"
#include "PX2PluginManager.hpp"
#include "PX2Log.hpp"
#include "FBXImporterToLua.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2LuaPlusContext.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Plugin *FBXImporterPlugin::sPlugin = 0;
//----------------------------------------------------------------------------
FBXImporterPlugin::FBXImporterPlugin()
{
	mName = "FBXImporterPlugin";
}
//----------------------------------------------------------------------------
FBXImporterPlugin::~FBXImporterPlugin()
{
}
//----------------------------------------------------------------------------
void FBXImporterPlugin::OnInstall()
{
	PX2_LOG_INFO("FBXImporterPlugin::OnInstall()");

	LuaPlusContext *luaContext = (LuaPlusContext*)PX2_SC_LUA;

	// Lua
	tolua_FBXImporter_open((lua_State*)luaContext->GetLuaState());
	LuaPlus::LuaState *luaPlusState = luaContext->GetLuaPlusState();
	PX2_UNUSED(luaPlusState);
}
//----------------------------------------------------------------------------
void FBXImporterPlugin::OnUninstall()
{
	PX2_LOG_INFO("FBXImporterPlugin::OnUninstall()");
}
//----------------------------------------------------------------------------
void FBXImporterPlugin::OnUpdate()
{
}
//----------------------------------------------------------------------------