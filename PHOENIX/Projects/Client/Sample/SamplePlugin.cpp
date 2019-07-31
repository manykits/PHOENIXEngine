// SamplePlugin.cpp

#include "SamplePlugin.hpp"
#include "PX2PluginManager.hpp"
#include "PX2Log.hpp"
#include "SampleToLua.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2LuaPlusContext.hpp"
#include "PX2Project.hpp"

#include "PX2InterpCurveAlphaCtrl.hpp"
#include "PX2UIButton.hpp"
#include "PX2FramesMesh.hpp"
#include "PX2UIFrame.hpp"
#include "PX2LanguageManager.hpp"
#include "PX2UISkinManager.hpp"
#include "PX2UIPicBox.hpp"
#include "PX2UIComboBox.hpp"
#include "PX2UIProgressBar.hpp"
#include "PX2UITree.hpp"
#include "PX2UIRound.hpp"
#include "PX2UIWebFrame.hpp"
#include "PX2StringHelp.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2SoundSystem.hpp"
#include "PX2SoundSystem.hpp"
#include "PX2ObjCast.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2EventWorld.hpp"
#include "PX2EngineClientConnector.hpp"
#include "PX2StreamSocket.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Plugin* SamplePlugin::sPlugin = 0;
auto FrameWidth = 150.0f;
Bluetooth* g_s_ClientConnector = 0;
const std::string* g_s_ClienMsgEditBox = 0;
//----------------------------------------------------------------------------
SamplePlugin::SamplePlugin()
:ch_MT(0.75),
ch_LT(2.0),
ch_LastDelay(0.8)
{
	mName = "SamplePlugin";
}
//----------------------------------------------------------------------------
SamplePlugin::~SamplePlugin()
{
}
//----------------------------------------------------------------------------
void SamplePlugin::OnInstall()
{
	PX2_LOG_INFO("SamplePlugin::OnInstall()");
	PX2_EW.ComeIn(this);
	LuaPlusContext *luaContext = (LuaPlusContext*)PX2_SC_LUA;
	// Lua
	tolua_Sample_open((lua_State*)luaContext->GetLuaState());
	LuaPlus::LuaState *luaPlusState = luaContext->GetLuaPlusState();
	PX2_UNUSED(luaPlusState);
	mUI = PX2_PROJ.GetUI();
}
//----------------------------------------------------------------------------
//添加“按钮”框中按钮1按钮
void UIButtonCallBack(UIFrame* frame, UICallType type)
{
	UIButton* cb = DynamicCast<UIButton>(frame);

	const  std::string &name = cb->GetName();
	if (UICT_PRESSED == type)
	{
		if ("But0" == name)
		{
			SamplePlugin::GetSingleton().PrintMsg("UICT_PRESSED But0");
		}
		else if ("But1" == name)
		{
			SamplePlugin::GetSingleton().PrintMsg("UICT_PRESSED But1");
		}
		else if ("But2" == name)
		{
			SamplePlugin::GetSingleton().PrintMsg("UICT_PRESSED But2");
		}
		else if ("ButtonMenu" == name)
		{
			SamplePlugin::GetSingleton().ShowABCD();
		}
		else if ("ButtonNewProject" == name)
		{
			SamplePlugin::GetSingleton().ShowProject();
		}
		else if ("Cancel"==name)
		{
			SamplePlugin::GetSingleton().ShowProject();
		}
	}
	else if (UICT_RELEASED == type)
	{
		if ("ButHome" == name)
		{
			SamplePlugin::GetSingleton().Show();
		}
		if ("But0" == name)
		{
			SamplePlugin::GetSingleton().PrintMsg("UICT_PRESSED But0");
		}
		else if ("But1" == name)
		{
			SamplePlugin::GetSingleton().PrintMsg("UICT_PRESSED But1");
		}
		else if ("But2" == name)
		{
			SamplePlugin::GetSingleton().PrintMsg("UICT_PRESSED But2");
		}
	}
	else if (UICT_RELEASED_NOTPICK == type)
	{
		SamplePlugin::GetSingleton().PrintMsg("UICT_RELEASED_NOTPICK");
	}
	else if (UICT_CHECKED == type)
	{
		SamplePlugin::GetSingleton().PrintMsg("UICT_CHECKED");
	}
	else if (UICT_DISCHECKED == type)
	{
		SamplePlugin::GetSingleton().PrintMsg("UICT_DISCHECKED");
	}
	else if (UICT_EDITBOX_TEXTCHANGED == type)
	{
		SamplePlugin::GetSingleton().PrintMsg("UICT_EDITBOX_TEXTCHANGED");
	}
	else if (UICT_EDITBOX_ENTER == type)
	{
		SamplePlugin::GetSingleton().PrintMsg("UICT_EDITBOX_ENTER");
	}
	else if (UICT_LIST_SELECTED == type)
	{
		SamplePlugin::GetSingleton().PrintMsg("UICT_EDITBOX_ENTER");
	}
	else if (UICT_COMBOBOX_CHOOSED == type)
	{
		SamplePlugin::GetSingleton().PrintMsg("UICT_COMBOBOX_CHOOSED");
	}
	else if (UICT_TREE_SELECTED == type)
	{
		SamplePlugin::GetSingleton().PrintMsg("UICT_TREE_SELECTED");
	}
}  
//----------------------------------------------------------------------------
void UIServerCallBack(UIFrame* frame, UICallType type)
{
	UITabFrame* ab = DynamicCast<UITabFrame>(frame);
	const std::string &name = ab->GetName();
	if (UICT_PRESSED == type)
	{
	}
	else if (UICT_RELEASED==type)
	{ 
		if (nullptr != g_s_ClientConnector)
		{
			if ("ButConnect" == name)
			{
				if (g_s_ClientConnector->IsConnected())
				{
					g_s_ClientConnector->DisConnect();
				}
				else
				{
					//g_s_ClientConnector->ConnectNB("127.0.0.1", 18180);
				}
			}
			else if ("ButSend" == name)
			{
				if (g_s_ClientConnector->IsConnected())
				{
					//const std::string* text = g_s_ClienMsgEditBox->GetText();
					//g_s_ClientConnector->SendString(text);
				}
			}
		}
	}	
}
//----------------------------------------------------------------------------
void SamplePlugin::OnUninstall()
{
	PX2_LOG_INFO("SamplePlugin::OnUninstall()");
	PX2_EW.GoOut(this);
}
//----------------------------------------------------------------------------
void SamplePlugin::OnUpdate()
{
}
//----------------------------------------------------------------------------
void SamplePlugin::OnEvent(Event *event)
{

	if (ProjectES::IsEqual(event, ProjectES::Play))
	{
		_Play();
	}
	else if (ProjectES::IsEqual(event, ProjectES::Stop))
	{
	}
	else if (ProjectES::IsEqual(event, ProjectES::NewProject))
	{
		mIsNewProject = true;
	}
}
//----------------------------------------------------------------------------
void PX2::SamplePlugin::_Play()
{
	//添加音乐
	AddMusic();

	//主按钮设置
	MainButton();

	//添加菜单框属性
	AddMenuBoxProperties();

	//创建新的框架类型
	AddFrameType();

	//关于框
	OnCreateUIFrameAbout();

	//框架框
	OnCreateUIFrame();

	//图片框
	OnCreateUIFPicBox();

	//文字框
	OnCreateUIFText();

	//按钮框
	OnCreateUIButton();

	//判断框
	OnCreateUICheckBox();

	//选择框
	OnCreateUIComboBox();
}
//----------------------------------------------------------------------------
void SamplePlugin::AddMusic()
{
	//添加音乐
	auto musicName = "thelittleadventurer.mp3";
	std::string filePath = "Data/Sample/media/music/";
	PX2_RM.ReWriteFileToWriteablePath(filePath + musicName, musicName);
	std::string dstPath = ResourceManager::GetWriteablePath() + musicName;
	PX2_SS.PlayMusic(0, dstPath.c_str(), true, 0.0f, 1.0f);
}
//----------------------------------------------------------------------------
void SamplePlugin::MainButton()
{
	//背景
	frame = UIFrame::New();
	mUI->AttachChild(frame);
	frame->LocalTransform.SetTranslateY(-1.0);
	frame->SetAnchorHor(0.0, 1.0);
	frame->SetAnchorVer(0.0, 1.0);
	//创建第一个主按钮
	mButBag = UIButton::New();
	frame->AttachChild(mButBag);
	mButBag->SetName("ButHome");
	mButBag->SetAnchorHor(0.0, 0.0);
	mButBag->SetAnchorVer(1.0, 1.0);
	mButBag->SetSize(50, 50);
	mButBag->SetAnchorParamHor(50, 50);
	mButBag->SetAnchorParamVer(-50, -50);
	mButBag->SetScriptHandler("S_Callback");
	//添加主按钮
	mButBag->AddUICallback(UIButtonCallBack);
	//框内图案
	UIFPicBox* but = UIFPicBox::New();
	but->GetUIPicBox()->SetTexture("Data/Sample/images/icons/run.png");
	but->SetSize(30.0, 30.0);
	mButBag->AttachChild(but);
	but->LocalTransform.SetTranslateY(-2.0);
}
//----------------------------------------------------------------------------
void SamplePlugin::AddMenuBoxProperties()
{
	//背景
	MenuBg = UIFrame::New();
	frame->AttachChild(MenuBg);
	MenuBg->SetAnchorHor(0.0, 1.0);
	MenuBg->SetAnchorVer(0.0, 1.0);
	MenuBg->SetAnchorParamHor(25.0, -25.0);
	MenuBg->SetAnchorParamVer(25.0, -100.0);
	//浏览菜单
	mMenuCase = UITabFrame::New();
	MenuBg->AttachChild(mMenuCase);
	mMenuCase->SetAnchorParamHor(0.0, -FrameWidth);
	mMenuCase->CreateAddBackgroundPicBox(true, Float3::MakeColor(200, 200, 200));
	mMenuCase->SetTabBarHeight(40);
	mMenuCase->SetTabHeight(35);
	mMenuCase->SetTabWidth(60);
}
//----------------------------------------------------------------------------
void SamplePlugin::AddFrameType()
{
	mFrameAbout = new0 UITabFrame();
	frameUIFrame = new0 UIFrame();
	frameUIFPicBox = new0 UIFrame();
	frameUIFText = new0 UIFrame();
	frameUIButtom = new0 UIFrame();
	frameUICheckBox = new0 UIFrame();
	frameUIComboBox = new0 UIFrame();
	frameUIEditBox = new0 UIFrame();
	frameUIProgress = new0 UIFrame();
	frameUIColorSelect = new0 UIFrame();
	frameUIList = new0 UIFrame();
	frameUITree = new0 UIFrame();
	frameUISplitter = new0 UIFrame();
	frameUISlider = new0 UIFrame();
	frameUIRound = new0 UIFrame();
	frameServer = new0 UIFrame();
}
//----------------------------------------------------------------------------
void SamplePlugin::OnCreateUIFrameAbout()
{
	//关于
	mMenuCase->AddTab("About", "About", mFrameAbout);
	mFrameAbout->LocalTransform.SetTranslateY(-1.0);
	mFrameAbout->SetAnchorHor(0.0, 1.0);
	mFrameAbout->SetAnchorVer(0.0, 1.0);
	mAboutfText = UIFText::New();
	mFrameAbout->AttachChild(mAboutfText);
	mAboutfText->SetAnchorHor(0.0, 1.0);
	mAboutfText->SetAnchorVer(0.0, 1.0);
	mAboutfText->GetText()->SetText("wecome to");
}
//----------------------------------------------------------------------------
void SamplePlugin::OnCreateUIFrame()
{
	//框架
	mMenuCase->AddTab("UIFrame", "UIFrame", frameUIFrame);
	frameUIFrame->LocalTransform.SetTranslateY(-1.0);
	frameUIFrame->SetAnchorHor(0.0, 1.0);
	frameUIFrame->SetAnchorVer(0.0, 1.0);
	UIFrame* frameUIFrame0 = UIFrame::New();
	frameUIFrame->AttachChild(frameUIFrame0);
	frameUIFrame0->CreateAddBackgroundPicBox(true, Float3::YELLOW);
	frameUIFrame0->SetAnchorHor(0.0, 1.0);
	frameUIFrame0->SetAnchorVer(1.0, 1.0);
	frameUIFrame0->SetSize(0.0, 100.0);
	frameUIFrame0->SetAnchorParamHor(50.0, -50.0);
	frameUIFrame0->SetAnchorParamVer(-50.0, -5.0);
	UIFrame* frameUIFrame1 = UIFrame::New();
	frameUIFrame->AttachChild(frameUIFrame1);
	frameUIFrame1->CreateAddBackgroundPicBox(true, Float3::GREEN);
	frameUIFrame1->SetPivot(0.5, 1.0);
	frameUIFrame1->SetAnchorHor(0.0, 1.0);
	frameUIFrame1->SetAnchorVer(1.0, 1.0);
	frameUIFrame1->SetAnchorParamHor(50.0, -50.0);
	frameUIFrame1->SetAnchorParamVer(-100.0, -100.0);
	UIFrame* frameUIFrame2 = UIFrame::New();
	frameUIFrame->AttachChild(frameUIFrame2);
	frameUIFrame2->CreateAddBackgroundPicBox(false);
	frameUIFrame2->SetPivot(0.5, 1.0);
	frameUIFrame2->SetAnchorHor(0.0, 0.0);
	frameUIFrame2->SetAnchorVer(1.0, 1.0);
	frameUIFrame2->SetSize(200.0, 200.0);
	frameUIFrame2->SetAnchorParamHor(150.0, 0.0);
	frameUIFrame2->SetAnchorParamVer(-200.0, 0.0);
}
//----------------------------------------------------------------------------
void SamplePlugin::OnCreateUIFPicBox()
{
	//图片
	mMenuCase->AddTab("UIFPicBox","UIFPicBox", frameUIFPicBox);
	frameUIFPicBox->LocalTransform.SetTranslateY(-1.0);
	frameUIFPicBox->SetAnchorHor(0.0, 1.0);
	frameUIFPicBox->SetAnchorVer(0.0, 1.0);
	auto PicSize = 120;
	UIFrameGridAlignControl* gridAliginCtrl = frameUIFPicBox->CreateAddGridAlignCtrl();
	//设置纵向格式位置：y轴往中部
	gridAliginCtrl->SetChildAlignment(UIFrameGridAlignControl::CAT_MIDDLE_CENTER);
	//设置横向格式位置：x轴往中部
	gridAliginCtrl->SetConstraintType(UIFrameGridAlignControl::CT_FIXED_COLUMN_COUNT);
	//设置x轴中的值为4个
	gridAliginCtrl->SetConstraintValue(4);
	gridAliginCtrl->SetBorder(10.0, 10.0, 10.0, 10.0);
	gridAliginCtrl->SetCellSize(Sizef(PicSize, PicSize));
	//第一张图片
	UIFPicBox* fPicBox = UIFPicBox::New();
	frameUIFPicBox->AttachChild(fPicBox);
	fPicBox->SetSize(PicSize, PicSize);
	fPicBox->GetUIPicBox()->SetTexture("Data/Sample/images/pics/arduino.png");
	fPicBox->RegistToScriptSystem();
	//第二张图片
	UIFPicBox* fPicBox1 = UIFPicBox::New();
	frameUIFPicBox->AttachChild(fPicBox1);
	fPicBox1->SetSize(PicSize, PicSize);
	fPicBox1->GetUIPicBox()->SetUVRepeat(3, 3);
	fPicBox1->GetUIPicBox()->SetTexture("Data/Sample/images/pics/raspberry.png");
	fPicBox1->RegistToScriptSystem();
	//第三张图片
	UIFPicBox* fPicBox2 = UIFPicBox::New();
	frameUIFPicBox->AttachChild(fPicBox2);
	fPicBox2->SetSize(PicSize, PicSize);
	fPicBox2->GetUIPicBox()->SetTexCornerSize(8, 8, 8, 8);
	fPicBox2->GetUIPicBox()->SetTexture("Data/Sample/images/pics/but.png");
	fPicBox2->RegistToScriptSystem();
	UIFPicBox* fPicBox3 = UIFPicBox::New();
	frameUIFPicBox->AttachChild(fPicBox3);
	fPicBox3->SetSize(PicSize, PicSize);
	fPicBox3->GetUIPicBox()->SetPicBoxType(UIPicBox::PBT_NINE);
	fPicBox3->GetUIPicBox()->SetTexCornerSize(8, 8, 8, 8);
	fPicBox3->GetUIPicBox()->SetTexture("Data/engine/common.xml", "but_normal");
	fPicBox3->RegistToScriptSystem();
}
//----------------------------------------------------------------------------
void SamplePlugin::OnCreateUIFText()
{
	//文字
	mMenuCase->AddTab("UIFText", "UIFText", frameUIFText);
	frameUIFText->LocalTransform.SetTranslateY(-1.0);
	frameUIFText->SetAnchorHor(0.0, 1.0);
	frameUIFText->SetAnchorVer(0.0, 1.0);
	auto Textsize = 200;

	UIFrameGridAlignControl* gridAliginCtrl0 = frameUIFText->CreateAddGridAlignCtrl();
	gridAliginCtrl0->SetChildAlignment(UIFrameGridAlignControl::CAT_MIDDLE_CENTER);
	gridAliginCtrl0->SetConstraintType(UIFrameGridAlignControl::CT_FIXED_COLUMN_COUNT);
	gridAliginCtrl0->SetConstraintValue(3);
	gridAliginCtrl0->SetBorder(10.0, 10.0, 10.0, 10.0);
	gridAliginCtrl0->SetCellSize(Sizef(Textsize, Textsize));

	UIFText* fText0 = UIFText::New();
	frameUIFText->AttachChild(fText0);
	fText0->SetSize(Textsize, Textsize);
	fText0->GetText()->SetFontColor(Float3::MakeColor(55, 0, 0));
	fText0->GetText()->SetFontScale(1.0);
	fText0->RegistToScriptSystem();

	UIFText* fText1 = UIFText::New();
	frameUIFText->AttachChild(fText1);
	fText1->SetSize(Textsize, Textsize);
	fText1->CreateAddBackgroundPicBox(true);
	fText1->GetText()->SetFont("Data/Sample/fonts/hyswlongfangsong.ttf", 24, 24, FES_UNDERLINE);
	fText1->GetText()->SetText("welcome");
	fText1->GetText()->SetBorderShadowColor(Float3(100, 0.0, 0.0));
	//添加下划线属性（格式样式）
	fText1->GetText()->SetDrawStyle(FD_SHADOW);
	//添加向上顶格（位置设置）
	fText1->GetText()->SetAligns(TEXTALIGN_LEFT + TEXTALIGN_TOP);
	fText1->RegistToScriptSystem();

	UIFText* fText2 = UIFText::New();
	frameUIFText->AttachChild(fText2);
	fText2->SetSize(Textsize, Textsize);
	fText2->CreateAddBackgroundPicBox(true, Float3::MakeColor(100, 200, 200));
	fText2->GetText()->SetText("intelligente underholdning");
	//紧靠方式
	fText2->GetText()->SetRectUseage(UIText::RU_CLIPWARP);
	//（根据形状的大小来控制间距）默认为false，意思为不控制
	fText2->GetText()->SetAutoWarp(true);
	fText2->RegistToScriptSystem();
}
//----------------------------------------------------------------------------
void SamplePlugin::OnCreateUIButton()
{
	//按钮
	mMenuCase->AddTab("UIButtom", "UIButton", frameUIButtom);
	frameUIButtom->LocalTransform.SetTranslateY(-1.0);
	frameUIButtom->SetAnchorHor(0.0, 1.0);
	frameUIButtom->SetAnchorVer(0.0, 1.0);
	auto ButtonSize = 200;

	UIFText* text1 = UIFText::New();
	frameUIButtom->AttachChild(text1);
	text1->SetAnchorVer(0.9, 0.9);
	text1->GetText()->SetText("hello");

	UIFrameGridAlignControl* gridAliginCtrl1 = frameUIButtom->CreateAddGridAlignCtrl();
	//位置纵方向（默认为顶格，这里设置为中心）
	gridAliginCtrl1->SetChildAlignment(UIFrameGridAlignControl::CAT_MIDDLE_CENTER);
	//位置横方向（默认为左对其，这里也是设置为居中）
	gridAliginCtrl1->SetConstraintType(UIFrameGridAlignControl::CT_FIXED_COLUMN_COUNT);
	//设置横向设置值，默认为2个
	gridAliginCtrl1->SetConstraintValue(3);
	gridAliginCtrl1->SetCellSize(Sizef(ButtonSize, ButtonSize));

	UIButton* but0 = UIButton::New();
	frameUIButtom->AttachChild(but0);
	but0->SetName("But0");
	but0->SetSize(ButtonSize, ButtonSize);
	but0->CreateAddText("But0");
	but0->SetScriptHandler("S_Callback");
	but0->RegistToScriptSystem();
	//添加按钮响应
	but0->AddUICallback(UIButtonCallBack);

	

	UIButton* but1 = UIButton::New();
	frameUIButtom->AttachChild(but1);
	but1->SetName("But1");
	but1->SetButType(UIButtonBase::BT_PICBOXSWAP);
	//but1->SetStateColorDefaultWhite();
	but1->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	but1->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(8, 8, 8, 8);
	but1->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture("Data/engine/common.xml", "but_normal");

	but1->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetPicBoxType(UIPicBox::PBT_NINE);
	but1->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetTexCornerSize(8, 8, 8, 8);
	but1->GetPicBoxAtState(UIButtonBase::BS_HOVERED)->SetTexture("Data/engine/common.xml", "but_over");

	but1->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetPicBoxType(UIPicBox::PBT_NINE);
	but1->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetTexCornerSize(8, 8, 8, 8);
	but1->GetPicBoxAtState(UIButtonBase::BS_PRESSED)->SetTexture("Data/engine/common.xml", "but_normal");

	but1->SetSize(ButtonSize, ButtonSize);
	but1->CreateAddText("button1");
	but1->SetScriptHandler("S_Callback");
	but1->RegistToScriptSystem();
	//添加按钮响应
	but1->AddUICallback(UIButtonCallBack);

	UIButton* but2 = UIButton::New();
	frameUIButtom->AttachChild(but2);
	but2->SetName("But2");
	but2->SetSize(ButtonSize, ButtonSize);
	but2->CreateAddText("button2");
	but2->SetScriptHandler("S_Callback");
	but2->RegistToScriptSystem();
	//添加按钮响应
	but2->AddUICallback(UIButtonCallBack);
}
//----------------------------------------------------------------------------
void SamplePlugin::OnCreateUICheckBox()
{
	//判断框
	mMenuCase->AddTab("UICheckBox", "UICheckBox", frameUICheckBox);
	frameUICheckBox->LocalTransform.SetTranslateY(-1.0);
	frameUICheckBox->SetAnchorHor(0.0, 1.0);
	frameUICheckBox->SetAnchorVer(0.0, 1.0);

	UICheckButton* checkButton = UICheckButton::New();
	frameUICheckBox->AttachChild(checkButton);
	checkButton->SetName("CheckButton");
	checkButton->SetSize(40.0, 40.0);
	checkButton->SetScriptHandler("S_Callback");
	checkButton->RegistToScriptSystem();
	checkButton->SetAnchorHor(0.2, 0.2);

	UIFText* text0 = UIFText::New();
	frameUICheckBox->AttachChild(text0);
	text0->SetAnchorHor(0.2, 0.2);
	text0->SetAnchorVer(0.8, 0.8);
	text0->GetText()->SetText("DefaultSele");

	UICheckButton* checkButton1 = UICheckButton::New();
	frameUICheckBox->AttachChild(checkButton1);
	checkButton1->SetName("CheckButton1");
	checkButton1->SetAnchorHor(0.8, 0.8);
	
	UIFText* text1 = UIFText::New();
	frameUICheckBox->AttachChild(text1);
	text1->SetAnchorHor(0.8, 0.8);
	text1->SetAnchorVer(0.8, 0.8);
	text1->GetText()->SetText("CustomSele");
}
//----------------------------------------------------------------------------
void SamplePlugin::OnCreateUIComboBox()
{
	//选择框
	mMenuCase->AddTab("UIComboBox", "UIComboBox", frameUIComboBox);
	frameUIComboBox->LocalTransform.SetTranslateY(-1.0);
	frameUIComboBox->SetAnchorHor(0.0, 1.0);
	frameUIComboBox->SetAnchorVer(0.0, 1.0);
	
	UIComboBox* comboBox = UIComboBox::New();
	frameUIComboBox->AttachChild(comboBox);
	comboBox->AddChooseStr("Choose0");
	comboBox->AddChooseStr("Choose1");
	comboBox->ChooseStr("Choose0");
	comboBox->SetScriptHandler("S_Callback");
	comboBox->RegistToScriptSystem();
	comboBox->SetAnchorHor(0.2, 0.2);
	comboBox->SetAnchorVer(0.6, 0.6);

	UIFText* text0 = UIFText::New();
	frameUIComboBox->AttachChild(text0);
	text0->SetAnchorHor(0.2, 0.2);
	text0->SetAnchorVer(0.8, 0.8);
	text0->GetText()->SetText("DefaultcheckBox");

	UIComboBox* comboBox1 = UIComboBox::New();
	frameUIComboBox->AttachChild(comboBox1);
	comboBox1->GetSelectButton()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	comboBox1->GetSelectButton()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(12, 12, 12, 12);
	comboBox1->GetSelectButton()->SetStateColorDefaultWhite();
	comboBox1->GetSelectButton()->SetColor(Float3::MakeColor(180,179,171));
	comboBox1->GetChooseList()->GetBackgroundPicBox()->SetColor(Float3::MakeColor(219, 218, 211));
	comboBox1->GetChooseList()->GetSlider()->GetButSlider()->SetStateColorDefaultWhite();
	comboBox1->GetChooseList()->GetSlider()->GetButSlider()->SetColor(Float3::MakeColor(219,218,211));
	comboBox1->GetChooseList()->GetSlider()->GetButSlider()->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
	comboBox1->AddChooseStr("Choose0");
	comboBox1->AddChooseStr("Choose1");
	comboBox1->ChooseStr("Choose0");
	comboBox1->SetAnchorHor(0.8, 0.8);
	comboBox1->SetAnchorVer(0.6, 0.6);

	UIFText* text1 = UIFText::New();
	frameUIComboBox->AttachChild(text1);
	text1->SetAnchorHor(0.8, 0.8);
	text1->SetAnchorVer(0.8, 0.8);
	text1->GetText()->SetText("CustomcheckBox");
}
//----------------------------------------------------------------------------
void SamplePlugin::PrintMsg(const std::string &str)
{
	MsgList->AddItem(str);
}
//----------------------------------------------------------------------------
void PX2::SamplePlugin::Progressmove()
{
	progressBar1->SetProgressAnimInit(0.0);
	progressBar1->SetProgressAnim(1.0);
	progressBar1->SetProgressAnimSpeed(1.0);
}
//----------------------------------------------------------------------------
//界面是否显示
void PX2::SamplePlugin::Show()
{
	MenuBg->Show(!MenuBg->IsShow());
}
//----------------------------------------------------------------------------
void PX2::SamplePlugin::ShowABCD()
{
	ButtonA->Show(!ButtonA->IsShow());
	ButtonB->Show(!ButtonB->IsShow());
	ButtonC->Show(!ButtonC->IsShow());
	ButtonD->Show(!ButtonD->IsShow());
}
//----------------------------------------------------------------------------

