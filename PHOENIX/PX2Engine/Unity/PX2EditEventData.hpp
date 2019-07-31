// PX2EditEventData.hpp

#ifndef PX2_EDITEVENTDATA_HPP
#define PX2_EDITEVENTDATA_HPP

#include "PX2EventSystem.hpp"
#include "PX2Float2.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM EED_CreateWindow
	{
	public:
		EED_CreateWindow();
		~EED_CreateWindow();

		bool IsMain;
		std::string Name;
		int ID;
		Float2 Pos;
	};

	class PX2_ENGINE_ITEM EED_AddMenu
	{
	public:
		EED_AddMenu();
		~EED_AddMenu();

		std::string Where;

		enum ItemType
		{
			IT_MAIN_MENU,
			IT_MAIN_SUBMENU,
			IT_MAIN_ITEM,
			IT_MAIN_ITEMSPARATER,
			IT_EDIT_MENU,
			IT_EDIT_SUBMENU,
			IT_EDIT_ITEM,
			IT_EDIT_ITEMSPARATER,
			IT_EDIT_POPUP,
			IT_MAX_TYPE
		};
		ItemType TheItemType;

		std::string ParentName;
		std::string Name;
		std::string Title;
		std::string Script;
		std::string ScriptParam;
		std::string Tag;

		APoint PopUpPos;
	};

	class PX2_ENGINE_ITEM EED_Tip
	{
	public:
		EED_Tip();
		~EED_Tip();

		std::string Title;
		std::string Content;
	};

}

#endif