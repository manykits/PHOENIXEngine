// PX2ApplicationInitTerm.cpp

#include "PX2Application.hpp"
#include "PX2Assert.hpp"
#include "PX2RendererInput.hpp"
#include "PX2LocalDateTime.hpp"
#include "PX2PluginManager.hpp"
#include "PX2ToLua.hpp"
#include "PX2NetInitTerm.hpp"
#include "PX2LuaContext.hpp"
#include "PX2LuaPlusContext.hpp"
#include "PX2LuaScriptController.hpp"
#include "PX2EngineSceneCanvas.hpp"
#include "PX2EngineUICanvas.hpp"
#include "PX2ProjectEventController.hpp"
#include "PX2ServerInfoManager.hpp"
#if defined PX2_USE_VOXEL
#include "PX2VoxelManager.hpp"
#endif
#include "PX2Bluetooth.hpp"
#include "PX2TimerManager.hpp"
#include "PX2ErrorHandler.hpp"
#include "PX2ThreadPool.hpp"
#include "PX2StringTokenizer.hpp"
#include "PX2DNS.hpp"
#include "PX2Arduino.hpp"
#include "PX2VoiceSDK.hpp"
#include "PX2FileIO.hpp"
#include "PX2StringTokenizer.hpp"
#include "PX2StringHelp.hpp"
#include "PX2Robot.hpp"
#include "PX2EngineNetDefine.hpp"
#include "PX2AppInitConfig.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Mutex sServerMutex;
//----------------------------------------------------------------------------
static std::string _sCmdString;
static bool _sIsHasInput = false;
static bool _sIsDoQuit = false;
void _InputThreadProc(void* data)
{
	PX2_UNUSED(data);

	std::cout << "_InputThreadProc start\n";

	while (!_sIsDoQuit)
	{
		bool isHasInput = false;
		{
			ScopedCS cs(&sServerMutex);
			isHasInput = _sIsHasInput;
		}
		if (!isHasInput)
		{
			{
				ScopedCS cs(&sServerMutex);
				std::cout << "please input your commond:\n";
			}
			
			char buffer[256];
			std::cin.getline(buffer, 256);
			std::string cmdbuf(buffer);
			_sCmdString = cmdbuf;

			{
				ScopedCS cs(&sServerMutex);
				_sIsHasInput = true;
			}
		}
	}
}
//----------------------------------------------------------------------------
void _AppClientConnectorRecvCallback(ClientConnector *connector,
	const std::string &recvStr)
{
	std::string name = connector->GetName();

	PX2_LOG_INFO("Name %s: Recved:%s", name.c_str(), recvStr.c_str());
}
//----------------------------------------------------------------------------
int _ProcessInputString(const std::string &buf)
{
	int ret = 0;
	std::string cmd;
	StringTokenizer stk(buf, " ");
	int count = (int)stk.Count();

	if (count > 0)
	{
		std::string cmd;
		std::string paramStr;
		std::string paramStr1;
		std::string paramStr2;
		std::string paramStr3;
		std::string paramStr4;
		std::string paramStr5;
		std::string paramStr6;

		if (stk.Count() > 0)
			cmd = stk[0];
		if (stk.Count() > 1)
			paramStr = stk[1];
		if (stk.Count() > 2)
			paramStr1 = stk[2];
		if (stk.Count() > 3)
			paramStr2 = stk[3];
		if (stk.Count() > 4)
			paramStr3 = stk[4];
		if (stk.Count() > 5)
			paramStr4 = stk[5];
		if (stk.Count() > 6)
			paramStr5 = stk[6];
		if (stk.Count() > 7)
			paramStr6 = stk[7];

		if ("quit" == cmd && count == 1)
		{
			ret = 1;
		}
		else if ("ask" == cmd)
		{
			wchar_t *unic = StringHelp::AnsiToUnicode(paramStr.c_str());
			const char *askStr = StringHelp::UnicodeToUTF8(unic);

			std::string strRet = PX2_VOICESDK.GetAnswer(askStr, true);
#if defined (_WIN32) || defined (WIN32) || defined(__LINUX__)
			const wchar_t *ansStr_w = StringHelp::UTF8ToUnicode(strRet.c_str());
			const char *ansStr = StringHelp::UnicodeToAnsi(ansStr_w);

			std::cout << ansStr << std::endl;
#endif
		}
		else if ("say" == cmd)
		{
			wchar_t *unic = StringHelp::AnsiToUnicode(paramStr.c_str());
			const char *sayStr = StringHelp::UnicodeToUTF8(unic);
			PX2_VOICESDK.Speak(sayStr);
		}
		else if ("music" == cmd)
		{
			wchar_t *unic = StringHelp::AnsiToUnicode(paramStr.c_str());
			const char *sayStr = StringHelp::UnicodeToUTF8(unic);
			PX2_VOICESDK.PlayMusic(sayStr);
		}
		else if ("musichot" == cmd)
		{
			PX2_VOICESDK.PlayHotMusic();
		}
		else if ("musicstop" == cmd)
		{
			PX2_SS.ClearAllSounds();
		}
		else if ("voicestart" == cmd)
		{
			PX2_VOICESDK.StartVoiceListening();
		}
		else if ("voicestop" == cmd)
		{
			PX2_VOICESDK.EndVoiceListening();
		}
		else if ("serial" == cmd)
		{
			std::string listStr;
	
			Serial serial;
			serial.UpdatePortList();
			int numPorts = serial.GetNumPorts();
			for (int i = 0; i<(int)numPorts; i++)
			{
				std::string port = serial.GetPort(i);
				std::string desc = serial.GetPortDesc(i);
				std::string portHardID = serial.GetPortHardID(i);

				listStr += port + "-" + desc + "-" + portHardID + "; ";
			}
			PX2_LOG_INFO("Serial list: %s", listStr.c_str());
		}
		else if ("net" == cmd)
		{
			// like this
			// net testname connect 127.0.0.1 8000
			std::string name = paramStr;

			if ("connect" == paramStr1)
			{
				GeneralClientConnector *cnt = PX2_APP.CreateGetGeneralClientConnector(
					name);
				if (cnt)
				{
					int port = StringHelp::StringToInt(paramStr3);
					cnt->ConnectB(paramStr2, port);

					cnt->AddRecvCallback(_AppClientConnectorRecvCallback);
				}
			}
			else if ("disconnect" == paramStr1)
			{
				GeneralClientConnector *cnt = PX2_APP.CreateGetGeneralClientConnector(
					name);
				if (cnt)
				{
					cnt->Disconnect();
				}
			}
			else if ("send" == paramStr1)
			{
				GeneralClientConnector *cnt = PX2_APP.CreateGetGeneralClientConnector(
					name);
				if (cnt)
				{
					cnt->SendString(paramStr2);
				}
			}
			else if ("sendraw" == paramStr1)
			{
				GeneralClientConnector *cnt = PX2_APP.CreateGetGeneralClientConnector(
					name);
				if (cnt)
				{
					cnt->SendRawBuffer(paramStr2.c_str(), paramStr2.length());
				}
			}
			else if ("ledscreen" == paramStr1)
			{
				GeneralClientConnector *cnt = PX2_APP.CreateGetGeneralClientConnector(
					name);
				if (cnt)
				{
					std::string cntStr = "text " + paramStr2;
					cnt->SendRawBuffer(cntStr.c_str(), cntStr.length());
				}
			}
		}

		Project *proj = Project::GetSingletonPtr();
		if (proj)
		{
			PX2_SC_LUA->CallString(std::string("engine_project_cmd") 
				+ "(\"" + cmd + "\"" 
				+ ", " + "\"" + paramStr + "\"" ", "
				+ "\"" + paramStr1 + "\"" + ", "
				+ "\"" + paramStr2 + "\")");
		}

		PX2_APP._CMDCallback(cmd, paramStr, paramStr1, paramStr2);
	}

	return ret;
}
//----------------------------------------------------------------------------
bool Application::Initlize(AppInitConfig *cfg)
{
	time_t ti; time(&ti); srand((unsigned int)ti);

#ifdef PX2_USE_MEMORY
	Memory::Initialize();
#endif

	StringHelp::Initlize();
	FString::Initlize();

	Logger *logger = new0 Logger();

#if defined(_WIN32) || defined(WIN32) || defined(__LINUX__)
	logger->AddFileHandler("PX2Application_Log.txt",
		LT_INFO | LT_ERROR | LT_USER);
#endif

#if defined (__LINUX__) || defined (_WIN32) || defined (WIN32)
	logger->AddConsoleHandler(LT_INFO | LT_ERROR | LT_USER);
#endif

	logger->AddOutputWindowHandler(LT_INFO | LT_ERROR | LT_USER);
	logger->StartLogger();

	LocalDateTime time;
	int year1 = time.Year();
	int month1 = time.Month();
	int week1 = time.Week();
	int day1 = time.Day();
	int dayOfWeek1 = time.DayOfWeek();
	int dayOfYear1 = time.DayOfYear();
	int hour1 = time.Hour();
	int minute1 = time.Minute();
	int second1 = time.Second();
	int millisecond1 = time.Millisecond();
	int microsecond1 = time.Microsecond();
	PX2_LOG_INFO("Year:%d; Month:%d; Week:%d; Day:%d; DayOfWeek:%d; DayOfYear:%d; Hour:%d; Minute:%d; Second:%d; Millisecond:%d; Microsecond:%d",
		year1, month1, week1, day1, dayOfWeek1, dayOfYear1, hour1, minute1, 
		second1, millisecond1, microsecond1);

	InitializeNetwork();

	mInputThread = new0 Thread();

	mDynLibMan = new0 DynLibManager();
	PX2_UNUSED(mDynLibMan);
	mPluginMan = new0 PluginManager();
	PX2_UNUSED(mPluginMan);

	mTimerMan = new0 TimerManager();

	mIMEDisp = new0 IMEDispatcher();

	mInputMan = new0 InputManager();

	mLanguageMan = new0 LanguageManager();
	mResMan = new0 ResourceManager();

	mEventWorld = new0 EventWorld();
	mEventWorld->ComeIn(this);

	mScriptMan = new0 ScriptManager();
	mScriptMan->AddContext(new0 LuaPlusContext());

	mBluetooth = new0 Bluetooth();
	mWifi = new0 Wifi();

	mHardCamera = new0 HardCamera();

	mRoot = new0 GraphicsRoot();
	mRoot->Initlize();

	mFontMan = new0 FontManager();
	mFontMan->Initlize();
	
	SoundSystemInitInfo info;
	if (cfg && cfg->IsInitSound)
	{
		mSoundSys = SoundSystem::Create(SoundSystem::ST_FMOD, info);
	}

	mADMan = new0 AddDeleteManager();
	PX2_UNUSED(mADMan);

	mSelectionMan = new0 SelectionManager();
	PX2_UNUSED(mSelectionMan);

	mURDoMan = new0 URDoManager();
	PX2_UNUSED(mURDoMan);

	mURStateMan = new0 URStateManager();
	PX2_UNUSED(mURStateMan);

	mFunObjectManager = new0 FunObjectManager();
	mFunObjectManager->Initlize();

	mAccoutManager = new0 AccoutManager();

	mUIAuiManager = new0 UIAuiManager();

	mUISkinManager = new0 UISkinManager();

	mLogicManager = new0 LogicManager();

#if defined PX2_USE_VOXEL
	mVoxelManager = new0 VoxelManager();
	mVoxelManager->Initlize(VoxelManager::T_TEX);
#endif

	mCreater = new0 Creater();

	mVoiceSDK = new0 VoiceSDK();
	if (cfg && cfg->IsInitVoice)
	{
		mVoiceSDK->Initlize();
	}

	VLC::Initlize();

	mSTEAMEduManager = new0 STEAMEduManager();
	mSTEAMEduManager->Initlize();

	Server::ServerType st;
#if defined (WIN32) || defined (_WIN32)
	st = Server::ST_IOCP;
#else
	st = Server::ST_POLL;
#endif
	st = Server::ST_POLL;
	mEngineServer = new0 EngineServer(st, EngineServerPort);
	mEngineServer->Start();
	mEngineClient = new0 EngineClientConnector();
	mEngineClient->SetBroadcastInfo(true);

	LuaPlusContext *luaContext = (LuaPlusContext*)PX2_SC_LUA;

	// Lua
	tolua_PX2_open((lua_State*)luaContext->GetLuaState());
	LuaPlus::LuaState *luaPlusState = luaContext->GetLuaPlusState();
	PX2_UNUSED(luaPlusState);
	LuaScriptController::RegisterScriptClass();

	PX2_SC_LUA->SetUserTypePointer("PX2_GR", "GraphicsRoot", GraphicsRoot::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_APP", "Application", Application::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_LM_ENGINE", "LanguagePackage", LanguageManager::GetSingletonPtr()->GetPackageEngine());
	PX2_SC_LUA->SetUserTypePointer("PX2_LM_EDITOR", "LanguagePackage", LanguageManager::GetSingletonPtr()->GetPackageEditor());
	PX2_SC_LUA->SetUserTypePointer("PX2_LM_APP", "LanguagePackage", LanguageManager::GetSingletonPtr()->GetPackageApp());
	PX2_SC_LUA->SetUserTypePointer("PX2_LM_APP1", "LanguagePackage", LanguageManager::GetSingletonPtr()->GetPackageApp1());
	PX2_SC_LUA->SetUserTypePointer("PX2_LOGGER", "Logger", Logger::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_RM", "ResourceManager", ResourceManager::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_SM", "ScriptManager", ScriptManager::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_SC_LUA", "LuaContext", PX2_SC_LUA);
	PX2_SC_LUA->SetUserTypePointer("PX2_CREATER", "Creater", Creater::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_SELECTM", "SelectionManager", SelectionManager::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_SELECTM_D", "Selection", SelectionManager::GetSingleton().GetSelecton("Default"));
	PX2_SC_LUA->SetUserTypePointer("PX2_SELECTM_E", "Selection", SelectionManager::GetSingleton().GetSelecton("Editor"));
	PX2_SC_LUA->SetUserTypePointer("PX2_URDOM", "URDoManager", URDoManager::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_LOGICM", "LogicManager", LogicManager::GetSingletonPtr());
#if defined PX2_USE_VOXEL
	PX2_SC_LUA->SetUserTypePointer("PX2_VOXELM", "VoxelManager", VoxelManager::GetSingletonPtr());
#endif
	PX2_SC_LUA->SetUserTypePointer("PX2_BLUETOOTH", "Bluetooth", Bluetooth::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_WIFI", "Wifi", Wifi::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_SS", "SoundSystem", SoundSystem::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_HARDCAMERA", "HardCamera", HardCamera::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_VOICESDK", "VoiceSDK", VoiceSDK::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_TIMERM", "TimerManager", TimerManager::GetSingletonPtr());
	PX2_SC_LUA->SetUserTypePointer("PX2_PFSDK", "PlatformSDK", PlatformSDK::GetSingletonPtr());
	// end Lua

	Canvas *mainCanvas = new0 Canvas();
	mainCanvas->SetMain(true);
	mainCanvas->CreateUICameraNode();
	mainCanvas->EnableAnchorLayout(true);
	mainCanvas->SetAnchorHor(0.0f, 1.0f);
	mainCanvas->SetAnchorVer(0.0f, 1.0f);
	mainCanvas->ComeInEventWorld();
	mainCanvas->SetName("MainCanvas");

	mItem = new0 MenuItem();
	mItem->Name = "ExtendMenu";
	mItem->TheType = Application::MenuItem::T_SUB;

	RenderWindow *rw = PX2_GR.GetMainWindow();
	rw->SetMainCanvas(mainCanvas);

	PX2_SC_LUA->CallFile("Data/engine/scripts/lua/engine_start.lua");

	mLogicManager->Initlize();

	PX2_LOG_INFO("End Application::Initlize.");

	LoadBoost("Data/boost.xml");

	PX2_SC_LUA->CallFile("Data/boost.lua");

	mHostName = DNS::GetHostName();

	Application::_LoadConfigs(mCFGs, mConfigName);

	return true;
}
//----------------------------------------------------------------------------
void Application::InitlizeDefaultEngineCanvas(Canvas *parentCanvas)
{
	mEngineCanvas = new0 EngineCanvas();
	parentCanvas->AttachChild(mEngineCanvas);
}
//----------------------------------------------------------------------------
Renderer * Application::InitlizeRenderer()
{
	Renderer *defRenderer = CreateRenderer("DefaultRenderer", mPt_Data,
		mPt_Data1, mPt_Data2, (int)mPt_Size.Width, (int)mPt_Size.Height, 0);
	Renderer::SetDefaultRenderer(defRenderer);
	PX2_GR.GetMainWindow()->SetRenderer(defRenderer);

	mScreenSize = mPt_Size;

	return defRenderer;
}
//----------------------------------------------------------------------------
Renderer *Application::CreateRenderer(const std::string &name, void *winHandle,
	void *ptDataLinuxWindow, void *ptDataLinuxVisual,
	int width, int height, int numMultisamples)
{
	RendererInput *rendererInput = 0;
	Renderer *renderer = Renderer::CreateRenderer(winHandle, ptDataLinuxWindow,
		ptDataLinuxVisual, width, height, numMultisamples, rendererInput);
	renderer->SetName(name);

	mRenderersMap[name] = renderer;
	mRendererInputMap[name] = rendererInput;

	return renderer;
}
//----------------------------------------------------------------------------
RendererInput *Application::GetRendererInput(const std::string &name)
{
	auto it = mRendererInputMap.find(name);
	if (it != mRendererInputMap.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
Renderer *Application::GetRenderer(const std::string &name)
{
	auto it = mRenderersMap.find(name);
	if (it != mRenderersMap.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
int Application::GetLocalAddressSize()
{
	if (mLocalAddresses.empty())
	{
		_RefreshLocalAddress();
	}

	return (int)mLocalAddresses.size();
}
//----------------------------------------------------------------------------
IPAddress Application::GetLocalAddress(int i)
{
	if (mLocalAddresses.empty())
	{
		_RefreshLocalAddress();
	}

	if (0 <= i && i < (int)mLocalAddresses.size())
	{
		return mLocalAddresses[i];
	}

	return IPAddress();
}
//----------------------------------------------------------------------------
std::string Application::GetLocalAddressStr(int i)
{
	return GetLocalAddress(i).ToString();
}
//----------------------------------------------------------------------------
void Application::_RefreshLocalAddress()
{
	mLocalAddresses.clear();

	HostEntry hostEntry = DNS::GetThisHost();
	HostEntry::AddressList addressList = hostEntry.GetAddresses();
	if (addressList.size() > 0)
	{
		for (int i = 0; i < (int)addressList.size(); i++)
		{
			IPAddress ipAddress = addressList[i];
			mLocalAddresses.push_back(ipAddress);
		}
	}
}
//----------------------------------------------------------------------------
void Application::_ShutdownClientConnectors()
{
	auto it = mGeneralClientConnectors.begin();
	for (; it != mGeneralClientConnectors.end(); it++)
	{
		GeneralClientConnector *cnt = it->second;
		if (cnt)
		{
			cnt->Disconnect();
		}
	}

	mGeneralClientConnectors.clear();
}
//----------------------------------------------------------------------------
void Application::_ShutdownGeneralServers()
{
	for (int i = 0; i < (int)mGeneralServers.size(); i++)
	{
		GeneralServer *server = mGeneralServers[i];
		if (server)
		{
			server->Shutdown();
		}
	}

	mGeneralServers.clear();
}
//----------------------------------------------------------------------------
void Application::SetInEditor(bool isInEditor)
{
	mIsInEditor = isInEditor;
	PX2_GR.SetInEditor(isInEditor);

#if defined (_WIN32) || defined (WIN32) || defined (__ANDROID__) || defined(__LINUX__)
	if (!mIsInEditor)
	{
		GetEngineClientConnector()->Enable(true);
		CreateEngineUDPServer();

		mInputThread->Start(_InputThreadProc);
	}
	else
	{
		GetEngineClientConnector()->Enable(false);
		CreateEngineUDPServerEditor();
	}
#endif
}
//----------------------------------------------------------------------------
void Application::SetQuit(bool quit)
{
	mIsQuit = quit;
}
//----------------------------------------------------------------------------
bool Application::IsQuit() const
{
	return mIsQuit;
}
//----------------------------------------------------------------------------
EngineServer *Application::GetEngineServer()
{
	return mEngineServer;
}
//----------------------------------------------------------------------------
EngineClientConnector *Application::GetEngineClientConnector()
{
	return mEngineClient;
}
//----------------------------------------------------------------------------
UDPServer *Application::CreateEngineUDPServer()
{
	mEngineUDPServer = 0;

	IPAddress ipAddr;
	SocketAddress udpAddr(ipAddr, EngineUDPPortClient);

	mEngineUDPServer = new0 UDPServer(udpAddr);
	mEngineUDPServer->AddRecvCallback(UDPNetInfo::UDPServerRecvCallback);
	mEngineUDPServer->Start();

	return mEngineUDPServer;
}
//----------------------------------------------------------------------------
UDPServer *Application::GetEngineUDPServer()
{
	return mEngineUDPServer;
}
//----------------------------------------------------------------------------
UDPServer *Application::CreateEngineUDPServerEditor()
{
	mEngineDUPServerEditor = 0;

	SocketAddress udpAddr(EngineUDPPortServerEditor);
	
	mEngineDUPServerEditor = new0 UDPServer(udpAddr);
	mEngineDUPServerEditor->AddRecvCallback(UDPNetInfo::UDPServerRecvCallback);
	mEngineDUPServerEditor->Start();

	return mEngineDUPServerEditor;
}
//----------------------------------------------------------------------------
UDPServer *Application::GetEngineUDPServerEditor()
{
	return mEngineDUPServerEditor;
}
//----------------------------------------------------------------------------
GeneralServer *Application::CreateGeneralServer(int port,
	int numMaxConnects, int numMaxMsgHandlers)
{
	Server::ServerType st = Server::ST_POLL;
#if defined (WIN32) || defined (_WIN32)
	st = Server::ST_IOCP;
#endif
	GeneralServer *server = new0 GeneralServer(st, port, numMaxConnects,
		numMaxMsgHandlers);
	mGeneralServers.push_back(server);

	return server;
}
//----------------------------------------------------------------------------
bool Application::ShutdownGeneralServer(GeneralServer *generalServer)
{
	if (generalServer)
	{
		generalServer->Shutdown();

		auto it = mGeneralServers.begin();
		for (; it != mGeneralServers.end(); it++)
		{
			if (*it == generalServer)
			{
				mGeneralServers.erase(it);
				return true;
			}
		}
	}

	return false;
}
//----------------------------------------------------------------------------
GeneralClientConnector *Application::CreateGetGeneralClientConnector(
	const std::string &name)
{
	GeneralClientConnector *cnt = GetGeneralClientConnector(name);
	if (cnt)
		return cnt;

	cnt = new0 GeneralClientConnector();
	cnt->SetName(name);
	mGeneralClientConnectors[name] = cnt;
	return cnt;
}
//----------------------------------------------------------------------------
GeneralClientConnector *Application::GetGeneralClientConnector(
	const std::string &name)
{
	auto it = mGeneralClientConnectors.find(name);
	if (it != mGeneralClientConnectors.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
bool Application::ShutdownGeneralClientConnector(const std::string &name)
{
	auto it = mGeneralClientConnectors.find(name);
	if (it == mGeneralClientConnectors.end())
	{
		return false;
	}

	it->second->Disconnect();
	mGeneralClientConnectors.erase(it);

	return true;
}
//----------------------------------------------------------------------------
void Application::_UpdateGeneralServerConnectors(float elapseSeconds)
{
	for (int i = 0; i < (int)mGeneralServers.size(); i++)
	{
		GeneralServer *gs = mGeneralServers[i];
		if (gs)
		{
			gs->Run((float)elapseSeconds);
		}
	}

	auto it = mGeneralClientConnectors.begin();
	for (; it != mGeneralClientConnectors.end(); it++)
	{
		GeneralClientConnector *cnt = it->second;
		if (cnt)
		{
			cnt->Update((float)elapseSeconds);
		}
	}
}
//----------------------------------------------------------------------------
void Application::WillEnterForeground(bool isFirstTime)
{
	PX2_LOG_INFO("WillEnterForeground %s", isFirstTime ? "true":"false");
	PX2_LOG_INFO("IsInBackground %s", mIsInBackground ? "true" : "false");

	if (!mIsInBackground) return;

	if (isFirstTime)
	{
		PX2_FM.Initlize();
	}
	else
	{
		Renderer::GetDefaultRenderer()->OnRestoreDevice();

		PX2_FM.SetNeedUpdate();
	}

	PX2_BLUETOOTH.ClearSendReceives();

	Event *ent = PX2_CREATEEVENTEX(ProjectES, WillEnterForeground);
	PX2_EW.BroadcastingLocalEvent(ent);

	mIsInBackground = false;
}
//----------------------------------------------------------------------------
void Application::DidEnterBackground()
{
	PX2_LOG_INFO("DidEnterBackground");

	Renderer::GetDefaultRenderer()->OnLostDevice();

	Event *ent = PX2_CREATEEVENTEX(ProjectES, DidEnterBackground);
	PX2_EW.BroadcastingLocalEvent(ent);

	mIsInBackground = true;
}
//----------------------------------------------------------------------------
bool Application::Terminate()
{
	Play(Application::PT_NONE);
	CloseProject();

	PX2_PLUGINMAN.UnloadPlugins();

	PX2_SC_LUA->CallFile("Data/engine/scripts/lua/engine_end.lua");

	mItem = 0;

	mEngineCanvas = 0;

	mUDPNetInfos.clear();
	_ShutdownClientConnectors();
	_ShutdownGeneralServers();
	mLocalAddresses.clear();

	PX2_EW.Shutdown(true);

	ErrorHandler *eh = ErrorHandler::GetSingletonPtr();
	if (eh)
	{
		delete0(eh);
		ErrorHandler::Set(0);
	}

	mScriptMan->TernimateAll();
	if (mScriptMan)
	{
		delete0(mScriptMan);
		ScriptManager::Set(0);
	}

	if (mEngineServer)
	{
		mEngineServer->Shutdown();
		mEngineServer = 0;
	}

	if (mEngineUDPServer)
	{
		mEngineUDPServer = 0;
	}

	if (mEngineDUPServerEditor)
	{
		mEngineDUPServerEditor = 0;
	}

	if (mEngineClient)
	{
		mEngineClient->Disconnect();
		mEngineClient = 0;
	}

	if (mCreater)
	{
		delete0(mCreater);
		Creater::Set(0);
	}

	if (mVoiceSDK)
	{
		delete0(mVoiceSDK);
		VoiceSDK::Set(0);
	}

	if (mSTEAMEduManager)
	{
		mSTEAMEduManager->Terminate();
		delete0(mSTEAMEduManager);
		STEAMEduManager::Set(0);
	}

#if defined PX2_USE_VOXEL
	if (mVoxelManager)
	{
		delete0(mVoxelManager);
		VoxelManager::Set(0);
	}
#endif

	if (mLogicManager)
	{
		delete0(mLogicManager);
		LogicManager::Set(0);
	}

	if (mUISkinManager)
	{
		delete0(mUISkinManager);
		UISkinManager::Set(0);
	}

	if (mUIAuiManager)
	{
		delete0(mUIAuiManager);
		UIAuiManager::Set(0);
	}

	if (mSelectionMan)
	{
		mSelectionMan->Clear();
		delete0(mSelectionMan);
		SelectionManager::Set(0);
	}

	if (mURDoMan)
	{
		delete0(mURDoMan);
		URDoManager::Set(0);
	}

	PX2_SVRINFOMAN.Clear();

	if (mURStateMan)
	{
		delete0(mURStateMan);
		URStateManager::Set(0);
	}

	if (mADMan)
	{
		mADMan->Clear();
		delete0(mADMan);
		AddDeleteManager::Set(0);
	}

	if (mFunObjectManager)
	{
		mFunObjectManager->Terminate();
		delete0(mFunObjectManager);
		FunObjectManager::Set(0);
	}

	if (mSoundSys)
	{
		delete0(mSoundSys);
		SoundSystem::Set(0);
	}

	if (mAccoutManager)
	{
		delete0(mAccoutManager);
		AccoutManager::Set(0);
	}

	if (mFontMan)
	{
		delete0(mFontMan);
		FontManager::Set(0);
	}

	bool isInEditor = mRoot->IsInEditor();
	PX2_UNUSED(isInEditor);
	if (mRoot)
	{
		mRoot->Terminate();
		delete0(mRoot);
		GraphicsRoot::Set(0);
	}

	if (mInputMan)
	{
		delete0(mInputMan);
		mInputMan = 0;
		InputManager::Set(0);
	}

	if (mResMan)
	{
		mResMan->Clear();
		delete0(mResMan);		
		ResourceManager::Set(0);
	}

	if (mBluetooth)
	{
		delete0(mBluetooth);
		Bluetooth::Set(0);
	}

	if (mWifi)
	{
		delete0(mWifi);
		Wifi::Set(0);
	}

	if (mHardCamera)
	{
		delete0(mHardCamera);
		HardCamera::Set(0);
	}

	if (mLanguageMan)
	{
		delete0(mLanguageMan);
		LanguageManager::Set(0);
	}

	if (mIMEDisp)
	{ // 需要在资源管理器之后释放
		delete0(mIMEDisp);
		IMEDispatcher::Set(0);
	}

	if (mEventWorld)
	{
		mEventWorld->GoOut(this);
		delete0(mEventWorld);
		EventWorld::Set(0);
	}

	VLC::Ternimate();

	if (mTimerMan)
	{
		delete0(mTimerMan);
		TimerManager::Set(0);
	}

	std::map<std::string, Renderer*>::iterator it = mRenderersMap.begin();
	for (; it != mRenderersMap.end(); it++)
	{
		Renderer *renderer = it->second;
		delete0(renderer);
	}
	Renderer::SetDefaultRenderer(0);
	mRenderersMap.clear();

	std::map<std::string, RendererInput*>::iterator itInput = mRendererInputMap.begin();
	for (; itInput != mRendererInputMap.end(); itInput++)
	{
		RendererInput *rendererInput = itInput->second;
		delete0(rendererInput);
	}
	mRendererInputMap.clear();

	PX2_PLUGINMAN.UnloadPlugins();
	if (mPluginMan)
	{
		delete0(mPluginMan);
		PluginManager::Set(0);
	}

	if (mDynLibMan)
	{
		delete0(mDynLibMan);
		DynLibManager::Set(0);
	}

	System::SleepSeconds(0.1f);
	mInputThread = 0;

	TerminateNetwork();

	PX2_LOG_INFO("Engine Terminate done.");
	Logger *logger = Logger::GetSingletonPtr();
	if (logger)
	{
		delete0(logger);
		Logger::Set(0);
	}

	FString::Ternimate();

#ifdef PX2_USE_MEMORY

	if (isInEditor)
	{
		Memory::Terminate("Editor_MemoryReport.txt", false);
	}
	else
	{
		Memory::Terminate("PX2Application_MemoryReport.txt", false);
	}

#endif

	return true;
}
//----------------------------------------------------------------------------
void Application::ClearAppCmdCallback()
{
	mAppCmdCallbacks.clear();
}
//----------------------------------------------------------------------------
bool Application::IsHasAppCmdCallback(AppCmdCallback callback)
{
	auto it = mAppCmdCallbacks.begin();
	for (; it != mAppCmdCallbacks.end(); it++)
	{
		if (callback == *it)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool Application::AddAppCmdCallback(AppCmdCallback callback)
{
	if (IsHasAppCmdCallback(callback))
		return false;

	mAppCmdCallbacks.push_back(callback);
	return true;
}
//----------------------------------------------------------------------------
bool Application::RemoveAppCmdCallback(AppCmdCallback callback)
{
	auto it = mAppCmdCallbacks.begin();
	for (; it != mAppCmdCallbacks.end(); it++)
	{
		if (callback == *it)
		{
			mAppCmdCallbacks.erase(it);
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void Application::_CMDCallback(const std::string &cmdStr,
	const std::string &paramStr, const std::string &paramStr1,
	const std::string &paramStr2)
{
	auto it = mAppCmdCallbacks.begin();
	for (; it != mAppCmdCallbacks.end(); it++)
	{
		if (*it)
		{
			(*it)(cmdStr, paramStr, paramStr1, paramStr2);
		}
	}
}
//----------------------------------------------------------------------------
bool Application::LoadBoost(const std::string &filename)
{
	XMLData data;

	int bufferSize = 0;
	char *buffer = 0;
	ResourceManager::GetSingleton().LoadBuffer(filename, bufferSize, buffer);
	if (!buffer || bufferSize == 0) 
	{
		mBoostInfo.BoostSize = Sizef(1136.0f, 600.0f);
		return false;
	}

	if (data.LoadBuffer(buffer, bufferSize))
	{
		XMLNode rootNode = data.GetRootNode();
		XMLNode nodeChild = rootNode.IterateChild();
		while (!nodeChild.IsNull())
		{
			const std::string &name = nodeChild.GetName();
			if ("app" == name)
			{
				mBoostInfo.BoostSize.Width = data.GetNodeByPath("app.config.var").AttributeToFloat("width");
				mBoostInfo.BoostSize.Height = data.GetNodeByPath("app.config.var").AttributeToFloat("height");
				mBoostInfo.ThePlayLogicMode = _StrToPlayLogicMode(data.GetNodeByPath("app.play.var").AttributeToString("playlogicmode"));
				mBoostInfo.IsShowInfo = data.GetNodeByPath("app.play.var").AttributeToBool("isshowinfo");
				mBoostInfo.IsDataReWriteToDataUpdate = data.GetNodeByPath("app.play.var").AttributeToBool("isdatawrite");
			}

			nodeChild = rootNode.IterateChild(nodeChild);
		}

		// local projects
		std::string strBuf;
		PX2_RM.LoadBuffer("Data/project.list", strBuf);
		PX2_RM.ClearRes("Data/project.list");
		if (!strBuf.empty())
		{
			_ReadInfosFromCnt(strBuf, mBoostInfo.Projects);
		}
		// update projects
		std::string writeablePath = PX2_RM.GetWriteablePath();
		std::string writeDataPath = writeablePath + "DataUpdate/";
		if (PX2_RM.IsFileFloderExist(writeDataPath))
		{
			std::string projectList = writeDataPath + "project.list";
			char *buf = 0;
			int bufSize = 0;
			if (FileIO::Load(projectList, true, bufSize, buf))
			{
				std::string bufStr1(buf, bufSize);
				_ReadInfosFromCnt(bufStr1, mBoostInfoUpdate.Projects);
				delete1(buf);
				bufSize = 0;
			}
		}

		// local plugins
		PX2_RM.LoadBuffer("Data/plugin.list", strBuf);
		PX2_RM.ClearRes("Data/plugin.list");
		if (!strBuf.empty())
		{
			_ReadInfosFromCnt(strBuf, mBoostInfo.Plugins);
		}

		// boost.list
		std::string strBufBoost;
		PX2_RM.LoadBuffer("Data/boost.list", strBufBoost);
		PX2_RM.ClearRes("Data/boost.list");
		if (!strBufBoost.empty())
		{
			std::string removeRStr;
			for (int i = 0; i < (int)strBufBoost.size(); i++)
			{
				char ch = strBufBoost[i];
				if ('\r' == ch)
				{
					/*_*/
				}
				else
				{
					removeRStr.push_back(ch);
				}
			}

			StringTokenizer stk(removeRStr, "\n");
			if (stk.Count() > 0)
			{
				mBoostInfo.ProjectName = stk[0];
			}
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
std::string Application::GetPlayLogicModeStr() const
{
	if (AppBoostInfo::PLM_SIMPLE == mBoostInfo.ThePlayLogicMode)
		return "simple";

	return "logic";
}
//----------------------------------------------------------------------------
AppBoostInfo::PlayLogicMode Application::_StrToPlayLogicMode(
	const std::string &str)
{
	if ("simple" == str)
		AppBoostInfo::PLM_SIMPLE;

	return AppBoostInfo::PLM_LOGIC;
}
//----------------------------------------------------------------------------
bool Application::IsShowInfo() const
{
	return mBoostInfo.IsShowInfo;
}
//----------------------------------------------------------------------------
void Application::SetBoostProjectName(const std::string &boostProjectName)
{
	mBoostInfo.ProjectName = boostProjectName;
}
//----------------------------------------------------------------------------
void Application::SetBoostSize(const Sizef &size)
{
	mBoostInfo.BoostSize = size;
}
//----------------------------------------------------------------------------
void Application::SetBoostSize(float width, float height)
{
	mBoostInfo.BoostSize.Width = width;
	mBoostInfo.BoostSize.Height = height;
}
//----------------------------------------------------------------------------
void Application::SetPlayLogicMode(AppBoostInfo::PlayLogicMode mode)
{
	mBoostInfo.ThePlayLogicMode = mode;
}
//----------------------------------------------------------------------------
void Application::SetShowInfo(bool show)
{
	mBoostInfo.IsShowInfo = show;
}
//----------------------------------------------------------------------------
bool Application::WriteBoost()
{
	XMLData data;

	data.Create();

	XMLNode boostNode = data.NewChild("boost");
	boostNode.SetAttributeString("name", "boost");

	// app Node
	XMLNode appNode = boostNode.NewChild("app");

	// config
	XMLNode configNode = appNode.NewChild("config");

	XMLNode varNode_config = configNode.NewChild("var");
	varNode_config.SetAttributeInt("width", (int)mBoostInfo.BoostSize.Width);
	varNode_config.SetAttributeInt("height", (int)mBoostInfo.BoostSize.Height);

	// play
	XMLNode playNode = appNode.NewChild("play");
	XMLNode varNode_play = playNode.NewChild("var");
	varNode_play.SetAttributeString("playlogicmode", GetPlayLogicModeStr());
	varNode_play.SetAttributeBool("isshowinfo", mBoostInfo.IsShowInfo);
	varNode_play.SetAttributeBool("isdatawrite", mBoostInfo.IsDataReWriteToDataUpdate);

	data.SaveFile("Data/boost.xml");

	_WriteInfos("Data/project.list", mBoostInfo.Projects);
	_WriteInfos("Data/plugin.list", mBoostInfo.Plugins);

	return true;
}
//----------------------------------------------------------------------------
std::set<std::string> Application::GetAllProjects()
{
	std::set<std::string> allProjects;

	for (int i = 0; i < (int)mBoostInfo.Projects.size(); i++)
	{
		if (!mBoostInfo.Projects[i].empty())
			allProjects.insert(mBoostInfo.Projects[i]);
	}

	for (int i = 0; i < (int)mBoostInfoUpdate.Projects.size(); i++)
	{
		if (!mBoostInfoUpdate.Projects[i].empty())
			allProjects.insert(mBoostInfoUpdate.Projects[i]);
	}

	return allProjects;
}
//----------------------------------------------------------------------------
bool Application::IsProjectUpdated(const std::string &name) const
{
	for (int i = 0; i < (int)mBoostInfoUpdate.Projects.size(); i++)
	{
		if (name == mBoostInfoUpdate.Projects[i])
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Application::_ReadInfosFromCnt(const std::string &cntStr1,
	std::vector<std::string> &projList)
{
	std::string cntStr;
	for (int i = 0; i < (int)cntStr1.size(); i++)
	{
		if ('\r' != cntStr1[i])
			cntStr += cntStr1[i];
	}

	StringTokenizer stk(cntStr, "\n");
	for (int i = 0; i < stk.Count(); i++)
	{
		std::string str = stk[i];
		if (!str.empty())
		{
			projList.push_back(str);
		}
	}
}
//----------------------------------------------------------------------------
void Application::_WriteInfos(const std::string &path,
	const std::vector<std::string> &list)
{
	std::string bufStr;

	for (int i = 0; i < (int)list.size(); i++)
	{
		std::string projName = list[i];
		if (!projName.empty())
			bufStr += projName + "\n";
	}

	if (bufStr.empty())
		bufStr = "\n";

	FileIO::Save(path, false, bufStr.length(), bufStr.c_str());
}
//----------------------------------------------------------------------------
void Application::SetScreenSize(const Sizef &screenSize)
{
	PX2_LOG_INFO("Application::SetScreenSize Width:%.2f, Height:%.2f", 
		screenSize.Width, screenSize.Height);

	mScreenSize = screenSize;
	PX2_GR.GetMainWindow()->SetScreenSize(mScreenSize);
	PX2_INPUTMAN.GetDefaultListener()->SetViewSize(mScreenSize);
}
//----------------------------------------------------------------------------
void Application::Update(float appSeconds, float elapsedSeconds)
{
	bool isHasInput = false;
	{
		ScopedCS cs(&sServerMutex);
		isHasInput = _sIsHasInput;
	}
	
	if (isHasInput)
	{
		int ret = _ProcessInputString(_sCmdString);
		if (0 == ret)
		{
			ScopedCS cs(&sServerMutex);
			 _sIsHasInput = false;

		}
		if (1 == ret)
		{
			PX2_LOG_INFO("quit...");
			_sIsDoQuit = true;
			this->SetQuit(true);
		}
	}

	// event
	if (mEventWorld)
		mEventWorld->Update((float)elapsedSeconds);

	// resource
	PX2_RM.Update(appSeconds, elapsedSeconds);

	// font
	PX2_FM.Update();

	if (mSoundSys)
		mSoundSys->Update(appSeconds, elapsedSeconds);

	// Plugin
	PX2_PLUGINMAN.Update();

	PX2_ADM.Update((float)elapsedSeconds);

	PX2_TIMERM.Update((float)appSeconds);

	if (mSTEAMEduManager)
		mSTEAMEduManager->Update((float)elapsedSeconds);

	// graph
	PX2_GR.Update(appSeconds, elapsedSeconds);

	PX2_BLUETOOTH.Update((float)elapsedSeconds);
	PX2_WIFI.Update((float)elapsedSeconds);
	Serial *ser = Serial::GetDefaultSerial();
	if (ser) ser->Update();

	if (mEngineServer)
		mEngineServer->Run((float)elapsedSeconds);

	if (mEngineClient)
		mEngineClient->Update((float)elapsedSeconds);

	if (mEngineUDPServer)
		mEngineUDPServer->Update((float)elapsedSeconds);

	if (mEngineDUPServerEditor)
		mEngineDUPServerEditor->Update((float)elapsedSeconds);

	_UpdateGeneralServerConnectors((float)elapsedSeconds);
	_UpdateUDPNetInfos((float)elapsedSeconds);

	std::string strAppSeconds = StringHelp::FloatToString(appSeconds, 6);
	std::string strElapsedSeconds = StringHelp::FloatToString(elapsedSeconds, 6);
	PX2_SC_LUA->CallString(std::string("engine_update") + "(\"" + strAppSeconds + "\""
		", " + "\"" + strElapsedSeconds + "\")");

	if (Project::GetSingletonPtr())
	{
		PX2_SC_LUA->CallString(std::string("engine_project_update") + "(\"" + strAppSeconds + "\""
			", " + "\"" + strElapsedSeconds + "\")");
	}

	if (mIsInBackground) return;

	PX2_GR.Draw();
}
//----------------------------------------------------------------------------