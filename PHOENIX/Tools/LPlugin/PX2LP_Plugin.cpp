// PX2LP_Plugin.cpp

#include "PX2LP_Plugin.hpp"
#include "PX2LP_Manager.hpp"
#include "PX2Time.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
LP_Plugin::LP_Plugin()
{
}
//----------------------------------------------------------------------------
LP_Plugin::~LP_Plugin()
{
}
//----------------------------------------------------------------------------
std::string LP_Plugin::GetName()
{
	return "Launcher";
}
//----------------------------------------------------------------------------
void LP_Plugin::OnInstall()
{
	LP_Manager::GetSingleton().Initlize();
}
//----------------------------------------------------------------------------
void LP_Plugin::OnUninstall()
{
	LP_Manager::GetSingleton().Terminate();
}
//----------------------------------------------------------------------------
void LP_Plugin::OnUpdate()
{
	LP_Manager *lp_Man = LP_Manager::GetSingletonPtr();
	if (lp_Man)
	{
		double timeInSeconds = Time::GetTimeInSeconds();
		double elapsedSeconds = Time::FrameElapsedSeconds;
		lp_Man->Update(timeInSeconds, elapsedSeconds);
	}
}
//----------------------------------------------------------------------------