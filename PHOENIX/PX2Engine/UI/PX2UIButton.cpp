// PX2UIButton.cpp

#include "PX2UIButton.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2UIDefine.hpp"
#include "PX2FunObject.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2UIEvent.hpp"
#include "PX2Time.hpp"
#include "PX2Log.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIButtonBase, UIButton);
PX2_IMPLEMENT_STREAM(UIButton);
PX2_IMPLEMENT_FACTORY(UIButton);
PX2_IMPLEMENT_DEFAULT_NAMES(UIButtonBase, UIButton);

//----------------------------------------------------------------------------
UIButton::UIButton(ButType butType) :
mAfterReleasedType(ART_NORMAL),
mIsRecoverBegin(false),
mAfterReleasedRecoverTime(6.0f),
mRecoverBeginTimeimg(0.0f),
mDoublePressedCount(0),
mDoublePressedTimeLimit(0.3f),
mDoublePressedTimeimg(0.0f)
{
	SetName("UIButton");

	mButType = butType;

	if (BT_COLOR == mButType)
	{
		UIPicBox *picNormal = new0 UIPicBox("Data/engine/white.png");
		picNormal->SetName("Normal");
		SetPicBox(BS_NORMAL, picNormal);
	}
	else
	{
		UIPicBox *picNormal = new0 UIPicBox("Data/engine/white.png");
		picNormal->SetName("Normal");
		SetPicBox(BS_NORMAL, picNormal);

		UIPicBox *picOver = new0 UIPicBox("Data/engine/white.png");
		picOver->SetName("Hovered");
		SetPicBox(BS_HOVERED, picOver);

		UIPicBox *picDown = new0 UIPicBox("Data/engine/white.png");
		picDown->SetName("Pressed");
		SetPicBox(BS_PRESSED, picDown);

		UIPicBox *picDis = new0 UIPicBox("Data/engine/white.png");
		picDis->SetName("Disabled");
		SetPicBox(BS_DISABLED, picDis);
	}

	SetButtonState(BS_NORMAL);

	SetSize(40.0f, 40.0f);
}
//----------------------------------------------------------------------------
UIButton::~UIButton()
{
}
//----------------------------------------------------------------------------
void UIButton::Enable(bool enable)
{
	UIButtonBase::Enable(enable);

	SetButtonState(enable ? UIButtonBase::BS_NORMAL:UIButtonBase::BS_DISABLED);
}
//----------------------------------------------------------------------------
void UIButton::SetRoundRadius(float radius)
{
	UIFrame::SetRoundRadius(radius);
}
//----------------------------------------------------------------------------
void UIButton::SetAfterReleasedType(AfterReleasedType type)
{
	if (type == mAfterReleasedType)
		return;

	Enable(true);
	mAfterReleasedType = type;

	if (ART_NORMAL == mAfterReleasedType ||
		ART_DISABLE == mAfterReleasedType)
	{
		mAfterReleasedRecoverTime = 0.0f;
		mIsRecoverBegin = false;
		mRecoverBeginTimeimg = (float)Time::GetTimeInSeconds();
	}
}
//----------------------------------------------------------------------------
void UIButton::OnEvent(Event *event)
{
	PX2_UNUSED(event);
}
//----------------------------------------------------------------------------
void UIButton::OnPressed()
{
	_UICallbacksCalls(UICT_PRESSED);

	mDoublePressedCount++;

	if (2 == mDoublePressedCount)
	{
		if (mDoublePressedTimeimg <= mDoublePressedTimeLimit)
		{
			OnDoublePressed();
		}
		else
		{
			_ResetDoublePressed();
		}
	}
}
//----------------------------------------------------------------------------
void UIButton::OnPressedNotPick()
{
	_UICallbacksCalls(UICT_PRESSED_NOTPICK);
}
//----------------------------------------------------------------------------
void UIButton::OnReleased()
{
	if (ART_DISABLE == mAfterReleasedType)
	{
		Enable(false);
	}
	else if (ART_DISABLE_RECOVER == mAfterReleasedType)
	{
		Enable(false);
		mIsRecoverBegin = true;
		mRecoverBeginTimeimg = (float)Time::GetTimeInSeconds();
	}

	_UICallbacksCalls(UICT_RELEASED);
}
//----------------------------------------------------------------------------
void UIButton::OnDoublePressed()
{
	_ResetDoublePressed();

	_UICallbacksCalls(UICT_DOUBLE_PRESSED);
}
//----------------------------------------------------------------------------
void UIButton::OnReleasedNotPick()
{
	_UICallbacksCalls(UICT_RELEASED_NOTPICK);
}
//----------------------------------------------------------------------------
void UIButton::SetActivate(bool act)
{
	UIButtonBase::SetActivate(act);

	ButType bt = GetButType();
	if (BT_COLOR == bt)
	{
		SetButtonState(GetButtonState());
	}
	else
	{
	}
}
//----------------------------------------------------------------------------
void UIButton::UpdateWorldData(double applicationTime, double elapsedTime)
{
	UIButtonBase::UpdateWorldData(applicationTime, elapsedTime);

	if (mIsRecoverBegin)
	{
		float curTime = (float)Time::GetTimeInSeconds();

		if ((curTime - mRecoverBeginTimeimg) > mAfterReleasedRecoverTime)
		{
			Enable(true);

			mIsRecoverBegin = false;
		}
	}

	if (1 == mDoublePressedCount)
	{
		mDoublePressedTimeimg += (float)elapsedTime;

		if (mDoublePressedTimeimg > mDoublePressedTimeLimit)
		{
			_ResetDoublePressed();
		}
	}
}
//----------------------------------------------------------------------------
void UIButton::OnWidgetPicked(const CanvasInputData &inputData)
{
	UIFrame::OnWidgetPicked(inputData);

	bool isDoPick = true;
	if (mRangeAcceptFrame)
	{
		Rectf worldRect = mRangeAcceptFrame->GetWorldRect();
		isDoPick = worldRect.IsInsize(inputData.LogicPos.X(),
			inputData.LogicPos.Z());
	}

	if (isDoPick)
	{
		ButtonState state = GetButtonState();

		if (UIPT_PRESSED == inputData.PickType)
		{
			mPressedTouchID = inputData.TouchID;

			if (state == BS_NORMAL || state == BS_HOVERED || state==BS_PRESSED)
			{
				SetButtonState(BS_PRESSED);
				OnPressed();
			}
		}
		else if (UIPT_RELEASED == inputData.PickType)
		{
			if (mPressedTouchID == inputData.TouchID)
			{
				if (state == BS_PRESSED)
				{
					SetButtonState(BS_HOVERED);
					OnReleased();
				}

				mPressedTouchID = -1;
			}
		}
#if defined (_WIN32) || defined (WIN32)
		else if (UIPT_MOVED == inputData.PickType)
		{
			if (mPressedTouchID == inputData.TouchID)
			{
				if (state == BS_NORMAL)
				{
					SetButtonState(BS_HOVERED);
				}
			}
		}
#endif
	}
}
//----------------------------------------------------------------------------
void UIButton::OnSizeNodeNotPicked(const CanvasInputData &inputData)
{
	mLastPickData = inputData;

	bool isDoPick = true;
	if (mRangeAcceptFrame)
	{
		Rectf worldRect = mRangeAcceptFrame->GetWorldRect();
		isDoPick = worldRect.IsInsize(inputData.LogicPos.X(),
			inputData.LogicPos.Z());
	}

	if (isDoPick)
	{
		ButtonState state = GetButtonState();

		if (UICT_PRESSED == inputData.PickType)
		{
			if (mPressedTouchID == inputData.TouchID)
			{
				OnPressedNotPick();
				mPressedTouchID = -1;
			}
		}
		else if (UICT_RELEASED == inputData.PickType)
		{
			if (BS_PRESSED == state)
			{
				if (mPressedTouchID == inputData.TouchID)
				{
					SetButtonState(BS_NORMAL);
					OnReleasedNotPick();
					mPressedTouchID = -1;
				}
			}
		}
		else if (UIPT_MOVED == inputData.PickType)
		{
			if (state == BS_HOVERED)
			{
				if (mPressedTouchID == inputData.TouchID)
				{
					SetButtonState(BS_NORMAL);
					mPressedTouchID = -1;
				}
			}
		}
	}

	UIButtonBase::OnSizeNodeNotPicked(inputData);
}
//----------------------------------------------------------------------------
void UIButton::_ResetDoublePressed()
{
	mDoublePressedTimeimg = 0.0f;
	mDoublePressedCount = 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void UIButton::RegistProperties()
{
	UIButtonBase::RegistProperties();

	AddPropertyClass("UIButton");

	std::vector<std::string> afterReleasedTypes;
	afterReleasedTypes.push_back("ART_NORMAL");
	afterReleasedTypes.push_back("ART_DISABLE");
	afterReleasedTypes.push_back("ART_DISABLE_RECOVER");
	AddPropertyEnum("AfterReleasedType", mAfterReleasedType,
		afterReleasedTypes);
	AddProperty("AfterReleasedRecoverTime", PT_FLOAT,
		GetAfterReleasedRecoverTime());
}
//----------------------------------------------------------------------------
void UIButton::OnPropertyChanged(const PropertyObject &obj)
{
	UIButtonBase::OnPropertyChanged(obj);

	if ("AfterReleasedType" == obj.Name)
	{
		SetAfterReleasedType((AfterReleasedType)*Any_Cast<int>(&obj.Data));
	}
	else if ("AfterReleasedRecoverTime" == obj.Name)
	{
		SetAfterReleasedRecoverTime(*Any_Cast<float>(&obj.Data));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIButton::UIButton(LoadConstructor value) :
UIButtonBase(value),
mAfterReleasedType(ART_NORMAL),
mAfterReleasedRecoverTime(6.0),
mIsRecoverBegin(false),
mRecoverBeginTimeimg(0.0f),
mDoublePressedCount(0),
mDoublePressedTimeLimit(0.3f),
mDoublePressedTimeimg(0.0f)
{
}
//----------------------------------------------------------------------------
void UIButton::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIButtonBase::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mAfterReleasedType);
	source.Read(mAfterReleasedRecoverTime);

	PX2_END_DEBUG_STREAM_LOAD(UIButton, source);
}
//----------------------------------------------------------------------------
void UIButton::Link(InStream& source)
{
	UIButtonBase::Link(source);
}
//----------------------------------------------------------------------------
void UIButton::PostLink()
{
	UIButtonBase::PostLink();
}
//----------------------------------------------------------------------------
bool UIButton::Register(OutStream& target) const
{
	return UIButtonBase::Register(target);
}
//----------------------------------------------------------------------------
void UIButton::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIButtonBase::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mAfterReleasedType);
	target.Write(mAfterReleasedRecoverTime);

	PX2_END_DEBUG_STREAM_SAVE(UIButton, target);
}
//----------------------------------------------------------------------------
int UIButton::GetStreamingSize(Stream &stream) const
{
	int size = UIButtonBase::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_ENUMSIZE(mAfterReleasedType);
	size += sizeof(mAfterReleasedRecoverTime);

	return size;
}
//----------------------------------------------------------------------------
