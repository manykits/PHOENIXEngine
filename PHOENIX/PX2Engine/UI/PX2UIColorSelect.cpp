// PX2UIColorSelectFrame.cpp

#include "PX2UIColorSelect.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIColorSelector);
PX2_IMPLEMENT_STREAM(UIColorSelector);
PX2_IMPLEMENT_FACTORY(UIColorSelector);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UIColorSelector);

//----------------------------------------------------------------------------
UIColorSelector::UIColorSelector() :
mIsPickedPattle(false)
{
	SetName("UIColorSelector");

	float colorPicWidth = 25.0f;

	mSliderBrightness = new0 UISlider();
	AttachChild(mSliderBrightness);
	mSliderBrightness->LocalTransform.SetTranslateY(-2.0f);
	mSliderBrightness->SetDirectionType(UISlider::DT_VERTICALITY);
	mSliderBrightness->EnableAnchorLayout(true);
	mSliderBrightness->SetPivot(0.0f, 0.0f);
	mSliderBrightness->SetAnchorHor(0.0f, 0.0f);
	mSliderBrightness->SetAnchorVer(0.0f, 1.0f);
	mSliderBrightness->SetAnchorParamVer(4.0f, -4.0f);
	mSliderBrightness->SetSize(colorPicWidth, 0.0f);
	mSliderBrightness->SetPercent(0.5f);
	mSliderBrightness->SetSliderLength(8.0f);
	mSliderBrightness->SetMemUICallback(this,
		(UIFrame::MemUICallback)(&UIColorSelector::_UICallback));

	float editBoxHeight = 20.0f;

	float textHorAdjust = 6.0f;
	float textWidth = 40.0f-textHorAdjust;

	float anchorVer = editBoxHeight*0.5f + 4.0f;
	mEditBoxBlue = new0 UIEditBox();
	AttachChild(mEditBoxBlue);
	mEditBoxBlue->LocalTransform.SetTranslateY(-1.0f);
	mEditBoxBlue->SetAnchorHor(0.0f, 1.0f);
	mEditBoxBlue->SetAnchorParamHor(colorPicWidth + textWidth, -10.0f);
	mEditBoxBlue->SetAnchorVer(0.0f, 0.0f);
	mEditBoxBlue->SetAnchorParamVer(anchorVer, anchorVer);
	mEditBoxBlue->SetSize(0.0f, editBoxHeight);
	mEditBoxBlue->GetInputText()->SetFontScale(0.7f);

	mEBTextBlue = new0 UIFText();
	AttachChild(mEBTextBlue);
	mEBTextBlue->LocalTransform.SetTranslateY(-1.0f);
	mEBTextBlue->SetPivot(0.0f, 0.5f);
	mEBTextBlue->SetAnchorHor(0.0f, 0.0f);
	mEBTextBlue->SetAnchorParamHor(colorPicWidth + textHorAdjust, colorPicWidth + textHorAdjust);
	mEBTextBlue->SetAnchorVer(0.0f, 0.0f);
	mEBTextBlue->SetAnchorParamVer(anchorVer, anchorVer);
	mEBTextBlue->SetSize(textWidth, editBoxHeight);
	mEBTextBlue->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	mEBTextBlue->GetText()->SetText("Red");
	mEBTextBlue->GetText()->SetFontScale(0.7f);

	anchorVer += editBoxHeight + 1.0f;
	mEditBoxGreen = new0 UIEditBox();
	AttachChild(mEditBoxGreen);
	mEditBoxGreen->LocalTransform.SetTranslateY(-1.0f);
	mEditBoxGreen->SetAnchorHor(0.0f, 1.0f);
	mEditBoxGreen->SetAnchorParamHor(colorPicWidth + textWidth, -10.0f);
	mEditBoxGreen->SetAnchorVer(0.0f, 0.0f);
	mEditBoxGreen->SetAnchorParamVer(anchorVer, anchorVer);
	mEditBoxGreen->SetSize(0.0f, editBoxHeight);
	mEditBoxGreen->GetInputText()->SetFontScale(0.7f);

	mEBTextGreen = new0 UIFText();
	AttachChild(mEBTextGreen);
	mEBTextGreen->SetPivot(0.0f, 0.5f);
	mEBTextGreen->LocalTransform.SetTranslateY(-1.0f);
	mEBTextGreen->SetAnchorHor(0.0f, 0.0f);
	mEBTextGreen->SetAnchorParamHor(colorPicWidth + textHorAdjust, colorPicWidth + textHorAdjust);
	mEBTextGreen->SetAnchorVer(0.0f, 0.0f);
	mEBTextGreen->SetAnchorParamVer(anchorVer, anchorVer);
	mEBTextGreen->SetSize(textWidth, editBoxHeight);
	mEBTextGreen->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	mEBTextGreen->GetText()->SetText("Green");
	mEBTextGreen->GetText()->SetFontScale(0.7f);

	anchorVer += editBoxHeight + 1.0f;
	mEditBoxRed = new0 UIEditBox();
	AttachChild(mEditBoxRed);
	mEditBoxRed->LocalTransform.SetTranslateY(-1.0f);
	mEditBoxRed->SetAnchorHor(0.0f, 1.0f);
	mEditBoxRed->SetAnchorParamHor(colorPicWidth + textWidth, -10.0f);
	mEditBoxRed->SetAnchorVer(0.0f, 0.0f);
	mEditBoxRed->SetAnchorParamVer(anchorVer, anchorVer);
	mEditBoxRed->SetSize(0.0f, editBoxHeight);
	mEditBoxRed->GetInputText()->SetFontScale(0.7f);

	mEBTextRed = new0 UIFText();
	AttachChild(mEBTextRed);
	mEBTextRed->SetPivot(0.0f, 0.5f);
	mEBTextRed->LocalTransform.SetTranslateY(-1.0f);
	mEBTextRed->SetAnchorHor(0.0f, 0.0f);
	mEBTextRed->SetAnchorParamHor(colorPicWidth + textHorAdjust, colorPicWidth + textHorAdjust);
	mEBTextRed->SetAnchorVer(0.0f, 0.0f);
	mEBTextRed->SetAnchorParamVer(anchorVer, anchorVer);
	mEBTextRed->SetSize(textWidth, editBoxHeight);
	mEBTextRed->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	mEBTextRed->GetText()->SetText("Blue");
	mEBTextRed->GetText()->SetFontScale(0.7f);

	float closeButSize = 12.0f;

	mColorPattle = new0 UIFPicBox();
	AttachChild(mColorPattle);
	mColorPattle->LocalTransform.SetTranslateY(-1.0f);
	mColorPattle->SetAnchorHor(0.0f, 1.0f);
	mColorPattle->SetAnchorParamHor(colorPicWidth + closeButSize, -closeButSize);
	mColorPattle->SetAnchorVer(0.0f, 1.0f);
	mColorPattle->SetAnchorParamVer(anchorVer + editBoxHeight*0.5f + closeButSize, -closeButSize);
	mColorPattle->GetUIPicBox()->SetTexture("Data/engine/colorpattle.png");

	mColorPattleCursor = new0 UIFPicBox();
	mColorPattle->AttachChild(mColorPattleCursor);
	mColorPattleCursor->GetUIPicBox()->SetTexture("Data/engine/white.png");
	mColorPattleCursor->LocalTransform.SetTranslateY(-1.0f);
	mColorPattleCursor->SetSize(5.0f, 5.0f);
	mColorPattleCursor->SetAlpha(0.5f);

	UIPicBox *picBox = CreateAddBackgroundPicBox();
	picBox->SetColor(Float3::MakeColor(180, 180, 180));

	mButClose = new0 UIButton();
	AttachChild(mButClose);
	mButClose->LocalTransform.SetTranslateY(-1.0f);
	mButClose->SetAnchorHor(1.0f, 1.0f);
	mButClose->SetAnchorParamHor(-closeButSize*0.5f, -closeButSize*0.5f);
	mButClose->SetAnchorVer(1.0f, 1.0f);
	mButClose->SetAnchorParamVer(-closeButSize*0.5f, -closeButSize*0.5f);
	mButClose->SetSize(closeButSize, closeButSize);
	mButClose->CreateAddText("X");
	mButClose->SetMemUICallback(this,
		(UIFrame::MemUICallback)(&UIColorSelector::_UICallback));

	SetWidget(true);

	SetSize(120.0f, 240.0f);
}
//----------------------------------------------------------------------------
UIColorSelector::~UIColorSelector()
{
}
//----------------------------------------------------------------------------
void UIColorSelector::SetInitColor(const Float3 &color)
{
	mSelectColor = color;
	int red = (int)(mSelectColor[0] * 255.0f);
	int green = (int)(mSelectColor[1] * 255.0f);
	int blue = (int)(mSelectColor[2] * 255.0f);

	mEditBoxRed->SetText(StringHelp::IntToString(red));
	mEditBoxGreen->SetText(StringHelp::IntToString(green));
	mEditBoxBlue->SetText(StringHelp::IntToString(blue));

	Float3 hsl = Float3::RGB2HSL(color);
	float u = hsl[0] / 360.0f;
	float v = hsl[1] / 100.0f;
	float l = hsl[2] / 100.0f;
	mCurUV = Float2(u, v);
	mCurPercent = 1.0f - l;

	mColorPattleCursor->SetAnchorHor(u, u);
	mColorPattleCursor->SetAnchorVer(v, v);
	mSliderBrightness->SetPercent(mCurPercent, false);
	mSliderBrightness->GetFPicBoxBack()->SetColor(color);
}
//----------------------------------------------------------------------------
void UIColorSelector::SetSelectColor(const Float3 &color)
{
	mSelectColor = color;
	int red = (int)(mSelectColor[0] * 255.0f);
	int green = (int)(mSelectColor[1] * 255.0f);
	int blue = (int)(mSelectColor[2] * 255.0f);

	mEditBoxRed->SetText(StringHelp::IntToString(red));
	mEditBoxGreen->SetText(StringHelp::IntToString(green));
	mEditBoxBlue->SetText(StringHelp::IntToString(blue));

	mSliderBrightness->GetFPicBoxBack()->SetColor(color);

	_UICallbacksCalls(UICT_COLOR_SELECT);
}
//----------------------------------------------------------------------------
const Float3 &UIColorSelector::GetSelectColor() const
{
	return mSelectColor;
}
//----------------------------------------------------------------------------
Float2 UIColorSelector::_GetUV(const Float2 &pickPos, const Rectf &worldRect)
{
	float xDis = pickPos[0] - worldRect.Left;
	float yDis = pickPos[1] - worldRect.Bottom;

	float xPercent = xDis / worldRect.Width();
	float yPercent = yDis / worldRect.Height();

	float u = Mathf::Clamp(xPercent, 0.0f, 1.0f);
	float v = Mathf::Clamp(yPercent, 0.0f, 1.0f);

	return Float2(u, v);
}
//----------------------------------------------------------------------------
Float3 UIColorSelector::_GetColor(const Float2 &uv, float percent)
{
	Float3 color = Float3::HSL2RGB(Float3(uv[0] * 360.0f, uv[1] * 100.0f,
		percent*100.0f));
	return color;
}
//----------------------------------------------------------------------------
void UIColorSelector::OnWidgetPicked(const CanvasInputData &inputData)
{
	UIFrame::OnWidgetPicked(inputData);

	APoint worldPos = inputData.LogicPos;

	const Rectf &pattleRect = mColorPattle->GetWorldRect();
	Float2 pos2D = Float2(worldPos.X(), worldPos.Z());

	if (UIPT_PRESSED == inputData.PickType)
	{
		mIsPickedPattle = false;

		if (pattleRect.IsInsize(pos2D))
		{
			mIsPickedPattle = true;

			mCurUV = _GetUV(pos2D, pattleRect);
			_UpdateColor();
		}
	}
	else if (UIPT_RELEASED == inputData.PickType)
	{
		mIsPickedPattle = false;
	}
	else if (UIPT_MOVED == inputData.PickType)
	{
		if (mIsPickedPattle)
		{
			mCurUV = _GetUV(pos2D, pattleRect);
			_UpdateColor();
		}
	}
}
//----------------------------------------------------------------------------
void UIColorSelector::_UpdateColor()
{
	mColorPattleCursor->SetAnchorHor(mCurUV[0], mCurUV[0]);
	mColorPattleCursor->SetAnchorVer(mCurUV[1], mCurUV[1]);
	Float3 color = _GetColor(mCurUV, 1.0f-mCurPercent);
	SetSelectColor(color);
}
//----------------------------------------------------------------------------
void UIColorSelector::OnSizeNodeNotPicked(const CanvasInputData &inputData)
{
	UIFrame::OnSizeNodeNotPicked(inputData);

	if (UIPT_PRESSED == inputData.PickType ||
		UIPT_RELEASED == inputData.PickType)
	{
		mIsPickedPattle = false;
	}
}
//----------------------------------------------------------------------------
void UIColorSelector::_UICallback(UIFrame *frame, UICallType type)
{
	PX2_UNUSED(frame);

	UIButton *but = DynamicCast<UIButton>(frame);
	UISlider *slider = DynamicCast<UISlider>(frame);

	if (but)
	{
		if (UICT_RELEASED == type)
		{
			Show(false);
		}
	}
	else if (slider)
	{
		if (UICT_SLIDERCHANGED == type)
		{
			mCurPercent = mSliderBrightness->GetPercent();
			_UpdateColor();
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
void UIColorSelector::RegistProperties()
{
	UIFrame::RegistProperties();

	AddPropertyClass("UIColorSelector");
}
//----------------------------------------------------------------------------
void UIColorSelector::OnPropertyChanged(const PropertyObject &obj)
{
	UIFrame::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIColorSelector::UIColorSelector(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UIColorSelector::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIColorSelector, source);
}
//----------------------------------------------------------------------------
void UIColorSelector::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UIColorSelector::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIColorSelector::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIColorSelector::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIColorSelector, target);
}
//----------------------------------------------------------------------------
int UIColorSelector::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------