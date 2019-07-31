// PX2UITree.cpp

#include "PX2UITree.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UITree);
PX2_IMPLEMENT_STREAM(UITree);
PX2_IMPLEMENT_FACTORY(UITree);

//----------------------------------------------------------------------------
UITree::UITree() :
mSliderSize(10),
mItemHeight(20.0f),
mIconArrowSpace(16.0f),
mIsShowRootItem(false),
mIsNeedUpdateContentPos(true)
{
	mMaskFrame = new0 UIFrame();
	AttachChild(mMaskFrame);
	mMaskFrame->CreateAddMask();
	mMaskFrame->LocalTransform.SetTranslateY(-1.0f);
	mMaskFrame->SetAnchorHor(0.0f, 1.0f);
	mMaskFrame->SetAnchorParamHor(0.0f, -mSliderSize);
	mMaskFrame->SetAnchorVer(0.0f, 1.0f);
	mMaskFrame->SetAnchorParamVer(0.0f, 0.0f);

	mContentFrame = new0 UIFrame();
	mMaskFrame->AttachChild(mContentFrame);
	mContentFrame->LocalTransform.SetTranslateY(-1.0f);
	mContentFrame->SetAnchorHor(0.0f, 1.0f);
	mContentFrame->SetAnchorParamHor(0.0f, 1.0f);
	mContentFrame->SetAnchorVer(1.0f, 1.0f);
	mContentFrame->SetAnchorParamVer(0.0f, 0.0f);
	mContentFrame->SetPivot(0.5f, 1.0f);

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
		(UIFrame::MemUICallback)(&UITree::_SliderCallback));

	mRootItem = new0 UIItem();
	mContentFrame->AttachChild(mRootItem);
	mRootItem->LocalTransform.SetTranslateY(-2.0f);
	mRootItem->SetSize(0.0f, mItemHeight);
	mRootItem->SetAnchorHor(0.0f, 1.0f);
	mRootItem->SetAnchorVer(1.0f, 1.0f);
	mRootItem->SetAnchorParamVer(-mItemHeight / 2.0f, 0.0f);
	mRootItem->SetLabel("RootItem");
	mRootItem->SetName("RootItem");
	mRootItem->SetRootItem(true);

	SetChildPickOnlyInSizeRange(true);
}
//----------------------------------------------------------------------------
UITree::~UITree()
{
}
//----------------------------------------------------------------------------
void UITree::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	bool isNeedRecal = mRootItem->_IsNeedRecal();

	if (mIsNeedUpdateContentPos)
	{
		_UpdateContentPos();
	}

	UIFrame::UpdateWorldData(applicationTime, elapsedTime);

	if (isNeedRecal)
	{
		float level = mRootItem->_GetLevel();
		int numAll = mRootItem->GetNumAllChildsExpand();
		float height = (numAll + level) * mRootItem->GetSize().Height;
		mContentFrame->SetSize(0.0f, height);
		mContentFrame->Update(applicationTime, 0.0f);

		mIsNeedUpdateContentPos = true;
	}
}
//----------------------------------------------------------------------------
void UITree::OnSizeNodePicked(const CanvasInputData &inputData)
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
void UITree::_UpdateContentPos()
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

	mIsNeedUpdateContentPos = false;
}
//----------------------------------------------------------------------------
void UITree::_SliderCallback(UIFrame *frame, UICallType type)
{
	UISlider *slider = DynamicCast<UISlider>(frame);
	if (slider)
	{
		if (UICT_SLIDERCHANGED == type)
		{
			mIsNeedUpdateContentPos = true;
		}
	}
}
//----------------------------------------------------------------------------
void UITree::OnSizeChanged()
{
	mIsNeedUpdateContentPos = true;

	UIFrame::OnSizeChanged();
}
//----------------------------------------------------------------------------
void UITree::SetItemHeight(float height)
{
	mItemHeight = height;
}
//----------------------------------------------------------------------------
void UITree::SetIconArrowSpace(float space)
{
	mIconArrowSpace = space;
}
//----------------------------------------------------------------------------
void UITree::SelectCallback(Object *frame, UICallType type)
{
	if (type == UICT_PRESSED)
	{
		UIButton *button = DynamicCast<UIButton>(frame);
		if (button)
		{
			UIItem *uiItem = DynamicCast<UIItem>(button->GetParent());
			if (uiItem)
			{
				ClearAllSelectItems(true);
				AddSelectItem(uiItem, false, true);
			}
		}
	}
	else if (type == UICT_DOUBLE_PRESSED)
	{
		UIButton *button = DynamicCast<UIButton>(frame);
		if (button)
		{
			UIItem *uiItem = DynamicCast<UIItem>(button->GetParent());
			if (uiItem)
			{
				ClearAllSelectItems(true);
				AddSelectItem(uiItem, true, true);
			}
		}
	}
}
//----------------------------------------------------------------------------
UIItem *UITree::AddItem(UIItem *parentItem, const std::string &label, 
	const std::string &name, Object *obj)
{
	UIItem *item = parentItem->AddItem(label, name, obj);
	UIButton *butBack = item->GetButBack();
	if (butBack)
	{
		butBack->SetMemUICallback(this,
			(UIFrame::MemUICallback)(&UITree::SelectCallback));
	}

	if (!name.empty())
	{
		mTagItems[name] = item;
	}

	mMaskFrame->SetNeedAdjustChildrenMask(true);

	return item;
}
//----------------------------------------------------------------------------
UIItem *UITree::GetItemByObject(Object *obj)
{
	if (mRootItem)
		return mRootItem->GetItemByObject(obj);

	return 0;
}
//----------------------------------------------------------------------------
bool UITree::RemoveItem(UIItem *item)
{
	if (!item) return false;

	if (item)
	{
		UIItem *itemParent = DynamicCast<UIItem>(item->GetParent());
		if (itemParent)
		{
			return itemParent->RemoveItem(item);
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void UITree::OnSelected(UIItem *item, bool isDoubleSelect)
{
	UICallType callType = UICT_TREE_SELECTED;
	if (isDoubleSelect) callType = UICT_TREE_DOUBLE_SELECTED;
	_UICallbacksCalls(callType);

	PX2_UNUSED(item);
}
//----------------------------------------------------------------------------
void UITree::RemoveAllItemsExceptRoot()
{
	if (mRootItem)
	{
		mRootItem->RemoveAllChildItems();
	}

	mTagItems.clear();
}
//----------------------------------------------------------------------------
void UITree::ShowRootItem(bool show)
{
	mIsShowRootItem = show;

	if (mRootItem)
	{
		mRootItem->ShowItem(show);
	}
}
//----------------------------------------------------------------------------
void UITree::AddSelectItem(UIItem *item, bool isDoubleSelect, 
	bool callOnSelected)
{
	if (item->IsSelected())
		return;

	item->Select(true);

	mSelectedItems.push_back(item);

	if (callOnSelected)
		OnSelected(item, isDoubleSelect);
}
//----------------------------------------------------------------------------
void UITree::ClearAllSelectItems(bool callOnSelected)
{
	for (int i = 0; i < (int)mSelectedItems.size(); i++)
	{
		mSelectedItems[i]->Select(false);
	}

	mSelectedItems.clear();

	if (callOnSelected)
		OnSelected(0, false);
}
//----------------------------------------------------------------------------
UIItem *UITree::GetSelectedItem()
{
	if ((int)mSelectedItems.size() > 0)
	{
		return mSelectedItems[0];
	}

	return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UITree::UITree(LoadConstructor value) :
UIFrame(value),
mSliderSize(10),
mItemHeight(20.0f),
mIconArrowSpace(12.0f),
mIsShowRootItem(false),
mIsNeedUpdateContentPos(true)
{
	SetChildPickOnlyInSizeRange(true);
}
//----------------------------------------------------------------------------
void UITree::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UITree, source);
}
//----------------------------------------------------------------------------
void UITree::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UITree::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UITree::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UITree::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UITree, target);
}
//----------------------------------------------------------------------------
int UITree::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------