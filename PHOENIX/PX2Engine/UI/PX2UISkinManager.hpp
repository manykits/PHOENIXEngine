// PX2UISkinManager.hpp

#ifndef PX2UISKINMANAGER_HPP
#define PX2UISKINMANAGER_HPP

#include "PX2UIPre.hpp"
#include "PX2Singleton_NeedNew.hpp"
#include "PX2Float3.hpp"
#include "PX2Float4.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM UISkinManager : public Singleton<UISkinManager>
	{
	public:
		UISkinManager();
		virtual ~UISkinManager();

		// aui
	public:
		Float3 Color_ViewBackground;
		Float4 Color_ViewBackground4;

		float Size_ToolBar;
		float Size_ToolBarBut;

		Float3 Color_ToolBar;
		Float3 Color_ToolBarSpliter;

		float Size_SearchBar;

		float Size_Splitter;
		Float3 Color_Splitter;
		Float3 Color_Splitter_Over;
		Float3 Color_Splitter_Draging;

		Float3 Color_AuiButTab_Normal;
		Float3 Color_AuiButTab_Horvered;
		Float3 Color_AuiButTab_Pressed;
		Float3 Color_AuiButTab_Active;

		Float3 Color_ButTab_Normal;
		Float3 Color_ButTab_Horvered;
		Float3 Color_ButTab_Pressed;
		Float3 Color_ButTab_Active;

		Float3 Color_Combo_But_Normal;
		Float3 Color_Combo_But_Horvered;
		Float3 Color_Combo_But_Pressed;

		float Size_MainToolBar;
		Float3 Color_MainToolBar;

		float Size_StatusBar;
		Float3 Color_StatusBar;

		Float3 Color_SelectItem;

		// property
		Float3 Color_PropertyClass;
		float Size_PropertyFontScale;
		float Size_PropertyFontScaleCombo;
		float Size_PropertyItem;

		// font
		Float3 Color_ContentFont;
		Float3 Color_ContentFontDisable;
		Float3 Color_ContentBackground;

		Float3 Color_TableFont;
	};

#define PX2_UISM UISkinManager::GetSingleton()

}

#endif