// PX2UISkinManager.cpp

#include "PX2UISkinManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
UISkinManager::UISkinManager()
{
	Color_ViewBackground = Float3::MakeColor(30, 30, 30);
	Color_ViewBackground4 = Float4::MakeColor(30, 30, 30, 255);

	Size_ToolBar = 30.0f;
	Size_ToolBarBut = Size_ToolBar - 2.0f;

	Color_ToolBar = Float3::MakeColor(100, 100, 100);
	Color_ToolBarSpliter = Float3::MakeColor(80, 80, 80);

	Size_SearchBar = 20.0f;

	Size_Splitter = 6.0f;
	Color_Splitter = Float3::MakeColor(80, 80, 80);
	Color_Splitter_Over = Float3::MakeColor(120, 120, 120);
	Color_Splitter_Draging = Float3::MakeColor(25, 25, 25);

	Color_AuiButTab_Normal = Float3::MakeColor(120, 120, 120);
	Color_AuiButTab_Horvered = Float3::MakeColor(150, 150, 150);
	Color_AuiButTab_Pressed = Float3::MakeColor(180, 180, 180);
	Color_AuiButTab_Active = Float3::MakeColor(140, 140, 120);

	Color_ButTab_Normal = Float3::MakeColor(80, 80, 80);
	Color_ButTab_Horvered = Float3::MakeColor(110, 110, 110);
	Color_ButTab_Pressed = Float3::MakeColor(140, 140, 140);
	Color_ButTab_Active = Color_ViewBackground;

	Color_Combo_But_Normal = Float3::MakeColor(80, 80, 80);
	Color_Combo_But_Horvered = Float3::MakeColor(110, 110, 110);
	Color_Combo_But_Pressed = Float3::MakeColor(140, 140, 140);

	Size_MainToolBar = 25.0f;
	Color_MainToolBar = Float3::MakeColor(110, 110, 110);

	Size_StatusBar = 25.0f;
	Color_StatusBar = Float3::MakeColor(104, 33, 122);

	Color_SelectItem = Float3::MakeColor(0, 162, 232);

	Color_PropertyClass = Float3::MakeColor(160, 160, 160);
	Size_PropertyFontScale = 0.61f;
	Size_PropertyFontScaleCombo = 0.75f;
	Size_PropertyItem = 20.0f;

	Color_ContentFont = Float3::MakeColor(255, 255, 255);
	Color_ContentFontDisable = Float3::MakeColor(150, 150, 150);
	Color_ContentBackground = Float3::MakeColor(120, 120, 120);

	Color_TableFont = Float3::MakeColor(255, 255, 255);

	//Color_ViewBackground = Float3::MakeColor(41, 57, 85);

	//Size_ToolBar = 20.0f;
	//Color_ToolBar = Float3::MakeColor(207, 214, 229);

	//Size_Splitter = 6.0f;
	//Color_Splitter = Float3::MakeColor(46, 64, 95);
	//Color_Splitter_Over = Float3::MakeColor(46, 64, 95);
	//Color_Splitter_Draging = Float3::MakeColor(46, 64, 95);

	//Color_AuiButTab_Normal = Float3::MakeColor(77, 96, 130);
	//Color_AuiButTab_Horvered = Float3::MakeColor(77, 96, 130);
	//Color_AuiButTab_Pressed = Float3::MakeColor(77, 96, 130);
	//Color_AuiButTab_Active = Float3::YELLOW;

	//Color_ButTab_Normal = Float3::MakeColor(80, 80, 80);
	//Color_ButTab_Horvered = Float3::MakeColor(110, 110, 110);
	//Color_ButTab_Pressed = Float3::MakeColor(140, 140, 140);
	//Color_ButTab_Active = Color_ViewBackground;

	//Size_MainToolBar = 25.0f;
	//Color_MainToolBar = Float3::MakeColor(207, 214, 229);

	//Size_StatusBar = 25.0f;
	//Color_StatusBar = Float3::MakeColor(0, 122, 204);

	//Color_SelectItem = Float3::MakeColor(0, 162, 232);
}
//----------------------------------------------------------------------------
UISkinManager::~UISkinManager()
{
}
//----------------------------------------------------------------------------