// PX2UISlider.cpp

#include "PX2UISlider.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2ScriptManager.hpp"
#include "PX2UIDefine.hpp"
#include "PX2Math.hpp"
#include "PX2RenderWindow.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UISlider);
PX2_IMPLEMENT_STREAM(UISlider);
PX2_IMPLEMENT_FACTORY(UISlider);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UISlider);

//----------------------------------------------------------------------------
UISlider::UISlider() :
mIsNeedReGenSliderLayout(true),
mButSliderLength(20.0f),
mPercent(0.0f),
mIsNeededUpdate(true),
mIsDraging(false),
mPickTouchID(-1)
{
	SetName("UISlider");

	mDirectionType = DT_HORIZONTAL;

	SetAnchorHor(0.5f, 0.5f);
	SetAnchorVer(0.5f, 0.5f);
	SetSize(200.0f, 20.0f);

	_GenSlider();

	SetWidget(true);
}
//----------------------------------------------------------------------------
UISlider::~UISlider()
{
}
//----------------------------------------------------------------------------
void UISlider::SetDirectionType(DirectionType dt)
{
	mDirectionType = dt;
	mIsNeedReGenSliderLayout = true;
}
//----------------------------------------------------------------------------
void UISlider::SetSliderLength(float length)
{
	mButSliderLength = length;
	mIsNeededUpdate = true;
}
//----------------------------------------------------------------------------
void UISlider::SetPercent(float percent, bool doCall)
{
	mPercent = percent;

	mIsNeededUpdate = true;

	if (doCall)
	{
		_UICallbacksCalls(UICT_SLIDERCHANGED);
	}
}
//----------------------------------------------------------------------------
void UISlider::SetContentFrame(UIFrame *contentFrame)
{
	if (mContentFrame)
	{
		mContentFrame->SetSizeChangeCallback(0, 0);
	}

	mContentFrame = contentFrame;

	if (mContentFrame)
	{
		mContentFrame->SetSizeChangeCallback(this,
			(SizeNode::SizeChangeCallback)(&UISlider::_RelativeSizeChangeCallback));
	}
}
//----------------------------------------------------------------------------
UIFText *UISlider::CreateAddText(const std::string &text,
	const Float3& color)
{
	if (mFText)
	{
		DetachChild(mFText);
		mFText = 0;
	}

	mFText = new0 UIFText();
	AttachChild(mFText);
	mFText->SetWidget(false);
	mFText->SetDoPick(false);
	mFText->LocalTransform.SetTranslateY(-5.0f);
	mFText->GetText()->SetFontColor(color);
	mFText->GetText()->SetText(text);
	mFText->SetAnchorHor(0.0f, 1.0f);
	mFText->SetAnchorVer(0.0f, 1.0f);
	mFText->SetColorSelfCtrled(true);

	return mFText;
}
//----------------------------------------------------------------------------
UIFText *UISlider::GetFText()
{
	return mFText;
}
//----------------------------------------------------------------------------
UIText *UISlider::GetText()
{
	if (!mFText)
		return 0;

	return mFText->GetText();
}
//----------------------------------------------------------------------------
void UISlider::_RelativeSizeChangeCallback(SizeNode *tellObject)
{
	PX2_UNUSED(tellObject);

	if (mContentFrame)
		_UpdateSliderLength();
}
//----------------------------------------------------------------------------
void UISlider::OnBeAttached()
{
	UIFrame::OnBeAttached();

	SizeNode *sz = DynamicCast<SizeNode>(GetParent());
	if (sz)
	{
		sz->SetSizeChangeCallback(this, 
			(SizeNode::SizeChangeCallback)(&UISlider::_RelativeSizeChangeCallback));
	}
}
//----------------------------------------------------------------------------
void UISlider::OnBeDetach()
{
	UIFrame::OnBeDetach();

	SizeNode *parent = DynamicCast<SizeNode>(GetParent());
	if (parent)
	{
		parent->SetSizeChangeCallback(0, 0);
	}
}
//----------------------------------------------------------------------------
void UISlider::OnSizeChanged()
{
	UIFrame::OnSizeChanged();

	mIsNeededUpdate = true;
}
//----------------------------------------------------------------------------
void UISlider::UpdateWorldData(double applicationTime, double elapsedTime)
{
	if (mIsNeedReGenSliderLayout)
		_ReGenSliderLayout();

	if (mIsNeededUpdate)
		_UpdateSlider();

	UIFrame::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void UISlider::_GenSlider()
{
	mFPicBoxBack = new0 UIFPicBox();
	AttachChild(mFPicBoxBack);
	mFPicBoxBack->GetUIPicBox()->SetTexture("Data/engine/white.png");

	mFPicBoxBack->SetAnchorHor(0.0f, 1.0f);
	mFPicBoxBack->SetAnchorVer(0.0f, 1.0f);

	mButSlider = new0 UIButton();
	AttachChild(mButSlider);
	mButSlider->LocalTransform.SetTranslateY(-1.0f);

	UIPicBox *picBoxNormal = mButSlider->GetPicBoxAtState(UIButtonBase::BS_NORMAL);
	PX2_UNUSED(picBoxNormal);
	mButSlider->SetStateColor(UIButtonBase::BS_NORMAL, Float3::MakeColor(150, 150, 150));
	mButSlider->SetStateColor(UIButtonBase::BS_HOVERED, Float3::MakeColor(175, 175, 175));
	mButSlider->SetStateColor(UIButtonBase::BS_PRESSED, Float3::MakeColor(200, 200, 200));

	mButSlider->SetMemUICallback(this, (UIFrame::MemUICallback)(&UISlider::_SliderDrag));
}
//----------------------------------------------------------------------------
void UISlider::_UpdateSliderLength()
{
	SizeNode *parentSizeNode = DynamicCast<UIFrame>(GetParent());
	const Sizef &cotSize = mContentFrame->GetSize();

	float sliderButLength = 20.0f;

	float judgeLengthPar = 0.0f;
	float judgeLengthCot = 0.0f;

	if (DT_HORIZONTAL == mDirectionType)
	{
		judgeLengthPar = parentSizeNode->GetWidth();
		judgeLengthCot = cotSize.Width;
	}
	else
	{
		judgeLengthPar = parentSizeNode->GetHeight();
		judgeLengthCot = cotSize.Height;
	}

	if (judgeLengthCot <= judgeLengthPar)
	{
		sliderButLength = judgeLengthPar;
	}
	else
	{
		sliderButLength = judgeLengthPar*(judgeLengthPar / judgeLengthCot);
	}

	SetSliderLength(sliderButLength);
}
//----------------------------------------------------------------------------
void UISlider::_ReGenSliderLayout()
{
	if (DT_HORIZONTAL == mDirectionType)
	{
		mButSlider->SetSize(Sizef(mButSliderLength, 0.0f));

		mButSlider->SetAnchorHor(0.0f, 0.0f);
		mButSlider->SetAnchorParamHor(mButSliderLength / 2.0f, 0.0f);
		mButSlider->SetAnchorVer(0.0f, 1.0f);
		mButSlider->SetAnchorParamVer(0.0f, 0.0f);
	}
	else
	{
		mButSlider->SetSize(Sizef(0.0f, mButSliderLength));

		mButSlider->SetAnchorHor(0.0f, 1.0f);
		mButSlider->SetAnchorParamHor(0.0f, 0.0f);
		mButSlider->SetAnchorVer(1.0f, 1.0f);
		mButSlider->SetAnchorParamVer(-mButSliderLength / 2.0f, 0.0f);
	}

	mIsNeedReGenSliderLayout = false;
}
//----------------------------------------------------------------------------
void UISlider::_SetDraging(bool draging)
{
	mIsDraging = draging;

	if (mIsDraging)
	{
		const CanvasInputData &lastPickData = mButSlider->GetLastPickData();
		mPickTouchID = lastPickData.TouchID;
	}
	else if (!mIsDraging)
	{
		mPickTouchID = -1;
	}
}
//----------------------------------------------------------------------------
void UISlider::_SliderDrag(UIFrame *frame, UICallType type)
{
	if (UICT_PRESSED == type)
	{
		if (frame == mButSlider)
		{
			_SetDraging(true);
		}
	}
	else if (UICT_RELEASED == type)
	{
		if (frame == mButSlider)
		{
			_SetDraging(false);
		}
	}
	else if (UICT_RELEASED_NOTPICK == type)
	{
		if (frame == mButSlider)
		{
			_SetDraging(false);
		}
	}
}
//----------------------------------------------------------------------------
void UISlider::OnSizeNodePicked(const CanvasInputData &inputData)
{
	if (UIPT_MOVED == inputData.PickType && mPickTouchID == inputData.TouchID)
	{
		if (IsDraging())
		{
			const Sizef &size = GetSize();
			AVector moveDelta = inputData.MoveDelta;

			float percent = GetPercent();
			float addPercent = 0.0f;

			if (DT_HORIZONTAL == mDirectionType)
			{
				float dist = size.Width - mButSliderLength;
				if (0.0f != dist)
					addPercent = moveDelta.X() / dist;
			}
			else
			{
				float dist = size.Height - mButSliderLength;
				if (0.0f != dist)
					addPercent = -moveDelta.Z() / dist;
			}

			percent += addPercent;

			if (percent > 1.0f) 
				percent = 1.0f;
			if (percent < 0.0f) 
				percent = 0.0f;

			SetPercent(percent);
		}
	}
}
//----------------------------------------------------------------------------
void UISlider::OnSizeNodeNotPicked(const CanvasInputData &inputData)
{
	UIFrame::OnSizeNodeNotPicked(inputData);

	if (UIPT_MOVED == inputData.PickType && mPickTouchID == inputData.TouchID)
	{
		if (IsDraging())
		{
			const Sizef &size = GetSize();
			const AVector &moveDelta = inputData.MoveDelta;

			float percent = GetPercent();
			float addPercent = 0.0f;

			if (DT_HORIZONTAL == mDirectionType)
			{
				float dist = size.Width - mButSliderLength;
				if (0.0f != dist)
					addPercent = moveDelta.X() / dist;
			}
			else
			{
				float dist = size.Height - mButSliderLength;
				if (0.0f != dist)
					addPercent = -moveDelta.Z() / dist;
			}
			
			percent += addPercent;

			if (percent > 1.0f) percent = 1.0f;
			if (percent < 0.0f) percent = 0.0f;

			SetPercent(percent);
		}
	}
}
//----------------------------------------------------------------------------
void UISlider::_UpdateSlider()
{
	const Sizef &size = GetSize();

	if (DT_HORIZONTAL == mDirectionType)
	{
		float paramHor = Lerp<float, float>(mButSliderLength / 2.0f,
			size.Width - mButSliderLength/2.0f, mPercent);

		mButSlider->SetSize(mButSliderLength, GetSize().Height);
		mButSlider->SetAnchorParamHor(paramHor, 0.0f);
	}
	else
	{
		float paramVer = Lerp<float, float>(mButSliderLength / 2.0f,
			size.Height - mButSliderLength / 2.0f, mPercent);

		mButSlider->SetSize(GetSize().Width, mButSliderLength);
		mButSlider->SetAnchorVer(1.0f, 1.0f);
		mButSlider->SetAnchorParamVer(-paramVer, 0.0f);
	}

	mIsNeededUpdate = false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void UISlider::RegistProperties()
{
	UIFrame::RegistProperties();

	AddPropertyClass("UISlider");
}
//----------------------------------------------------------------------------
void UISlider::OnPropertyChanged(const PropertyObject &obj)
{
	UIFrame::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UISlider::UISlider(LoadConstructor value) :
UIFrame(value),
mIsNeedReGenSliderLayout(true),
mButSliderLength(20.0f),
mPercent(0.0f),
mIsNeededUpdate(true),
mIsDraging(false),
mPickTouchID(-1)
{
}
//----------------------------------------------------------------------------
void UISlider::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);
	
	source.ReadEnum(mDirectionType);
	source.ReadPointer(mFPicBoxBack);
	source.ReadPointer(mButSlider);
	source.Read(mButSliderLength);
	source.Read(mPercent);

	source.ReadPointer(mContentFrame);

	source.ReadPointer(mFText);

	PX2_END_DEBUG_STREAM_LOAD(UISlider, source);
}
//----------------------------------------------------------------------------
void UISlider::Link(InStream& source)
{
	UIFrame::Link(source);

	if (mFPicBoxBack)
	{
		source.ResolveLink(mFPicBoxBack);
	}

	if (mButSlider)
	{
		source.ResolveLink(mButSlider);

		mButSlider->SetMemUICallback(this, 
			(UIFrame::MemUICallback)(&UISlider::_SliderDrag));
	}

	if (mContentFrame)
	{
		source.ResolveLink(mContentFrame);
	}

	if (mFText)
	{
		source.ResolveLink(mFText);
	}
}
//----------------------------------------------------------------------------
void UISlider::PostLink()
{
	UIFrame::PostLink();

	if (mFPicBoxBack)
	{
		mFPicBoxBack->PostLink();
	}

	if (mButSlider)
	{
		mButSlider->PostLink();
	}

	if (mContentFrame)
	{
		mContentFrame->PostLink();
	}

	if (mFText)
	{
		mFText->PostLink();
	}
}
//----------------------------------------------------------------------------
bool UISlider::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		target.Register(mFPicBoxBack);
		
		target.Register(mButSlider);

		target.Register(mContentFrame);

		target.Register(mFText);
		
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UISlider::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mDirectionType);

	target.WritePointer(mFPicBoxBack);

	target.WritePointer(mButSlider);

	target.Write(mButSliderLength);
	target.Write(mPercent);

	target.WritePointer(mContentFrame);

	target.WritePointer(mFText);

	PX2_END_DEBUG_STREAM_SAVE(UISlider, target);
}
//----------------------------------------------------------------------------
int UISlider::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_ENUMSIZE(mDirectionType);

	size += PX2_POINTERSIZE(mFPicBoxBack);
	size += PX2_POINTERSIZE(mButSlider);

	size += sizeof(mButSliderLength);
	size += sizeof(mPercent);

	size += PX2_POINTERSIZE(mContentFrame);

	size += PX2_POINTERSIZE(mFText);

	return size;
}
//----------------------------------------------------------------------------
