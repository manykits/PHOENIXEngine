// AppPlayerApp.cpp

#include "AppPlayerApp.hpp"
#include "PX2Renderer.hpp"
#include "PX2Application.hpp"
#include "PX2ScriptManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_APPLICATION(AppPlayerApp)
//----------------------------------------------------------------------------
AppPlayerApp::AppPlayerApp ()
{

}
//----------------------------------------------------------------------------
AppPlayerApp::~AppPlayerApp ()
{
}
//----------------------------------------------------------------------------
bool AppPlayerApp::Initlize()
{
	if (App::Initlize())
	{
		PX2_LOG_INFO("App::Initlize() end");

		std::string projectName = PX2_APP.GetBoostProjectName();

		if (!mCmdProjectName.empty())
			projectName = mCmdProjectName;

		PX2_LOG_INFO("BoostProject Name %s", projectName.c_str());

		if (!projectName.empty())
		{
			PX2_APP.LoadProject(projectName);
			PX2_APP.Play(Application::PT_PLAY);
		}

		PX2_LOG_INFO("AppPlayerApp::GetTitleProj");

		std::string titleProj = GetTitleProj(projectName);
		SetTitle(titleProj);

		PX2_LOG_INFO("AppPlayerApp::Initlize");

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
#if !defined (__IOS__)

#if defined (_WIN32) || defined (WIN32)
// windows,console
#pragma comment( linker, "/subsystem:\"console\" /entry:\"mainCRTStartup\"" )
#endif

int main(int numArguments, char* arguments[])
{
	AppBase::msAppInitlizeFun();

	int exitCode = AppBase::msEntry(numArguments, arguments);

	AppBase::msAppTernamateFun();

	return exitCode;
}
#endif
//----------------------------------------------------------------------------
