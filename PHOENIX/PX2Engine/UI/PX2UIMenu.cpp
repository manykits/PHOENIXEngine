// PX2UIMenu.cpp

#include "PX2UIMenu.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIMenu);
PX2_IMPLEMENT_STREAM(UIMenu);
PX2_IMPLEMENT_FACTORY(UIMenu);
//----------------------------------------------------------------------------
UIMenu::UIMenu(MenuType mt) :
mMenuType(mt)
{
	SetName("UIMenu");
	mMainItemSize = Sizef(35.0f, 20.0f);
	mItemFontSize = 24;

	UIFrameGridAlignControl *gridCtrl = CreateAddGridAlignCtrl();
	PX2_UNUSED(gridCtrl);
}
//----------------------------------------------------------------------------
UIMenu::~UIMenu()
{
}
//----------------------------------------------------------------------------
void UIMenu::SetMainItemSize(const Sizef &mainItemSize)
{
	mMainItemSize = mainItemSize;
}
//----------------------------------------------------------------------------
void UIMenu::SetMenuItemFontSize(int fontSize)
{
	mItemFontSize = fontSize;

	for (int i=0; i<(int)mMainItemsVec.size(); i++)
	{
		mMainItemsVec[i]->GetText()->GetText()->SetFontWidthHeight(fontSize, fontSize);
	}
}
//----------------------------------------------------------------------------
UIMenuItem *UIMenu::AddMainItem(const std::string &name,
								const std::string &title,
								const std::string &scriptFun)
{
	UIMenuItem *item = new0 UIMenuItem(name, title, scriptFun);
	item->SetBeMainItem(true);
	item->LocalTransform.SetTranslateY(-1.0f);
	item->GetText()->GetText()->SetFontWidthHeight(mItemFontSize, mItemFontSize);
	item->GetText()->LocalTransform.SetTranslateY(-1.0f);

	mMainItemsMap[name] = item;
	mMainItemsVec.push_back(item);
	AttachChild(item);

	return item;
}
//----------------------------------------------------------------------------
int UIMenu::GetNunMainItems() const
{
	return (int)mMainItemsVec.size();
}
//----------------------------------------------------------------------------
void UIMenu::UpdateWorldData(double applicationTime, double elapsedTime)
{
	UIFrame::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void UIMenu::RegistProperties()
{
	UIFrame::RegistProperties();
	AddPropertyClass("UIMenu");
}
//----------------------------------------------------------------------------
void UIMenu::OnPropertyChanged(const PropertyObject &obj)
{
	UIFrame::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIMenu::UIMenu(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UIMenu::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIMenu, source);
}
//----------------------------------------------------------------------------
void UIMenu::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UIMenu::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIMenu::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIMenu::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIMenu, target);
}
//----------------------------------------------------------------------------
int UIMenu::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------