// PX2UIMenuItem.cpp

#include "PX2UIMenuItem.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIMenuItem);
PX2_IMPLEMENT_STREAM(UIMenuItem);
PX2_IMPLEMENT_FACTORY(UIMenuItem);
//----------------------------------------------------------------------------
UIMenuItem::UIMenuItem(const std::string name, const std::string &title,
					   const std::string &scriptFun) 
:
mIsMainItem(false),
mTitle(title),
mScriptFun(scriptFun)
{
	SetName(name);

	mBut = new0 UIButton();
	AttachChild(mBut);

	mTitleText = new0 UIFText();
	AttachChild(mTitleText);
	mTitleText->LocalTransform.SetTranslateY(-4.0f);
	mTitleText->GetText()->SetText(title);
}
//----------------------------------------------------------------------------
UIMenuItem::~UIMenuItem()
{
}
//----------------------------------------------------------------------------
void UIMenuItem::SetBeMainItem(bool isMainItem)
{
	mIsMainItem = isMainItem;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIMenuItem::UIMenuItem(LoadConstructor value) :
UIFrame(value),
mIsMainItem(false)
{
}
//----------------------------------------------------------------------------
void UIMenuItem::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIMenuItem, source);
}
//----------------------------------------------------------------------------
void UIMenuItem::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UIMenuItem::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIMenuItem::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIMenuItem::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIMenuItem, target);
}
//----------------------------------------------------------------------------
int UIMenuItem::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------