// PX2UIFText.cpp

#include "PX2UIFText.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIFText);
PX2_IMPLEMENT_STREAM(UIFText);
PX2_IMPLEMENT_FACTORY(UIFText);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UIFText);

//----------------------------------------------------------------------------
UIFText::UIFText()
{
	SetName("UIFText");

	mText = new0 UIText();
	AttachChild(mText);
	mText->LocalTransform.SetTranslateY(-0.1f);
	mText->SetRectUseage(UIText::RU_ALIGNS);
	mText->SetAligns(TEXTALIGN_HCENTER | TEXTALIGN_VCENTER);

	SetSize(Sizef(100.0f, 20.0f));
}
//----------------------------------------------------------------------------
UIFText::~UIFText()
{
}
//----------------------------------------------------------------------------
void UIFText::OnSizeChanged()
{
	UIFrame::OnSizeChanged();

	Rectf rect = GetLocalRect();
	mText->SetRect(rect);
}
//----------------------------------------------------------------------------
void UIFText::UpdateLeftBottomCornerOffset(Movable *parent)
{
	UIFrame::UpdateLeftBottomCornerOffset(parent);

	Rectf rect = GetLocalRect();
	mText->SetRect(rect);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIFText::UIFText(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UIFText::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mText);

	PX2_END_DEBUG_STREAM_LOAD(UIFText, source);
}
//----------------------------------------------------------------------------
void UIFText::Link(InStream& source)
{
	UIFrame::Link(source);

	if (mText)
	{
		source.ResolveLink(mText);
	}
}
//----------------------------------------------------------------------------
void UIFText::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIFText::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		if (mText)
		{
			target.Register(mText);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIFText::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mText);

	PX2_END_DEBUG_STREAM_SAVE(UIFText, target);
}
//----------------------------------------------------------------------------
int UIFText::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mText);

	return size;
}
//-----------------------------------------------------------------------