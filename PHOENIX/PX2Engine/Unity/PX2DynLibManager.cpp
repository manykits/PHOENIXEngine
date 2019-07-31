// PX2DynLibManager.cpp

#include "PX2DynLibManager.hpp"
#include "PX2Log.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
DynLibManager::DynLibManager()
{
}
//----------------------------------------------------------------------------
DynLibManager::~DynLibManager()
{
}
//----------------------------------------------------------------------------
DynLib *DynLibManager::Load(const std::string &filename)
{
	PluginList::iterator i = mPluginList.find(filename);
	if (i != mPluginList.end())
	{
		return i->second;
	}
	else
	{
		DynLib* plugin = new DynLib(filename);
		plugin->Load();
		mPluginList[filename] = plugin;

		return plugin;
	}
}
//----------------------------------------------------------------------------
void DynLibManager::Unload(DynLib *dynlib)
{
	PluginList::iterator i = mPluginList.find(dynlib->GetName());
	if (i != mPluginList.end())
	{
		mPluginList.erase(i);
	}

	dynlib->Unload();
	
	delete dynlib;
}
//----------------------------------------------------------------------------