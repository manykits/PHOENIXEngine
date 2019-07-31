// PX2UIList.cpp

#include "PX2UIList.hpp"
#include "PX2Log.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIList);
PX2_IMPLEMENT_STREAM(UIList);
PX2_IMPLEMENT_FACTORY(UIList);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UIList);

//----------------------------------------------------------------------------
UIList::UIList() :
mIsNeedRecal(false),
mFontSize(16),
mIsUpdateSliderVisible(true),
mSliderSize(10),
mItemHeight(20.0f),
mTextAlignLeftPos(10.0f),
mIsUpdateContentPos(true),
mIsReleasedDoSelect(false),
mSelectedIndex(-1),
mNumMaxItems(-1)
{
	mMaskFrame = new0 UIFrame();
	AttachChild(mMaskFrame);
	mMaskFrame->LocalTransform.SetTranslateY(-1.0f);
	mMaskFrame->CreateAddMask();
	mMaskFrame->SetAnchorHor(0.0f, 1.0f);
	mMaskFrame->SetAnchorParamHor(0.0f, -mSliderSize);
	mMaskFrame->SetAnchorVer(0.0f, 1.0f);

	mContentFrame = new0 UIFrame();
	mMaskFrame->AttachChild(mContentFrame);
	mContentFrame->LocalTransform.SetTranslateY(-1.0f);
	mContentFrame->SetAnchorHor(0.0f, 1.0f);
	mContentFrame->SetAnchorParamHor(0.0f, 0.0f);
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
	mSlider->SetMemUICallback(this, (UIFrame::MemUICallback)(&UIList
		::_SliderCallback));
	mSlider->SetPercent(0.0f, false);

	SetChildPickOnlyInSizeRange(true);

	mTextColor = Float3::WHITE;
}
//----------------------------------------------------------------------------
UIList::~UIList()
{
}
//----------------------------------------------------------------------------
UISlider *UIList::GetSlider()
{
	return mSlider;
}
//----------------------------------------------------------------------------
void UIList::_SliderCallback(UIFrame *frame, UICallType type)
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
void UIList::_UpdateContentPos()
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
void UIList::_UpdateItemVisible()
{
	for (int i = 0; i < mContentFrame->GetNumChildren(); i++)
	{
		UIItem *item = DynamicCast<UIItem>(mContentFrame->GetChild(i));

		if (item)
		{
			bool intersect = IsIntersectSizeRange(item);
			item->Show(intersect);
		}
	}

	mIsUpdateSliderVisible = false;
}
//----------------------------------------------------------------------------
void UIList::_SelectButCallback(UIFrame *frame, UICallType type)
{
	UIButton *button = DynamicCast<UIButton>(frame);
	if (button)
	{
		UIItem *uiItem = DynamicCast<UIItem>(button->GetParent());
		if (uiItem)
		{
			if (!mIsReleasedDoSelect)
			{
				if (UICT_PRESSED == type)
				{
					_OnSelectProcess(uiItem);
				}
			}
			else
			{
				if (UICT_RELEASED == type)
				{
					_OnSelectProcess(uiItem);
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void UIList::_OnSelectProcess(UIItem *uiItem)
{
	ClearAllSelectItems();
	AddSelectItem(uiItem);

	OnSelected(uiItem);
}
//----------------------------------------------------------------------------
void UIList::SetSliderSize(float size)
{
	mSliderSize = size;
	mSlider->SetSize(mSliderSize, 0.0f);
	mSlider->SetAnchorParamHor(-mSliderSize*0.5f, 0.0f);

	mMaskFrame->SetAnchorParamHor(0.0f, -mSliderSize);
}
//----------------------------------------------------------------------------
void UIList::SetItemHeight(float height)
{
	mItemHeight = height;

	mIsNeedRecal = true;
}
//----------------------------------------------------------------------------
void UIList::SetItemBackPicBox(const std::string &backPicBox)
{
	mItemBackPicBox = backPicBox;
}
//----------------------------------------------------------------------------
void UIList::SetTextAlignLeftPos(float isAlignLeft)
{
	mTextAlignLeftPos = isAlignLeft;
}
//----------------------------------------------------------------------------
float UIList::GetTextAlignLeftPos() const
{
	return mTextAlignLeftPos;
}
//----------------------------------------------------------------------------
UIItem *UIList::AddItem(const std::string &text)
{
	if (mNumMaxItems > 0 && GetNumItems() >= GetNumMaxItems())
	{
		RemoveItem(GetItemByIndex(0));
	}

	mMaskFrame->SetNeedAdjustChildrenMask(true);

	UIItem *item = new0 UIItem();
	item->GetButBack()->SetStateColor(UIButtonBase::BS_NORMAL, mItemBackColor);
	item->GetButBack()->SetStateColor(UIButtonBase::BS_HOVERED, mItemBackColor);
	item->GetButBack()->SetStateColor(UIButtonBase::BS_PRESSED, mItemBackColor);
	mContentFrame->AttachChild(item);
	mItems.push_back(item);

	item->GetFText()->SetAnchorParamHor(mTextAlignLeftPos, mTextAlignLeftPos);
	item->GetFText()->GetText()->SetText(text);
	item->GetFText()->GetText()->SetAutoWarp(true);
	item->GetFText()->GetText()->SetFontColor(mTextColor);
	if (!mFont.empty())
		item->GetFText()->GetText()->SetFont(mFont);
	item->GetFText()->GetText()->SetFontWidthHeight(mFontSize, mFontSize);

	UIButton *butBack = item->GetButBack();
	if (butBack)
	{
		butBack->SetMemUICallback(this,
			(UIFrame::MemUICallback)(&UIList::_SelectButCallback));

		if (!mItemBackPicBox.empty())
		{
			butBack->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetPicBoxType(UIPicBox::PBT_NINE);
			butBack->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexCornerSize(16, 16, 16, 16);
			butBack->GetPicBoxAtState(UIButtonBase::BS_NORMAL)->SetTexture(mItemBackPicBox);
		}
	}

	item->SetUserData("index", (int)(mItems.size() - 1));

	mIsNeedRecal = true;
	mIsUpdateSliderVisible = true;

	return item;
}
//----------------------------------------------------------------------------
UIItem *UIList::GetItem(const std::string &text)
{
	for (int i = 0; i < (int)mItems.size(); i++)
	{
		if (text == mItems[i]->GetLabel())
			return mItems[i];
	}

	return 0;
}
//----------------------------------------------------------------------------
UIItem *UIList::GetItemByIndex(int index)
{
	if (0 <= index && index < (int)mItems.size())
	{
		return mItems[index];
	}

	return 0;
}
//----------------------------------------------------------------------------
void UIList::RemoveItem(UIItem *item)
{
	mContentFrame->DetachChild(item);

	auto it = mItems.begin();
	for (; it != mItems.end();)
	{
		if (item == *it)
		{
			mItems.erase(it);
			return;
		}
		else
		{
			it++;
		}
	}
}
//----------------------------------------------------------------------------
UIItem *UIList::GetItemByUserDataString(const std::string &userDataName, 
	const std::string &userDataString)
{
	for (int i = 0; i < (int)mItems.size(); i++)
	{
		UIItem *item = mItems[i];

		bool isValied = false;
		std::string userDataStr = item->GetUserDataString(userDataName, &isValied);
		if (isValied)
		{
			if (userDataStr == userDataString)
			{
				return item;
			}
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
int UIList::GetNumItems() const
{
	return (int)mItems.size();
}
//----------------------------------------------------------------------------
int UIList::GetItemIndex(const std::string &text) const
{
	for (int i = 0; i < (int)mItems.size(); i++)
	{
		if (text == mItems[i]->GetLabel())
			return i;
	}

	return -1;
}
//----------------------------------------------------------------------------
void UIList::RemoveAllItems()
{
	for (int i = 0; i < (int)mItems.size(); i++)
	{
		mContentFrame->DetachChild(mItems[i]);
	}
	mItems.clear();

	for (int i = 0; i < (int)mSelectedItems.size(); i++)
	{
		if (mSelectedItems[i])
		{
			mSelectedItems[i]->Select(false);
		}
	}
	mSelectedItems.clear();

	mIsNeedRecal = true;
}
//----------------------------------------------------------------------------
float UIList::GetContentHeight() const
{
	return mItemHeight * (int)mItems.size();
}
//----------------------------------------------------------------------------
void UIList::SetNumMaxItems(int numMax)
{
	mNumMaxItems = numMax;
}
//----------------------------------------------------------------------------
void UIList::SetReleasedDoSelect(bool releasedDoSelect)
{
	mIsReleasedDoSelect = releasedDoSelect;
}
//----------------------------------------------------------------------------
bool UIList::IsReleasedDoSelect() const
{
	return mIsReleasedDoSelect;
}
//----------------------------------------------------------------------------
void UIList::SelectItem(int index)
{
	if (0 <= index && index < (int)mItems.size())
	{
		UIItem *item = mItems[index];

		ClearAllSelectItems();
		AddSelectItem(item);
		OnSelected(item);
	}
}
//----------------------------------------------------------------------------
void UIList::AddSelectItem(UIItem *item)
{
	if (item->IsSelected())
		return;

	item->Select(true);

	mSelectedItems.push_back(item);
}
//----------------------------------------------------------------------------
void UIList::ClearAllSelectItems()
{
	for (int i = 0; i < (int)mSelectedItems.size(); i++)
	{
		mSelectedItems[i]->Select(false);
	}

	mSelectedItems.clear();
}
//----------------------------------------------------------------------------
UIItem *UIList::GetSelectedItem()
{
	if ((int)mSelectedItems.size() > 0)
	{
		return mSelectedItems[0];
	}

	if (0 <= mSelectedIndex && mSelectedIndex<(int)mItems.size())
	{
		return mItems[mSelectedIndex];
	}

	return 0;
}
//----------------------------------------------------------------------------
std::string UIList::GetSelectedItemText()
{
	UIItem *item = GetSelectedItem();
	if (item)
	{
		std::string text = item->GetFText()->GetText()->GetText();
		PX2_LOG_INFO("SelectText:%s", text.c_str());
		return text;
	}
	else
	{
		PX2_LOG_INFO("Select null");
	}
	
	return "";
}
//----------------------------------------------------------------------------
void UIList::OnSelected(UIItem *item)
{
	mSelectedIndex = item->GetUserData<int>("index");

	_UICallbacksCalls(UICT_LIST_SELECTED);
}
//----------------------------------------------------------------------------
void UIList::SetTextColor(const Float3 &textColor)
{
	mTextColor = textColor;

	for (int i = 0; i < (int)mItems.size(); i++)
	{
		UIItem *item = mItems[i];
		if (item)
		{
			item->GetFText()->GetText()->SetColor(textColor);
		}
	}
}
//----------------------------------------------------------------------------
void UIList::SetFont(const std::string &font)
{
	mFont = mFont;

	if (!mFont.empty())
	{
		for (int i = 0; i < (int)mItems.size(); i++)
		{
			UIItem *item = mItems[i];
			if (item)
			{
				item->GetFText()->GetText()->SetFont(mFont);
			}
		}
	}
}
//----------------------------------------------------------------------------
void UIList::SetFontSize(int size)
{
	mFontSize = size;

	for (int i = 0; i < (int)mItems.size(); i++)
	{
		UIItem *item = mItems[i];
		if (item)
		{
			item->GetFText()->GetText()->SetFontWidthHeight(mFontSize, mFontSize);
		}
	}
}
//----------------------------------------------------------------------------
void UIList::SetItemBackColor(const Float3 &backColor)
{
	mItemBackColor = backColor;

	UIPicBox *background = GetBackgroundPicBox();
	if (background)
	{
		background->SetColor(mItemBackColor);
	}
}
//----------------------------------------------------------------------------
const Float3 &UIList::GetItemBackColor() const
{
	return mItemBackColor;
}
//----------------------------------------------------------------------------
void UIList::OnSizeChanged()
{
	UIFrame::OnSizeChanged();

	mIsNeedRecal = true;
	mIsUpdateSliderVisible = true;
	mIsUpdateContentPos = true;
}
//----------------------------------------------------------------------------
void UIList::UpdateWorldData(double applicationTime,
	double elapsedTime)
{
	if (mIsNeedRecal)
	{
		_Recal();
	}

	if (mIsUpdateContentPos)
	{
		_UpdateContentPos();
	}

	UIFrame::UpdateWorldData(applicationTime, elapsedTime);

	if (mIsUpdateSliderVisible)
		_UpdateItemVisible();
}
//----------------------------------------------------------------------------
void UIList::_Recal()
{
	int itemIndex = 0;
	for (int i = 0; i < mContentFrame->GetNumChildren(); i++)
	{
		UIItem *item = DynamicCast<UIItem>(mContentFrame->GetChild(i));

		if (item)
		{
			item->SetAnchorHor(0.0f, 1.0f);
			item->SetAnchorParamHor(0.0f, 0.0f);
			item->SetAnchorVer(1.0f, 1.0f);
			item->SetSize(0.0f, mItemHeight);
			float height = mItemHeight*0.5f + mItemHeight*itemIndex;
			item->SetAnchorParamVer(-height, 0.0f);

			itemIndex++;
		}
	}

	mContentFrame->SetSize(0.0f, GetContentHeight());
	mIsUpdateContentPos = true;

	mIsNeedRecal = false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIList::UIList(LoadConstructor value) :
UIFrame(value),
mIsNeedRecal(false),
mIsUpdateSliderVisible(true),
mSliderSize(10),
mItemHeight(20.0f),
mIsUpdateContentPos(true),
mIsReleasedDoSelect(false),
mSelectedIndex(-1),
mNumMaxItems(-1)
{
	SetChildPickOnlyInSizeRange(true);
}
//----------------------------------------------------------------------------
void UIList::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mSliderSize);
	source.Read(mItemHeight);
	source.Read(mTextAlignLeftPos);
	source.ReadPointer(mMaskFrame);
	source.ReadPointer(mContentFrame);

	int numItems = 0;
	source.Read(numItems);
	mItems.resize(numItems);
	for (int i = 0; i < numItems; i++)
	{
		source.ReadPointer(mItems[i]);
	}

	source.ReadPointer(mSlider);

	source.ReadAggregate(mTextColor);

	source.Read(mNumMaxItems);

	PX2_END_DEBUG_STREAM_LOAD(UIList, source);
}
//----------------------------------------------------------------------------
void UIList::Link(InStream& source)
{
	UIFrame::Link(source);

	if (mMaskFrame)
	{
		source.ResolveLink(mMaskFrame);
	}

	if (mContentFrame)
	{
		source.ResolveLink(mContentFrame);
	}

	for (int i = 0; i < (int)mItems.size(); i++)
	{
		source.ResolveLink(mItems[i]);
	}

	if (mSlider)
	{
		source.ResolveLink(mSlider);
	}
}
//----------------------------------------------------------------------------
void UIList::PostLink()
{
	UIFrame::PostLink();

	if (mMaskFrame)
	{
		mMaskFrame->PostLink();
	}

	if (mContentFrame)
	{
		mContentFrame->PostLink();
	}

	for (int i = 0; i < (int)mItems.size(); i++)
	{
		if (mItems[i])
		{
			mItems[i]->PostLink();
		}
	}

	if (mSlider)
	{
		mSlider->PostLink();

		mSlider->SetMemUICallback(this, (UIFrame::MemUICallback)(&UIList
			::_SliderCallback));
	}
}
//----------------------------------------------------------------------------
bool UIList::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		if (mMaskFrame)
		{
			target.Register(mMaskFrame);
		}

		if (mContentFrame)
		{
			target.Register(mContentFrame);
		}

		for (int i = 0; i < (int)mItems.size(); i++)
		{
			if (mItems[i])
			{
				target.Register(mItems[i]);
			}
		}

		if (mSlider)
		{
			target.Register(mSlider);
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIList::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mSliderSize);
	target.Write(mItemHeight);
	target.Write(mTextAlignLeftPos);
	target.WritePointer(mMaskFrame);
	target.WritePointer(mContentFrame);
	int numItems = (int)mItems.size();
	target.Write(numItems);
	for (int i = 0; i < numItems; i++)
	{
		target.WritePointer(mItems[i]);
	}

	target.WritePointer(mSlider);

	target.WriteAggregate(mTextColor);

	target.Write(mNumMaxItems);

	PX2_END_DEBUG_STREAM_SAVE(UIList, target);
}
//----------------------------------------------------------------------------
int UIList::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mSliderSize);
	size += sizeof(mItemHeight);
	size += sizeof(mTextAlignLeftPos);
	size += PX2_POINTERSIZE(mMaskFrame);
	size += PX2_POINTERSIZE(mContentFrame);
	int numItems = (int)mItems.size();
	size += sizeof(numItems);
	if (numItems > 0)
		size += numItems * PX2_POINTERSIZE(mItems[0]);
	size += PX2_POINTERSIZE(mSlider);

	size += sizeof(mTextColor);

	size += sizeof(mNumMaxItems);

	return size;
}
//----------------------------------------------------------------------------