// PX2EngineCanvas.cpp

#include "PX2EngineCanvas.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2Project.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2UIFPicBox.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2ProjectEventController.hpp"
#include "PX2Application.hpp"
#include "PX2UIProgressBar.hpp"
#include "PX2DNS.hpp"
#include "PX2HostEntry.hpp"
#include "PX2VoiceSDK.hpp"
#include "PX2Float3.hpp"
#include "PX2GraphicsEventType.hpp"
#include "PX2EngineNetEvent.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2Bluetooth.hpp"
#include "PX2Serial.hpp"
#include "PX2StringTokenizer.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2PlatformSDKEventType.hpp"
#include "PX2InputManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Canvas, EngineCanvas);
PX2_IMPLEMENT_STREAM(EngineCanvas);
PX2_IMPLEMENT_FACTORY(EngineCanvas);
PX2_IMPLEMENT_DEFAULT_NAMES(Canvas, EngineCanvas);

//----------------------------------------------------------------------------
void _InitEngineFrameShow(UIFrame *engineFrame)
{
	UIEditBox *editBoxIP = DynamicCast<UIEditBox>(
		engineFrame->GetObjectByName("EditBoxIP"));
	UIEditBox *editBoxPort = DynamicCast<UIEditBox>(
		engineFrame->GetObjectByName("EditBoxPort"));
	UICheckButton *checkBut = DynamicCast<UICheckButton>(
		engineFrame->GetObjectByName("CheckAutoConnect"));

	if (editBoxIP)
	{
		EngineClientConnector *cnt = PX2_APP.GetEngineClientConnector();
		bool isAutoCnt = cnt->IsAutoConnect();
		std::string ip = cnt->GetAutoConnectIP();
		int port = cnt->GetAutoConnectPort();

		editBoxIP->SetText(ip);
		editBoxPort->SetText(StringHelp::IntToString(port));
		checkBut->Check(isAutoCnt, false);
	}
}
//----------------------------------------------------------------------------
void _EngineUICallback (UIFrame *frame, UICallType type)
{
	EngineCanvas *engineCanvas = EngineCanvas::GetSingletonPtr();
	UIFrame *engineFrameBack = engineCanvas->GetEngineFrameBack();
	UIFrame *engineFrame = engineCanvas->GetEngineFrame();

	const std::string &name = frame->GetName();
	UIButton *but = DynamicCast<UIButton>(frame);
	UICheckButton *checkBut = DynamicCast<UICheckButton>(frame);
	if (UICT_PRESSED == type)
	{
		if (but)
		{
			if ("ButEngine" == name)
			{
				PX2_GR.PlayScale(frame);
			}
			else if ("ButReload" == name)
			{
				PX2_GR.PlayScale(frame);
			}
		}
	}
	else if (UICT_RELEASED_NOTPICK == type)
	{
		if (but)
		{
			if ("ButEngine" == name)
			{
				PX2_GR.PlayNormal(frame);
			}
			else if ("ButReload" == name)
			{
				PX2_GR.PlayNormal(frame);
			}
		}
	}
	else if (UICT_RELEASED == type)
	{
		if (but)
		{
			if ("ButEngine" == name)
			{
				engineFrameBack->Show(!engineFrameBack->IsShow());
				if (engineFrameBack->IsShow())
				{
					_InitEngineFrameShow(engineFrame);
				}

				PX2_GR.PlayNormal(frame);
			}
			else if ("ButReload" == name)
			{
				PX2_GR.PlayNormal(frame);

				std::string projName = PX2_APP.GetProjectName();
				PX2_APP.CloseProject();
				PX2_APP.LoadProject(projName);
				PX2_APP.Play(Application::PT_PLAY);
			}
			else if ("ButClose" == name)
			{
				engineFrameBack->Show(false);
			}
			else if ("ButProjectOpen" == name)
			{
				UIItem *item = engineCanvas->GetEngineProjectList()->GetSelectedItem();
				if (item)
				{
					std::string projName = item->GetName();
					if (!projName.empty())
					{
						PX2_APP.CloseProject();
						PX2_APP.LoadProject(projName);
						PX2_APP.Play(Application::PT_PLAY);
					}
				}
			}
			else if ("ButProjectClose" == name)
			{
				PX2_APP.CloseProject();
			}
			else if ("ButSpeak" == name)
			{
				UIFText *text = DynamicCast<UIFText>(
					engineCanvas->GetEngineFrame()->GetObjectByName("TextIP"));
				const std::string &strText = text->GetText()->GetText();
				if (!strText.empty())
				{
					PX2_VOICESDK.Speak(strText);
				}
			}
			else if ("ButStartEngineServer" == name)
			{
				EngineServer *es = PX2_APP.GetEngineServer();
				if (es)
				{
					if (!es->IsStarted())
					{
						bool ret = es->Start();
						if (ret)
						{
							but->GetText()->SetText("ShutdownEngineServer");

							UIFText *text = DynamicCast<UIFText>(
								engineCanvas->GetEngineFrame()->GetObjectByName("TextIP"));
							std::string ipStr;
							int port = es->GetPort();
#if defined (__ANDROID__)
							ipStr = HostEntry::GetAndroidIP(); 
#else
							// get last ip
							HostEntry hostEntry = DNS::GetThisHost();
							HostEntry::AddressList addressList = hostEntry.GetAddresses();
							if (addressList.size() > 0)
							{
								for (int i = 0; i < (int)addressList.size(); i++)
								{
									IPAddress ipAddress = addressList[i];
									ipStr = ipAddress.ToString();
								}
							}
#endif
							std::string ipPortStr = ipStr + ":" + StringHelp::IntToString(port);
							text->GetText()->SetText(ipPortStr);
						}
					}
					else
					{
						es->Shutdown();
						but->GetText()->SetText("StartEngineServer");

						UIFText *text = DynamicCast<UIFText>(
							engineCanvas->GetEngineFrame()->GetObjectByName("TextIP"));
						text->GetText()->SetText("");
					}
				}
			}
			else if ("ButClientConnect" == name)
			{
				bool isConnected = PX2_APP.GetEngineClientConnector()->IsConnected();
				if (!isConnected)
				{
					UIEditBox *editBoxIP = DynamicCast<UIEditBox>(
						engineCanvas->GetEngineFrame()->GetObjectByName("EditBoxIP"));
					const std::string &editBoxIPText = editBoxIP->GetText();

					UIEditBox *editBoxPort = DynamicCast<UIEditBox>(
						engineCanvas->GetEngineFrame()->GetObjectByName("EditBoxPort"));
					const std::string &editBoxPortText = editBoxPort->GetText();
					int port = StringHelp::StringToInt(editBoxPortText);

					std::string cntStr = editBoxIPText + ":" + editBoxPortText;
					engineCanvas->GetEngineInfoList()->AddItem(cntStr);

					PX2_APP.GetEngineClientConnector()->ConnectNB(editBoxIPText, port);
				}
				else
				{
					PX2_APP.GetEngineClientConnector()->Disconnect();
					but->GetText()->SetText("ConnectToServer");

					UIEditBox *editBoxIP = DynamicCast<UIEditBox>(
						engineCanvas->GetEngineFrame()->GetObjectByName("EditBoxIP"));
					editBoxIP->Enable(true);
					UIEditBox *editBoxPort = DynamicCast<UIEditBox>(
						engineCanvas->GetEngineFrame()->GetObjectByName("EditBoxPort"));
					editBoxPort->Enable(true);
				}
			}
			else if ("ButBluetoothScan" == name)
			{
				engineCanvas->GetEngineBluetoothList()->RemoveAllItems();
				std::vector<std::string> deviceList = PX2_BLUETOOTH.GetPairedDevices();
				for (int i = 0; i < (int)deviceList.size(); i++)
					engineCanvas->GetEngineBluetoothList()->AddItem(deviceList[i]);

				PX2_BLUETOOTH.DoDiscovery();
			}
			else if ("ButBluetoothConnect" == name)
			{
				if (PX2_BLUETOOTH.IsConnected())
				{
					PX2_BLUETOOTH.DisConnect();
				}
				else
				{
					UIList *list = engineCanvas->GetEngineBluetoothList();
					UIItem *selectItem = list->GetSelectedItem();
					if (selectItem)
					{
						const std::string &label = selectItem->GetLabel();
						StringTokenizer stkenizer(label, "$");
						if (stkenizer.Count() > 0)
						{
							std::string address = stkenizer.GetAt(1);
							if (!address.empty())
							{
								PX2_BLUETOOTH.Connect(address);
							}
						}
					}
				}
			}
			else if ("ButSerialCan" == name)
			{
				UIList *list = engineCanvas->GetEngineSerialList();
				list->RemoveAllItems();
				Serial serial;
				serial.UpdatePortList();
				std::vector<std::string> serials = serial.GetPortList();
				for (int i = 0; i < (int)serials.size(); i++)
				{
					std::string serialName = serials[i];
					list->AddItem(serialName);
				}
			}
			else if ("ButInfoClear" == name)
			{
				engineCanvas->GetEngineInfoList()->RemoveAllItems();
			}
		}
	}
	else if (UICT_CHECKED == type)
	{
		if (checkBut && "CheckAutoConnect" == name)
		{
			UIEditBox *editBoxIP = DynamicCast<UIEditBox>(
				engineCanvas->GetEngineFrame()->GetObjectByName("EditBoxIP"));
			UIEditBox *editBoxPort = DynamicCast<UIEditBox>(
				engineCanvas->GetEngineFrame()->GetObjectByName("EditBoxPort"));
			const std::string &textIP = editBoxIP->GetText();
			const std::string &textPort = editBoxPort->GetText();
			int port = StringHelp::StringToInt(textPort);

			EngineClientConnector *cnt = PX2_APP.GetEngineClientConnector();
			cnt->SetAutoConnectIP(textIP);
			cnt->SetAutoConnectPort(port);
			cnt->SetAutoConnect(true);
		}
	}
	else if (UICT_DISCHECKED == type)
	{
		if (checkBut && "CheckAutoConnect" == name)
		{
			UIEditBox *editBoxIP = DynamicCast<UIEditBox>(
				engineCanvas->GetEngineFrame()->GetObjectByName("EditBoxIP"));
			UIEditBox *editBoxPort = DynamicCast<UIEditBox>(
				engineCanvas->GetEngineFrame()->GetObjectByName("EditBoxPort"));
			const std::string &textIP = editBoxIP->GetText();
			const std::string &textPort = editBoxPort->GetText();
			int port = StringHelp::StringToInt(textPort);

			EngineClientConnector *cnt = PX2_APP.GetEngineClientConnector();
			cnt->SetAutoConnectIP(textIP);
			cnt->SetAutoConnectPort(port);
			cnt->SetAutoConnect(false);
		}
	}
}
//----------------------------------------------------------------------------
EngineCanvas::EngineCanvas() :
mNumInfoPick(0),
mLastPickTime(0.0f)
{
	CreateUICameraNode();

	SetClearFlag(true, true, true);
	SetClearColor(Float4::MakeColor(0, 0, 0, 255));

	EngineSceneCanvas *sceneCanvas = new0 EngineSceneCanvas();
	AttachChild(sceneCanvas);
	mEngineSceneCanvas = sceneCanvas;

	sceneCanvas->AttachController(new0 ProjectEventController());
	sceneCanvas->SetName("ProjectSceneCanvas");
	sceneCanvas->EnableAnchorLayout(true);
	sceneCanvas->SetAnchorHor(0.0f, 1.0f);
	sceneCanvas->SetAnchorVer(0.0f, 1.0f);
	sceneCanvas->SetClearFlag(true, true, true);
	sceneCanvas->SetClearColor(Float4::MakeColor(30, 30, 30, 255));
	sceneCanvas->SetRenderNodeUpdate(true);

	EngineUICanvas *uiCanvas = new0 EngineUICanvas();
	AttachChild(uiCanvas);
	mEngineUICanvas = uiCanvas;
	uiCanvas->SetUpdatePriority(10);
	uiCanvas->LocalTransform.SetTranslateY(-10.0f);
	uiCanvas->AttachController(new0 ProjectEventController());
	uiCanvas->SetName("ProjectUICanvas");
	uiCanvas->EnableAnchorLayout(true);
	uiCanvas->SetAnchorHor(0.0f, 1.0f);
	uiCanvas->SetAnchorVer(0.0f, 1.0f);
	uiCanvas->SetClearFlag(false, true, true);
	uiCanvas->SetRenderNodeUpdate(true);

	UIFramePtr frame = new0 UIFrame();
	AttachChild(frame);
	frame->SetAnchorHor(0.0f, 1.0f);
	frame->SetAnchorVer(1.0f, 1.0f);
	frame->SetSize(0.0f, 32.0f);
	frame->SetPivot(0.5f, 1.0f);
	mInfoFrame = frame;
	mInfoFrame->LocalTransform.SetTranslateY(-50.0f);
	mInfoFrame->SetOnlyShowUpdate(true);

	UIFPicBox *btnBackPic = new0 UIFPicBox();
	mInfoFrame->AttachChild(btnBackPic);
	btnBackPic->GetUIPicBox()->SetTexture("Data/engine/white.png");
	btnBackPic->SetSize(48.0f, 32.0f);
	btnBackPic->SetAnchorHor(1.0, 1.0);
	btnBackPic->SetAnchorVer(1.0, 1.0);
	btnBackPic->SetPivot(1.0, 1.0);
	btnBackPic->SetAlpha(0.2f);
	mInfoFrame->SetHeight(btnBackPic->GetHeight());

	UIFTextPtr infoText = new0 UIFText();
	frame->AttachChild(infoText);
	infoText->SetAnchorHor(0.0f, 1.0f);
	infoText->SetAnchorVer(0.0f, 1.0f);
	infoText->SetPivot(0.5f, 0.5f);
	infoText->GetText()->SetText("FPS:0");
	infoText->GetText()->SetFontWidthHeight(16, 16);
	infoText->GetText()->SetFontColor(Float3::WHITE);
	infoText->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	mInfoText = infoText;

	UIFTextPtr infoTextRight = new0 UIFText();
	frame->AttachChild(infoTextRight);
	infoTextRight->SetAnchorHor(0.5f, 1.0f);
	infoTextRight->SetAnchorVer(0.0f, 1.0f);
	infoTextRight->SetPivot(0.5f, 0.5f);
	infoTextRight->GetText()->SetText("");
	infoTextRight->GetText()->SetFontWidthHeight(16, 16);
	infoTextRight->GetText()->SetFontColor(Float3::WHITE);
	infoTextRight->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	mInfoTextMiddle = infoTextRight;

	mEngineBut = new0 UIButton();
	btnBackPic->AttachChild(mEngineBut);
	mEngineBut->SetAnchorHor(1.0f, 1.0f);
	mEngineBut->SetAnchorParamHor(-12.0f, -12.0f);
	mEngineBut->SetAnchorVer(0.5f, 0.5f);
	mEngineBut->SetPivot(0.5f, 0.5f);
	mEngineBut->LocalTransform.SetTranslateY(-1.0f);
	mEngineBut->SetName("ButEngine");
	auto text = mEngineBut->CreateAddText(PX2_LM_ENGINE.V("Engine"));
	text->SetAnchorParamVer(-25.0f, -25.0f);
	text->GetText()->SetFontScale(0.7f);
	text->GetText()->SetFontColor(Float3(0.7f, 0.7f, 0.7f));
	mEngineBut->AddUICallback(_EngineUICallback);
	mEngineBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/iconengine.png");
	mEngineBut->SetStateColorDefaultWhite();
	mEngineBut->SetSize(24.0f, 24.0f);

	mReloadBut = new0 UIButton();
	btnBackPic->AttachChild(mReloadBut);
	mReloadBut->SetAnchorHor(1.0f, 1.0f);
	mReloadBut->SetAnchorVer(0.5f, 0.5f);
	mReloadBut->SetPivot(0.5f, 0.5f);
	mReloadBut->SetAnchorParamHor(-36.0f, -36.0f);
	mReloadBut->LocalTransform.SetTranslateY(-1.0f);
	mReloadBut->SetName("ButReload");
	auto textReload = mReloadBut->CreateAddText(PX2_LM_ENGINE.V("Reload"));
	textReload->SetAnchorParamVer(-25.0f, -25.0f);
	textReload->GetText()->SetFontScale(0.7f);
	textReload->GetText()->SetFontColor(Float3(0.7f, 0.7f, 0.7f));
	mReloadBut->AddUICallback(_EngineUICallback);
	mReloadBut->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/iconprojectrefresh.png");
	mReloadBut->SetStateColorDefaultWhite();
	mReloadBut->SetSize(24.0f, 24.0f);

	mEngineCursor = new0 UIFPicBox();
	AttachChild(mEngineCursor);
	mEngineCursor->LocalTransform.SetTranslateY(-50.0f);
	mEngineCursor->GetUIPicBox()->SetTexture("Data/engine/aim.png");
	mEngineCursor->AutoMakeSizeFixable();
	mEngineCursor->Show(false);

	_CreateEngineFrame();

	mFPS = 0;
	mFPSUpdate = 0;
	mFPSUpdateTime = 0.0f;

	SetName("EngineCanvas");

	EnableAnchorLayout(true);
	SetSize(800.0f, 800.0f);
	SetAnchorHor(0.5f, 0.5f);
	SetAnchorVer(0.5f, 0.5f);
	EnableScreenRectLayout(false);

	ComeInEventWorld();

	PX2_SC_LUA->SetUserTypePointer("PX2_ENGINECANVAS", "EngineCanvas", this);
}
//----------------------------------------------------------------------------
EngineCanvas::~EngineCanvas()
{
}
//----------------------------------------------------------------------------
void EngineCanvas::_CreateEngineFrame()
{
	mEngineFrameBack = new0 UIFrame();
	AttachChild(mEngineFrameBack);
	mEngineFrameBack->SetAnchorHor(0.0f, 1.0f);
	mEngineFrameBack->SetAnchorVer(0.0f, 1.0f);
	mEngineFrameBack->LocalTransform.SetTranslateY(-60.0f);
	mEngineFrameBack->Show(false);

	mEngineFrame = new0 UIFrame();
	mEngineFrameBack->AttachChild(mEngineFrame);
	mEngineFrame->SetWidget(true);
	UIPicBox *picBox = mEngineFrame->CreateAddBackgroundPicBox();
	picBox->UseAlphaBlend(true);
	picBox->SetPicBoxType(UIPicBox::PBT_NINE);
	picBox->SetTexture("Data/engine/white.png");
	picBox->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
	picBox->SetColor(Float3::MakeColor(180, 180, 180));
	mEngineFrame->SetAnchorHor(0.0f, 1.0f);
	mEngineFrame->SetAnchorParamHor(20.0f, -20.0f);
	mEngineFrame->SetAnchorVer(0.0f, 1.0f);
	mEngineFrame->SetAnchorParamVer(20.0f, -20.0f);

	UIButton *butClose = new0 UIButton();
	mEngineFrame->AttachChild(butClose);
	butClose->SetName("ButClose");
	butClose->LocalTransform.SetTranslateY(-1.0f);
	butClose->SetAnchorHor(1.0f, 1.0f);
	butClose->SetAnchorParamHor(-5.0, -5.0);
	butClose->SetAnchorVer(1.0f, 1.0f);
	butClose->SetAnchorParamVer(-5.0, -5.0);
	butClose->SetPivot(1.0f, 1.0f);
	butClose->SetSize(40.0f, 40.0f);
	butClose->CreateAddText("X");
	butClose->GetText()->SetFontColor(Float3::WHITE);
	butClose->AddUICallback(_EngineUICallback);
	butClose->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/white.png");
	butClose->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	picBox->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	Float4 clearColor = Float4::MakeColor(150, 150, 150, 255);
	Float3 clearColor3 = Float3::MakeColor(150, 150, 150);

	Canvas *canvas = new0 Canvas();
	mEngineFrame->AttachChild(canvas);
	canvas->LocalTransform.SetTranslateY(-1.0f);
	canvas->SetAnchorHor(0.0, 1.0f);
	canvas->SetAnchorVer(0.0, 1.0f);
	canvas->SetAnchorParamHor(20.0f, -20.0f);
	canvas->SetAnchorParamVer(20.0f, -60.0f);
	canvas->CreateUICameraNode();
	canvas->SetClearColor(clearColor);

	float listItemHeight = 40.0f;

	mEngineCollapsePanel = new0 UICollapsePanel();
	canvas->AttachChild(mEngineCollapsePanel);
	mEngineCollapsePanel->LocalTransform.SetTranslateY(-1.0f);
	mEngineCollapsePanel->SetSliderWidth(40.0f);
	mEngineCollapsePanel->SetAnchorHor(0.0, 1.0f);
	mEngineCollapsePanel->SetAnchorVer(0.0, 1.0f);
	mEngineCollapsePanel->GetSlider()->GetFPicBoxBack()->GetUIPicBox()->SetTexture("Data/engine/white.png");
	mEngineCollapsePanel->GetSlider()->GetFPicBoxBack()->GetUIPicBox()->SetPicBoxType(UIPicBox::PBT_NINE);
	mEngineCollapsePanel->GetSlider()->GetFPicBoxBack()->GetUIPicBox()->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	mEngineCollapsePanel->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexture("Data/engine/white.png");
	mEngineCollapsePanel->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetPicBoxType(UIPicBox::PBT_NINE);
	mEngineCollapsePanel->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	//UIPicBox *picBoxCollapseItem = new0 UIPicBox();
	//picBoxCollapseItem->SetPicBoxType(UIPicBox::PBT_NINE);
	//picBoxCollapseItem->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
	//picBoxCollapseItem->SetTexture("Data/engine/white.png");
	//mEngineCollapsePanel->SetBarPicBox(picBoxCollapseItem);
	//picBoxCollapseItem->SetColor(Float3::MakeColor(150, 150, 150));

	mEngineCollapsePanel->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexture("Data/engine/white.png");

	float butHeight = 30.0f;
	float textHeight = 20.0f;
	float sliderWidth = 40.0f;

	float spaceHeight = 5.0f;
	float posVer = -textHeight*0.5f;
	posVer -= spaceHeight;

	UICollapseItem *itemEngineProjects = 
		mEngineCollapsePanel->AddItem("Project", PX2_LM_ENGINE.V("Project"));

	itemEngineProjects->Expand(false);
	itemEngineProjects->SetContentHeight(200.0f);

	UIFrame *projectFrame = new0 UIFrame();
	itemEngineProjects->GetContentFrame()->AttachChild(projectFrame);
	projectFrame->LocalTransform.SetTranslateY(-1.0f);
	projectFrame->SetAnchorHor(0.0f, 1.0f);
	projectFrame->SetAnchorVer(0.0f, 1.0f);

	posVer = -butHeight*0.5f;
	posVer -= spaceHeight;

	UIButton *butProjectOpen = new0 UIButton();
	mEngineProjectButtonOpen = butProjectOpen;
	projectFrame->AttachChild(butProjectOpen);
	butProjectOpen->SetName("ButProjectOpen");
	butProjectOpen->SetAnchorHor(0.0f, 0.5f);
	butProjectOpen->SetAnchorVer(1.0f, 1.0f);
	butProjectOpen->SetAnchorParamHor(5.0f, -5.0f);
	butProjectOpen->SetAnchorParamVer(posVer, posVer);
	butProjectOpen->LocalTransform.SetTranslateY(-1.0f);
	butProjectOpen->SetHeight(butHeight);
	butProjectOpen->CreateAddText(PX2_LM_ENGINE.V("Open"));
	butProjectOpen->AddUICallback(_EngineUICallback);
	butProjectOpen->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexture("Data/engine/white.png");
	butProjectOpen->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetPicBoxType(UIPicBox::PBT_NINE);
	butProjectOpen->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	UIButton *butProjectClose = new0 UIButton();
	mEngineProjectButtonClose = butProjectClose;
	projectFrame->AttachChild(butProjectClose);
	butProjectClose->SetName("ButProjectClose");
	butProjectClose->SetAnchorHor(0.5f, 1.0f);
	butProjectClose->SetAnchorVer(1.0f, 1.0f);
	butProjectClose->SetAnchorParamHor(5.0f, -5.0f);
	butProjectClose->SetAnchorParamVer(posVer, posVer);
	butProjectClose->LocalTransform.SetTranslateY(-1.0f);
	butProjectClose->SetHeight(butHeight);
	butProjectClose->CreateAddText(PX2_LM_ENGINE.V("Close"));
	butProjectClose->AddUICallback(_EngineUICallback);
	butProjectClose->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexture("Data/engine/white.png");
	butProjectClose->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetPicBoxType(UIPicBox::PBT_NINE);
	butProjectClose->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	posVer -= butHeight*0.5f;
	posVer -= spaceHeight;

	UIList *listProjects = new0 UIList();
	mEngineProjectList = listProjects;
	listProjects->SetName("ListProject");
	projectFrame->AttachChild(listProjects);
	listProjects->SetItemHeight(listItemHeight);
	listProjects->SetSliderSize(sliderWidth);
	listProjects->SetAnchorHor(0.0f, 1.0f);
	listProjects->SetAnchorVer(0.0f, 1.0f);
	listProjects->SetAnchorParamVer(spaceHeight, posVer);
	listProjects->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexture("Data/engine/white.png");
	listProjects->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetPicBoxType(UIPicBox::PBT_NINE);
	listProjects->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	_RefreshProjects();

	//// Engine Server
	//UICollapseItem *itemEngineServer = mEngineCollapsePanel->AddItem("Net_TCP_EngineServer");
	//itemEngineServer->Expand(false);

	//UIFrame *frameInfo = new0 UIFrame();
	//itemEngineServer->GetContentFrame()->AttachChild(frameInfo);
	//frameInfo->SetAnchorHor(0.0f, 1.0f);
	//frameInfo->SetAnchorVer(0.0f, 1.0f);
	//frameInfo->SetPivot(0.5f, 1.0f);

	//UIFText *textIP = new0 UIFText();
	//frameInfo->AttachChild(textIP);
	//textIP->SetName("TextIP");
	//UIPicBox *picBoxBack = textIP->CreateAddBackgroundPicBox();
	//picBoxBack->UseAlphaBlend(true);
	//picBoxBack->SetColor(Float3::MakeColor(200, 200, 200));
	//textIP->LocalTransform.SetTranslateY(-1.0f);
	//textIP->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	//textIP->SetAnchorHor(0.0f, 1.0f);
	//textIP->SetAnchorParamHor(100.0f, -100.0f);
	//textIP->SetAnchorVer(1.0f, 1.0f);
	//textIP->SetAnchorParamVer(posVer, posVer);
	//textIP->GetText()->SetText("");
	//textIP->SetHeight(textHeight);

	//UIButton *butSpeakIP = new0 UIButton();
	//frameInfo->AttachChild(butSpeakIP);
	//butSpeakIP->LocalTransform.SetTranslateY(-2.0f);
	//butSpeakIP->SetName("ButSpeak");
	//butSpeakIP->SetPivot(0.0f, 0.5f);
	//butSpeakIP->SetAnchorHor(1.0f, 1.0f);
	//butSpeakIP->SetAnchorParamHor(-95.0f, -95.0f);
	//butSpeakIP->SetAnchorVer(1.0f, 1.0f);
	//butSpeakIP->SetAnchorParamVer(posVer, posVer);
	//butSpeakIP->SetSize(20.0f, 20.0f);
	//butSpeakIP->CreateAddText("Say");
	//butSpeakIP->AddUICallback(_EngineUICallback);

	//posVer -= textHeight*0.5f;
	//posVer -= spaceHeight;

	//float progBarHeight = 30.0f;
	//posVer -= progBarHeight*0.5f;

	//UIProgressBar *pbBar = new0 UIProgressBar();
	//frameInfo->AttachChild(pbBar);
	//pbBar->LocalTransform.SetTranslateY(-1.0f);
	//pbBar->SetHeight(25.0f);
	//pbBar->SetProgress(0.0f, false);
	//pbBar->CreateAddProgressText();
	//pbBar->SetAnchorHor(0.0f, 1.0f);
	//pbBar->SetAnchorParamHor(100.0f, -100.0f);
	//pbBar->SetAnchorVer(1.0f, 1.0f);
	//pbBar->SetAnchorParamVer(posVer, posVer);

	//posVer -= progBarHeight*0.5f;
	//posVer -= spaceHeight;

	//float butStartEngineHeight = 30.0f;
	//posVer -= butStartEngineHeight*0.5f;

	//UIButton *butStartEngineServer = new0 UIButton();
	//frameInfo->AttachChild(butStartEngineServer);
	//butStartEngineServer->SetHeight(butStartEngineHeight);
	//butStartEngineServer->SetName("ButStartEngineServer");
	//butStartEngineServer->LocalTransform.SetTranslateY(-1.0f);
	//butStartEngineServer->SetAnchorVer(1.0f, 1.0f);
	//butStartEngineServer->SetAnchorParamVer(posVer, posVer);
	//butStartEngineServer->SetAnchorHor(0.0f, 1.0f);
	//butStartEngineServer->SetAnchorParamHor(100.0f, -100.0f);
	//butStartEngineServer->CreateAddText("StartEngineServer");
	//butStartEngineServer->GetText()->SetFontColor(Float3::WHITE);
	//butStartEngineServer->AddUICallback(_EngineUICallback);
	//butStartEngineServer->SetHeight(butHeight);

	//posVer -= butStartEngineHeight*0.5f;
	//posVer -= spaceHeight;

	//itemEngineServer->SetContentHeight(-posVer);

	//// Engine Client Connector
	//UICollapseItem *itemClientConnector = mEngineCollapsePanel->AddItem("Net_TCP_EngineClientConnector");
	//itemClientConnector->Expand(false);
	//UIFrame *frameClientConnector = new0 UIFrame();
	//itemClientConnector->GetContentFrame()->AttachChild(frameClientConnector);
	//frameClientConnector->SetAnchorHor(0.0f, 1.0f);
	//frameClientConnector->SetAnchorVer(0.0f, 1.0f);

	//posVer = -textHeight*0.5f;
	//posVer -= spaceHeight;

	//UIEditBox *editBoxIP = new0 UIEditBox();
	//frameClientConnector->AttachChild(editBoxIP);
	//editBoxIP->SetName("EditBoxIP");
	//editBoxIP->LocalTransform.SetTranslateY(-1.0f);
	//editBoxIP->GetBackPicBox()->SetColor(Float3::MakeColor(200, 200, 200));
	//editBoxIP->SetAnchorHor(0.0f, 0.5f);
	//editBoxIP->SetAnchorParamHor(100.0f, 0.0f);
	//editBoxIP->SetAnchorVer(1.0f, 1.0f);
	//editBoxIP->SetAnchorParamVer(posVer, posVer);
	//editBoxIP->SetHeight(textHeight);
	//editBoxIP->SetText("127.0.0.1");
	//UIFText *textCntIP = new0 UIFText();
	//frameClientConnector->AttachChild(textCntIP);
	//textCntIP->LocalTransform.SetTranslateY(-1.0f);
	//textCntIP->SetWidth(95.0f);
	//textCntIP->SetPivot(0.0f, 0.5f);
	//textCntIP->SetAnchorHor(0.0f, 0.0f);
	//textCntIP->SetAnchorVer(1.0f, 1.0f);
	//textCntIP->SetAnchorParamVer(posVer, posVer);
	//textCntIP->SetHeight(textHeight);
	//textCntIP->GetText()->SetAligns(TEXTALIGN_RIGHT | TEXTALIGN_VCENTER);
	//textCntIP->GetText()->SetText("IP:");
	//textCntIP->GetText()->SetFontScale(0.8f);

	//UIEditBox *editBoxPort = new0 UIEditBox();
	//frameClientConnector->AttachChild(editBoxPort);
	//editBoxPort->SetName("EditBoxPort");
	//editBoxPort->LocalTransform.SetTranslateY(-1.0f);
	//editBoxPort->GetBackPicBox()->SetColor(Float3::MakeColor(200, 200, 200));
	//editBoxPort->SetAnchorHor(0.5f, 1.0f);
	//editBoxPort->SetAnchorParamHor(50.0f, -100.0f);
	//editBoxPort->SetAnchorVer(1.0f, 1.0f);
	//editBoxPort->SetAnchorParamVer(posVer, posVer);
	//editBoxPort->SetHeight(textHeight);
	//editBoxPort->SetText(StringHelp::IntToString(EngineServerPort));
	//UIFText *textCntPort = new0 UIFText();
	//frameClientConnector->AttachChild(textCntPort);
	//textCntPort->SetName("EditBoxPort");
	//textCntPort->SetWidth(45.0f);
	//textCntPort->SetPivot(0.0f, 0.5f);
	//textCntPort->SetAnchorHor(0.5f, 0.5f);
	//textCntPort->SetAnchorVer(1.0f, 1.0f);
	//textCntPort->SetAnchorParamVer(posVer, posVer);
	//textCntPort->SetHeight(textHeight);
	//textCntPort->GetText()->SetAligns(TEXTALIGN_RIGHT | TEXTALIGN_VCENTER);
	//textCntPort->GetText()->SetText("Port:");
	//textCntPort->GetText()->SetFontScale(0.8f);

	//posVer -= textHeight*0.5f;
	//posVer -= spaceHeight;

	//posVer -= butHeight*0.5f;

	//UIButton *butConnect = new0 UIButton();
	//frameClientConnector->AttachChild(butConnect);
	//butConnect->SetHeight(butHeight);
	//butConnect->SetName("ButClientConnect");
	//butConnect->LocalTransform.SetTranslateY(-1.0f);
	//butConnect->SetAnchorVer(1.0f, 1.0f);
	//butConnect->SetAnchorParamVer(posVer, posVer);
	//butConnect->SetAnchorHor(0.0f, 1.0f);
	//butConnect->SetAnchorParamHor(100.0f + butHeight + 10.0f, -100.0f);
	//butConnect->CreateAddText("ConnectToServer");
	//butConnect->GetText()->SetFontColor(Float3::WHITE);
	//butConnect->AddUICallback(_EngineUICallback);

	//UICheckButton *checkAutoCnt = new0 UICheckButton();
	//frameClientConnector->AttachChild(checkAutoCnt);
	//checkAutoCnt->SetName("CheckAutoConnect");
	//checkAutoCnt->LocalTransform.SetTranslateY(-1.0f);
	//checkAutoCnt->SetPivot(0.0f, 0.5f);
	//checkAutoCnt->SetAnchorVer(1.0f, 1.0f);
	//checkAutoCnt->SetAnchorParamHor(100.0f, 100.0f);
	//checkAutoCnt->SetAnchorParamVer(posVer, posVer);
	//checkAutoCnt->SetAnchorHor(0.0f, 0.0f);
	//checkAutoCnt->SetSize(butHeight, butHeight);
	//checkAutoCnt->AddUICallback(_EngineUICallback);
	//checkAutoCnt->Check(false, false);

	//UIFText *textAutoCnt = new0 UIFText();
	//frameClientConnector->AttachChild(textAutoCnt);
	//textAutoCnt->LocalTransform.SetTranslateY(-1.0f);
	//textAutoCnt->SetWidth(95.0f);
	//textAutoCnt->SetPivot(0.0f, 0.5f);
	//textAutoCnt->SetAnchorHor(0.0f, 0.0f);
	//textAutoCnt->SetAnchorVer(1.0f, 1.0f);
	//textAutoCnt->SetAnchorParamVer(posVer, posVer);
	//textAutoCnt->SetHeight(butHeight);
	//textAutoCnt->GetText()->SetAligns(TEXTALIGN_RIGHT | TEXTALIGN_VCENTER);
	//textAutoCnt->GetText()->SetText("IsAutoConnect");
	//textAutoCnt->GetText()->SetFontScale(0.8f);

	//posVer -= butHeight*0.5f;
	//posVer -= spaceHeight;

	//itemClientConnector->SetContentHeight(-posVer);

	// udpNeighbors
	UICollapseItem *itemNeightbors = mEngineCollapsePanel->AddItem("Device", 
		PX2_LM_ENGINE.V("Device"));
	itemNeightbors->Expand(false);
	mEngineUDPNeighbors = new0 UIList();
	mEngineUDPNeighbors->SetNumMaxItems(20);
	UIPicBox *udpNeighborList = mEngineUDPNeighbors->CreateAddBackgroundPicBox(true);
	udpNeighborList->UseAlphaBlend(true);
	itemNeightbors->GetContentFrame()->AttachChild(mEngineUDPNeighbors);
	mEngineUDPNeighbors->LocalTransform.SetTranslateY(-2.0f);
	mEngineUDPNeighbors->SetAnchorHor(0.0f, 1.0f);
	mEngineUDPNeighbors->SetAnchorVer(0.0f, 1.0f);
	itemNeightbors->SetContentHeight(100.0f);
	mEngineUDPNeighbors->SetSliderSize(sliderWidth);
	mEngineUDPNeighbors->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexture("Data/engine/white.png");
	mEngineUDPNeighbors->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetPicBoxType(UIPicBox::PBT_NINE);
	mEngineUDPNeighbors->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	// bluetooth
	UICollapseItem *itemBluetooth = mEngineCollapsePanel->AddItem("Bluetooth",
		PX2_LM_ENGINE.V("Bluetooth"));
	itemBluetooth->Expand(false);
	itemBluetooth->SetContentHeight(200.0f);

	UIFrame *bluetoothFrame = new0 UIFrame();
	itemBluetooth->GetContentFrame()->AttachChild(bluetoothFrame);
	bluetoothFrame->LocalTransform.SetTranslateY(-1.0f);
	bluetoothFrame->SetAnchorHor(0.0f, 1.0f);
	bluetoothFrame->SetAnchorVer(0.0f, 1.0f);

	posVer = -butHeight*0.5f;
	posVer -= spaceHeight;

	UIButton *butBluetoothScan = new0 UIButton();
	bluetoothFrame->AttachChild(butBluetoothScan);
	butBluetoothScan->SetName("ButBluetoothScan");
	butBluetoothScan->SetAnchorHor(0.0f, 0.5f);
	butBluetoothScan->SetAnchorVer(1.0f, 1.0f);
	butBluetoothScan->SetAnchorParamHor(5.0f, -5.0f);
	butBluetoothScan->SetAnchorParamVer(posVer, posVer);
	butBluetoothScan->LocalTransform.SetTranslateY(-1.0f);
	butBluetoothScan->SetHeight(butHeight);
	butBluetoothScan->CreateAddText(PX2_LM_ENGINE.V("Scan"));
	butBluetoothScan->AddUICallback(_EngineUICallback);
	butBluetoothScan->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexture("Data/engine/white.png");
	butBluetoothScan->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetPicBoxType(UIPicBox::PBT_NINE);
	butBluetoothScan->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	UIButton *butBluetoothConnect = new0 UIButton();
	mEngineBluetoothButConnect = butBluetoothConnect;
	bluetoothFrame->AttachChild(butBluetoothConnect);
	butBluetoothConnect->SetName("ButBluetoothConnect");
	butBluetoothConnect->SetAnchorHor(0.5f, 1.0f);
	butBluetoothConnect->SetAnchorVer(1.0f, 1.0f);
	butBluetoothConnect->SetAnchorParamHor(5.0f, -5.0f);
	butBluetoothConnect->SetAnchorParamVer(posVer, posVer);
	butBluetoothConnect->LocalTransform.SetTranslateY(-1.0f);
	butBluetoothConnect->SetHeight(butHeight);
	butBluetoothConnect->CreateAddText(PX2_LM_ENGINE.V("Connect"));
	butBluetoothConnect->AddUICallback(_EngineUICallback);
	butBluetoothConnect->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexture("Data/engine/white.png");
	butBluetoothConnect->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetPicBoxType(UIPicBox::PBT_NINE);
	butBluetoothConnect->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	posVer -= butHeight*0.5f;
	posVer -= spaceHeight;

	UIList *listBle = new0 UIList();
	mEngineBluetoothList = listBle;
	listBle->SetItemHeight(listItemHeight);
	listBle->SetName("ListBluetooth");
	bluetoothFrame->AttachChild(listBle);
	listBle->SetSliderSize(sliderWidth);
	listBle->SetAnchorHor(0.0f, 1.0f);
	listBle->SetAnchorVer(0.0f, 1.0f);
	listBle->SetAnchorParamVer(spaceHeight, posVer);
	std::vector<std::string> deviceList = PX2_BLUETOOTH.GetPairedDevices();
	for (int i = 0; i < (int)deviceList.size(); i++)
		listBle->AddItem(deviceList[i]);
	listBle->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexture("Data/engine/white.png");
	listBle->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetPicBoxType(UIPicBox::PBT_NINE);
	listBle->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	// 串口
	UICollapseItem *itemSerial = mEngineCollapsePanel->AddItem("Serial",
		PX2_LM_ENGINE.V("Serial"));
	itemSerial->Expand(false);
	itemSerial->SetContentHeight(200.0f);

	UIFrame *serialFrame = new0 UIFrame();
	itemSerial->GetContentFrame()->AttachChild(serialFrame);
	serialFrame->LocalTransform.SetTranslateY(-1.0f);
	serialFrame->SetAnchorHor(0.0f, 1.0f);
	serialFrame->SetAnchorVer(0.0f, 1.0f);

	posVer = -butHeight*0.5f;
	posVer -= spaceHeight;

	UIButton *butSerialScan = new0 UIButton();
	serialFrame->AttachChild(butSerialScan);
	butSerialScan->SetName("ButSerialCan");
	butSerialScan->SetAnchorHor(0.0f, 0.5f);
	butSerialScan->SetAnchorVer(1.0f, 1.0f);
	butSerialScan->SetAnchorParamHor(5.0f, -5.0f);
	butSerialScan->SetAnchorParamVer(posVer, posVer);
	butSerialScan->LocalTransform.SetTranslateY(-1.0f);
	butSerialScan->SetHeight(butHeight);
	butSerialScan->CreateAddText(PX2_LM_ENGINE.V("Scan"));
	butSerialScan->AddUICallback(_EngineUICallback);
	butSerialScan->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexture("Data/engine/white.png");
	butSerialScan->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetPicBoxType(UIPicBox::PBT_NINE);
	butSerialScan->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	posVer -= butHeight*0.5f;
	posVer -= spaceHeight;

	UIList *listSerial = new0 UIList();
	mEngineSerialList = listSerial;
	listSerial->SetItemHeight(listItemHeight);
	listSerial->SetName("ListBluetooth");
	serialFrame->AttachChild(listSerial);
	listSerial->SetSliderSize(sliderWidth);
	listSerial->SetAnchorHor(0.0f, 1.0f);
	listSerial->SetAnchorVer(0.0f, 1.0f);
	listSerial->SetAnchorParamVer(spaceHeight, posVer);
	listSerial->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexture("Data/engine/white.png");
	listSerial->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetPicBoxType(UIPicBox::PBT_NINE);
	listSerial->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	// Infos
	UICollapseItem *itemInfos = mEngineCollapsePanel->AddItem("Infos", 
		PX2_LM_ENGINE.V("Infos"));

	UIFrame *infoFrame = new0 UIFrame();
	itemInfos->GetContentFrame()->AttachChild(infoFrame);
	infoFrame->LocalTransform.SetTranslateY(-1.0f);
	infoFrame->SetAnchorHor(0.0f, 1.0f);
	infoFrame->SetAnchorVer(0.0f, 1.0f);

	posVer = -butHeight*0.5f;
	posVer -= spaceHeight;

	UIButton *infoButClear = new0 UIButton();
	infoFrame->AttachChild(infoButClear);
	infoButClear->SetName("ButInfoClear");
	infoButClear->SetAnchorHor(0.0f, 0.5f);
	infoButClear->SetAnchorVer(1.0f, 1.0f);
	infoButClear->SetAnchorParamHor(5.0f, -5.0f);
	infoButClear->SetAnchorParamVer(posVer, posVer);
	infoButClear->LocalTransform.SetTranslateY(-1.0f);
	infoButClear->SetHeight(butHeight);
	infoButClear->CreateAddText(PX2_LM_ENGINE.V("Clear"));
	infoButClear->AddUICallback(_EngineUICallback);
	infoButClear->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexture("Data/engine/white.png");
	infoButClear->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetPicBoxType(UIPicBox::PBT_NINE);
	infoButClear->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);

	posVer -= butHeight*0.5f;
	posVer -= spaceHeight;

	mEngineInfoList = new0 UIList();
	mEngineInfoList->SetItemHeight(listItemHeight);
	mEngineInfoList->SetNumMaxItems(20);
	UIPicBox *backInfoList = mEngineInfoList->CreateAddBackgroundPicBox(true);
	backInfoList->UseAlphaBlend(true);
	infoFrame->AttachChild(mEngineInfoList);
	mEngineInfoList->LocalTransform.SetTranslateY(-2.0f);
	mEngineInfoList->SetAnchorHor(0.0f, 1.0f);
	mEngineInfoList->SetAnchorVer(0.0f, 1.0f);
	mEngineInfoList->SetAnchorParamVer(spaceHeight, posVer);
	itemInfos->SetContentHeight(300.0f);
	itemInfos->Expand(false);
	mEngineInfoList->SetSliderSize(sliderWidth);
	mEngineInfoList->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexture("Data/engine/white.png");
	mEngineInfoList->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetPicBoxType(UIPicBox::PBT_NINE);
	mEngineInfoList->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)
		->SetTexCornerSize(12.0f, 12.0f, 12.0f, 12.0f);
}
//----------------------------------------------------------------------------
void EngineCanvas::_RefreshProjects()
{
	mEngineProjectList->RemoveAllItems();
	std::set<std::string> allProjects = PX2_APP.GetAllProjects();
	auto it = allProjects.begin();
	for (; it != allProjects.end(); it++)
	{
		std::string projName = *it;
		UIItem *item = mEngineProjectList->AddItem(projName);
		item->SetName(projName);

		if (PX2_APP.IsProjectUpdated(projName))
		{
			UIFText *textUpdated = UIFText::New();
			item->AttachChild(textUpdated);
			textUpdated->LocalTransform.SetTranslateY(-2.0f);
			textUpdated->SetAnchorHor(0.0f, 1.0f);
			textUpdated->SetAnchorVer(0.0f, 1.0f);
			textUpdated->SetAnchorParamHor(0.0f , -10.0f);
			textUpdated->GetText()->SetAligns(TEXTALIGN_RIGHT | TEXTALIGN_VCENTER);
			textUpdated->GetText()->SetFontColor(Float3::WHITE);
			textUpdated->GetText()->SetText("Updated");
		}
	}
}
//----------------------------------------------------------------------------
void EngineCanvas::SetScreenRect(const Rectf &rect)
{
	Canvas::SetScreenRect(rect);
}
//----------------------------------------------------------------------------
void EngineCanvas::OnEvent(Event *event)
{
	if (ProjectES::IsEqual(event, ProjectES::NewProject) || 
		ProjectES::IsEqual(event, ProjectES::LoadedProject))
	{
		const Sizef &projSize = PX2_PROJ.GetSize();
		if (!PX2_PROJ.IsSizeSameWithScreen())
			_CalSize(projSize);
	}
	else if (GraphicsES::IsEqual(event, GraphicsES::WindowSizeChanged))
	{
		Project *proj = Project::GetSingletonPtr();
		if (proj)
		{
			const Sizef &projSize = proj->GetSize();
			if (!PX2_PROJ.IsSizeSameWithScreen())
				_CalSize(projSize);
		}
	}
	else if (EngineNetES::IsEqual(event, EngineNetES::OnEngineClientConnected))
	{
		UIButton *butClientConnect = DynamicCast<UIButton>(
			GetEngineFrame()->GetObjectByName("ButClientConnect"));
		if (butClientConnect)
			butClientConnect->GetText()->SetText(PX2_LM_ENGINE.V("DisConnect"));

		UIEditBox *editBoxIP = DynamicCast<UIEditBox>(
			GetEngineFrame()->GetObjectByName("EditBoxIP"));
		if (editBoxIP)
			editBoxIP->Enable(false);
		UIEditBox *editBoxPort = DynamicCast<UIEditBox>(
			GetEngineFrame()->GetObjectByName("EditBoxPort"));
		if (editBoxPort)
			editBoxPort->Enable(false);

		UIList *list = GetEngineInfoList();
		if (list)
			list->AddItem("EngineClientConnectorConnected");
	}
	else if (EngineNetES::IsEqual(event, EngineNetES::OnEngineClientDisConnected))
	{
		UIList *list = GetEngineInfoList();
		if (list)
			list->AddItem("EngineClientConnectorDisConnected");
	}
	else if (EngineNetES::IsEqual(event, EngineNetES::OnEngineServerBeConnected))
	{
		const std::string &cntStr = event->GetDataStr0();

		UIList *list = GetEngineInfoList();
		if (list)
			list->AddItem("EngineServerBeConnected :" + cntStr);
	}
	else if (EngineNetES::IsEqual(event, EngineNetES::OnEngineServerBeDisConnected))
	{
		const std::string &cntStr = event->GetDataStr0();

		UIList *list = GetEngineInfoList();
		if (list)
			list->AddItem("EngineServerBeDisConnected :" + cntStr);
	}
	else if (EngineNetES::IsEqual(event, EngineNetES::EngineClientUDPInfoChanged))
	{
		UIList *list = GetEngineUDPNeighborList();
		if (list)
			list->RemoveAllItems();

		int numUDPNetInfo = PX2_APP.GetNumUDPNetInfo();
		for (int i = 0; i < numUDPNetInfo; i++)
		{
			UDPNetInfo *udpNetInfo = PX2_APP.GetUDPNetInfo(i);
			if (udpNetInfo && udpNetInfo->TheType == UDPNetInfo::T_DEVICE)
			{
				std::string textStr = udpNetInfo->Name + " " + udpNetInfo->IP;
				if (list)
					list->AddItem(textStr);
			}
		}
	}
	else if (BluetoothES::IsEqual(event, BluetoothES::OnConnected))
	{
		mEngineBluetoothButConnect->GetText()->SetText(PX2_LM_ENGINE.V("DisConnect"));
	}
	else if (BluetoothES::IsEqual(event, BluetoothES::OnDisConnected))
	{
		mEngineBluetoothButConnect->GetText()->SetText(PX2_LM_ENGINE.V("Connect"));
	}
	else if (BluetoothES::IsEqual(event, BluetoothES::OnDisocveryNewDevice))
	{
		std::string devStr = event->GetDataStr0();
		if (!mEngineBluetoothList->GetItem(devStr))
			mEngineBluetoothList->AddItem(devStr);
	}
	else if (ProjectES_Internal::ProjectSizeSameWithScreenChanged)
	{
		Project *proj = Project::GetSingletonPtr();
		if (proj)
		{
			bool isSame = proj->IsSizeSameWithScreen();
			if (isSame)
			{
				EnableAnchorLayout(true);
				EnableScreenRectLayout(true);
				SetAnchorHor(0.0f, 1.0f);
				SetAnchorVer(0.0f, 1.0f);
			}
			else
			{
				EnableAnchorLayout(true);
				EnableScreenRectLayout(false);
				SetSize(800.0f, 800.0f);
				SetAnchorHor(0.5f, 0.5f);
				SetAnchorVer(0.5f, 0.5f);

				const Sizef &projSize = proj->GetSize();
				if (!PX2_PROJ.IsSizeSameWithScreen())
					_CalSize(projSize);
			}
		}
	}
	else if (ProjectES_Internal::IsEqual(event,
		ProjectES_Internal::AddUpdateProject))
	{
		_RefreshProjects();
	}
	else if (GraphicsES::IsEqual(event, GraphicsES::Info))
	{
		std::string infoStr = event->GetDataStr0();
		AddInfoText(infoStr);
	}
	else if (PlatformSDKSpace::IsEqual(event, PlatformSDKSpace::TVShowCorsor))
	{
		bool isShow = event->GetData<bool>();
		mEngineCursor->Show(isShow);
	}
	else if (PlatformSDKSpace::IsEqual(event, PlatformSDKSpace::TVSetCorsorPos))
	{
		APoint pos = event->GetData<APoint>();
		mEngineCursor->SetAnchorHor(pos.X(), pos.X());
		mEngineCursor->SetAnchorVer(pos.Y(), pos.Y());

		const Sizef &screenSize = PX2_APP.GetScreenSize();
		APoint toPos(pos.X()*screenSize.Width, 0.0f, pos.Y()*screenSize.Height);
		PX2_INPUTMAN.GetDefaultListener()->MouseMoved(toPos);
	}
	else if (PlatformSDKSpace::IsEqual(event, PlatformSDKSpace::TVSetCorsorFirePressed))
	{
		mEngineCursor->SetColor(Float3::RED);

		APoint pos = event->GetData<APoint>();
		mEngineCursor->SetAnchorHor(pos.X(), pos.X());
		mEngineCursor->SetAnchorVer(pos.Y(), pos.Y());

		const Sizef &screenSize = PX2_APP.GetScreenSize();
		APoint toPos(pos.X()*screenSize.Width, 0.0f, pos.Y()*screenSize.Height);
		PX2_INPUTMAN.GetDefaultListener()->MousePressed(MBID_LEFT,toPos);
	}
	else if (PlatformSDKSpace::IsEqual(event, PlatformSDKSpace::TVSetCorsorFireReleased))
	{
		mEngineCursor->SetColor(Float3::WHITE);

		APoint pos = event->GetData<APoint>();
		mEngineCursor->SetAnchorHor(pos.X(), pos.X());
		mEngineCursor->SetAnchorVer(pos.Y(), pos.Y());

		const Sizef &screenSize = PX2_APP.GetScreenSize();
		APoint toPos(pos.X()*screenSize.Width, 0.0f, pos.Y()*screenSize.Height);
		PX2_INPUTMAN.GetDefaultListener()->MouseReleased(MBID_LEFT, toPos);
	}
}
//----------------------------------------------------------------------------
void EngineCanvas::OnSizeChanged()
{
	Canvas::OnSizeChanged();
}
//----------------------------------------------------------------------------
void EngineCanvas::_CalSize(const Sizef &projSize)
{
	SizeNode *parent = DynamicCast<SizeNode>(GetParent());
	if (!parent) return;

	// 重置，让reloadpojrect 可以激活重新计算大小
	// 否则大小还是一样，不会触发OnSizeChanged
	mEngineSceneCanvas->SetSize(0.0f, 0.0f);
	mEngineUICanvas->SetSize(0.0f, 0.0f);

	const Sizef &parentSize = parent->GetSize();

	float parentHeightOverWidth = parentSize.Height / parentSize.Width;
	float projHeightOverWidth = projSize.Height / projSize.Width;

	float useWidth = projSize.Width;
	float useHeight = projSize.Height;

	if (parentHeightOverWidth > projHeightOverWidth)
	{
		useWidth = projSize.Width;
		useHeight = useWidth * parentHeightOverWidth;
	}
	else
	{
		useHeight = projSize.Height;
		useWidth = useHeight / parentHeightOverWidth;
	}

	SetSize(useWidth, useHeight);
}
//----------------------------------------------------------------------------
void EngineCanvas::UpdateLayout(Movable *parent)
{
	bool isLayoutChanged = mIsLayoutChanged;
	Canvas::UpdateLayout(parent);
	if (isLayoutChanged)
	{
		Project *proj = Project::GetSingletonPtr();
		if (proj)
		{
			if (!PX2_PROJ.IsSizeSameWithScreen())
			{
				const Sizef &projSize = proj->GetSize();
				_CalSize(projSize);
			}
		}
	}
}
//----------------------------------------------------------------------------
void EngineCanvas::AddInfoText(const std::string &infoText)
{
	mEngineInfoList->AddItem(infoText);
	mInfoTextMiddle->GetText()->SetText(infoText);
}
//----------------------------------------------------------------------------
UIFrame *EngineCanvas::GetEngineFrameBack()
{
	return mEngineFrameBack;
}
//----------------------------------------------------------------------------
UIFrame *EngineCanvas::GetEngineFrame()
{
	return mEngineFrame;
}
//----------------------------------------------------------------------------
UIList *EngineCanvas::GetEngineProjectList()
{
	return mEngineProjectList;
}
//----------------------------------------------------------------------------
UIList *EngineCanvas::GetEngineUDPNeighborList()
{
	return mEngineUDPNeighbors;
}
//----------------------------------------------------------------------------
UIList *EngineCanvas::GetEngineBluetoothList()
{
	return mEngineBluetoothList;
}
//----------------------------------------------------------------------------
UIList *EngineCanvas::GetEngineSerialList()
{
	return mEngineSerialList;
}
//----------------------------------------------------------------------------
UIList *EngineCanvas::GetEngineInfoList()
{
	return mEngineInfoList;
}
//----------------------------------------------------------------------------
void EngineCanvas::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	Canvas::UpdateWorldData(applicationTime, elapsedTime);

	Project *proj = Project::GetSingletonPtr();
	if (!proj)
	{
		SetAnchorHor(0.0, 1.0);
		SetAnchorVer(0.0, 1.0);
	}
	else
	{
		if (proj->IsSizeSameWithScreen())
		{
			SetAnchorHor(0.0f, 1.0f);
			SetAnchorVer(0.0f, 1.0f);
		}
		else
		{
			SetAnchorHor(0.5f, 0.5f);
			SetAnchorVer(0.5f, 0.5f);
		}
	}

	SizeNode *parent = DynamicCast<SizeNode>(GetParent());
	if (!parent) return;
	SetScreenRect(parent->GetScreenRect());

	bool isShowInfo = PX2_APP.IsShowInfo();
	mInfoFrame->Show(isShowInfo);
	if (!isShowInfo)
		return;

	mFPSUpdate++;
	mFPSUpdateTime += (float)elapsedTime;

	if (mFPSUpdateTime > 0.1f)
	{
		mFPS = (int)((float)mFPSUpdate / (float)mFPSUpdateTime);
		mFPSUpdate = 0;
		mFPSUpdateTime = 0.0f;
	}

	int numPerFrame = 0;
	Renderer *renderer = Renderer::GetDefaultRenderer();
	if (renderer)
	{
		numPerFrame = renderer->GetNumDrawPrimitivePerFrame();
	}		

	std::string infoStr = std::string("FPS:" + StringHelp::IntToString(mFPS) +
		" DrCall:" + StringHelp::IntToString(numPerFrame));
	mInfoText->GetText()->SetText(infoStr);
}
//----------------------------------------------------------------------------
void EngineCanvas::OnLeftDown(const PickInputData &data)
{
	Canvas::OnLeftDown(data);

	float infoH = mInfoFrame->GetHeight();
	Rectf screenRect = GetScreenRect();
	APoint pos = data.ScreenPos;
	if (pos.X() < 200.0f && pos.Z() > (screenRect.Height() - infoH))
	{
		float time = (float)Time::GetTimeInSeconds();
		float spaceTime = time - mLastPickTime;
		if (spaceTime < 1.0f || 0.0f == mLastPickTime)
		{
			mNumInfoPick++;
			if (mNumInfoPick > 7)
			{
				PX2_APP.SetShowInfo(!PX2_APP.IsShowInfo());
				mNumInfoPick = 0;
				mLastPickTime = 0.0f;
			}
			else
			{
				mLastPickTime = time;
			}
		}
		else
		{
			mNumInfoPick = 0;
			mLastPickTime = 0.0f;
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// EngineCanvas
//----------------------------------------------------------------------------
EngineCanvas::EngineCanvas(LoadConstructor value) :
Canvas(value)
{
	mFPS = 0;
	mFPSUpdate = 0;
	mFPSUpdateTime = 0.0f;
	mNumInfoPick = 0;
	mLastPickTime = 0.0f;
}
//----------------------------------------------------------------------------
void EngineCanvas::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Canvas::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(EngineCanvas, source);
}
//----------------------------------------------------------------------------
void EngineCanvas::Link(InStream& source)
{
	Canvas::Link(source);
}
//----------------------------------------------------------------------------
void EngineCanvas::PostLink()
{
	Canvas::PostLink();
}
//----------------------------------------------------------------------------
bool EngineCanvas::Register(OutStream& target) const
{
	if (Canvas::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EngineCanvas::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Canvas::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(EngineCanvas, target);
}
//----------------------------------------------------------------------------
int EngineCanvas::GetStreamingSize(Stream &stream) const
{
	int size = Canvas::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------