void PX2::SamplePlugin::OnCreateProject()
{
	//背景
	ProjectBg = UIFrame::New();
	frame->AttachChild(ProjectBg);
	ProjectBg->SetAnchorHor(0.3, 0.7);
	ProjectBg->SetAnchorVer(0.3, 0.7);
	ProjectBg->SetAnchorParamHor(25.0, -25.0);
	ProjectBg->SetAnchorParamVer(25.0, -100.0);
	UITabFrame* ProjectCase = UITabFrame::New();
	ProjectBg->AttachChild(ProjectCase);
	ProjectCase->SetAnchorParamHor(0.0, -20);
	ProjectCase->CreateAddBackgroundPicBox(true, Float3::MakeColor(200, 200, 200));
	ProjectCase->SetTabBarHeight(20);
	//添加文本“创建新项目”名称
	UIFText* text = UIFText::New();
	ProjectCase->AttachChild(text);
	text->SetSize(90,90);
	text->GetText()->SetText("Create new project");
	text->SetWidth(300.0);
	text->SetAnchorVer(0.7,0.7);
	//添加编辑框
	UIEditBox* editbox1 = UIEditBox::New();
	ProjectCase->AttachChild(editbox1);
	editbox1->LocalTransform.SetTranslateY(-2.0);
	//确定按钮
	UIButton* ConfirmButton = UIButton::New();
	ProjectCase->AttachChild(ConfirmButton);
	ConfirmButton->SetAnchorHor(0.35, 0.45);
	ConfirmButton->SetAnchorVer(0.2, 0.3);
	ConfirmButton->LocalTransform.SetTranslateY(-2.0);
	ConfirmButton->CreateAddText("Certain");
	//取消按钮
	UIButton* CancelButton = UIButton::New();
	ProjectCase->AttachChild(CancelButton);
	CancelButton->SetAnchorHor(0.55,0.65);
	CancelButton->SetAnchorVer(0.2, 0.3);
	CancelButton->LocalTransform.SetTranslateY(-2.0);
	CancelButton->CreateAddText("Cancel");
	CancelButton->SetName("Cancel");
	CancelButton->AddUICallback(UIButtonCallBack);
	ProjectBg->Show(false);
}
//----------------------------------------------------------------------------
void PX2::SamplePlugin::ShowProject()
{
	ProjectBg->Show(!ProjectBg->IsShow());
}
//----------------------------------------------------------------------------
































