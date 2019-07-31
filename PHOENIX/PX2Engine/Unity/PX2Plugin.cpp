// PX2Plugin.cpp

#include "PX2Plugin.hpp"
using namespace PX2;

PX2_IMPLEMENT_EVENT(PluginES);

//----------------------------------------------------------------------------
Plugin::Plugin() 
{
	mName = "Plugin";
}
//----------------------------------------------------------------------------
Plugin::~Plugin() 
{
}
//----------------------------------------------------------------------------
std::string Plugin::GetName()
{
	return mName;
}
//----------------------------------------------------------------------------
void Plugin::OnInstall()
{
	std::string name = GetName();

	Event *ent = PX2_CREATEEVENTEX(PluginES, Install);
	ent->SetDataStr0(name);
	ent->SetData<std::string>(name);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Plugin::OnUninstall()
{
	std::string name = GetName();

	Event *ent = PX2_CREATEEVENTEX(PluginES, UnInstall);
	ent->SetDataStr0(name);
	ent->SetData<std::string>(name);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Plugin::OnUpdate()
{
}
//----------------------------------------------------------------------------