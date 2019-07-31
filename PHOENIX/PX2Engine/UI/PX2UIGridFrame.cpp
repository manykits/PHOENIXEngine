// PX2UIGridFrame.cpp

#include "PX2UIGridFrame.hpp"
#include "PX2UIFrameGridAlignControl.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIGridFrame);
PX2_IMPLEMENT_STREAM(UIGridFrame);
PX2_IMPLEMENT_FACTORY(UIGridFrame);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UIGridFrame);

//----------------------------------------------------------------------------
UIGridFrame::UIGridFrame():
mIsUpdateSliderVisible(true),
mSliderSize(10),
mItemHeight(20.0f),
mIsUpdateContentPos(true),
mSelectedIndex(-1)
{
	mMaskFrame = new0 UIFrame();
	AttachChild(mMaskFrame);
	mMaskFrame->LocalTransform.SetTranslateY(-1.0f);
	mMaskFrame->CreateAddMask();
	mMaskFrame->SetAnchorHor(0.0f, 1.0f);
	mMaskFrame->SetAnchorParamHor(0.0f, -mSliderSize);
	mMaskFrame->SetAnchorVer(0.0f, 1.0f);
	mMaskFrame->SetAnchorParamVer(0.0f, 0.0f);

	mContentFrame = new0 UIFrame();
	mMaskFrame->AttachChild(mContentFrame);
	mContentFrame->LocalTransform.SetTranslateY(-1.0f);
	mContentFrame->SetAnchorHor(0.0f, 1.0f);
	mContentFrame->SetAnchorParamHor(0.0f, 0.0f);
	mContentFrame->SetAnchorVer(1.0f, 1.0f);
	mContentFrame->SetAnchorParamVer(0.0f, 0.0f);
	mContentFrame->SetPivot(0.5f, 1.0f);
	UIFrameGridAlignControl *gridAlignCtrl =
		mContentFrame->CreateAddGridAlignCtrl();
	gridAlignCtrl->SetChildAlignment(UIFrameGridAlignControl::CAT_UPPER_CENTER);
	gridAlignCtrl->SetBorder(10.0f, 10.0f, 10.0f, 10.0f);

	mSlider = new0 UISlider();
	mSlider->LocalTransform.SetTranslateY(-1.0f);
	mSlider->SetDirectionType(UISlider::DT_VERTICALITY);
	mSlider->EnableAnchorLayout(true);
	mSlider->SetAnchorHor(1.0f, 1.0f);
	mSlider->SetAnchorParamHor(-mSliderSize*0.5f, 0.0f);
	mSlider->SetAnchorVer(0.0f, 1.0f);
	mSlider->SetSize(mSliderSize, 0.0f);
	mSlider->SetPivot(0.5f, 0.5f);
	AttachChild(mSlider);
	mSlider->SetContentFrame(mContentFrame);
	mSlider->SetMemUICallback(this, 
		(UIFrame::MemUICallback)(&UIGridFrame::_SliderCallback));

	SetChildPickOnlyInSizeRange(true);
}
//----------------------------------------------------------------------------
UIGridFrame::~UIGridFrame()
{
}
//----------------------------------------------------------------------------
void UIGridFrame::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	if (mIsUpdateContentPos)
	{
		_UpdateContentPos();
	}

	UIFrame::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void UIGridFrame::OnSizeNodePicked(const CanvasInputData &inputData)
{
	UIFrame::OnSizeNodePicked(inputData);

	if (UIPT_WHELLED == inputData.PickType)
	{
		float percent = mSlider->GetPercent();
		percent -= inputData.Wheel * 0.008f;
		percent = Mathf::Clamp(percent, 0.0f, 1.0f);
		mSlider->SetPercent(percent);
	}
}
//----------------------------------------------------------------------------
void UIGridFrame::_UpdateContentPos()
{
	float heightDist = mContentFrame->GetSize().Height - GetSize().Height;
	if (heightDist > 0.0f)
	{
		float downMove = heightDist * mSlider->GetPercent();
		mContentFrame->SetAnchorParamVer(downMove, 0.0f);
	}
	else
	{
		mContentFrame->SetAnchorParamVer(0.0f, 0.0f);
	}

	mIsUpdateContentPos = false;
}
//----------------------------------------------------------------------------
void UIGridFrame::_SliderCallback(UIFrame *frame, UICallType type)
{
	UISlider *slider = DynamicCast<UISlider>(frame);
	if (slider)
	{
		if (UICT_SLIDERCHANGED == type)
		{
			mIsUpdateContentPos = true;
			mIsUpdateSliderVisible = true;
		}
	}
}
//----------------------------------------------------------------------------
void UIGridFrame::AddItem(UIFrame *item)
{
	mContentFrame->AttachChild(item);
	mItems.push_back(item);

	item->SetUserData("index", (int)(mItems.size() - 1));
}
//----------------------------------------------------------------------------
void UIGridFrame::RemoveAllItems()
{
	for (int i = 0; i < (int)mItems.size(); i++)
	{
		mContentFrame->DetachChild(mItems[i]);
	}
	mItems.clear();
}
//----------------------------------------------------------------------------
void UIGridFrame::SetSliderPercent(float percent)
{
	mSlider->SetPercent(percent);
}
//----------------------------------------------------------------------------
UIFrame *UIGridFrame::GetContentFrame()
{
	return mContentFrame;
}
//----------------------------------------------------------------------------
void UIGridFrame::OnSizeChanged()
{
	UIFrame::OnSizeChanged();

	UIFrameGridAlignControl *gridAlignCtrl = mContentFrame->GetGridAlignCtrl();
	if (gridAlignCtrl)
	{
		gridAlignCtrl->MarkLayoutChanged();
	}

	mIsUpdateContentPos = true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIGridFrame::UIGridFrame(LoadConstructor value) :
UIFrame(value)
{
}
//----------------------------------------------------------------------------
void UIGridFrame::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIGridFrame, source);
}
//----------------------------------------------------------------------------
void UIGridFrame::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UIGridFrame::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIGridFrame::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIGridFrame::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIGridFrame, target);
}
//----------------------------------------------------------------------------
int UIGridFrame::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------