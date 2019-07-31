// PX2AppBoostInfo.cpp

#include "PX2AppBoostInfo.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
AppBoostInfo::AppBoostInfo()
{
	ThePlayLogicMode = PLM_LOGIC;
	IsShowInfo = false;
	IsDataReWriteToDataUpdate = false;
}
//----------------------------------------------------------------------------
AppBoostInfo::~AppBoostInfo()
{
}
//----------------------------------------------------------------------------
bool AppBoostInfo::IsHasProject(const std::string &projectName) const
{
	for (int i = 0; i < (int)Projects.size(); i++)
	{
		if (projectName == Projects[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void AppBoostInfo::ClearProjects()
{
	Projects.clear();
}
//----------------------------------------------------------------------------
bool AppBoostInfo::AddProject(const std::string &projectName)
{
	if (projectName.empty())
		return false;

	if (IsHasProject(projectName))
		return false;

	Projects.push_back(projectName);

	return true;
}
//----------------------------------------------------------------------------
bool AppBoostInfo::RemoveProject(const std::string &projectName)
{
	for (auto it = Projects.begin(); it != Projects.end(); it++)
	{
		if (projectName == *it)
		{
			Projects.erase(it);
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool AppBoostInfo::IsHasPlugin(const std::string &pluginName) const
{
	for (int i = 0; i < (int)Plugins.size(); i++)
	{
		if (pluginName == Plugins[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void AppBoostInfo::ClearPlugins()
{
	Plugins.clear();
}
//----------------------------------------------------------------------------
bool AppBoostInfo::AddPlugin(const std::string &pluginName)
{
	if (pluginName.empty())
		return false;

	if (IsHasPlugin(pluginName))
		return false;

	Plugins.push_back(pluginName);

	return true;
}
//----------------------------------------------------------------------------
bool AppBoostInfo::RemovePlugin(const std::string &pluginName)
{
	for (auto it = Plugins.begin(); it != Plugins.end(); it++)
	{
		if (pluginName == *it)
		{
			Plugins.erase(it);
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------