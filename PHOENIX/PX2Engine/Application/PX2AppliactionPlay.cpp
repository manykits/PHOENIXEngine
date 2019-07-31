// PX2AppliactionPlay.cpp

#include "PX2Application.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2ProjectEvent.hpp"
#if defined _WIN32 || defined (WIN32)
#include <ShellApi.h>
#endif
using namespace PX2;

//----------------------------------------------------------------------------
void Application::Play(PlayType type)
{
	if (mPlayType == type) 
		return;

	if (mProjectName.empty())
		return;

	std::string dirScirpt = "/scripts";
	if (PT_PLAY == type)
	{
		PX2_LOG_INFO("Begin PT_PLAY");

		std::string callFilenameLua = "Data/" + mProjectName + dirScirpt 
			+ "/lua/play.lua";

		PX2_LOG_INFO("Begin PT_PLAY CallFileFunction %s", callFilenameLua.c_str());

		PX2_SC_LUA->CallFileFunction(callFilenameLua.c_str(), "engine_project_preplay");

		PX2_LOG_INFO("Begin PT_PLAY SetPlayType");

		PX2_GR.SetPlayType((GraphicsRoot::PlayType)type);
		mPlayType = type;

		PX2_LOG_INFO("Begin PT_PLAY broadcast event");

		Event *entPlay = PX2_CREATEEVENTEX(ProjectES, Play);
		PX2_EW.BroadcastingLocalEvent(entPlay);

		PX2_LOG_INFO("Begin PT_PLAY call script");

		PX2_SC_LUA->CallFileFunction(callFilenameLua.c_str(), "engine_project_play");

		PX2_LOG_INFO("End PT_PLAY");
	}
	else if (PT_NONE == type)
	{
		PX2_LOG_INFO("Begin PT_NONE");

		std::string callFilename = "Data/" + mProjectName + dirScirpt + 
			"/lua/stop.lua";
		PX2_SC_LUA->CallFileFunction(callFilename.c_str(), "engine_project_prestop");

		PX2_TIMERM.ClearTimers();

		PX2_GR.SetPlayType((GraphicsRoot::PlayType)type);
		mPlayType = type;

		Event *entPlay = PX2_CREATEEVENTEX(ProjectES, Stop);
		PX2_EW.BroadcastingLocalEvent(entPlay);

		PX2_SC_LUA->CallFileFunction(callFilename.c_str(), "engine_project_stop");

		PX2_LOG_INFO("End PT_NONE");
	}
}
//----------------------------------------------------------------------------
void Application::RunFile(const std::string &pathname)
{
#if defined _WIN32 || defined (WIN32)
	std::string cmdStr = pathname;
	ShellExecute(NULL, "open", cmdStr.c_str(), NULL, NULL, SW_SHOWNORMAL);
#endif
}
//----------------------------------------------------------------------------