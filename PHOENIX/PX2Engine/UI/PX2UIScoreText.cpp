// PX2UIScoreText.cpp

#include "PX2UIScoreText.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIText, UIScoreText);
PX2_IMPLEMENT_STREAM(UIScoreText);
PX2_IMPLEMENT_FACTORY(UIScoreText);
PX2_IMPLEMENT_DEFAULT_NAMES(UIText, UIScoreText);

//----------------------------------------------------------------------------
UIScoreText::UIScoreText() :
mNum(0)
{
	mSSTCtrl = new0 UIScoreStaticTextCtrl();
	AttachController(mSSTCtrl);
}
//----------------------------------------------------------------------------
UIScoreText::~UIScoreText()
{
}
//----------------------------------------------------------------------------
void UIScoreText::SetNumAnimInit(int num)
{
	mSSTCtrl->SetNumInit(num);
}
//----------------------------------------------------------------------------
void UIScoreText::SetNumAnim(int num)
{
	mSSTCtrl->SetNum(num);
}
//----------------------------------------------------------------------------
void UIScoreText::SetNum(int num)
{
	mNum = num;

	std::string str = StringHelp::IntToString(mNum);

	SetText(str);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void UIScoreText::RegistProperties()
{
	UIText::RegistProperties();

	AddPropertyClass("UIScoreText");

	AddProperty("Speed", PT_FLOAT, mSSTCtrl->GetSpeed());
}
//----------------------------------------------------------------------------
void UIScoreText::OnPropertyChanged(const PropertyObject &obj)
{
	UIText::OnPropertyChanged(obj);

	if ("Speed" == obj.Name)
	{
		mSSTCtrl->SetSpeed(PX2_ANY_AS(obj.Data, float));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIScoreText::UIScoreText(LoadConstructor value)
:
UIText(value),
mNum(0)
{
}
//----------------------------------------------------------------------------
void UIScoreText::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIText::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mNum);
	source.ReadPointer(mSSTCtrl);

	PX2_END_DEBUG_STREAM_LOAD(UIScoreText, source);
}
//----------------------------------------------------------------------------
void UIScoreText::Link(InStream& source)
{
	UIText::Link(source);
	source.ResolveLink(mSSTCtrl);
}
//----------------------------------------------------------------------------
void UIScoreText::PostLink()
{
	UIText::PostLink();
}
//----------------------------------------------------------------------------
bool UIScoreText::Register(OutStream& target) const
{
	if (UIText::Register(target))
	{
		target.Register(mSSTCtrl);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIScoreText::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIText::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mNum);
	target.WritePointer(mSSTCtrl);

	PX2_END_DEBUG_STREAM_SAVE(UIScoreText, target);
}
//----------------------------------------------------------------------------
int UIScoreText::GetStreamingSize(Stream &stream) const
{
	int size = UIText::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mNum);
	size += PX2_POINTERSIZE(mSSTCtrl);

	return size;
}
//----------------------------------------------------------------------------