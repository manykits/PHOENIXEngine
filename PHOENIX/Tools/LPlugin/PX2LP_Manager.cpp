// PX2LP_Manager.cpp

#include "PX2LP_Manager.hpp"
#include "PX2EventWorld.hpp"
#include "PX2StringHelp.hpp"
#include "PX2Project.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2UIFText.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2InputEvent.hpp"
#include "PX2InputEventData.hpp"
#include "PX2Application.hpp"
#include "PX2NetInitTerm.hpp"
#include "PX2UIWebFrame.hpp"
#include "PX2EngineNetEvent.hpp"
#include "PX2Dir.hpp"
#include "PX2LanguageManager.hpp"
#include "PX2LP_HandlerFactory.hpp"
#include "PX2EngineNetDefine.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2LanguageManager.hpp"
using namespace PX2;

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#include "shellapi.h "
#endif
//----------------------------------------------------------------------------
LP_Manager::LP_Manager()
:mSceneOrientation(1),
mProjHeight(500),
mProjWidth(500),
mIsStartNoWindow(false)
{
}
//----------------------------------------------------------------------------
LP_Manager::~LP_Manager()
{
}
//----------------------------------------------------------------------------
void _UICallback (UIFrame *frame, UICallType type)
{
	const std::string &name = frame->GetName();
	UITabFrame *tabFrame = DynamicCast<UITabFrame>(frame);

	if (tabFrame && "TheTabFrame" == name)
	{
		if (UICT_TABFRAME_SETACTIVE == type)
		{
		}
	}
	if (UICT_RELEASED == type)
	{
		if ("OpenProject" == name)
		{
#if defined(_WIN32) || defined(WIN32)
			CHAR szPath[MAX_PATH];
			GetCurrentDirectory(sizeof(szPath), szPath);

			UIItem *item = LP_Manager::GetSingleton().mProjectList->GetSelectedItem();
			if (item)
			{
				const std::string &name = item->GetName();
				if (!name.empty())
				{
					std::string fullPath = szPath + std::string("\\") + "Data\\" + name;
					ShellExecute(0, "open", fullPath.c_str(), 0, 0, SW_SHOW);
				}
			}
#endif
		}
		else if ("NewProject" == name)
		{
			LP_Manager::GetSingleton()._ShowProjectCreate(true);
		}
		else if ("Cancel" == name)
		{
			LP_Manager::GetSingleton()._ShowProjectCreate(false);
		}
		else if ("MakeProjectOK" == name)
		{
			int width = 1136;
			int height = 640;
			std::string edname = LP_Manager::GetSingleton().GetCreateProjectUIEditBox()->GetText();
			std::string textWidth = LP_Manager::GetSingleton().GetCreateProjectWidthUIEditBox()->GetText();
			std::string textHeight = LP_Manager::GetSingleton().GetCreateProjectHeightUIEditBox()->GetText();
			bool isLanscape = LP_Manager::GetSingleton().GetCreateProjectLandscapeButton()->IsCheck();
			if (!textWidth.empty())
				width = StringHelp::StringToInt(textWidth);
			if (!textHeight.empty())
				height = StringHelp::StringToInt(textHeight);

			if (!edname.empty())
			{
				PX2_APP.MakeAProject(edname, isLanscape?Project::ScreenOrientation::SO_LANDSCAPE:
					Project::ScreenOrientation::SO_PORTRAIT, width, height);

				LP_Manager::GetSingleton()._ShowProjectCreate(false);
			}
		}
	}
}
//----------------------------------------------------------------------------
void _ProgressCallback(CurlObj *obj, double dltotal,
	double dlnow, double ultotal, double ulnow)
{
	int i = 0;
	PX2_UNUSED(obj);
	PX2_UNUSED(dltotal);
	PX2_UNUSED(dlnow);
	PX2_UNUSED(ultotal);
	PX2_UNUSED(ulnow);
}
void _CurlWriteCallback (CurlObj *obj, void *buffer, size_t size,
	size_t numSize)
{
	int i = 0;
	PX2_UNUSED(obj);
	PX2_UNUSED(buffer);
	PX2_UNUSED(size);
	PX2_UNUSED(numSize);
}
//----------------------------------------------------------------------------
void LP_Manager::Initlize()
{
	std::string lanStr = PX2_APP.GetConfig("LauncherLanguage");
	int lanIndex = StringHelp::StringToInt(lanStr);
	PX2_LM_APP.SetLanguage(lanIndex);

	PX2_EW.ComeIn(this);
	PX2_SC_LUA->CallFile("Data/LPlugin/scripts/play.lua");
	RenderWindow *rw = PX2_GR.GetMainWindow();
	Canvas* mainCanvas = rw->GetMainCanvas();
	UIFrame* uiMainFrame = new0 UIFrame();
	mUIMainFrame = uiMainFrame;
	mainCanvas->AttachChild(uiMainFrame);
	UIPicBox *picBox = uiMainFrame->CreateAddBackgroundPicBox();
	uiMainFrame->SetAnchorHor(0.0f, 1.0f);
	uiMainFrame->SetAnchorVer(0.0f, 1.0f);
	picBox->SetColor(Float3::BLACK);

	// updater frame
	mUpdaterFrame = new0 UIFrame();
	uiMainFrame->AttachChild(mUpdaterFrame);
	mUpdaterFrame->LocalTransform.SetTranslateY(-50.0f);
	mUpdaterFrame->SetAnchorHor(0.0, 1.0f);
	mUpdaterFrame->SetAnchorVer(0.0f, 1.0f);
	mUpdaterFrame->SetHeight(200.0f);
	mUpdaterFrame->SetWidget(true);

	UIFrame *frameUpdater = UIFrame::New();
	mUpdaterFrame->AttachChild(frameUpdater);
	frameUpdater->SetAnchorHor(0.0f, 1.0f);
	frameUpdater->SetAnchorVer(0.5f, 0.5f);
	frameUpdater->SetHeight(200.0f);
	frameUpdater->CreateAddBackgroundPicBox();

	UIFText *text = UIFText::New();
	frameUpdater->AttachChild(text);
	text->LocalTransform.SetTranslateY(-2.0f);
	text->GetText()->SetText(PX2_LM_APP.GetValue("Updating"));

	// top
	float topHeight = 20.0f;
	mFrameTop = new0 UIFrame();
	uiMainFrame->AttachChild(mFrameTop);
	auto picBoxBack = mFrameTop->CreateAddBackgroundPicBox();
	picBoxBack->SetColor(Float3::MakeColor(0, 0, 0));
	mFrameTop->LocalTransform.SetTranslateY(-20.0f);
	mFrameTop->SetAnchorHor(0.0f, 1.0f);
	mFrameTop->SetAnchorVer(1.0f, 1.0f);
	mFrameTop->SetPivot(0.5f, 1.0f);
	mFrameTop->SetHeight(topHeight);
	mFrameTop->SetChildPickOnlyInSizeRange(false);

	UIComboBox *box = UIComboBox::New("ComboLanguage");
	mFrameTop->AttachChild(box);
	mLanguageBox = box;
	box->AddChooseStr(PX2_LM_APP.GetValue("Chinese"));
	box->AddChooseStr(PX2_LM_APP.GetValue("English"));
	box->SetChooseListHeightSameWithChooses();
	box->SetAnchorHor(0.0f, 0.0f);
	box->SetPivot(0.0f, 0.5f);
	box->SetWidth(80.0f);
	box->SetAnchorVer(0.0f, 1.0f);
	box->AddVisitor(this);
	box->Choose(lanIndex, false);
	box->SetFontSize(15);
	box->GetSelectButton()->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	box->GetSelectButton()->GetFText()->SetAnchorParamHor(10.0f, 10.0f);

	mRegistButton = new0 UIButton();
	mFrameTop->AttachChild(mRegistButton);
	mRegistButton->SetName("BtnRegist");
	mRegistButton->LocalTransform.SetTranslateY(-1.0f);
	mRegistButton->SetAnchorHor(1.0f, 1.0f);
	mRegistButton->SetPivot(1.0f, 0.5f);
	mRegistButton->SetWidth(60.0f);
	mRegistButton->SetAnchorVer(0.0f, 1.0f);
	mRegistButton->AddVisitor(this);
	mRegistButton->CreateAddText(PX2_LM_APP.GetValue("Regist"));

	mUserButton = new0 UIButton();
	mFrameTop->AttachChild(mUserButton);
	mUserButton->SetName("BtnUser");
	mUserButton->LocalTransform.SetTranslateY(-1.0f);
	mUserButton->SetAnchorHor(1.0f, 1.0f);
	mUserButton->SetPivot(1.0f, 0.5f);
	mUserButton->SetWidth(60.0f);
	mUserButton->SetAnchorVer(0.0f, 1.0f);
	mUserButton->AddVisitor(this);
	mUserButton->CreateAddText("");
	mUserButton->Show(false);

	mUserButtonListFrame = new0 UIList();
	mFrameTop->AttachChild(mUserButtonListFrame);
	mUserButtonListFrame->SetName("UserButtonList");
	mUserButtonListFrame->LocalTransform.SetTranslateY(-10.0f);
	mUserButtonListFrame->LocalTransform.SetTranslateY(-1.0f);
	mUserButtonListFrame->SetAnchorHor(1.0f, 1.0f);
	mUserButtonListFrame->SetPivot(1.0f, 1.0f);
	mUserButtonListFrame->SetWidth(60.0f);
	mUserButtonListFrame->SetAnchorHor(1.0f, 1.0f);
	mUserButtonListFrame->SetAnchorVer(0.0f, 0.0f);
	mUserButtonListFrame->AddItem(PX2_LM_APP.GetValue("LoginOut"));
	mUserButtonListFrame->SetHeight(mUserButtonListFrame->GetContentHeight());
	mUserButtonListFrame->Show(false);
	mUserButtonListFrame->AddVisitor(this);

	mLoginButton = new0 UIButton();
	mFrameTop->AttachChild(mLoginButton);
	mLoginButton->LocalTransform.SetTranslateY(-1.0f);
	mLoginButton->SetName("BtnLogin");
	mLoginButton->SetAnchorHor(1.0f, 1.0f);
	mLoginButton->SetAnchorParamHor(-65.0f, -65.0f);
	mLoginButton->SetPivot(1.0f, 0.5f);
	mLoginButton->SetWidth(60.0f);
	mLoginButton->SetAnchorVer(0.0f, 1.0f);
	mLoginButton->AddVisitor(this);
	mLoginButton->CreateAddText(PX2_LM_APP.GetValue("Login"));

	// tab
	mTableFrame = new0 UITabFrame();
	mTableFrame->SetName("TheTabFrame");
	mTableFrame->AddUICallback(_UICallback);
	uiMainFrame->AttachChild(mTableFrame);
	mTableFrame->SetUseSkin(false);
	mTableFrame->SetFontScale(0.75f);
	mTableFrame->SetFontColor(Float3::WHITE);
	mTableFrame->SetTabBarHeight(30.0f);
	mTableFrame->SetTabHeight(30.0f);
	mTableFrame->LocalTransform.SetTranslateY(-1.0f);
	UIPicBox *tabBackPic = mTableFrame->CreateAddBackgroundPicBox();
	tabBackPic->SetColor(Float3::WHITE);
	mTableFrame->SetAnchorHor(0.0f, 1.0f);
	mTableFrame->SetAnchorVer(0.0f, 1.0f);
	mTableFrame->SetAnchorParamVer(0.0f, -20.0f);
	mTableFrame->AddTab("ManyKit", PX2_LM_APP.GetValue("ManyKit"), CreateManyKitFrame());
	mTableFrame->AddTab("Engine", PX2_LM_APP.GetValue("Engine"), CreateEngineFrame());
	mTableFrame->AddTab("Robot", PX2_LM_APP.GetValue("01Robot"), _CreateZeroneRobotFrame());
	//mTableFrame->AddTab("CodePlay", PX2_LM_APP.GetValue("CodePlay"), CreateCodePlayFrame());
	//mTableFrame->AddTab("Intell-Toy", PX2_LM_APP.GetValue("Intell-Toy"), CreateToy());
	mTableFrame->GetTitleBarFrame()->CreateAddBackgroundPicBox()->SetColor(Float3::WHITE);
	float brightness = 1.2f;
	mTableFrame->GetTabButton("ManyKit")->SetStateColorDefaultWhite();
	mTableFrame->GetTabButton("ManyKit")->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	mTableFrame->GetTabButton("ManyKit")->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/white.png");
	mTableFrame->GetTabButton("ManyKit")->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
	mTableFrame->GetTabButton("ManyKit")->SetStateColor(UIButtonBase::BS_NORMAL, Float3::MakeColor(237, 28, 36));
	mTableFrame->GetTabButton("ManyKit")->SetStateColor(UIButtonBase::BS_HOVERED, Float3::MakeColor(237, 28, 36));
	mTableFrame->GetTabButton("ManyKit")->SetStateColor(UIButtonBase::BS_PRESSED, Float3::MakeColor(237, 28, 36));
	mTableFrame->GetTabButton("ManyKit")->SetActivateColor(Float3::MakeColor(237, 28, 36));
	mTableFrame->GetTabButton("ManyKit")->SetActivateBrightness(brightness);

	mTableFrame->GetTabButton("Engine")->SetStateColorDefaultWhite();
	mTableFrame->GetTabButton("Engine")->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	mTableFrame->GetTabButton("Engine")->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/white.png");
	mTableFrame->GetTabButton("Engine")->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
	mTableFrame->GetTabButton("Engine")->SetStateColor(UIButtonBase::BS_NORMAL, Float3::MakeColor(255, 127, 39));
	mTableFrame->GetTabButton("Engine")->SetStateColor(UIButtonBase::BS_HOVERED, Float3::MakeColor(255, 127, 39));
	mTableFrame->GetTabButton("Engine")->SetStateColor(UIButtonBase::BS_PRESSED, Float3::MakeColor(255, 127, 39));
	mTableFrame->GetTabButton("Engine")->SetActivateColor(Float3::MakeColor(255, 127, 39));
	mTableFrame->GetTabButton("Engine")->SetActivateBrightness(brightness);

	mTableFrame->GetTabButton("Robot")->SetStateColorDefaultWhite();
	mTableFrame->GetTabButton("Robot")->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	mTableFrame->GetTabButton("Robot")->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/white.png");
	mTableFrame->GetTabButton("Robot")->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
	mTableFrame->GetTabButton("Robot")->SetStateColor(UIButtonBase::BS_NORMAL, Float3::MakeColor(255, 210, 10));
	mTableFrame->GetTabButton("Robot")->SetStateColor(UIButtonBase::BS_HOVERED, Float3::MakeColor(255, 210, 10));
	mTableFrame->GetTabButton("Robot")->SetStateColor(UIButtonBase::BS_PRESSED, Float3::MakeColor(255, 210, 10));
	mTableFrame->GetTabButton("Robot")->SetActivateColor(Float3::MakeColor(255, 210, 10));
	mTableFrame->GetTabButton("Robot")->SetActivateBrightness(brightness);

	//mTableFrame->GetTabButton("CodePlay")->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	//mTableFrame->GetTabButton("CodePlay")->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/white.png");
	//mTableFrame->GetTabButton("CodePlay")->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
	//mTableFrame->GetTabButton("CodePlay")->SetStateColor(UIButtonBase::BS_NORMAL, Float3::MakeColor(24, 177, 76));
	//mTableFrame->GetTabButton("CodePlay")->SetStateColor(UIButtonBase::BS_HOVERED, Float3::MakeColor(24, 177, 76));
	//mTableFrame->GetTabButton("CodePlay")->SetStateColor(UIButtonBase::BS_PRESSED, Float3::MakeColor(24, 177, 76));
	//mTableFrame->GetTabButton("CodePlay")->SetActivateColor(Float3::MakeColor(24, 177, 76));
	//mTableFrame->GetTabButton("CodePlay")->SetActivateBrightness(brightness);

	//mTableFrame->GetTabButton("Intell-Toy")->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	//mTableFrame->GetTabButton("Intell-Toy")->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/white.png");
	//mTableFrame->GetTabButton("Intell-Toy")->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
	//mTableFrame->GetTabButton("Intell-Toy")->SetStateColor(UIButtonBase::BS_NORMAL, Float3::MakeColor(17,167,210));
	//mTableFrame->GetTabButton("Intell-Toy")->SetStateColor(UIButtonBase::BS_HOVERED, Float3::MakeColor(17,167,210));
	//mTableFrame->GetTabButton("Intell-Toy")->SetStateColor(UIButtonBase::BS_PRESSED, Float3::MakeColor(17, 167, 210));
	//mTableFrame->GetTabButton("Intell-Toy")->SetActivateColor(Float3::MakeColor(17, 167, 210));
	//mTableFrame->GetTabButton("Intell-Toy")->SetActivateBrightness(brightness);

	mTableFrame->SetActiveTab("ManyKit");

	_CreateNewProjectFrame();
	_CreateLoginFrame();
	_CreateZeroneRobotFrame();
	
	ServerSocket socket(EngineUDPPortHttp);
	HTTPServerParamsPtr pParams = new0 HTTPServerParams();
	pParams->SetMaxQueued(50);
	pParams->SetMaxThreads(4);
	mHttpServer = new0 HTTPServer(new0 HandlerFactory(), socket, pParams);
	mHttpServer->Start();

	mCurlObject = new0 CurlObj();
	mCurlObject->SetGetType(CurlObj::GT_MEMORY);
	mCurlObject->Initlize();
	mCurlObject->SetCurlWriteCallback(_CurlWriteCallback);
	mCurlObject->SetCurlProgressCallback(_ProgressCallback);

	mCfgUserName = PX2_APP.GetConfig("UserName");
	mCfgPassword = PX2_APP.GetConfig("Password");

	if (!mCfgUserName.empty() && !mCfgPassword.empty())
	{
		_CurlLogin(mCfgUserName, mCfgPassword);
	}

	_RefreshProjects();
}
//----------------------------------------------------------------------------
void LP_Manager::Terminate()
{
	if (mHttpServer)
	{
		mHttpServer->Stop();
		mHttpServer = 0;
	}
	if (mBEventQue)
	{
		delete(mBEventQue);
		mBEventQue = 0;
	}
	mUIMainFrame = 0;
	mEngineFrame = 0;
	mLoginInFrame = 0;
	mCreateProjectBackground = 0;
	mCreateProjectUiFrame = 0;
	mTableFrame = 0;
	mProjectFrame = 0;
	mGitHubBut = 0;
	mEditBoxVersion = 0;
	mCheckCMDStart = 0;
	mProjectList = 0;
	mPluginList = 0;
	mServerFrame = 0;
	mDeviceInput = 0;
	mDeviceList = 0;
	mHttpServer = 0;
	mBEventQue = 0;
	mPromptUIFrame = 0;
	mProjectName = 0;
	PX2_UIAUIM.Clear();

	mCurlObject = 0;

	PX2_EW.GoOut(this);

	TerminateNetwork();
}
//----------------------------------------------------------------------------
void LP_Manager::Update(double appSeconds, double elapsedSeconds)
{
	PX2_UNUSED(appSeconds);
	PX2_UNUSED(elapsedSeconds);
}
//----------------------------------------------------------------------------
void LP_Manager::OnEvent(Event *event)
{
	if (EngineNetES::IsEqual(event, EngineNetES::EngineClientUDPInfoChanged))
	{
		_RefreshAddresses();	
	}
	else if (GraphicsES::IsEqual(event, GraphicsES::GeneralString))
	{
		std::string str = event->GetData<std::string>();
		if (str == "showupdating")
			ShowUpdating(true);
		else if (str == "hideupdating")
			ShowUpdating(false);
	}
}
//----------------------------------------------------------------------------
int LP_Manager::_CurlLogin(const std::string &userName,
	const std::string &password)
{
	std::string url = "http://www.manykit.com/res/login?";
	std::string data = "username=" + userName;
	data += std::string("&password=") + password;
	int ret = mCurlObject->Post(url, data);

	if (1 == ret)
	{
		char *chMem = mCurlObject->GetGettedMemory();
		int size = mCurlObject->GetGettedMemorySize();
		std::string ret = std::string(chMem, size);

		JSONData jsData;
		if (jsData.LoadBuffer(ret))
		{
			if (jsData.IsHasMember("data"))
			{
				JSONValue jsValue = jsData.GetMember("data");

				mLoginInFrame->Show(false);

				mLoginButton->Show(false);
				mRegistButton->Show(false);
				mUserButton->Show(true);
				mUserButton->GetText()->SetText(userName);

				mCfgUserName = userName;
				mCfgPassword = password;

				PX2_APP.SetConfig("UserName", userName);
				PX2_APP.SetConfig("Password", password);

				LP_ProjectItem *item = new0 LP_ProjectItem();
				item->Name = "111";
				item->ID = 111;
				item->IsCloud = true;
				AddProject_Cloud(item);

				LP_ProjectItem *item1 = new0 LP_ProjectItem();
				item1->Name = "222";
				item1->ID = 222;
				item1->IsCloud = true;
				AddProject_Cloud(item1);

				LP_ProjectItem *itemZERONE = new0 LP_ProjectItem();
				itemZERONE->Name = "ZERONE";
				itemZERONE->ID = 12345;
				itemZERONE->IsCloud = true;
				AddProject_Cloud(itemZERONE);
			}
			if (jsData.IsHasMember("errmsg"))
			{
				JSONValue jsValue = jsData.GetMember("errmsg");

			}
		}
	}

	return ret;
}
//----------------------------------------------------------------------------
void LP_Manager::_CurlLogout()
{
	std::string url = "http://www.manykit.com/res/logout";
	int ret = mCurlObject->Get(url);
	if (1 == ret)
	{
		char *chMem = mCurlObject->GetGettedMemory();
		int size = mCurlObject->GetGettedMemorySize();
		std::string ret = std::string(chMem, size);

		JSONData jsData;
		if (jsData.LoadBuffer(ret))
		{
			if (jsData.IsHasMember("data"))
			{
				JSONValue jsValue = jsData.GetMember("data");
				bool bValue = jsValue.ToBool();

				if (bValue)
				{
					mCfgUserName.clear();
					mCfgPassword.clear();
					PX2_APP.SetConfig("UserName", mCfgUserName);
					PX2_APP.SetConfig("Password", mCfgPassword);

					mLoginButton->Show(true);
					mRegistButton->Show(true);
					mUserButton->Show(false);
					mUserButtonListFrame->Show(false);
				}
			}
		}
	}

	mUserButtonListFrame->Show(false);
}
//----------------------------------------------------------------------------
void LP_Manager::Visit(Object *obj, int info)
{	
	const std::string &name = obj->GetName();

	if (UICT_RELEASED == info)
	{
		UIButton *but = DynamicCast<UIButton>(obj);
		if (but)
		{
			if ("ButLaunch" == name)
			{
				UIItem *item = mProjectList->GetSelectedItem();
				if (item)
				{
					const std::string &name = item->GetName();
					if (!name.empty())
					{
						_SimuApp(name);
					}
				}
			}
			else if ("ButGenProjInfo" == name)
			{
				_OnGenProjInfo();
			}
			else if ("ButRefresh" == name)
			{
				_RefreshProjects();
			}
			else if ("ButNetLoadProject" == name)
			{
				_OnLoad();
			}
			else if ("ButNetCloseProject" == name)
			{
				_OnClose();
			}
			else if ("ButNetSend" == name)
			{
				_OnSend();
			}
			else if ("ButNetSyn" == name)
			{
				_OnSyn();
			}
			else if ("ButGitHub" == name)
			{
				std::string text = mGitHubBut->GetText()->GetText();

#if defined(_WIN32) || defined(WIN32)
				WCHAR wszPath[MAX_PATH];
				GetCurrentDirectoryW(sizeof(wszPath), wszPath);
				ShellExecute(0, "open", text.c_str(), 0, 0, SW_SHOW);
#endif
			}
			else if ("ButShop" == name)
			{
				std::string text = "https://shop194048616.taobao.com";

#if defined(_WIN32) || defined(WIN32)
				WCHAR wszPath[MAX_PATH];
				GetCurrentDirectoryW(sizeof(wszPath), wszPath);
				ShellExecute(0, "open", text.c_str(), 0, 0, SW_SHOW);
#endif
			}
			else if ("ButEditor" == name)
			{
				_OpenEditor();
			}
			else if ("BtnRegist" == name)
			{
				std::string text = "http://www.manykit.com";

#if defined(_WIN32) || defined(WIN32)
				WCHAR wszPath[MAX_PATH];
				GetCurrentDirectoryW(sizeof(wszPath), wszPath);
				ShellExecute(0, "open", text.c_str(), 0, 0, SW_SHOW);
#endif
			}
			else if ("BtnLogin" == name)
			{
				mLoginInFrame->Show(true);
			}
			else if ("BtnForgetPassword" == name)
			{
				std::string text = "http://www.manykit.com";

#if defined(_WIN32) || defined(WIN32)
				WCHAR wszPath[MAX_PATH];
				GetCurrentDirectoryW(sizeof(wszPath), wszPath);
				ShellExecute(0, "open", text.c_str(), 0, 0, SW_SHOW);
#endif
			}
			else if ("BtnLoginOK" == name)
			{
				std::string userName = mEditBoxUserName->GetText();
				std::string password = mEditBoxPassword->GetRealText();

				_CurlLogin(userName, password);
			}
			else if ("BtnLoginCancel" == name)
			{
				mLoginInFrame->Show(false);
			}
			else if ("BtnUser" == name)
			{
				mUserButtonListFrame->Show(!mUserButtonListFrame->IsShow());
			}
		}
	}
	else if (UICT_CHECKED == info)
	{
		if ("CheckCMDStart" == name)
		{
			mIsStartNoWindow = true;
		}
		else
		{
			bool isPlugin = obj->GetUserData<bool>("IsPlugin");
			if (!isPlugin)
			{
				PX2_APP.GetBoostInfo().AddProject(name);
				PX2_APP.WriteBoost();
			}
			else
			{
				PX2_APP.GetBoostInfo().AddPlugin(name);
				PX2_APP.WriteBoost();
			}
		}
	}
	else if (UICT_DISCHECKED == info)
	{
		if ("CheckCMDStart" == name)
		{
			mIsStartNoWindow = false;
		}
		else
		{
			bool isPlugin = obj->GetUserData<bool>("IsPlugin");
			if (!isPlugin)
			{
				PX2_APP.GetBoostInfo().RemoveProject(name);
				PX2_APP.WriteBoost();
			}
			else
			{
				PX2_APP.GetBoostInfo().RemovePlugin(name);
				PX2_APP.WriteBoost();
			}
		}
	}
	else if (UICT_LIST_SELECTED == info)
	{
		if ("ListProject" == name)
		{
			std::string itemText = mProjectList->GetSelectedItemText();
			std::string path = "Data/" + itemText + "/filelist.xml";
			std::string versionText = PX2_APP.GetProjectVersionByPath(path);
			mEditBoxVersion->SetText(versionText);
		}
		else if ("UserButtonList" == name)
		{
			std::string selectItemText = mUserButtonListFrame->GetSelectedItemText();
			if (PX2_LM_APP.GetValue("LoginOut") == selectItemText)
			{
				_CurlLogout();
			}
		}
	}
	else if (UICT_COMBOBOX_CHOOSED == info)
	{
		if ("ComboLanguage" == name)
		{
			int choose = mLanguageBox->GetChoose();
			PX2_LM_APP.SetLanguage(choose);

			PX2_APP.SetConfig("LauncherLanguage", 
				StringHelp::IntToString(choose));

			PX2_GR.SendGeneralEvent("restartlauncher");
		}
	}
}
//----------------------------------------------------------------------------
void LP_Manager::_OnSend()
{
	const std::string &textName = mDeviceInput->GetText();
	if (!textName.empty())
	{
		UDPServer *udpServer = PX2_APP.GetEngineUDPServerEditor();
		if (udpServer)
		{
			int selectIndex = mDeviceList->GetSelectIndex();
			UDPNetInfo *info = PX2_APP.GetUDPNetInfo(selectIndex);
			if (info)
			{
				SocketAddress skAddr(info->IP, EngineUDPPortClient);
				udpServer->GetSocket().SendTo(textName.c_str(),
					(int)textName.length(), skAddr);
			}
		}
	}
}
//----------------------------------------------------------------------------
void LP_Manager::_SimuApp(const std::string &projName)
{
	std::string renderTag = Renderer::GetRenderTag();
	if ("Dx9" == renderTag)
		renderTag = "";

	std::string exeName = "AppPlayer" + renderTag;

	std::string appName;

	std::string strWidth = "800";
	std::string strHeight = "600";
	std::string projXMLPath = "Data/" + projName + "/" +
		projName + ".px2proj";
	Sizef sz = Project::GetConfigSize(projXMLPath);
	if (sz.Width != 0.0f && sz.Height != 0.0f)
	{
		strWidth = StringHelp::IntToString((int)sz.Width);
		strHeight = StringHelp::IntToString((int)sz.Height);
	}

	std::string strNoWin = mIsStartNoWindow ? StringHelp::IntToString(0) :
		StringHelp::IntToString(1);
	std::string strCfg =
		std::string(" ") + "proj" + std::string("=") + projName +
		std::string(" ") + "w" + std::string("=") + strWidth +
		std::string(" ") + "h" + std::string("=") + strHeight +
		std::string(" ") + "window" + std::string("=") + strNoWin;

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
}
//----------------------------------------------------------------------------
void LP_Manager::_OpenEditor()
{
	std::string renderTag = Renderer::GetRenderTag();

	std::string appName;
	std::string exeName = "NIRVANAwx";
	std::string strCfg;

	//if ("Dx9" == renderTag)
		renderTag = "";

	exeName += renderTag;

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
}
//----------------------------------------------------------------------------
UIFrame *LP_Manager::AddTabFrame(const std::string &name, 
	const std::string &title)
{
	UIFrame *uiFrame = new0 UIFrame();
	uiFrame->SetAnchorHor(0.0f, 1.0f);
	uiFrame->SetAnchorVer(0.0f, 1.0f);
	uiFrame->SetActivateSelfCtrled(true);

	UIFPicBox *fpicBox = new0 UIFPicBox();
	uiFrame->AttachChild(fpicBox);
	fpicBox->LocalTransform.SetTranslateY(-10.0f);
	fpicBox->SetAnchorHor(0.0f, 1.0f);
	fpicBox->SetAnchorVer(1.0f, 1.0f);
	fpicBox->SetPivot(0.5f, 1.0f);
	fpicBox->SetSize(0.0f, 3.0f);
	UIPicBox *picBox = fpicBox->GetUIPicBox();
	picBox->SetTexture("Data/engine/white.png");
	picBox->SetColor(Float3::YELLOW);
	
	mTableFrame->AddTab(name, title, uiFrame);
	UIText *text = mTableFrame->GetTabButton(name)->GetText();
	text->SetColorSelfCtrled(true);
	text->SetBrightnessSelfCtrled(true); 
	text->SetFontScale(0.65f);
	text->SetFontColor(Float3::WHITE);
	text->SetColor(Float3::WHITE);
	text->SetDrawStyle(FD_SHADOW);
	text->SetBorderShadowAlpha(0.8f);

	UIButton *tabBut = mTableFrame->GetTabButton(name);
	tabBut->SetStateBrightness(UIButtonBase::BS_HOVERED, 1.2f);
	tabBut->SetStateBrightness(UIButtonBase::BS_PRESSED, 1.0f);

	return uiFrame;
}
//----------------------------------------------------------------------------
UIFrame *LP_Manager::CreateManyKitFrame()
{
	mManyKitFrame = new0 UIFrame();
	mManyKitFrame->SetAnchorHor(0.0f, 1.0f);
	mManyKitFrame->SetAnchorVer(0.0f, 1.0f);

	auto fPicBoxBack = UIFPicBox::New();
	mManyKitFrame->AttachChild(fPicBoxBack);
	fPicBoxBack->LocalTransform.SetTranslateY(-5.0f);
	fPicBoxBack->SetAnchorHor(0.0f, 1.0f);
	fPicBoxBack->SetAnchorVer(1.0f, 1.0f);
	fPicBoxBack->SetPivot(0.5f, 1.0f);
	fPicBoxBack->SetHeight(5.0f);
	fPicBoxBack->GetUIPicBox()->SetTexture("Data/engine/white.png");
	fPicBoxBack->SetBrightness(1.2f);
	fPicBoxBack->SetColor(Float3::MakeColor(237, 28, 36));

	auto fPicBox = UIFPicBox::New();
	mManyKitFrame->AttachChild(fPicBox);
	fPicBox->SetAnchorHor(0.0f, 1.0f);
	fPicBox->SetAnchorVer(0.0f, 1.0f);
	fPicBox->GetUIPicBox()->SetTexture("Data/LPlugin/images/manykit.png");

	return mManyKitFrame;
}
//----------------------------------------------------------------------------
UIFrame *LP_Manager::CreateEngineFrame()
{
	mEngineFrame = new0 UIFrame();
	mEngineFrame->LocalTransform.SetTranslateY(-1.0f);
	mEngineFrame->SetAnchorHor(0.0f, 1.0f);
	mEngineFrame->SetAnchorVer(0.0f, 1.0f);

	float texEngineHeight = 20.0f;
	auto picTextEngine = UIFText::New();
	mEngineFrame->AttachChild(picTextEngine);
	picTextEngine->LocalTransform.SetTranslateY(-2.0f);
	picTextEngine->SetAnchorHor(0.0f, 1.0f);
	picTextEngine->SetAnchorVer(1.0f, 1.0f);
	picTextEngine->SetPivot(0.5f, 1.0f);
	picTextEngine->SetHeight(texEngineHeight);
	picTextEngine->CreateAddBackgroundPicBox(true, Float3::BLACK);
	picTextEngine->GetText()->SetText(PX2_LM_APP.GetValue("Projects"));
	picTextEngine->GetText()->SetFontScale(0.7f);
	picTextEngine->GetText()->SetFontColor(Float3::WHITE);
	picTextEngine->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	picTextEngine->GetText()->SetOffset(Float2(10.0f, 0.0f));

	float topHeight = 28.0f;
	UIFrame *topFrame = UIFrame::New();
	mEngineFrame->AttachChild(topFrame);
	topFrame->LocalTransform.SetTranslateY(-1.0f);
	topFrame->SetAnchorHor(0.0f, 1.0f);
	topFrame->SetAnchorVer(1.0f, 1.0f);
	topFrame->SetAnchorParamVer(-texEngineHeight, -texEngineHeight);
	topFrame->SetPivot(0.5f, 1.0f);
	topFrame->SetHeight(topHeight);

	float textWdith = 40.0f;
	UIFText *textVersion = UIFText::New();
	topFrame->AttachChild(textVersion);
	textVersion->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	textVersion->SetAnchorHor(0.0f, 0.0f);
	textVersion->SetAnchorParamHor(10.0f, 10.0f);
	textVersion->SetSize(textWdith, topHeight);
	textVersion->SetPivot(0.0f, 0.5f);
	textVersion->SetAnchorVer(0.5f, 0.5f);
	textVersion->GetText()->SetFontWidthHeight(12, 12);
	textVersion->GetText()->SetText(PX2_LM_APP.GetValue("Testing"));

	mGitHubBut = UIButton::New("ButGitHub");
	topFrame->AttachChild(mGitHubBut);
	mGitHubBut->SetAnchorHor(0.0f, 1.0f);
	mGitHubBut->SetAnchorParamHor(textWdith + 15.0f,-140.0f);
	mGitHubBut->SetHeight(20.0f);
	mGitHubBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	mGitHubBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/whitern.png");
	mGitHubBut->SetStateBrightness(UIButtonBase::BS_HOVERED, 1.2f);
	mGitHubBut->SetStateBrightness(UIButtonBase::BS_PRESSED, 1.1f);
	mGitHubBut->CreateAddText("https://github.com/manykits/PHOENIXEngine");
	mGitHubBut->GetFText()->SetAnchorParamHor(4.0f, 10.0f);
	mGitHubBut->GetText()->SetFontScale(0.62f);
	mGitHubBut->GetText()->SetFontStyle(FES_UNDERLINE);
	mGitHubBut->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	mGitHubBut->AddVisitor(this);

	UIButton* newProjectBut = UIButton::New("NewProject");
	topFrame->AttachChild(newProjectBut);
	newProjectBut->CreateAddText(PX2_LM_APP.GetValue("CreateNewProject"));
	newProjectBut->SetAnchorHor(1.0f, 1.0f);
	newProjectBut->SetAnchorParamHor(-115.0f, 0.0f);
	newProjectBut->SetAnchorVer(0.5f, 0.5f);
	newProjectBut->SetHeight(20.0f);
	newProjectBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	newProjectBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/whitern.png");
	newProjectBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
	newProjectBut->AddUICallback(_UICallback);
	newProjectBut->GetText()->SetAligns(TEXTALIGN_HCENTER | TEXTALIGN_VCENTER);

	UIButton* openProjectBut = UIButton::New("OpenProject");
	topFrame->AttachChild(openProjectBut);
	openProjectBut->CreateAddText(PX2_LM_APP.GetValue("FileProject"));
	openProjectBut->SetAnchorHor(1.0f, 1.0f);
	openProjectBut->SetAnchorParamHor(-70.0f, 0.0f);
	openProjectBut->SetAnchorVer(0.5f, 0.5f);
	openProjectBut->SetHeight(20.0f);
	openProjectBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	openProjectBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/whitern.png");
	openProjectBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
	openProjectBut->AddUICallback(_UICallback);
	openProjectBut->GetText()->SetAligns(TEXTALIGN_HCENTER | TEXTALIGN_VCENTER);

	UIButton* packageProjectBut = UIButton::New("PackageProject");
	topFrame->AttachChild(packageProjectBut);
	packageProjectBut->CreateAddText(PX2_LM_APP.GetValue("Package"));
	packageProjectBut->SetAnchorHor(1.0f, 1.0f);
	packageProjectBut->SetAnchorParamHor(-25.0f, 0.0f);
	packageProjectBut->SetAnchorVer(0.5f, 0.5f);
	packageProjectBut->SetHeight(20.0f);
	packageProjectBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	packageProjectBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/whitern.png");
	packageProjectBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
	packageProjectBut->AddUICallback(_UICallback);
	packageProjectBut->GetText()->SetAligns(TEXTALIGN_HCENTER | TEXTALIGN_VCENTER);

	float butLuancherWidth = 85.0f;
	float butHeight = 28.0f;
	UIButton *butLaunch = UIButton::New("ButLaunch");
	mEngineFrame->AttachChild(butLaunch);
	auto textLaunch = butLaunch->CreateAddText(PX2_LM_APP.GetValue("Launch"));
	textLaunch->GetText()->SetAligns(TEXTALIGN_RIGHT | TEXTALIGN_VCENTER);
	butLaunch->GetText()->SetFontColor(Float3::BLACK);
	butLaunch->GetText()->SetFontScale(0.8f);
	//butLaunch->GetText()->SetDrawStyle(FD_BORDER);
	//butLaunch->GetText()->SetBorderShadowColor(Float3::WHITE);
	//butLaunch->GetText()->SetShadowBorderSize(0.2f);
	butLaunch->GetFText()->SetAnchorParamHor(0.0, -14.0f);
	butLaunch->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	butLaunch->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/whitern.png");
	butLaunch->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f,12.0f,12.0f,12.0f);
	butLaunch->SetStateColor(UIButtonBase::BS_NORMAL, Float3::MakeColor(237, 28, 36));
	butLaunch->SetStateColor(UIButtonBase::BS_HOVERED, Float3::MakeColor(237, 28, 36));
	butLaunch->SetStateColor(UIButtonBase::BS_PRESSED, Float3::MakeColor(237, 28, 36));
	butLaunch->SetStateBrightness(UIButtonBase::BS_NORMAL, 1.0f);
	butLaunch->SetStateBrightness(UIButtonBase::BS_HOVERED, 1.2f);
	butLaunch->SetStateBrightness(UIButtonBase::BS_PRESSED, 0.8f);
	butLaunch->SetAnchorHor(0.0f, 0.0f);
	butLaunch->SetAnchorParamHor(5.0f, 5.0f);
	butLaunch->SetAnchorVer(1.0f, 1.0f);
	butLaunch->SetAnchorParamVer(-topHeight- texEngineHeight, -topHeight- texEngineHeight);
	butLaunch->SetSize(butLuancherWidth, butHeight);
	butLaunch->SetPivot(0.0f, 1.0f);
	butLaunch->AddVisitor(this);

	mCheckCMDStart = UICheckButton::New("CheckCMDStart");
	butLaunch->AttachChild(mCheckCMDStart);
	mCheckCMDStart->LocalTransform.SetTranslateY(-4.0f);
	mCheckCMDStart->SetPivot(0.0f, 0.5f);
	mCheckCMDStart->SetAnchorHor(0.0f, 0.0f);
	mCheckCMDStart->SetAnchorParamHor(5.0f, 5.0f);
	mCheckCMDStart->SetAnchorVer(0.5f, 0.5f);
	mCheckCMDStart->SetSize(butHeight - 10.0f, butHeight - 10.0f);
	mCheckCMDStart->AddVisitor(this);
	UIFText *txtCMD = mCheckCMDStart->CreateAddText("cmd");
	txtCMD->GetText()->SetFontScale(0.5f);
	txtCMD->SetAlpha(0.8f);

	float butWidthGenInfo = 60.0f;
	UIButton *butGenInfo = UIButton::New("ButGenProjInfo");
	mEngineFrame->AttachChild(butGenInfo);
	butGenInfo->CreateAddText(PX2_LM_APP.GetValue("GenProjInfo"));
	butGenInfo->GetText()->SetFontColor(Float3::WHITE);
	butGenInfo->GetText()->SetFontScale(0.6f);
	butGenInfo->SetAnchorHor(0.0f, 0.0f);
	butGenInfo->SetAnchorParamHor(5.0f + butLuancherWidth + 5.0f, 0.0f);
	butGenInfo->SetAnchorVer(1.0f, 1.0f);
	butGenInfo->SetAnchorParamVer(-topHeight - texEngineHeight, -topHeight - texEngineHeight);
	butGenInfo->SetSize(butWidthGenInfo, butHeight);
	butGenInfo->SetPivot(0.0f, 1.0f);
	butGenInfo->AddVisitor(this);
	butGenInfo->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	butGenInfo->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/whitern.png");
	butGenInfo->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	float editBoxWidth = 40.0f;
	mEditBoxVersion = UIEditBox::New("EditBoxVersion");
	mEngineFrame->AttachChild(mEditBoxVersion);
	mEditBoxVersion->SetPivot(0.0f, 1.0f);
	mEditBoxVersion->SetAnchorHor(0.0f, 0.0f);
	mEditBoxVersion->SetAnchorParamHor(5.0f + butLuancherWidth + 5.0f + butWidthGenInfo + 5.0f, 0.0f);
	mEditBoxVersion->SetAnchorVer(1.0f, 1.0f);
	mEditBoxVersion->SetAnchorParamVer(-topHeight - texEngineHeight, -topHeight - texEngineHeight);
	mEditBoxVersion->SetSize(editBoxWidth, butHeight);
	mEditBoxVersion->GetInputText()->SetFontScale(0.68f);
	mEditBoxVersion->SetText("1.0.0");
	mEditBoxVersion->GetBackPicBox()->GetUIPicBox()->SetPicBoxType(UIPicBox::PBT_NINE);
	mEditBoxVersion->GetBackPicBox()->GetUIPicBox()->SetTexture("Data/engine/whitern.png");
	mEditBoxVersion->GetBackPicBox()->GetUIPicBox()->SetTexCornerSize(12.0f,12.0f,12.0f,12.0f);
	mEditBoxVersion->GetBackPicBox()->SetColor(Float3::MakeColor(200, 200, 200));
	
	float butRefreshWidth = 40.0f;
	UIButton *butReFresh = UIButton::New("ButRefresh");
	mEngineFrame->AttachChild(butReFresh);
	butReFresh->CreateAddText(PX2_LM_APP.GetValue("Refresh"));
	butReFresh->GetText()->SetFontColor(Float3::WHITE);
	butReFresh->GetText()->SetFontScale(0.8f);
	butReFresh->SetStateColor(UIButtonBase::BS_NORMAL, Float3::MakeColor(100, 100, 100));
	butReFresh->SetStateColor(UIButtonBase::BS_HOVERED, Float3::MakeColor(100, 100, 100));
	butReFresh->SetStateColor(UIButtonBase::BS_PRESSED, Float3::MakeColor(100, 100, 100));
	butReFresh->SetStateBrightness(UIButtonBase::BS_NORMAL, 1.0f);
	butReFresh->SetStateBrightness(UIButtonBase::BS_HOVERED, 1.2f);
	butReFresh->SetStateBrightness(UIButtonBase::BS_PRESSED, 0.8f);
	butReFresh->SetAnchorHor(0.0f, 0.0f);
	butReFresh->SetAnchorParamHor(5.0f + butLuancherWidth + 5.0f + butWidthGenInfo + 5.0f + editBoxWidth + 5.0f, 0.0f);
	butReFresh->SetAnchorVer(1.0f, 1.0f);
	butReFresh->SetAnchorParamVer(-topHeight - texEngineHeight, -topHeight - texEngineHeight);
	butReFresh->SetSize(butRefreshWidth, butHeight);
	butReFresh->SetPivot(0.0f, 1.0f);
	butReFresh->AddVisitor(this);
	butReFresh->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	butReFresh->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/whitern.png");
	butReFresh->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	float editorWidth = 60.0f;
	UIButton *butEditor = UIButton::New("ButEditor");
	mEngineFrame->AttachChild(butEditor);
	auto textEditor = butEditor->CreateAddText(PX2_LM_APP.GetValue("Editor"));
	butEditor->GetText()->SetAligns(TEXTALIGN_HCENTER | TEXTALIGN_VCENTER);
	butEditor->GetText()->SetFontColor(Float3::BLACK);
	butEditor->GetText()->SetFontScale(0.8f);
	//butEditor->GetText()->SetDrawStyle(FD_BORDER);
	//butEditor->GetText()->SetShadowBorderSize(0.2f);
	//butEditor->GetText()->SetBorderShadowColor(Float3::WHITE);
	butEditor->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	butEditor->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/whitern.png");
	butEditor->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
	butEditor->SetStateColor(UIButtonBase::BS_NORMAL, Float3::MakeColor(34, 177, 76));
	butEditor->SetStateColor(UIButtonBase::BS_HOVERED, Float3::MakeColor(34, 177, 76));
	butEditor->SetStateColor(UIButtonBase::BS_PRESSED, Float3::MakeColor(34, 177, 76));
	butEditor->SetStateBrightness(UIButtonBase::BS_NORMAL, 1.0f);
	butEditor->SetStateBrightness(UIButtonBase::BS_HOVERED, 1.2f);
	butEditor->SetStateBrightness(UIButtonBase::BS_PRESSED, 0.8f);
	butEditor->SetAnchorHor(1.0f, 1.0f);
	butEditor->SetAnchorParamHor(-5.0f, -5.0f);
	butEditor->SetAnchorVer(1.0f, 1.0f);
	butEditor->SetAnchorParamVer(-topHeight - texEngineHeight, -topHeight - texEngineHeight);
	butEditor->SetSize(editorWidth, butHeight);
	butEditor->SetPivot(1.0f, 1.0f);
	butEditor->AddVisitor(this);

	float spaceHeight = 22.0f;

	auto textProject = UIFText::New();
	mEngineFrame->AttachChild(textProject);
	textProject->LocalTransform.SetTranslateY(-1.0f);
	float verPosTextProjectPlugin = -topHeight - butHeight - texEngineHeight - spaceHeight *0.5f;
	textProject->SetAnchorHor(0.0f, 0.0f);
	textProject->SetAnchorParamHor(0.0f, 0.0f);
	textProject->SetAnchorVer(1.0f, 1.0f);
	textProject->SetAnchorParamVer(verPosTextProjectPlugin, verPosTextProjectPlugin);
	textProject->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	textProject->GetText()->SetOffset(Float2(10.0f, 0.0f));
	textProject->GetText()->SetFontScale(0.7f);
	textProject->SetPivot(0.0f, 0.5f);
	textProject->SetWidth(200.0f);
	textProject->GetText()->SetText(PX2_LM_APP.GetValue("Project"));

	auto textPlugin = UIFText::New();
	mEngineFrame->AttachChild(textPlugin);
	textPlugin->LocalTransform.SetTranslateY(-1.0f);
	textPlugin->SetAnchorHor(0.6f, 0.6f);
	textPlugin->SetAnchorParamHor(0.0f, 0.0f);
	textPlugin->SetAnchorVer(1.0f, 1.0f);
	textPlugin->SetAnchorParamVer(verPosTextProjectPlugin, verPosTextProjectPlugin);
	textPlugin->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	textPlugin->GetText()->SetOffset(Float2(10.0f, 0.0f));
	textPlugin->GetText()->SetFontScale(0.7f);
	textPlugin->SetPivot(0.0f, 0.5f);
	textPlugin->SetWidth(200.0f);
	textPlugin->GetText()->SetText(PX2_LM_APP.GetValue("Plugins"));

	float projectFrameHeight = 286.0f;
	mProjectFrame = UIFrame::New();
	mEngineFrame->AttachChild(mProjectFrame);
	mProjectFrame->SetAnchorHor(0.0f, 1.0f);
	mProjectFrame->SetAnchorParamHor(0.0f, -0.0f);
	mProjectFrame->SetAnchorVer(1.0f, 1.0f);
	float verPos = -topHeight - butHeight - spaceHeight - texEngineHeight;
	mProjectFrame->SetAnchorParamVer(verPos, verPos);
	mProjectFrame->SetHeight(projectFrameHeight);
	mProjectFrame->SetPivot(0.5f, 1.0f);

	mProjectList = UIList::New("ListProject");
	mProjectFrame->AttachChild(mProjectList);
	mProjectList->LocalTransform.SetTranslateY(-1.0f);
	mProjectList->SetAnchorHor(0.0f, 0.6f);
	mProjectList->SetAnchorParamHor(0.0f, 0.0f);
	mProjectList->SetAnchorVer(0.0f, 1.0f);
	mProjectList->SetPivot(0.5f, 0.5f);
	mProjectList->SetItemHeight(28.0f);
	mProjectList->SetItemBackColor(Float3::MakeColor(100, 100, 100));
	mProjectList->CreateAddBackgroundPicBox(true, Float3::MakeColor(50, 0, 0));
	mProjectList->AddVisitor(this);
	mProjectList->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	mProjectList->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/ButSlider.png");
	mProjectList->GetSlider()->GetButSlider()->SetStateColor(UIButtonBase::BS_NORMAL, Float3::MakeColor(196, 196, 196));
	mProjectList->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	mPluginList = UIList::New();
	mProjectFrame->AttachChild(mPluginList);
	mPluginList->LocalTransform.SetTranslateY(-1.0f);
	mPluginList->SetAnchorHor(0.6f, 1.0f);
	mPluginList->SetAnchorParamHor(0.0f, -5.0f);
	mPluginList->SetAnchorVer(0.0f, 1.0f);
	mPluginList->SetPivot(0.5f, 0.5f);
	mPluginList->SetItemHeight(30.0f);
	mPluginList->SetItemBackColor(Float3::MakeColor(120, 120, 120));
	mPluginList->CreateAddBackgroundPicBox(true, Float3::MakeColor(0, 0, 50));
	mPluginList->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	mPluginList->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/ButSlider.png");
	mPluginList->GetSlider()->GetButSlider()->SetStateColor(UIButtonBase::BS_NORMAL, Float3::MakeColor(196, 196, 196));
	mPluginList->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	mServerFrame = UIFrame::New();
	mEngineFrame->AttachChild(mServerFrame);
	mServerFrame->SetAnchorHor(0.0f, 1.0f);
	mServerFrame->SetAnchorParamHor(0.0f, -0.0f);
	mServerFrame->SetAnchorVer(0.0f, 1.0f);
	mServerFrame->SetAnchorParamVer(10.0f, -topHeight - butHeight - 10.0f - projectFrameHeight -15.0f - texEngineHeight);
	mServerFrame->SetPivot(0.5f, 1.0f);
	mServerFrame->SetHeight(250.0f);

	auto picTextServer = UIFText::New();
	mServerFrame->AttachChild(picTextServer);
	picTextServer->LocalTransform.SetTranslateY(-2.0f);
	picTextServer->SetAnchorHor(0.0f, 1.0f);
	picTextServer->SetAnchorVer(1.0f, 1.0f);
	picTextServer->SetPivot(0.5f, 1.0f);
	picTextServer->SetHeight(texEngineHeight);
	picTextServer->CreateAddBackgroundPicBox(true, Float3::BLACK);
	picTextServer->GetText()->SetText(PX2_LM_APP.GetValue("NetSyn"));
	picTextServer->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	picTextServer->GetText()->SetFontScale(0.7f);
	picTextServer->GetText()->SetFontColor(Float3::WHITE);
	picTextServer->GetText()->SetOffset(Float2(10.0f, 0.0f));

	float serverTopHeight = 30.0f;
	UIFrame *serverTop = UIFrame::New();
	mServerFrame->AttachChild(serverTop);
	serverTop->SetAnchorHor(0.0f, 1.0f);
	serverTop->SetAnchorVer(1.0f, 1.0f);
	serverTop->SetAnchorParamVer(-texEngineHeight, -texEngineHeight);
	serverTop->SetPivot(0.5f, 1.0f);
	serverTop->SetHeight(serverTopHeight);

	float butWidth1 = butLuancherWidth;
	for (int i = 0; i < 3; i++)
	{
		UIButton *but = UIButton::New();
		serverTop->AttachChild(but);
		but->LocalTransform.SetTranslateY(-1.0f);
		but->SetAnchorHor(0.0f, 0.0f);
		but->SetAnchorParamHor(5.0f + (butWidth1 + 5.0f) * i, 0.0f);
		but->SetAnchorVer(0.0f, 1.0f);
		but->SetAnchorParamVer(2.0f, -2.0f);
		but->SetSize(butWidth1, serverTopHeight);
		but->SetPivot(0.0f, 0.5f);

		UIFText *text = 0;
		if (0 == i)
		{
			but->SetName("ButNetSyn");
			text = but->CreateAddText(PX2_LM_APP.GetValue("Syn"));
			but->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
			but->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/whitern.png");
			but->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
		}
		else if (1 == i)
		{
			but->SetName("ButNetLoadProject");
			text = but->CreateAddText(PX2_LM_APP.GetValue("LoadProject"));
			but->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
			but->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/whitern.png");
			but->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
		}
		else if (2 == i)
		{
			but->SetName("ButNetCloseProject");
			text = but->CreateAddText(PX2_LM_APP.GetValue("CloseProject"));
			but->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
			but->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/whitern.png");
			but->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
		}

		text->GetText()->SetFontScale(0.7f);
		but->AddVisitor(this);
	}

	float inputHeight = 20.0f;
	float sendButWidth = 40.0f;
	mDeviceInput = new0 UIEditBox();
	mServerFrame->AttachChild(mDeviceInput);
	mDeviceInput->SetAnchorHor(0.0f, 1.0f);
	mDeviceInput->SetAnchorParamHor(5.0f, -sendButWidth - 10.0f);
	mDeviceInput->SetAnchorVer(1.0f, 1.0f);
	mDeviceInput->SetAnchorParamVer(-serverTopHeight - texEngineHeight, -serverTopHeight-texEngineHeight);
	mDeviceInput->SetPivot(0.5f, 1.0f);
	mDeviceInput->SetHeight(20.0f);
	mDeviceInput->GetBackPicBox()->SetColor(Float3::MakeColor(200, 200, 200));
	mDeviceInput->GetInputText()->SetText("");
	mDeviceInput->GetInputText()->SetFontScale(0.7f);
	mDeviceInput->GetBackPicBox()->GetUIPicBox()->SetPicBoxType(UIPicBox::PBT_NINE);
	mDeviceInput->GetBackPicBox()->GetUIPicBox()->SetTexture("Data/engine/whitern.png");
	mDeviceInput->GetBackPicBox()->GetUIPicBox()->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	UIButton *butSend = UIButton::New("ButNetSend");
	mServerFrame->AttachChild(butSend);
	butSend->SetName("ButSend");
	butSend->SetAnchorHor(1.0f, 1.0f);
	butSend->SetAnchorParamHor(-sendButWidth*0.5f - 5.0f, -sendButWidth*0.5f - 5.0f);
	butSend->SetAnchorVer(1.0f, 1.0f);
	butSend->SetAnchorParamVer(-serverTopHeight - texEngineHeight, -serverTopHeight - texEngineHeight);
	butSend->SetPivot(0.5f, 1.0f);
	butSend->SetHeight(20.0f);
	butSend->SetWidth(sendButWidth);
	butSend->CreateAddText(PX2_LM_APP.GetValue("Send"));
	butSend->AddVisitor(this);
	butSend->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	butSend->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/whitern.png");
	butSend->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	mDeviceList = UIList::New();
	mServerFrame->AttachChild(mDeviceList);
	mDeviceList->SetAnchorHor(0.0f, 1.0f);
	mDeviceList->SetAnchorParamHor(5.0f, -5.0f);
	mDeviceList->SetAnchorVer(0.0f, 1.0f);
	mDeviceList->SetAnchorParamVer(0.0f, -inputHeight - serverTopHeight-5.0f - texEngineHeight);
	mDeviceList->SetItemHeight(30.0f);
	mDeviceList->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	mDeviceList->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/ButSlider.png");
	mDeviceList->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
	return mEngineFrame;
}
//----------------------------------------------------------------------------
int LP_Manager::_GetProjType(const std::string &projName)
{
	if ("engine" == projName || "engine_mtls" == projName)
	{
		return 0;
	}

	std::string projPath = "Data/" + projName;
	if (!PX2_RM.IsFileFloderExist(projPath))
	{
		return 0;
	}

	std::string projFilename = projPath + "/project" + ".px2proj";
	if (!PX2_RM.IsFileFloderExist(projFilename))
		return 2;

	return 1;
}
//----------------------------------------------------------------------------
void LP_Manager::_RefreshProjects()
{
	mProjectList->RemoveAllItems();
	mPluginList->RemoveAllItems();
	ClearProjects();

	const AppBoostInfo &boostInfo = PX2_APP.GetBoostInfo();

	DirP d;
	std::string eachFilename;
	if (d.Open("Data/"))
	{
		if (!d.HasFiles() && !d.HasSubDirs())
			return;

		int flags =	DirP::DIR_DIRS;

		if (d.GetFirst(&eachFilename, "", flags))
		{
			do
			{

				UIItem *item = 0;
				std::string projName = eachFilename;

					std::string projXMLPath = "Data/" + projName + "/" +
					projName + ".px2proj";
				int id = Project::GetProjectID(projXMLPath);

				int projType = _GetProjType(projName);
				if (1 == projType)
				{
					LP_ProjectItem *projItem = new0 LP_ProjectItem();
					AddProject(projItem);
					item = _AddProjectItem(projName, id, true, projItem);
				}
				else if (2 == projType)
				{
					item = mPluginList->AddItem(projName);
					item->SetName(projName);

					UICheckButton *cb = UICheckButton::New(projName);
					item->AttachChild(cb);

					cb->LocalTransform.SetTranslateY(-2.0f);
					cb->SetAnchorHor(1.0f, 1.0f);
					cb->SetAnchorVer(0.5f, 0.5f);
					cb->SetAnchorParamHor(-20.0f, -20.0f);
					cb->SetSize(16.0f, 16.0f);
					cb->Check(boostInfo.IsHasPlugin(projName), false);
					cb->AddVisitor(this);
					cb->SetUserData("IsPlugin", true);
				}

				if (item)
				{
					item->GetFText()->GetText()->SetFontScale(0.85f);
				}

			} while (d.GetNext(&eachFilename));
		}
	}

	for (int i = 0; i < GetNumProjects_Cloud(); i++)
	{
		LP_ProjectItem *cloudItem = GetProjectItem_Cloud(i);
		if (!IsHasProject(cloudItem->ID))
		{
			AddProject(cloudItem);

			UIItem *projItem = _AddProjectItem(cloudItem->Name,
				cloudItem->ID, false, cloudItem);
			cloudItem->TheCloudButton->Show(true);
			cloudItem->TheCloudButton->SetAlpha(0.2f);
			cloudItem->TheItem->GetFText()->SetAlpha(0.7f);
			cloudItem->TheCheckButton->Show(false);
		}
	}

	mProjectList->SelectItem(0);
	mPluginList->SelectItem(0);

	PX2_APP.WriteBoost();
}
//----------------------------------------------------------------------------
UIItem *LP_Manager::_AddProjectItem(const std::string &name, int id,
	bool isLocalExist, LP_ProjectItem * projItem)
{
	const AppBoostInfo &boostInfo = PX2_APP.GetBoostInfo();

	UIItem *item = mProjectList->AddItem(name);
	item->SetName(name);

	projItem->ID = id;
	projItem->Name = name;
	projItem->IsLocalExist = isLocalExist;

	projItem->IsCloud = IsHasProject_Cloud(id);

	UICheckButton *cb = UICheckButton::New(name);
	item->AttachChild(cb);
	cb->LocalTransform.SetTranslateY(-2.0f);
	cb->SetAnchorHor(1.0f, 1.0f);
	cb->SetAnchorVer(0.5f, 0.5f);
	cb->SetAnchorParamHor(-20.0f, -20.0f);
	cb->SetSize(16.0f, 16.0f);
	cb->Check(boostInfo.IsHasProject(name), false);
	cb->AddVisitor(this);
	cb->SetUserData("IsPlugin", false);

	auto cloudBtn = UIButton::New("CloudPic");
	item->AttachChild(cloudBtn);
	cloudBtn->LocalTransform.SetTranslateY(-2.0f);
	cloudBtn->SetStateColorDefaultWhite();
	cloudBtn->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexture("Data/LPlugin/images/cloud.png");
	cloudBtn->SetSize(25.0f, 16.0f);
	cloudBtn->SetAnchorParamHor(-50.0f, -50.0f);
	cloudBtn->SetAnchorHor(1.0f, 1.0f);

	cloudBtn->Show(projItem->IsCloud);

	if (projItem->IsCloud)
	{
		cloudBtn->SetAlpha(1.0f);
	}

	projItem->TheCheckButton = cb;
	projItem->TheCloudButton = cloudBtn;
	projItem->TheItem = item;

	return item;
}
//----------------------------------------------------------------------------
void LP_Manager::_RefreshAddresses()
{
	mDeviceList->RemoveAllItems();

	int numUDPNetInfos = PX2_APP.GetNumUDPNetInfo();
	for (int i = 0; i < numUDPNetInfos; i++)
	{
		UDPNetInfo *info = PX2_APP.GetUDPNetInfo(i);

		std::string strName = info->Name + ":" + info->IP;
		UIItem *item = mDeviceList->AddItem(strName);
		item->SetName(info->Name);
		item->SetUserData("IP", info->IP);
	}
}
//----------------------------------------------------------------------------
void LP_Manager::_OnGenProjInfo()
{
	std::string versionText = mEditBoxVersion->GetText();
	UIItem *item = mProjectList->GetSelectedItem();
	if (item)
	{
		const std::string &name = item->GetName();
		if (!name.empty())
		{
			PX2_APP.GenerateFileList("Data/", name, versionText);
		}
	}
}
//----------------------------------------------------------------------------
void LP_Manager::_OnSyn()
{
	UIItem *itemProj = mProjectList->GetSelectedItem();
	if (!itemProj)
		return;

	std::string projName = itemProj->GetName();

	_OnGenProjInfo();

	int selectIndex = mDeviceList->GetSelectIndex();
	UIItem *item = mDeviceList->GetSelectedItem();
	if (item)
	{
		std::string name = item->GetName();

		UDPServer *udpServer = PX2_APP.GetEngineUDPServerEditor();
		if (udpServer)
		{
			UDPNetInfo *info = PX2_APP.GetUDPNetInfo(selectIndex);
			if (info && UDPNetInfo::T_DEVICE == info->TheType)
			{
				SocketAddress skAddr(info->IP, EngineUDPPortClient);
				std::string strBuf = CMD_PushProject + " " + projName 
					+ " " + "http";
				udpServer->GetSocket().SendTo(strBuf.c_str(),
					(int)strBuf.length(), skAddr);
			}
		}
	}
}
//----------------------------------------------------------------------------
void LP_Manager::_OnLoad()
{
	UIItem *itemProj = mProjectList->GetSelectedItem();
	if (!itemProj)
		return;

	std::string projName = itemProj->GetName();

	int selectIndex = mDeviceList->GetSelectIndex();
	UIItem *item = mDeviceList->GetSelectedItem();
	if (item)
	{
		std::string name = item->GetName();

		UDPServer *udpServer = PX2_APP.GetEngineUDPServerEditor();
		if (udpServer)
		{
			UDPNetInfo *info = PX2_APP.GetUDPNetInfo(selectIndex);
			if (info)
			{
				SocketAddress skAddr(info->IP, EngineUDPPortClient);

				std::string strBuf = CMD_LoadProject + " " + projName;
				udpServer->GetSocket().SendTo(strBuf.c_str(),
					(int)strBuf.length(), skAddr);
			}
		}
	}
}
//----------------------------------------------------------------------------
void LP_Manager::_OnClose()
{
	int selectIndex = mDeviceList->GetSelectIndex();
	UIItem *item = mDeviceList->GetSelectedItem();
	if (item)
	{
		std::string name = item->GetName();

		UDPServer *udpServer = PX2_APP.GetEngineUDPServerEditor();
		if (udpServer)
		{
			UDPNetInfo *info = PX2_APP.GetUDPNetInfo(selectIndex);
			if (info)
			{
				SocketAddress skAddr(info->IP, EngineUDPPortClient);

				std::string strBuf = CMD_CloseProject;
				udpServer->GetSocket().SendTo(strBuf.c_str(),
					(int)strBuf.length(), skAddr);
			}
		}
	}
}
//----------------------------------------------------------------------------
void PX2::LP_Manager::_CreateNewProjectFrame()
{
	mCreateProjectBackground = new0 UIFrame();
	mEngineFrame->AttachChild(mCreateProjectBackground);
	mCreateProjectBackground->LocalTransform.SetTranslateY(-10.0);
	mCreateProjectBackground->SetWidget(true);
	mCreateProjectBackground->SetAnchorHor(0, 1.0f);
	mCreateProjectBackground->SetAnchorVer(0, 1.0f);
	auto picBack = mCreateProjectBackground->CreateAddBackgroundPicBox(true, Float3::BLACK);
	picBack->UseAlphaBlend(true);
	picBack->SetFakeTransparent(false);
	picBack->SetAlpha(0.9f);
	
	UIFrame* projectFrame = new0 UIFrame();
	mCreateProjectBackground->AttachChild(projectFrame);
	projectFrame->LocalTransform.SetTranslateY(-1.0f);
	projectFrame->SetAnchorHor(0.0f, 1.0f);
	projectFrame->SetAnchorVer(0.5f, 0.5f);
	projectFrame->SetHeight(200.0);
	projectFrame->CreateAddBackgroundPicBox(true, Float3::MakeColor(200, 200, 200));

	UIFText* mCreateProText = new0 UIFText();
	projectFrame->AttachChild(mCreateProText);
	mCreateProText->LocalTransform.SetTranslateY(-1.0f);
	mCreateProText->GetText()->SetText(PX2_LM_APP.GetValue("Create new project"));
	mCreateProText->SetSize(300.0f, 20.0f);
	mCreateProText->SetAnchorVer(1.0f, 1.0f);
	mCreateProText->SetPivot(0.5f, 1.0f);
	mCreateProText->SetAnchorParamVer(-5.0f, -5.0f);

	mCreateProEditbox = new0 UIEditBox();
	projectFrame->AttachChild(mCreateProEditbox);
	mCreateProEditbox->SetAnchorHor(0.0f, 1.0f);
	mCreateProEditbox->LocalTransform.SetTranslateY(-2.0);
	mCreateProEditbox->SetText("");
	mCreateProEditbox->SetAnchorParamHor(10.0f, -10.0f);
	mCreateProEditbox->SetAnchorParamVer(50.0f, 50.0f);

	UIFText *textProjectName = new0 UIFText();
	mCreateProEditbox->AttachChild(textProjectName);
	textProjectName->LocalTransform.SetTranslateY(-2.0f);
	textProjectName->SetAnchorHor(0.0f, 1.0f);
	textProjectName->SetAnchorVer(0.0f, 1.0f);
	textProjectName->SetWidth(50.0f);
	textProjectName->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	textProjectName->GetText()->SetText("ProjectName");
	textProjectName->SetAlpha(0.2f);

	mCreateProWidthEditbox = new0 UIEditBox();
	projectFrame->AttachChild(mCreateProWidthEditbox);
	mCreateProWidthEditbox->LocalTransform.SetTranslateY(-2.0);
	mCreateProWidthEditbox->SetAnchorHor(0.0f, 0.5f);
	mCreateProWidthEditbox->SetAnchorParamHor(10.0f, -10.0f);
	mCreateProWidthEditbox->SetAnchorParamVer(0.0f, 0.0f);
	mCreateProWidthEditbox->SetWidth(100.0f);
	mCreateProWidthEditbox->SetText("1136");

	UIFText *textWidth = new0 UIFText();
	mCreateProWidthEditbox->AttachChild(textWidth);
	textWidth->LocalTransform.SetTranslateY(-2.0f);
	textWidth->SetAnchorHor(0.0f, 1.0f);
	textWidth->SetAnchorVer(0.0f, 1.0f);
	textWidth->SetWidth(50.0f);
	textWidth->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	textWidth->GetText()->SetText("Width");
	textWidth->SetAlpha(0.2f);

	mCreateProHeightEditbox = new0 UIEditBox();
	projectFrame->AttachChild(mCreateProHeightEditbox);
	mCreateProHeightEditbox->LocalTransform.SetTranslateY(-2.0);
	mCreateProHeightEditbox->SetAnchorHor(0.5f, 1.0f);
	mCreateProHeightEditbox->SetAnchorParamHor(10.0f, -10.0f);
	mCreateProHeightEditbox->SetAnchorParamVer(0.0f, 0.0f);
	mCreateProHeightEditbox->SetWidth(100.0f);
	mCreateProHeightEditbox->SetText("640");

	UIFText *textHeight = new0 UIFText();
	mCreateProHeightEditbox->AttachChild(textHeight);
	textHeight->LocalTransform.SetTranslateY(-2.0f);
	textHeight->SetAnchorHor(0.0f, 1.0f);
	textHeight->SetAnchorVer(0.0f, 1.0f);
	textHeight->SetWidth(50.0f);
	textHeight->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	textHeight->GetText()->SetText("Height");
	textHeight->SetAlpha(0.2f);

	mCreateProLandscapeCheckButton = UICheckButton::New("CheckLandscape");
	projectFrame->AttachChild(mCreateProLandscapeCheckButton);
	mCreateProLandscapeCheckButton->LocalTransform.SetTranslateY(-2.0);
	mCreateProLandscapeCheckButton->SetAnchorHor(0.0f, 0.0f);
	mCreateProLandscapeCheckButton->SetAnchorParamHor(20.0f, 20.0f);
	mCreateProLandscapeCheckButton->SetAnchorVer(0.5f, 0.5f);
	mCreateProLandscapeCheckButton->SetAnchorParamVer(-40.0f, -40.0f);
	mCreateProLandscapeCheckButton->SetSize(20.0f, 20.0f);
	mCreateProLandscapeCheckButton->AddUICallback(_UICallback);
	mCreateProLandscapeCheckButton->Check(true);

	UIFText *text = UIFText::New();
	mCreateProLandscapeCheckButton->AttachChild(text);
	text->SetAnchorHor(0.0f, 0.0f);
	text->SetAnchorParamHor(20.0f + 5.0f, 20.0f + 5.0f);
	text->SetAnchorVer(0.5f, 0.5f);
	text->SetPivot(0.0f, 0.5f);
	text->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	text->GetText()->SetText("Landscape");

	UIButton* makeProjectOK = UIButton::New("MakeProjectOK");
	projectFrame->AttachChild(makeProjectOK);
	makeProjectOK->SetAnchorParamHor(-80.0f, -80.0f);
	makeProjectOK->SetAnchorVer(0.0f, 0.0f);
	makeProjectOK->SetAnchorParamVer(30.0f, 30.0f);
	makeProjectOK->SetSize(110.0f, 30.0f);
	makeProjectOK->LocalTransform.SetTranslateY(-2.0);
	makeProjectOK->CreateAddText(PX2_LM_APP.GetValue("OK"));
	makeProjectOK->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	makeProjectOK->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/whitern.png");
	makeProjectOK->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
	makeProjectOK->AddUICallback(_UICallback);

	UIButton* butCancel = UIButton::New();
	projectFrame->AttachChild(butCancel);
	butCancel->SetAnchorParamHor(80.0f, 80.0f);
	butCancel->SetAnchorVer(0.0f, 0.0f);
	butCancel->SetAnchorParamVer(30.0f, 30.0f);
	butCancel->SetSize(110.0f, 30.0f);
	butCancel->LocalTransform.SetTranslateY(-2.0);
	butCancel->CreateAddText(PX2_LM_APP.GetValue("Cancel"));
	butCancel->SetName("Cancel");
	butCancel->AddUICallback(_UICallback);
	butCancel->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	butCancel->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/whitern.png");
	butCancel->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
	mCreateProjectBackground->Show(false);
	mCreateProjectBackground->SetWidget(true);
}
//----------------------------------------------------------------------------
void LP_Manager::ShowUpdating(bool show)
{
	mUpdaterFrame->Show(show);
}
//----------------------------------------------------------------------------
void LP_Manager::_ShowProjectCreate(bool isShow)
{
	mCreateProjectBackground->Show(isShow);
}
//----------------------------------------------------------------------------
void LP_Manager::_CreateLoginFrame()
{
	mLoginInFrame = new0 UIFrame();
	mUIMainFrame->AttachChild(mLoginInFrame);
	mLoginInFrame->LocalTransform.SetTranslateY(-30.0);
	mLoginInFrame->SetWidget(true);
	mLoginInFrame->SetAnchorHor(0.0, 1.0);
	mLoginInFrame->SetAnchorVer(0.0, 1.0);
	auto picBack = mLoginInFrame->CreateAddBackgroundPicBox(true, Float3::BLACK);
	picBack->UseAlphaBlend(true);
	picBack->SetFakeTransparent(false);
	picBack->SetAlpha(0.9f);

	UIFrame* signBack = UIFrame::New();
	mLoginInFrame->AttachChild(signBack);
	signBack->LocalTransform.SetTranslateY(-1.0f);
	signBack->SetAnchorHor(0.0, 1.0);
	signBack->SetAnchorVer(0.5, 0.5);
	signBack->SetHeight(400);
	signBack->CreateAddBackgroundPicBox(true, Float3::MakeColor(0, 0, 0));

	auto text = UIFText::New();
	mLoginInFrame->AttachChild(text);
	text->LocalTransform.SetTranslateY(-5.0f);
	text->GetText()->SetText(PX2_LM_APP.GetValue("Login"));
	text->GetText()->SetFontColor(Float3::WHITE);
	text->SetAnchorVer(0.5f, 0.5f);
	text->SetAnchorParamVer(100.0f, 100.0f);
	text->GetText()->SetFontSize(24.0f);
	text->SetHeight(60.0f);

	// use name
	mEditBoxUserName = UIEditBox::New();
	signBack->AttachChild(mEditBoxUserName);
	mEditBoxUserName->LocalTransform.SetTranslateY(-1.0f);
	mEditBoxUserName->SetAnchorHor(0.2f, 0.8f);
	mEditBoxUserName->SetAnchorVer(0.5f, 0.5f);
	mEditBoxUserName->SetAnchorParamVer(25.0f, 25.0f);
	auto textLogin = UIFText::New();
	signBack->AttachChild(textLogin);
	textLogin->LocalTransform.SetTranslateY(-2.0f);
	textLogin->GetText()->SetText(PX2_LM_APP.GetValue("Account"));
	textLogin->GetText()->SetFontColor(Float3::WHITE);
	textLogin->SetAnchorHor(0.2f, 0.2f);
	textLogin->SetAnchorParamHor(-25.0f, -25.0f);
	textLogin->SetAnchorVer(0.5f, 0.5f);
	textLogin->SetAnchorParamVer(25.0f, 25.0f);

	// password
	mEditBoxPassword = UIEditBox::New();
	signBack->AttachChild(mEditBoxPassword);
	mEditBoxPassword->LocalTransform.SetTranslateY(-1.0f);
	mEditBoxPassword->SetAnchorHor(0.2f, 0.8f);
	mEditBoxPassword->SetAnchorVer(0.5f, 0.5f);
	mEditBoxPassword->SetAnchorParamVer(-30.0f, -30.0f);
	mEditBoxPassword->SetPassword(true);

	auto textPassword = UIFText::New();
	signBack->AttachChild(textPassword);
	textPassword->LocalTransform.SetTranslateY(-2.0f);
	textPassword->GetText()->SetText(PX2_LM_APP.GetValue("Password"));
	textPassword->GetText()->SetFontColor(Float3::WHITE);
	textPassword->SetColor(Float3::WHITE);
	textPassword->SetAnchorHor(0.2f, 0.2f);
	textPassword->SetAnchorParamHor(-25.0f, -25.0f);
	textPassword->SetAnchorVer(0.5f, 0.5f);
	textPassword->SetAnchorParamVer(-30.0f, -30.0f);

	//添加复选框
	UICheckButton* checkBut = UICheckButton::New();
	signBack->AttachChild(checkBut);
	checkBut->LocalTransform.SetTranslateY(-2.0f);
	checkBut->SetAnchorHor(0.2f, 0.2f);
	checkBut->SetAnchorParamHor(10.0f, 10.0f);
	checkBut->SetAnchorParamVer(-70.0f, -70.0f);
	checkBut->SetSize(17.0f, 17.0f);

	//添加文字"记住我"
	UIFText* remembertext = UIFText::New();
	signBack->AttachChild(remembertext);
	remembertext->LocalTransform.SetTranslateY(-2.0f);
	remembertext->SetAnchorHor(0.2f, 0.2f);
	remembertext->SetAnchorParamHor(45.0f, 45.0f);
	remembertext->SetAnchorParamVer(-70.0f, -70.0f);
	remembertext->GetText()->SetText(PX2_LM_APP.GetValue("RememberMe"));
	remembertext->GetText()->SetFontScale(0.7f);
	remembertext->GetText()->SetFontColor(Float3::WHITE);

	//添加文字“忘记密码”
	UIButton* btnFroget = UIButton::New();
	signBack->AttachChild(btnFroget);
	btnFroget->SetName("BtnForgetPassword");
	btnFroget->SetAnchorHor(0.8f, 0.8f);
	btnFroget->SetAnchorParamHor(-30.0f, 30.0f);
	btnFroget->SetAnchorParamVer(-70.0f, -70.0f);
	btnFroget->CreateAddText(PX2_LM_APP.GetValue("ForgetPassword"));
	btnFroget->SetSize(60.0f, 20.0f);
	btnFroget->GetText()->SetFontScale(0.7f);
	btnFroget->AddVisitor(this);

	//登入按钮
	UIButton* loginBtn = UIButton::New();
	signBack->AttachChild(loginBtn);
	loginBtn->SetName("BtnLoginOK");
	loginBtn->LocalTransform.SetTranslateY(-2.0f);
	loginBtn->SetAnchorParamHor(-60.0f, -60.0f);
	loginBtn->SetAnchorParamVer(-120.0f, -120.0f);
	loginBtn->SetSize(100.0f, 30.0f);
	loginBtn->SetStateColorDefaultWhite();
	loginBtn->CreateAddText(PX2_LM_APP.GetValue("Login"));
	loginBtn->SetColor(Float3::MakeColor(133, 201, 145));
	loginBtn->AddVisitor(this);

	UIButton* loginCancel = UIButton::New();
	signBack->AttachChild(loginCancel);
	loginCancel->SetName("BtnLoginCancel");
	loginCancel->LocalTransform.SetTranslateY(-2.0f);
	loginCancel->SetAnchorParamHor(60.0f, 60.0f);
	loginCancel->SetAnchorParamVer(-120.0f, -120.0f);
	loginCancel->SetSize(100.0f, 30.0f);
	loginCancel->SetStateColorDefaultWhite();
	loginCancel->CreateAddText(PX2_LM_APP.GetValue("Cancel"));
	loginCancel->SetColor(Float3::MakeColor(133, 201, 145));
	loginCancel->AddVisitor(this);

	mLoginInFrame->AddUICallback(_UICallback);
	mLoginInFrame->Show(false);
	mLoginInFrame->SetWidget(true);
}
//----------------------------------------------------------------------------
UIFrame *LP_Manager::_CreateZeroneRobotFrame()
{
	mZeroneRobotFrame = new0 UIFrame();
	mZeroneRobotFrame->SetAnchorHor(0.0f, 1.0f);
	mZeroneRobotFrame->SetAnchorVer(0.0f, 1.0f);

	UIFPicBox *picBox = UIFPicBox::New();
	mZeroneRobotFrame->AttachChild(picBox);
	picBox->GetUIPicBox()->SetTexture("Data/LPlugin/images/zeronerobot.png");
	picBox->SetAnchorHor(0.0f, 1.0f);
	picBox->SetAnchorVer(1.0f, 1.f);
	picBox->SetPivot(0.5f, 1.0f);
	picBox->SetHeight(96.0f);

	UIButton *butShop = UIButton::New("ButShop");
	mZeroneRobotFrame->AttachChild(butShop);
	butShop->LocalTransform.SetTranslateY(-2.0f);
	butShop->SetAnchorHor(0.0f, 1.0f);
	butShop->SetAnchorParamHor(5.0f, -5.0f);
	butShop->SetAnchorVer(1.0f, 1.0f);
	butShop->SetPivot(0.5f, 1.0f);
	butShop->SetAnchorParamVer(-100.0f, -100.0f);
	butShop->SetHeight(45.0f);
	auto text = butShop->CreateAddText(PX2_LM_APP.GetValue("GetFromTaobao"));
	text->GetText()->SetFontSize(24.0f);
	butShop->SetStateColorDefaultWhite();
	butShop->SetColor(Float3::MakeColor(0, 71, 118));
	butShop->AddVisitor(this);

	UIFrame *frameContent = UIFrame::New();
	mZeroneRobotFrame->AttachChild(frameContent);
	frameContent->SetAnchorHor(0.0f, 1.0f);
	frameContent->SetAnchorVer(0.0f, 1.0f);
	frameContent->SetAnchorParamVer(5.0f, -150.0f);

	int numCnt = 3;
	for (int i = 0; i < numCnt; i++)
	{
		UIFPicBox *picBox = new0 UIFPicBox();
		frameContent->AttachChild(picBox);
		picBox->SetAnchorHor(0.0f, 1.0f);
		float verStart = i * (1.0f / (numCnt*1.0f));
		float verEnd = (i+1) * (1.0f / (numCnt*1.0f));
		picBox->SetAnchorVer(verStart, verEnd);
		picBox->SetAnchorParamVer(1.0f, -1.0f);

		if (i == 2)
		{
			picBox->GetUIPicBox()->SetTexture("Data/LPlugin/images/pic1.png");
		}
		else if (i == 1)
		{
			picBox->GetUIPicBox()->SetTexture("Data/LPlugin/images/pic2.png");
		}
		else if (i == 0)
		{
			picBox->GetUIPicBox()->SetTexture("Data/LPlugin/images/pic3.png");
		}
	}

	return mZeroneRobotFrame;
}
//----------------------------------------------------------------------------
UIFrame *LP_Manager::CreateCodePlayFrame()
{
	mCodePlayFrame = new0 UIFrame();
	mCodePlayFrame->SetAnchorHor(0.0f, 1.0f);
	mCodePlayFrame->SetAnchorVer(0.0f, 1.0f);

	UIButton *butShop = UIButton::New("ButCodePlay");
	mCodePlayFrame->AttachChild(butShop);
	butShop->LocalTransform.SetTranslateY(-2.0f);
	butShop->SetAnchorHor(0.0f, 1.0f);
	butShop->SetAnchorParamHor(5.0f, -5.0f);
	butShop->SetAnchorVer(1.0f, 1.0f);
	butShop->SetPivot(0.5f, 1.0f);
	butShop->SetAnchorParamVer(-5.0f, -5.0f);
	butShop->SetHeight(20.0f);
	auto text = butShop->CreateAddText("http://www.manykit.com");
	text->GetText()->SetFontScale(0.8f);
	butShop->AddVisitor(this);

	UIFrame *frameContent = UIFrame::New();
	mCodePlayFrame->AttachChild(frameContent);
	frameContent->SetAnchorHor(0.0f, 1.0f);
	frameContent->SetAnchorVer(0.0f, 1.0f);
	frameContent->SetAnchorParamVer(5.0f, -28.0f);

	int numCnt = 3;
	for (int i = 0; i < numCnt; i++)
	{
		UIFPicBox *picBox = new0 UIFPicBox();
		frameContent->AttachChild(picBox);
		picBox->SetAnchorHor(0.0f, 1.0f);
		float verStart = i * (1.0f / (numCnt*1.0f));
		float verEnd = (i + 1) * (1.0f / (numCnt*1.0f));
		picBox->SetAnchorVer(verStart, verEnd);
		picBox->SetAnchorParamVer(1.0f, -1.0f);

		if (i == 2)
		{
			picBox->GetUIPicBox()->SetTexture("Data/LPlugin/images/prog_bat.png");
		}
		else if (i == 1)
		{
			picBox->GetUIPicBox()->SetTexture("Data/LPlugin/images/prog_snap.png");
		}
		else if (i == 0)
		{
			picBox->GetUIPicBox()->SetTexture("Data/LPlugin/images/prog_scratch.png");
		}
	}

	return mCodePlayFrame;
}
//----------------------------------------------------------------------------
UIFrame *LP_Manager::CreateToy()
{
	UIFrame *frame = new0 UIFrame();
	frame->SetAnchorHor(0.0, 1.0f);
	frame->SetAnchorVer(0.0, 1.0f);

	auto btn = UIButton::New("BtnToy");
	frame->AttachChild(btn);
	btn->LocalTransform.SetTranslateY(-2.0f);
	btn->SetStateColorDefaultWhite();
	btn->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture(
		"Data/LPlugin/images/toy.png");
	btn->SetAnchorHor(0.0, 1.0f);
	btn->SetAnchorVer(0.0, 1.0f);
	
	return frame;
}
//----------------------------------------------------------------------------
void LP_Manager::_ShowSignIn(bool isShow)
{
	mLoginInFrame->Show(isShow);
}
//----------------------------------------------------------------------------
UIEditBox* LP_Manager::GetCreateProjectUIEditBox()
{
	return mCreateProEditbox;
}
//----------------------------------------------------------------------------
UIEditBox *LP_Manager::GetCreateProjectWidthUIEditBox()
{
	return mCreateProWidthEditbox;
}
//----------------------------------------------------------------------------
UIEditBox *LP_Manager::GetCreateProjectHeightUIEditBox()
{
	return mCreateProHeightEditbox;
}
//----------------------------------------------------------------------------
UICheckButton *LP_Manager::GetCreateProjectLandscapeButton()
{
	return mCreateProLandscapeCheckButton;
}
//----------------------------------------------------------------------------
void LP_Manager::ClearProjects()
{
	mProjects.clear();
}
//----------------------------------------------------------------------------
bool LP_Manager::IsHasProject(int id)
{
	if (0 == id)
		return false;

	for (int i = 0; i < (int)mProjects.size(); i++)
	{
		if (id == mProjects[i]->ID)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool LP_Manager::AddProject(LP_ProjectItem *projItem)
{
	if (IsHasProject(projItem->ID))
		return false;

	mProjects.push_back(projItem);

	return true;
}
//----------------------------------------------------------------------------
int LP_Manager::GetNumProjects() const
{
	return (int)mProjects.size();
}
//----------------------------------------------------------------------------
LP_ProjectItem *LP_Manager::GetProjectItem(int index)
{
	if (0<=index && index<(int)mProjects.size())
		return mProjects[index];

	return 0;
}
//----------------------------------------------------------------------------
void LP_Manager::ClearProjects_Cloud()
{
	mProjects_Cloud.clear();
}
//----------------------------------------------------------------------------
bool LP_Manager::IsHasProject_Cloud(int id)
{
	if (0 == id)
		return false;

	for (int i = 0; i < (int)mProjects_Cloud.size(); i++)
	{
		if (id == mProjects_Cloud[i]->ID)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool LP_Manager::AddProject_Cloud(LP_ProjectItem *projItem)
{
	if (IsHasProject_Cloud(projItem->ID))
		return false;

	mProjects_Cloud.push_back(projItem);

	return true;
}
//----------------------------------------------------------------------------
int LP_Manager::GetNumProjects_Cloud() const
{
	return (int)mProjects_Cloud.size();
}
//----------------------------------------------------------------------------
LP_ProjectItem *LP_Manager::GetProjectItem_Cloud(int index)
{
	if (0 <= index && index < (int)mProjects_Cloud.size())
		return mProjects_Cloud[index];

	return 0;
}
//----------------------------------------------------------------------------