// PX2UIGridItem.cpp

#include "PX2UIGridItem.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIGridItem);
PX2_IMPLEMENT_STREAM(UIGridItem);
PX2_IMPLEMENT_FACTORY(UIGridItem);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UIGridItem);

//----------------------------------------------------------------------------
UIGridItem::UIGridItem()
{
}
//----------------------------------------------------------------------------
UIGridItem::~UIGridItem()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIGridItem::UIGridItem(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UIGridItem::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIGridItem, source);
}
//----------------------------------------------------------------------------
void UIGridItem::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UIGridItem::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIGridItem::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIGridItem::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIGridItem, target);
}
//----------------------------------------------------------------------------
int UIGridItem::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------