// PX2UIFPicBox.cpp

#include "PX2UIFPicBox.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIFPicBox);
PX2_IMPLEMENT_STREAM(UIFPicBox);
PX2_IMPLEMENT_FACTORY(UIFPicBox);

//----------------------------------------------------------------------------
UIFPicBox::UIFPicBox()
{
	SetColor(Float3::WHITE);

	SetName("UIFPicBox");

	mPicBox = new0 UIPicBox();
	AttachChild(mPicBox);
	mPicBox->SetPivot(Float2(0.5f, 0.5f));

	SetSize(Sizef(100.0f, 100.0f));
}
//----------------------------------------------------------------------------
UIFPicBox::~UIFPicBox()
{
}
//----------------------------------------------------------------------------
void UIFPicBox::AutoMakeSizeFixable(float scale)
{
	if (mPicBox)
	{
		mPicBox->MakeSizeWithTex(scale);
		const Sizef &size = mPicBox->GetSize();
		SetSize(size);
	}
}
//----------------------------------------------------------------------------
void UIFPicBox::OnSizeChanged()
{
	if (mPicBox)
	{
		mPicBox->SetSize(mSize);
	}

	UIFrame::OnSizeChanged();
}
//----------------------------------------------------------------------------
void UIFPicBox::OnPivotChanged()
{
	if (mPicBox)
	{
		mPicBox->SetPivot(mPivot);
	}

	UIFrame::OnPivotChanged();
}
//----------------------------------------------------------------------------
void UIFPicBox::UpdateWorldData(double applicationTime, double elapsedTime)
{
	UIFrame::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIFPicBox::UIFPicBox(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UIFPicBox::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadPointer(mPicBox);

	PX2_END_DEBUG_STREAM_LOAD(UIFPicBox, source);
}
//----------------------------------------------------------------------------
void UIFPicBox::Link(InStream& source)
{
	UIFrame::Link(source);

	if (mPicBox)
	{
		source.ResolveLink(mPicBox);
	}
}
//----------------------------------------------------------------------------
void UIFPicBox::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIFPicBox::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		if (mPicBox)
		{
			target.Register(mPicBox);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIFPicBox::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WritePointer(mPicBox);

	PX2_END_DEBUG_STREAM_SAVE(UIFPicBox, target);
}
//----------------------------------------------------------------------------
int UIFPicBox::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_POINTERSIZE(mPicBox);

	return size;
}
//-----------------------------------------------------------------------