//void SamplePlugin::DoNewProject()
//{
//	mIsNewProject = false;
//	SamplePlugin dlg(Singleton);
//	int showModel =5001;
// 	if (5001==showModel)
// 	{
//		std::string name = mProjName;
//		int screenOriention = mSceenOrientation;
//		int width = mProjWidth;
//		int height = mProjHeight;
//		std::string pathName = name + "/";
//		bool isProjctFolderExist = PX2_RM.IsFileFloderExist("Data/" + pathName);
//		if (isProjctFolderExist)
//		{
//			//MessageBox(PX2_LM_EDITOR.V("n_Notice"), PX2_LM_EDITOR.V("n_TipCreateProject"),0);
//			return;
//		}
//		else
//		{
//			PX2_APP.CloseProject();
//			PX2_RM.CreateFloder("Data/", pathName);
//			PX2_RM.CreateFloder("Data/", pathName + "images/"); 
//			PX2_RM.CreateFloder("Data/", pathName + "models/");
//			PX2_RM.CreateFloder("Data/", pathName + "scenes/");
//			PX2_RM.CreateFloder("Data/", pathName + "scripts/");
//			PX2_RM.CreateFloder("Data/", pathName + "scripts/lua/");
//			PX2_RM.CreateFloder("Data/", pathName + "scripts/lua/editor/");
//			PX2_RM.CreateFloder("Data/", pathName + "scripts_server/");
//			PX2_RM.CreateFloder("Data/", pathName + "scripts_server/lua/");
//			PX2_RM.CreateFloder("Data/", pathName + "scripts_server/lua/editor/");
//
//// 			_CreateScriptFile(pathName, "scripts/lua/", "play.lua", "play");
//// 			_CreateScriptFile(pathName, "scripts/lua/", "stop.lua", "stop");
//// 			_CreateScriptFile(pathName, "scripts/lua/editor/", "editor.lua", "editorplay");
//// 
//// 			_CreateScriptFile(pathName, "scripts_server/lua/", "play.lua", "play");
//// 			_CreateScriptFile(pathName, "scripts_server/lua/", "stop.lua", "stop");
//// 			_CreateScriptFile(pathName, "scripts_server/lua/editor/", "editor.lua", "editorstop");
//
//			std::string path = "Data/" + pathName + name + ".px2proj";
//			PX2_APP.NewProject(path, name, screenOriention, width,
//				height);
//
//		}
// 	}	
//	
//	
//}















