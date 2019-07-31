// PX2UIButtonBase.cpp

#include "PX2UIButtonBase.hpp"
#include "PX2FunObject.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIButtonBase);
PX2_IMPLEMENT_STREAM(UIButtonBase);
PX2_IMPLEMENT_ABSTRACT_FACTORY(UIButtonBase);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UIButtonBase);

//----------------------------------------------------------------------------
UIButtonBase::UIButtonBase() :
	mButType(BT_COLOR),
	mButtonState(BS_NORMAL),
mPressedTouchID(-1)
{
	mNormalColor = Float3::MakeColor(80, 80, 80);
	mHoveredColor = Float3::MakeColor(100, 100, 100);
	mPressedColor = Float3::MakeColor(60, 60, 60);
	mDisableColor = Float3::MakeColor(100, 100, 100);

	mNormalAlpha = 1.0f;
	mHoveredAlpha = 1.0f;
	mPressedAlpha = 1.0f;
	mDisableAlpha = 1.0f;
	mNormalBrightness = 1.0f;
	mHoveredBrightness = 1.0f;
	mPressedBrightness = 1.0f;
	mDisableBrightness = 1.0f;

	mIsPicBoxSizeSameWithButton = true;

	SetWidget(true);

	RegistToScriptSystem();
}
//----------------------------------------------------------------------------
UIButtonBase::~UIButtonBase()
{
}
//----------------------------------------------------------------------------
void UIButtonBase::OnPivotChanged()
{
	UIFrame::OnPivotChanged();

	if (mPicBoxNormal)
		mPicBoxNormal->SetPivot(mPivot);

	if (mPicBoxOver)
		mPicBoxOver->SetPivot(mPivot);

	if (mPicBoxDown)
		mPicBoxDown->SetPivot(mPivot);

	if (mPicBoxDisabled)
		mPicBoxDisabled->SetPivot(mPivot);
}
//----------------------------------------------------------------------------
void UIButtonBase::SetButType(ButType butType)
{
	mButType = butType;

	if (BT_PICBOXSWAP == mButType)
	{
		UIPicBox *picOver = new0 UIPicBox("Data/engine/white.png");
		picOver->SetName("Hovered");
		SetPicBox(BS_HOVERED, picOver);

		UIPicBox *picDown = new0 UIPicBox("Data/engine/white.png");
		picDown->SetName("Pressed");
		SetPicBox(BS_PRESSED, picDown);

		UIPicBox *picDis = new0 UIPicBox("Data/engine/white.png");
		picDis->SetName("Disabled");
		SetPicBox(BS_DISABLED, picDis);

		SetButtonState(UIButtonBase::BS_NORMAL);

		SetStateColor(UIButtonBase::BS_NORMAL, Float3::WHITE);
		SetStateColor(UIButtonBase::BS_HOVERED, Float3::WHITE);
		SetStateColor(UIButtonBase::BS_PRESSED, Float3::WHITE);
		SetStateBrightness(UIButtonBase::BS_NORMAL, 1.0f);
		SetStateBrightness(UIButtonBase::BS_HOVERED, 1.0f);
		SetStateBrightness(UIButtonBase::BS_PRESSED, 1.0f);
	}
}
//----------------------------------------------------------------------------
void UIButtonBase::SetButtonState(ButtonState state)
{
	mButtonState = state;

	if (BT_COLOR == mButType)
	{
		mPicBoxNormal->SetColor(_GetStateColorWithActivated(mButtonState));
		mPicBoxNormal->SetAlpha(_GetStateAlphaWithActivated(mButtonState));
		mPicBoxNormal->SetBrightness(_GetStateBrightnessWithActivated(mButtonState));

		SetColor(GetColor());
		SetAlpha(GetAlpha());
		SetBrightness(GetBrightness());
	}
	else
	{
		Movable::CullingMode normalCull = Movable::CULL_ALWAYS;
		Movable::CullingMode overCull = Movable::CULL_ALWAYS;
		Movable::CullingMode downCull = Movable::CULL_ALWAYS;
		Movable::CullingMode disableCull = Movable::CULL_ALWAYS;

		if (BS_NORMAL == mButtonState)
		{
			normalCull = Movable::CULL_DYNAMIC;
			if (mPicBoxNormal)
				mPicBoxNormal->SetColor(_GetStateColorWithActivated(mButtonState));
		}
		else if (BS_HOVERED == mButtonState)
		{
			overCull = Movable::CULL_DYNAMIC;
			if (mPicBoxOver)
				mPicBoxOver->SetColor(_GetStateColorWithActivated(mButtonState));
		}
		else if (BS_PRESSED == mButtonState)
		{
			downCull = Movable::CULL_DYNAMIC;
			if (mPicBoxDown)
				mPicBoxDown->SetColor(_GetStateColorWithActivated(mButtonState));
		}
		else if (BS_DISABLED == mButtonState)
		{
			disableCull = Movable::CULL_DYNAMIC;
			if (mPicBoxDisabled)
				mPicBoxDisabled->SetColor(_GetStateColorWithActivated(mButtonState));
		}

		if (mPicBoxNormal)
			mPicBoxNormal->Culling = normalCull;

		if (mPicBoxOver)
			mPicBoxOver->Culling = overCull;

		if (mPicBoxDown)
			mPicBoxDown->Culling = downCull;

		if (mPicBoxDisabled)
			mPicBoxDisabled->Culling = disableCull;
	}
}
//----------------------------------------------------------------------------
void UIButtonBase::SetPicBox(ButtonState state, UIPicBox *pic)
{
	switch (state)
	{
	case BS_NORMAL:
		if (mPicBoxNormal)
		{
			DetachChild(mPicBoxNormal);
		}
		mPicBoxNormal = pic;
		break;

	case BS_HOVERED:
		if (mPicBoxOver)
		{
			DetachChild(mPicBoxOver);
		}
		mPicBoxOver = pic;
		break;

	case BS_PRESSED:
		if (mPicBoxDown)
		{
			DetachChild(mPicBoxDown);
		}
		mPicBoxDown = pic;
		break;

	case BS_DISABLED:
		if (mPicBoxDisabled)
		{
			DetachChild(mPicBoxDisabled);
		}
		mPicBoxDisabled = pic;
		break;

	default:
		break;
	}

	if (pic)
	{
		AttachChild(pic);
		pic->SetPivot(GetPivot());
	}
}
//----------------------------------------------------------------------------
UIPicBox *UIButtonBase::GetPicBoxAtState(ButtonState state)
{
	switch (state)
	{
	case BS_NORMAL:
		return mPicBoxNormal;
		break;
	case BS_HOVERED:
		if (mPicBoxOver)
			return mPicBoxOver;
		break;
	case BS_PRESSED:
		if (mPicBoxDown)
			return mPicBoxDown;
		break;
	case BS_DISABLED:
		if (mPicBoxDisabled)
			return mPicBoxDisabled;
		break;
	default:
		break;
	}

	return 0;
}
//----------------------------------------------------------------------------
void UIButtonBase::SetPicBoxSizeSameWithButton(bool same)
{
	mIsPicBoxSizeSameWithButton = same;
}
//----------------------------------------------------------------------------
bool UIButtonBase::IsPicBoxSizeSameWithButton() const
{
	return mIsPicBoxSizeSameWithButton;
}
//----------------------------------------------------------------------------
void UIButtonBase::AutoMakeSizeFixable(float scale)
{
	Sizef sz;
	if (mPicBoxNormal)
	{
		mPicBoxNormal->MakeSizeWithTex(scale);
		sz = mPicBoxNormal->GetSize();
	}
	if (mPicBoxOver)
	{
		mPicBoxOver->MakeSizeWithTex(scale);
	}
	if (mPicBoxDown)
	{
		mPicBoxDown->MakeSizeWithTex(scale);
	}
	if (mPicBoxDisabled)
	{
		mPicBoxDisabled->MakeSizeWithTex(scale);
	}

	SetSize(sz);
}
//----------------------------------------------------------------------------
void UIButtonBase::SetStateColor(ButtonState state, const Float3 &color)
{
	switch (state)
	{
	case BS_NORMAL:
		mNormalColor = color;
		break;
	case BS_HOVERED:
		mHoveredColor = color;
		break;
	case BS_PRESSED:
		mPressedColor = color;
		break;
	case BS_DISABLED:
		mDisableColor = color;
		break;
	default:
		break;
	}

	SetButtonState(GetButtonState());
}
//----------------------------------------------------------------------------
const Float3 &UIButtonBase::GetStateColor(ButtonState state) const
{
	switch (state)
	{
	case BS_NORMAL:
		return mNormalColor;
		break;
	case BS_HOVERED:
		return mHoveredColor;
		break;
	case BS_PRESSED:
		return mPressedColor;
		break;
	case BS_DISABLED:
		return mDisableColor;
		break;
	default:
		break;
	}

	return Float3::WHITE;
}
//----------------------------------------------------------------------------
void UIButtonBase::SetStateAlpha(ButtonState state, float alpha)
{
	switch (state)
	{
	case BS_NORMAL:
		mNormalAlpha = alpha;
		break;
	case BS_HOVERED:
		mHoveredAlpha = alpha;
		break;
	case BS_PRESSED:
		mPressedAlpha = alpha;
		break;
	case BS_DISABLED:
		mDisableAlpha = alpha;
		break;
	default:
		break;
	}

	SetButtonState(GetButtonState());
}
//----------------------------------------------------------------------------
float UIButtonBase::GetStateAlpha(ButtonState state) const
{
	switch (state)
	{
	case BS_NORMAL:
		return mNormalAlpha;
		break;
	case BS_HOVERED:
		return mHoveredAlpha;
		break;
	case BS_PRESSED:
		return mPressedAlpha;
		break;
	case BS_DISABLED:
		return mDisableAlpha;
		break;
	default:
		break;
	}

	return 1.0f;
}
//----------------------------------------------------------------------------
void UIButtonBase::SetStateBrightness(ButtonState state, float brightness)
{
	switch (state)
	{
	case BS_NORMAL:
		mNormalBrightness = brightness;
		break;
	case BS_HOVERED:
		mHoveredBrightness = brightness;
		break;
	case BS_PRESSED:
		mPressedBrightness = brightness;
		break;
	case BS_DISABLED:
		mDisableBrightness = brightness;
		break;
	default:
		break;
	}

	SetButtonState(GetButtonState());
}
//----------------------------------------------------------------------------
float UIButtonBase::GetStateBrightness(ButtonState state) const
{
	switch (state)
	{
	case BS_NORMAL:
		return mNormalBrightness;
		break;
	case BS_HOVERED:
		return mHoveredBrightness;
		break;
	case BS_PRESSED:
		return mPressedBrightness;
		break;
	case BS_DISABLED:
		return mDisableBrightness;
		break;
	default:
		break;
	}

	return 1.0f;
}
//----------------------------------------------------------------------------
void UIButtonBase::SetStateColorDefaultWhite()
{
	SetStateColor(UIButtonBase::BS_NORMAL, Float3::WHITE);
	SetStateColor(UIButtonBase::BS_HOVERED, Float3::WHITE);
	SetStateColor(UIButtonBase::BS_PRESSED, Float3::WHITE);
	SetStateBrightness(UIButtonBase::BS_NORMAL, 1.0f);
	SetStateBrightness(UIButtonBase::BS_HOVERED, 1.1f);
	SetStateBrightness(UIButtonBase::BS_PRESSED, 1.2f);
}
//----------------------------------------------------------------------------
const Float3 &UIButtonBase::_GetStateColorWithActivated(
	ButtonState state) const
{
	if (IsActivated())
		return mActivatedColor;

	return GetStateColor(state);
}
//----------------------------------------------------------------------------
float UIButtonBase::_GetStateAlphaWithActivated(ButtonState state) const
{
	if (IsActivated())
		return mActivatedAlpha;

	return GetStateAlpha(state);
}
//----------------------------------------------------------------------------
float UIButtonBase::_GetStateBrightnessWithActivated(ButtonState state) const
{
	if (IsActivated())
		return mActivatedBrightness;

	return GetStateBrightness(state);
}
//----------------------------------------------------------------------------
void UIButtonBase::SetActivate(bool act)
{
	UIFrame::SetActivate(act);
}
//----------------------------------------------------------------------------
UIFText *UIButtonBase::CreateAddText(const std::string &text,
	const Float3& color)
{
	if (mFText)
	{
		DetachChild(mFText);
		mFText = 0;
	}

	mFText = new0 UIFText();
	AttachChild(mFText);
	mFText->LocalTransform.SetTranslateY(-5.0f);
	mFText->GetText()->SetFontColor(color);
	mFText->GetText()->SetText(text);
	mFText->SetAnchorHor(0.0f, 1.0f);
	mFText->SetAnchorVer(0.0f, 1.0f);
	mFText->SetColorSelfCtrled(true);

	return mFText;
}
//----------------------------------------------------------------------------
UIFText *UIButtonBase::GetFText()
{
	return mFText;
}
//----------------------------------------------------------------------------
UIText *UIButtonBase::GetText()
{
	if (!mFText)
		return 0;

	return mFText->GetText();
}
//----------------------------------------------------------------------------
void UIButtonBase::OnSizeChanged()
{
	UIFrame::OnSizeChanged();

	if (mIsPicBoxSizeSameWithButton)
	{
		if (mPicBoxNormal)
			mPicBoxNormal->SetSize(mSize);

		if (mPicBoxOver)
			mPicBoxOver->SetSize(mSize);

		if (mPicBoxDown)
			mPicBoxDown->SetSize(mSize);

		if (mPicBoxDisabled)
			mPicBoxDisabled->SetSize(mSize);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void UIButtonBase::RegistProperties()
{
	UIFrame::RegistProperties();

	AddPropertyClass("UIButtonBase");
}
//----------------------------------------------------------------------------
void UIButtonBase::OnPropertyChanged(const PropertyObject &obj)
{
	UIFrame::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIButtonBase::UIButtonBase(LoadConstructor value) :
UIFrame(value),
mButType(BT_COLOR),
mButtonState(BS_NORMAL),
mPressedTouchID(-1)
{
	mIsPicBoxSizeSameWithButton = true;
}
//----------------------------------------------------------------------------
void UIButtonBase::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mButType);

	source.ReadAggregate(mNormalColor);
	source.ReadAggregate(mHoveredColor);
	source.ReadAggregate(mPressedColor);
	source.ReadAggregate(mDisableColor);
	source.Read(mNormalAlpha);
	source.Read(mHoveredAlpha);
	source.Read(mPressedAlpha);
	source.Read(mDisableAlpha);
	source.Read(mNormalBrightness);
	source.Read(mHoveredBrightness);
	source.Read(mPressedBrightness);
	source.Read(mDisableBrightness);

	source.ReadPointer(mPicBoxNormal);
	source.ReadPointer(mPicBoxOver);
	source.ReadPointer(mPicBoxDown);
	source.ReadPointer(mPicBoxDisabled);
	source.ReadEnum(mButtonState);

	source.ReadBool(mIsPicBoxSizeSameWithButton);

	PX2_END_DEBUG_STREAM_LOAD(UIButtonBase, source);
}
//----------------------------------------------------------------------------
void UIButtonBase::Link(InStream& source)
{
	UIFrame::Link(source);

	source.ResolveLink(mPicBoxNormal);
	source.ResolveLink(mPicBoxOver);
	source.ResolveLink(mPicBoxDown);
	source.ResolveLink(mPicBoxDisabled);
}
//----------------------------------------------------------------------------
void UIButtonBase::PostLink()
{
	UIFrame::PostLink();

	RegistToScriptSystem();
}
//----------------------------------------------------------------------------
bool UIButtonBase::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		target.Register(mPicBoxNormal);
		target.Register(mPicBoxOver);
		target.Register(mPicBoxDown);
		target.Register(mPicBoxDisabled);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIButtonBase::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mButType);

	target.WriteAggregate(mNormalColor);
	target.WriteAggregate(mHoveredColor);
	target.WriteAggregate(mPressedColor);
	target.WriteAggregate(mDisableColor);
	target.Write(mNormalAlpha);
	target.Write(mHoveredAlpha);
	target.Write(mPressedAlpha);
	target.Write(mDisableAlpha);
	target.Write(mNormalBrightness);
	target.Write(mHoveredBrightness);
	target.Write(mPressedBrightness);
	target.Write(mDisableBrightness);

	target.WritePointer(mPicBoxNormal);
	target.WritePointer(mPicBoxOver);
	target.WritePointer(mPicBoxDown);
	target.WritePointer(mPicBoxDisabled);
	target.WriteEnum(mButtonState);

	target.WriteBool(mIsPicBoxSizeSameWithButton);

	PX2_END_DEBUG_STREAM_SAVE(UIButtonBase, target);
}
//----------------------------------------------------------------------------
int UIButtonBase::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_ENUMSIZE(mButType);

	size += sizeof(mNormalColor);
	size += sizeof(mHoveredColor);
	size += sizeof(mPressedColor);
	size += sizeof(mDisableColor);

	size += sizeof(mNormalAlpha);
	size += sizeof(mHoveredAlpha);
	size += sizeof(mPressedAlpha);
	size += sizeof(mDisableAlpha);

	size += sizeof(mNormalBrightness);
	size += sizeof(mHoveredBrightness);
	size += sizeof(mPressedBrightness);
	size += sizeof(mDisableBrightness);

	size += PX2_POINTERSIZE(mPicBoxNormal);
	size += PX2_POINTERSIZE(mPicBoxOver);
	size += PX2_POINTERSIZE(mPicBoxDown);
	size += PX2_POINTERSIZE(mPicBoxDisabled);
	size += PX2_ENUMSIZE(mButtonState);

	size += PX2_BOOLSIZE(mIsPicBoxSizeSameWithButton);

	return size;
}
//----------------------------------------------------------------------------
