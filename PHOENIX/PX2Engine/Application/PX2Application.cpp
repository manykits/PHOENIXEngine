// PX2Application.cpp

#include "PX2Application.hpp"
#include "PX2TimerManager.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2EngineNetEvent.hpp"
#include "PX2Arduino.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Application::Application() :
mPt_Data(0),
mPt_Data1(0),
mPt_Data2(0),

mDynLibMan(0),
mPluginMan(0),
mTimerMan(0),
mEventWorld(0),
mLanguageMan(0),
mBluetooth(0),
mHardCamera(0),
mRoot(0),
mIMEDisp(0),
mInputMan(0),
mResMan(0),
mScriptMan(0),
mFontMan(0),
mSoundSys(0),
mADMan(0),
mSelectionMan(0),
mURDoMan(0),
mFunObjectManager(0),
mAccoutManager(0),
mUIAuiManager(0),
mUISkinManager(0),
mLogicManager(0),
mCreater(0),
mVoiceSDK(0),
mSTEAMEduManager(0),

mIsInBackground(false),
mBeforeInBackgroundMusicEnable(true),
mBeforeInBackgroundSoundEnable(true),
mIsQuit(false),

mPlayType(PT_NONE),

mAppTime(0),
mLastAppTime(0),
mElapsedTime(0),

mConfigName("Application")
{
}
//----------------------------------------------------------------------------
Application::~Application()
{
}
//----------------------------------------------------------------------------
void Application::SetPt_Data(void *data)
{
	mPt_Data = data;
}
//----------------------------------------------------------------------------
void *Application::GetPt_Data()
{
	return mPt_Data;
}
//----------------------------------------------------------------------------
void Application::SetPt_Data1(void *data)
{
	mPt_Data1 = data;
}
//----------------------------------------------------------------------------
void *Application::GetPt_Data1()
{
	return mPt_Data1;
}
//----------------------------------------------------------------------------
void Application::SetPt_Data2(void *data)
{
	mPt_Data2 = data;
}
//----------------------------------------------------------------------------
void *Application::GetPt_Data2()
{
	return mPt_Data2;
}
//----------------------------------------------------------------------------
void Application::SetPt_Size(const Sizef &size)
{
	mPt_Size = size;
}
//----------------------------------------------------------------------------
const Sizef &Application::GetPt_Size() const
{
	return mPt_Size;
}
//----------------------------------------------------------------------------
Application::PlatformType Application::GetPlatformType() const
{
#if defined (_WIN32) || defined(WIN32)
	return PLT_WINDOWS;
#elif defined (__LINUX__)
	return PLT_LINUX;
#elif defined (__ANDROID__)
	return PLT_ANDROID;
#else
	return PLT_IOS;
#endif
}
//----------------------------------------------------------------------------
float Application::GetElapsedTime()
{
	return (float)mElapsedTime;
}
//----------------------------------------------------------------------------
float Application::_CalElapsedTime()
{
	return (float)(mAppTime - mLastAppTime);
}
//----------------------------------------------------------------------------
void Application::Update()
{
	mAppTime = Time::GetTimeInSeconds();
	mElapsedTime = _CalElapsedTime();
	mLastAppTime = mAppTime;
	if (mElapsedTime > 1.0f)
		mElapsedTime = 0.1f;

	Time::FrameElapsedSeconds = mElapsedTime;
	Time::FrameRunnedSeconds += mElapsedTime;

	Update((float)mAppTime, (float)mElapsedTime);
}
//----------------------------------------------------------------------------
void Application::OnEvent(Event *ent)
{
	if (EngineNetES::IsEqual(ent, EngineNetES::OnEngineServerBeConnected))
	{
		std::string ip = ent->GetDataStr1();
		UDPNetInfo *info = GetUDPNetInfo(ip);
		if (info)
		{
			info->IsConnected = true;
		}
	}
	else if (EngineNetES::IsEqual(ent, EngineNetES::OnEngineServerBeDisConnected))
	{
		std::string ip = ent->GetDataStr1();
		UDPNetInfo *info = GetUDPNetInfo(ip);
		if (info)
		{
			info->IsConnected = false;
		}
	}
}
//----------------------------------------------------------------------------
void Application::SetConfigName(const std::string &cfgName)
{
	mConfigName = cfgName;
}
//----------------------------------------------------------------------------
const std::string &Application::GetConfigName()
{
	return mConfigName;
}
//----------------------------------------------------------------------------
void Application::SetConfig(const std::string &name, const std::string &cfgStr)
{
	mCFGs[name] = cfgStr;
	_WriteConfigs(mCFGs, mConfigName);
}
//----------------------------------------------------------------------------
std::string Application::GetConfig(const std::string &name)
{
	auto it = mCFGs.find(name);
	if (it != mCFGs.end())
		return it->second;

	return "";
}
//----------------------------------------------------------------------------
std::string Application::_GetWritePath(const std::string &projName)
{
	return "Write_" + projName;
}
//----------------------------------------------------------------------------
void Application::_WriteConfigs(std::map<std::string, std::string> &cfgs,
	const std::string &cfgName)
{
	std::string wirteablePath = PX2_RM.GetWriteablePath();
	std::string appPath = _GetWritePath(cfgName) + "/";

	if (!PX2_RM.IsFileFloderExist(wirteablePath + appPath))
	{
		PX2_RM.CreateFloder(wirteablePath, appPath);
	}

	_CreateSaveConfigXML(cfgs, cfgName);
}
//----------------------------------------------------------------------------
void Application::_CreateSaveConfigXML(std::map<std::string, std::string> &cfgs, 
	const std::string &cfgName)
{
	std::string wirteablePath = PX2_RM.GetWriteablePath();
	std::string projectsXML = wirteablePath + _GetWritePath(cfgName) + "/"
		+ "config.xml";

	XMLData data;
	data.Create();

	XMLNode rootNode = data.NewChild("config");

	auto it = cfgs.begin();
	for (; it != cfgs.end(); it++)
	{
		const std::string &name = it->first;
		const std::string &val = it->second;

		XMLNode node = rootNode.NewChild(name);
		node.SetAttributeString("name", name);
		node.SetAttributeString("value", val);
	}

	data.SaveFile(projectsXML);
}
//----------------------------------------------------------------------------
void Application::_LoadConfigs(std::map<std::string, std::string> &cfgs, 
	const std::string &projName)
{
	cfgs.clear();

	std::string wirteablePath = PX2_RM.GetWriteablePath();
	std::string projectsXML = wirteablePath + _GetWritePath(projName) + "/"
		+ "config.xml";

	XMLData data;
	if (data.LoadFile(projectsXML))
	{
		XMLNode rootNode = data.GetRootNode();

		XMLNode childNode = rootNode.IterateChild();
		while (!childNode.IsNull())
		{
			std::string name = childNode.AttributeToString("name");
			std::string value = childNode.AttributeToString("value");

			cfgs[name] = value;

			childNode = childNode.IterateChild(childNode);
		}
	}
}
//----------------------------------------------------------------------------