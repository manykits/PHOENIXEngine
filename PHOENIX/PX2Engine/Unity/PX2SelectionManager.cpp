// PX2SelectionManager.cpp

#include "PX2SelectionManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
SelectionManager::SelectionManager()
{
	CreateAddSelection("Default");
	CreateAddSelection("Editor");
}
//----------------------------------------------------------------------------
SelectionManager::~SelectionManager()
{
}
//----------------------------------------------------------------------------
void SelectionManager::Clear()
{
	std::map<std::string, SelectionPtr>::iterator it = mSelectionMap.begin();
	for (; it != mSelectionMap.end(); it++)
	{
		it->second->Clear();
	}
}
//----------------------------------------------------------------------------
Selection *SelectionManager::CreateAddSelection(const std::string &name)
{
	DestorySelecton(name);

	Selection *sel = new0 Selection();
	mSelectionMap[name] = sel;

	return sel;
}
//----------------------------------------------------------------------------
void SelectionManager::DestorySelecton(const std::string &name)
{
	mSelectionMap.erase(name);
}
//----------------------------------------------------------------------------
Selection *SelectionManager::GetSelecton(const std::string &name)
{
	std::map<std::string, SelectionPtr>::iterator it =
		mSelectionMap.find(name);
	if (it != mSelectionMap.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------