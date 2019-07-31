// PX2N_App.cpp

#include "PX2L_App.hpp"
#include "PX2L_Frame.hpp"
#include "PX2Application.hpp"
#include "PX2EditEventType.hpp"
#include "PX2RenderWindow.hpp"
#include "PX2EditEventType.hpp"
#include "PX2Memory.hpp"
#include "PX2UIFrame.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2AppInitConfig.hpp"
using namespace PX2;
using namespace Launcher;

//-----------------------------------------------------------------------------
IMPLEMENT_APP(Launcher::L_App)
//-----------------------------------------------------------------------------
L_App *L_App::msL_App = 0;
//-----------------------------------------------------------------------------
L_App::L_App() :
mIsDoStartUpdater(true),
mIsUpdaterStart(true),
mIsUpdaterOver(false)
{
	L_App::msL_App = this;
}
//-----------------------------------------------------------------------------
L_App::~L_App()
{
}
//-----------------------------------------------------------------------------
void L_App::_UpdateUpdater(const std::string &wwwAddr,
	const std::string &toDirPath)
{
	ResourceManager::ResourceFileTable mDataUpdateFileTable;

	std::string fileList_local = toDirPath + "/updatefilelist.xml";
	if (!PX2_RM.IsFileFloderExist(fileList_local))
	{
		mIsDoStartUpdater = false;
		return;
	}

	PX2_RM.LoadFileTableXML(mDataUpdateFileTable, fileList_local);

	std::string filelist_www = wwwAddr + "PHOENIXEngine/Updater/updatefilelist.xml";

	std::string filelistPrepare = toDirPath + "/updatefilelist_prepare.xml";
	std::string filelistToUpdate = toDirPath + "/updatefilelist_temp.xml";
	std::string filelist = toDirPath + "/updatefilelist.xml";

	int type = 1;

	if (!PX2_RM.Download(filelistPrepare, filelist_www, type)) // 预备下载
		return;

	PX2_LOG_INFO("Downloaded filelist_prepare %s", filelistPrepare.c_str());

	if (!PX2_RM.Download(filelistToUpdate, filelist_www, type)) // 下载
		return;

	PX2_LOG_INFO("Downloaded filelist_toupdate %s", filelistToUpdate.c_str());

	ResourceManager::ResourceFileTable tableToUpdate;
	PX2_RM.LoadFileTableXML(tableToUpdate, filelistToUpdate);
	int numUpdateFiles = (int)tableToUpdate.size();

	PX2_LOG_INFO("num %d files to update", numUpdateFiles);

	if (0 == numUpdateFiles)
	{
	}
	else
	{
		ResourceManager::ResourceFileTable newTable;
		auto itUpdate = tableToUpdate.begin();
		for (; itUpdate != tableToUpdate.end(); itUpdate++)
		{
			FString filename = itUpdate->first;
			std::string strFilename = filename;
			auto itFind = strFilename.find("PHOENIXEngine/Updater");
			if (itFind != std::string::npos)
			{
				ResourceFileInfoItem *itemUpdate = itUpdate->second;
				newTable[filename] = itemUpdate;

				bool isNeedDownload = false;

				std::string downloadFilename;
				auto it = mDataUpdateFileTable.find(filename);
				if (it == mDataUpdateFileTable.end())
				{
					isNeedDownload = true;
					downloadFilename = filename;
				}
				else
				{
					if (itemUpdate->MD5 != it->second->MD5)
					{
						isNeedDownload = true;
						downloadFilename = filename;
					}
					else
					{
						isNeedDownload = false;
					}
				}

				if (isNeedDownload)
				{
					std::string wwwURL = wwwAddr + downloadFilename;
					PX2_RM.Download("../" + downloadFilename, wwwURL, type);

					PX2_LOG_INFO("download from %s to %s", wwwURL.c_str(), downloadFilename);
				}
			}
		}

		// set table
		mDataUpdateFileTable = newTable;

		// down last

		PX2_LOG_INFO("download last from %s to %s", filelist_www.c_str(), filelist);
		if (!PX2_RM.Download(filelist, filelist_www, type))
			return;
	}
}
//-----------------------------------------------------------------------------
void L_App::_StartUpdater(const std::string &appNamePath, 
	const std::string &toDoStr)
{
	std::string renderTag = Renderer::GetRenderTag();
	if ("Dx9" == renderTag)
		renderTag = "";

	std::string exeName = appNamePath + renderTag;

	std::string appName;

	std::string strCfg = std::string(" ") + toDoStr;

#if defined(_WIN32) || defined(WIN32)

#ifdef _DEBUG

	#if defined(_WIN64) || defined(WIN64)
		appName = exeName + "64D.exe" + strCfg;
	#else
		appName = exeName + "D.exe" + strCfg;
	#endif

#else

	#if defined(_WIN64) || defined(WIN64)
		appName = exeName + "64.exe" + strCfg;
	#else
		appName = exeName + ".exe" + strCfg;
	#endif

#endif
		WinExec(appName.c_str(), SW_SHOW);
#endif

	Exit();
}
//-----------------------------------------------------------------------------
void L_App::_ReStart()
{
	Exit();
	_StartUpdater("Updater/Updater", "startlauncher");
}
//-----------------------------------------------------------------------------
void _BackgroundFun ()
{
	// 更新Updater
	if (L_App::msL_App->mIsDoStartUpdater)
	{
		L_App::msL_App->mIsUpdaterStart = true;

		std::string wwwwAddr = "ftp://182.254.213.85/";
		L_App::msL_App->_UpdateUpdater(wwwwAddr, "../PHOENIXEngine/Updater");

		if (L_App::msL_App->mIsDoStartUpdater)
			L_App::msL_App->_StartUpdater("Updater/Updater", "update_startlauncher");
	}

	L_App::msL_App->mIsUpdaterOver = true;
}
//-----------------------------------------------------------------------------
void L_App::OnEvent(PX2::Event *event)
{
	if (GraphicsES::IsEqual(event, GraphicsES::GeneralString))
	{
		std::string strData = event->GetData<std::string>();
		if ("restartlauncher" == strData)
		{
			_ReStart();
		}
	}
}
//-----------------------------------------------------------------------------
bool L_App::OnInit()
{
	wxApp::OnInit();

	Application *app = new Application();
	PX2_UNUSED(app);
	
	PX2_APP.SetConfigName("Launcher");
	
	AppInitConfig cfg;
	PX2_APP.Initlize(&cfg);
	PX2_APP.SetInEditor(true);
	PX2_EW.ComeIn(this);

	PX2_APP.Play(Application::PT_NONE);

	wxLog::SetLogLevel(0);

	wxImage::AddHandler(new wxBMPHandler());
	wxImage::AddHandler(new wxPNGHandler());
	wxImage::AddHandler(new wxGIFHandler());
	wxFileSystem::AddHandler(new wxArchiveFSHandler());
	wxXmlResource::Get()->InitAllHandlers();
	wxXmlResource::Get()->Load(wxT("DataNIRVANAwx/wxfbp/*.xrc"));

	L_Frame *frame = CreateMainFrame("Main");

#ifdef __WXMSW__
	frame->SetIcon(wxICON(launcherIcon));
#else
	frame->SetIcon(wxICON(launcherIcon));
#endif // __WXMSW__

	frame->Show(true);

	PX2_RM.BackgroundDoFun("UpdateUpdater", _BackgroundFun);

	return true;
}
//-----------------------------------------------------------------------------
int L_App::OnExit()
{
	PX2_EW.GoOut(this);
	PX2_APP.Terminate();

	Application *app = Application::GetSingletonPtr();
	delete app;
	Application::Set(0);

	return 0;
}
//-----------------------------------------------------------------------------
static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
	{ wxCMD_LINE_SWITCH, ("n"), ("noupdate"), ("not use update"),
		wxCMD_LINE_VAL_NONE, wxCMD_LINE_PARAM_OPTIONAL },
	{ wxCMD_LINE_NONE }
};
//-----------------------------------------------------------------------------
void L_App::OnInitCmdLine(wxCmdLineParser& parser)
{
	parser.SetDesc(g_cmdLineDesc);
	// must refuse '/' as parameter starter or cannot use "/path" style paths
	parser.SetSwitchChars(wxT("-"));
}
//-----------------------------------------------------------------------------
bool L_App::OnCmdLineParsed(wxCmdLineParser& parser)
{
	mIsDoStartUpdater = !parser.Found(wxT("n"));

	return true;
}
//-----------------------------------------------------------------------------
L_Frame *L_App::CreateMainFrame(const std::string &name)
{
	PX2_UNUSED(name);

	int displayWidth, displayHeight;
	wxDisplaySize(&displayWidth, &displayHeight);

	std::string title = "Launcher";
	std::string renderTag = Renderer::GetRenderTag();
	title += renderTag;

#if defined(_WIN64) || defined(WIN64)
	title += "64";
#endif

#ifdef _DEBUG
	title += "D";
#endif

	RenderWindow *rw = PX2_GR.GetMainWindow();
	rw->SetScreenSize(Sizef(440.0, 680.0f));
	rw->SetTitle(title);

	// load dll
	_LoadPlugin("LPlugin");

	// create wxFrame
	L_Frame *mainFrame = new L_Frame(rw, 0, wxDEFAULT_FRAME_STYLE);
	mainFrame->Show(false);
	SetTopWindow(mainFrame);
	mainFrame->Center();

	const Sizef &size = PX2_GR.GetMainWindow()->GetScreenSize();
	PX2_APP.SetPt_Data(mainFrame->GerRenderView()->GetHandle());
	PX2_APP.SetPt_Size(size);
	PX2_APP.InitlizeRenderer();

	mainFrame->Show(true);

	return mainFrame;
}
//-----------------------------------------------------------------------------
void L_App::_LoadPlugin(const std::string &plgName)
{
	std::string renderTag = Renderer::GetRenderTag();

	std::string debugTag = "";
#if defined (_DEBUG) 
	debugTag = "D";
#endif
	if (renderTag == "Dx9")
		renderTag = "";

	std::string x64Str = "";
#if defined(_WIN64) || defined(WIN64)
	x64Str = "64";
#endif

	std::string lpDllPath = "Data/" + plgName + "/" + plgName +
		renderTag + x64Str + debugTag + ".dll";
	PX2_PLUGINMAN.Load(lpDllPath);
}
//-----------------------------------------------------------------------------