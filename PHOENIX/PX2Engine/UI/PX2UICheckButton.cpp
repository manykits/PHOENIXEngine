// PX2UICheckButton.cpp

#include "PX2UICheckButton.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2UIDefine.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIButtonBase, UICheckButton);
PX2_IMPLEMENT_STREAM(UICheckButton);
PX2_IMPLEMENT_FACTORY(UICheckButton);
PX2_IMPLEMENT_DEFAULT_NAMES(UIButtonBase, UICheckButton);

//----------------------------------------------------------------------------
UICheckButton::UICheckButton()
{
	SetName("UICheckButton");

	UIPicBox *picNormal = new0 UIPicBox("Data/engine/common.xml", "check_null");
	picNormal->SetName("Normal");
	picNormal->SetPicBoxType(UIPicBox::PBT_NORMAL);

	UIPicBox *picOver = new0 UIPicBox("Data/engine/common.xml", "check_null");
	picOver->SetName("Over");
	picOver->SetPicBoxType(UIPicBox::PBT_NORMAL);

	UIPicBox *picDown = new0 UIPicBox("Data/engine/common.xml", "check_ed");
	picDown->SetName("Down");
	picDown->SetPicBoxType(UIPicBox::PBT_NORMAL);

	UIPicBox *picDis = new0 UIPicBox("Data/engine/common.xml", "check_dis");
	picDis->SetName("Dis");
	picDis->SetPicBoxType(UIPicBox::PBT_NORMAL);

	SetButType(BT_PICBOXSWAP);

	SetPicBox(BS_NORMAL, picNormal);
	SetPicBox(BS_HOVERED, picOver);
	SetPicBox(BS_PRESSED, picDown);
	SetPicBox(BS_DISABLED, picDis);

	SetButtonState(BS_NORMAL);

	SetSize(40.0f, 40.0f);
}
//----------------------------------------------------------------------------
UICheckButton::~UICheckButton()
{
}
//----------------------------------------------------------------------------
void UICheckButton::Enable(bool enable)
{
	UIButtonBase::Enable(enable);

	SetButtonState(enable ? 
		UIButtonBase::BS_NORMAL : UIButtonBase::BS_DISABLED);
}
//----------------------------------------------------------------------------
void UICheckButton::OnEvent(Event *event)
{
	PX2_UNUSED(event);
}
//----------------------------------------------------------------------------
void UICheckButton::Check(bool check, bool doCall)
{
	if (check)
	{
		SetButtonState(BS_PRESSED);
		if (doCall)
		{
			OnCheck();
		}
	}
	else
	{
		SetButtonState(BS_NORMAL);

		if (doCall)
		{
			OnDisCheck();
		}
	}
}
//----------------------------------------------------------------------------
bool UICheckButton::IsCheck() const
{
	return BS_PRESSED == GetButtonState();
}
//----------------------------------------------------------------------------
void UICheckButton::OnCheck()
{
	_UICallbacksCalls(UICT_CHECKED);
}
//----------------------------------------------------------------------------
void UICheckButton::OnDisCheck()
{
	_UICallbacksCalls(UICT_DISCHECKED);
}
//----------------------------------------------------------------------------
void UICheckButton::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	UIButtonBase::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void UICheckButton::OnWidgetPicked(const CanvasInputData &inputData)
{
	UIFrame::OnWidgetPicked(inputData);

	if (UIPT_RELEASED == inputData.PickType)
	{
		ButtonState state = GetButtonState();

		if (BS_NORMAL == state)
		{
			Check(true);
		}
		else if (BS_PRESSED == state)
		{
			Check(false);
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void UICheckButton::RegistProperties()
{
	UIButtonBase::RegistProperties();

	AddPropertyClass("UICheckButton");
}
//----------------------------------------------------------------------------
void UICheckButton::OnPropertyChanged(const PropertyObject &obj)
{
	UIButtonBase::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UICheckButton::UICheckButton(LoadConstructor value)
	:
	UIButtonBase(value)
{
}
//----------------------------------------------------------------------------
void UICheckButton::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIButtonBase::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UICheckButton, source);
}
//----------------------------------------------------------------------------
void UICheckButton::Link(InStream& source)
{
	UIButtonBase::Link(source);
}
//----------------------------------------------------------------------------
void UICheckButton::PostLink()
{
	UIButtonBase::PostLink();
}
//----------------------------------------------------------------------------
bool UICheckButton::Register(OutStream& target) const
{
	return UIButtonBase::Register(target);
}
//----------------------------------------------------------------------------
void UICheckButton::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIButtonBase::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UICheckButton, target);
}
//----------------------------------------------------------------------------
int UICheckButton::GetStreamingSize(Stream &stream) const
{
	int size = UIButtonBase::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
