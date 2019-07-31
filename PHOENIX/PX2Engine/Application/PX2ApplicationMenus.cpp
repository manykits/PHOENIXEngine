// PX2ApplicationMenus.cpp

#include "PX2Application.hpp"
#include "PX2ProjectEvent.hpp"
#include "PX2ProjectEventData.hpp"
#include "PX2EditEventType.hpp"
#include "PX2EditEventData.hpp"
#include "PX2UIAuiManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void Application::Menu_Clear()
{
	mItem->Name = "ExtendMenu";
}
//----------------------------------------------------------------------------
void Application::Menu_AddSubItemCatalogue(
	const std::string &parentAllName,
	const std::string &name,
	const std::string &title)
{
	MenuItem *parentItem = 0;

	if (!parentAllName.empty())
	{
		parentItem = mItem->GetMenuItem(parentAllName);
	}
	else
	{
		parentItem = mItem;
	}

	if (parentItem)
	{
		MenuItem *menuItem = new0 MenuItem();
		menuItem->TheType = MenuItem::T_SUB;
		menuItem->Name = name;
		menuItem->Title = title;
		menuItem->AllName = parentItem->AllName + name;

		parentItem->Items.push_back(menuItem);
	}
}
//----------------------------------------------------------------------------
void Application::Menu_AddItem(
	const std::string &parentAllName,
	const std::string &name,
	const std::string &title,
	const std::string &script)
{
	MenuItem *parentItem = 0;

	if (!parentAllName.empty())
	{
		parentItem = mItem->GetMenuItem(parentAllName);
	}
	else
	{
		parentItem = mItem;
	}

	if (parentItem)
	{
		MenuItem *menuItem = new0 MenuItem();
		menuItem->TheType = MenuItem::T_ITEM;
		menuItem->Name = name;
		menuItem->Title = title;
		menuItem->Script = script;
		menuItem->AllName = parentItem->AllName + name;

		parentItem->Items.push_back(menuItem);
	}
}
//----------------------------------------------------------------------------
Application::MenuItem::MenuItem()
{
	TheType = T_ITEM;
}
//----------------------------------------------------------------------------
Application::MenuItem::~MenuItem()
{
}
//----------------------------------------------------------------------------
void Application::MenuItem::Clear()
{
	for (int i = 0; i < (int)Items.size(); i++)
	{
		MenuItem *item = Items[i];
		item->Clear();
	}

	Items.clear();
}
//----------------------------------------------------------------------------
Application::MenuItem *Application::MenuItem::GetMenuItem(
	const std::string &parentAllName)
{
	for (int i = 0; i < (int)Items.size(); i++)
	{
		MenuItem *item = Items[i];

		if (parentAllName == item->AllName)
			return item;

		MenuItem *subItem = item->GetMenuItem(parentAllName);
		if (subItem)
			return subItem;
	}

	return 0;
}
//----------------------------------------------------------------------------
Application::MenuItem *Application::GetMenuItem()
{
	return mItem;
}
//----------------------------------------------------------------------------
void Application::Menu_Main_AddMainItem(const std::string &name,
	const std::string &title)
{
	EED_AddMenu data;
	data.Where = "Main";
	data.TheItemType = EED_AddMenu::IT_MAIN_MENU;
	data.Name = name;
	data.Title = title;

	Event *ent = EditES::CreateEventX(EditES::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Application::Menu_Main_AddSubItem(const std::string &parentName,
	const std::string &name, const std::string &title)
{
	EED_AddMenu data;
	data.Where = "Main";
	data.TheItemType = EED_AddMenu::IT_MAIN_SUBMENU;
	data.ParentName = parentName;
	data.Name = name;
	data.Title = title;

	Event *ent = EditES::CreateEventX(EditES::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Application::Menu_Main_AddItem(const std::string &parentName,
	const std::string &name,
	const std::string &title, const std::string &script, 
	const std::string &scriptParam,
	const std::string &tag)
{
	EED_AddMenu data;
	data.Where = "Main";
	data.TheItemType = EED_AddMenu::IT_MAIN_ITEM;
	data.ParentName = parentName;
	data.Name = name;
	data.Title = title;
	data.Script = script;
	data.ScriptParam = scriptParam;
	data.Tag = tag;

	Event *ent = EditES::CreateEventX(EditES::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Application::Menu_Main_AddItemSeparater(const std::string &parentName)
{
	EED_AddMenu data;
	data.Where = "Main";
	data.TheItemType = EED_AddMenu::IT_MAIN_ITEMSPARATER;
	data.ParentName = parentName;

	Event *ent = EditES::CreateEventX(EditES::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Application::Menu_Edit_Begin(const std::string &whe, const std::string &name)
{
	EED_AddMenu data;
	data.Where = whe;
	data.Name = name;
	data.TheItemType = EED_AddMenu::IT_EDIT_MENU;

	Event *ent = EditES::CreateEventX(EditES::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Application::Menu_Edit_AddSubItem(const std::string &whe,
	const std::string &parentName,
	const std::string &name, const std::string &title)
{
	EED_AddMenu data;
	data.Where = whe;
	data.TheItemType = EED_AddMenu::IT_EDIT_SUBMENU;
	data.ParentName = parentName;
	data.Name = name;
	data.Title = title;

	Event *ent = EditES::CreateEventX(EditES::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Application::Menu_Edit_AddItem(const std::string &whe,
	const std::string &parentName, const std::string &name,
	const std::string &title,
	const std::string &script,
	const std::string &scriptParam,
	const std::string &tag)
{
	EED_AddMenu data;
	data.Where = whe;
	data.TheItemType = EED_AddMenu::IT_EDIT_ITEM;
	data.ParentName = parentName;
	data.Name = name;
	data.Title = title;
	data.Script = script;
	data.ScriptParam = scriptParam;
	data.Tag = tag;

	Event *ent = EditES::CreateEventX(EditES::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Application::Menu_Edit_AddItemSeparater(const std::string &whe,
	const std::string &parentName)
{
	EED_AddMenu data;
	data.Where = whe;
	data.TheItemType = EED_AddMenu::IT_EDIT_ITEMSPARATER;
	data.ParentName = parentName;

	Event *ent = EditES::CreateEventX(EditES::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
void Application::Menu_Edit_EndPopUp(const std::string &whe,
	const APoint &pos)
{
	EED_AddMenu data;
	data.Where = whe;
	data.TheItemType = EED_AddMenu::IT_EDIT_POPUP;
	data.PopUpPos = pos;

	Event *ent = EditES::CreateEventX(EditES::N_AddMenu);
	ent->SetData<EED_AddMenu>(data);
	PX2_EW.BroadcastingLocalEvent(ent);
}
//----------------------------------------------------------------------------
RenderWindow *Application::CreateUIWindow(RenderWindow *parent,
	const std::string &name, const std::string &title, const APoint &pos,
	const Sizef &size, bool isFloat)
{
	RenderWindow *rw = new0 RenderWindow();
	rw->SetID(RenderWindow::GetNextWindowID());
	rw->SetName(name);
	rw->SetTitle(title);
	rw->SetFloat(isFloat);
	rw->SetMain(false);
	rw->SetScreenSize(size);
	rw->SetPosition(pos);
	PX2_GR.AddRenderWindow(name, rw);
	rw->SetParentRenderWindow(parent);

	Canvas *mainCanvas = new0 Canvas();
	mainCanvas->CreateUICameraNode();
	mainCanvas->SetMain(true);
	mainCanvas->EnableAnchorLayout(true);
	mainCanvas->SetAnchorHor(0.0f, 1.0f);
	mainCanvas->SetAnchorVer(0.0f, 1.0f);
	mainCanvas->ComeInEventWorld();
	mainCanvas->SetName("MainCanvas");
	rw->SetMainCanvas(mainCanvas);

	UIFrame *frame = new0 UIFrame();
	frame->SetName("CanvasContentFrame");
	mainCanvas->AttachChild(frame);
	frame->SetAnchorHor(Float2(0.0f, 1.0f));
	frame->SetAnchorVer(Float2(0.0f, 1.0f));
	frame->SetSize(500.0f, 300.0f);
	frame->SetPivot(0.5f, 0.5f);

	Event *ent = EditES::CreateEventX(EditES::N_Window);
	ent->SetData<RenderWindow*>(rw);
	PX2_EW.BroadcastingLocalEvent(ent);

	return rw;
}
//----------------------------------------------------------------------------