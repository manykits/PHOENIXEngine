// PX2UIItem.cpp

#include "PX2UIItem.hpp"
#include "PX2UITree.hpp"
#include "PX2UISkinManager.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, UIFrame, UIItem);
PX2_IMPLEMENT_STREAM(UIItem);
PX2_IMPLEMENT_FACTORY(UIItem);
PX2_IMPLEMENT_DEFAULT_NAMES(UIFrame, UIItem);

//----------------------------------------------------------------------------
UIItem::UIItem() :
mIsRootItem(false),
mIsShowItem(true),
mIsNeedRecal(true),
mIsExpand(true),
mIsNumAllChildExpandNeedRecal(true),
mNumAllChildExpand(0),
mIconArrowState(IconArrowState::IAS_NONE),
mLevelAdjust(0),
mLevel(0),
mIsSelected(false),
mObject(0)
{
	mButBack = new0 UIButton();
	AttachChild(mButBack);
	mButBack->LocalTransform.SetTranslateY(-1.0f);
	mButBack->SetStateColor(UIButtonBase::BS_NORMAL, PX2_UISM.Color_ViewBackground);
	mButBack->SetStateColor(UIButtonBase::BS_HOVERED, PX2_UISM.Color_ViewBackground);
	mButBack->SetAnchorHor(0.0f, 1.0f);
	mButBack->SetAnchorVer(0.0f, 1.0f);
	mButBack->SetAnchorParamVer(1.0f, 0.0f);
	mButBack->SetActivateColor(PX2_UISM.Color_SelectItem);
	SetActivateColor(PX2_UISM.Color_SelectItem);

	mFText = new0 UIFText();
	AttachChild(mFText);
	mFText->GetText()->SetAligns(TEXTALIGN_LEFT|TEXTALIGN_VCENTER);
	mFText->SetAnchorHor(0.0f, 1.0f);
	mFText->SetAnchorVer(0.0f, 1.0f);
	mFText->LocalTransform.SetTranslateY(-2.0f);
	mFText->GetText()->SetFontColor(Float3::MakeColor(120, 120, 120));

	SetPivot(0.5f, 0.5f);
}
//----------------------------------------------------------------------------
UIItem::~UIItem()
{
}
//----------------------------------------------------------------------------
void UIItem::SetRootItem(bool root)
{
	mIsRootItem = root;
}
//----------------------------------------------------------------------------
void UIItem::ShowItem(bool show)
{
	mIsShowItem = show;

	if (mButBack)
		mButBack->Show(mIsShowItem);

	if (mFText)
		mFText->Show(mIsShowItem);

	if (mButArrow)
		mButArrow->Show(mIsShowItem);

	if (IsRootItem())
	{
		if (mIsShowItem)
		{
			mLevel = 0 + mLevelAdjust;
		}
		else
		{
			mLevel = -1 + mLevelAdjust;
		}

		mIsNeedRecal = true;
		mIsNumAllChildExpandNeedRecal = true;
	}
}
//----------------------------------------------------------------------------
UIItem *UIItem::AddItem(const std::string &label, const std::string &name,
	Object *obj)
{
	UIItem *item = new0 UIItem();
	AttachChild(item);
	item->SetName(name);
	item->SetSize(GetSize());
	item->GetFText()->GetText()->SetText(label);
	item->GetFText()->GetText()->SetAligns(TEXTALIGN_LEFT | TEXTALIGN_VCENTER);
	item->GetFText()->GetText()->SetFontColor(Float3::MakeColor(220, 220, 220));
	item->mObject = obj;

	return item;
}
//----------------------------------------------------------------------------
void UIItem::OnChildAttached(Movable *child)
{
	UIFrame::OnChildAttached(child);

	UIItem *item = DynamicCast<UIItem>(child);
	if (item)
	{
		mIsNeedRecal = true;
		mIsNumAllChildExpandNeedRecal = true;

		mChildItems.push_back(item);

		_TellParentChildrenRecal();
		_TellParentChildrenExpand();
	}
}
//----------------------------------------------------------------------------
bool UIItem::RemoveItem(const std::string &name)
{
	// remove item
	std::vector<Pointer0<UIItem> >::iterator it = mChildItems.begin();
	for (; it != mChildItems.end();)
	{
		UIItemPtr item = (*it);
		if (name == item->GetName())
		{
			it = mChildItems.erase(it);
			DetachChild(item);

			int numChildItems = GetNumChildItem();
			if (0 == numChildItems)
			{
				DestoryArrowBut();
			}

			return true;
		}
		else
		{
			it++;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool UIItem::RemoveItem(UIItem *item)
{
	// remove item
	std::vector<Pointer0<UIItem> >::iterator it = mChildItems.begin();
	for (; it != mChildItems.end();)
	{
		if (item == *it)
		{
			it = mChildItems.erase(it);
			DetachChild(item);

			int numChildItems = GetNumChildItem();
			if (0 == numChildItems)
			{
				DestoryArrowBut();
			}

			return true;
		}
		else
		{
			it++;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void UIItem::OnChildDetach(Movable *child)
{
	UIFrame::OnChildDetach(child);

	UIItem *item = DynamicCast<UIItem>(child);
	if (item)
	{
		mIsNeedRecal = true;
		_TellParentChildrenRecal();

		mIsNumAllChildExpandNeedRecal = true;
		_TellParentChildrenExpand();
	}
}
//----------------------------------------------------------------------------
void UIItem::RemoveAllChildItems()
{
	for (int i = 0; i < (int)mChildItems.size(); i++)
	{
		DetachChild(mChildItems[i]);
	}

	mChildItems.clear();

	_TellParentChildrenRecal();
}
//----------------------------------------------------------------------------
UIItem *UIItem::GetChildItemByIndex(int index)
{
	if (0 <= index && index < (int)mChildItems.size())
	{
		return mChildItems[index];
	}

	return 0;
}
//----------------------------------------------------------------------------
void UIItem::SetItemObject(Object* obj)
{
	mObject = obj;
}
//----------------------------------------------------------------------------
UIItem *UIItem::GetItemByObject(Object *obj)
{
	if (obj == mObject)
		return this;

	for (int i = 0; i<(int)mChildItems.size(); i++)
	{
		UIItem *item = mChildItems[i];
		UIItem *objItem = item->GetItemByObject(obj);
		if (objItem)
			return objItem;
	}

	return 0;
}
//----------------------------------------------------------------------------
UICheckButton *UIItem::CreateButArrow()
{
	DestoryArrowBut();

	mButArrow = new0 UICheckButton();
	AttachChild(mButArrow);
	mButArrow->LocalTransform.SetTranslateY(-3.0f);
	mButArrow->SetSize(12.0f, 12.0f);
	mButArrow->SetPivot(Float2(0.5f, 0.5f));
	mButArrow->SetAnchorHor(0.0f, 0.0f);
	mButArrow->SetAnchorVer(0.5f, 0.5f);

	_TellParentChildrenExpand();
	_TellParentChildrenRecal();

	return mButArrow;
}
//----------------------------------------------------------------------------
void UIItem::DestoryArrowBut()
{
	if (mButArrow)
	{
		DetachChild(mButArrow);
		mButArrow = 0;
	}
}
//----------------------------------------------------------------------------
void UIItem::Expand(bool expand)
{
	mIsExpand = expand;

	SetIconArrowState(expand ? IAS_ARROW1 : IAS_ARROW0);

	mIsNumAllChildExpandNeedRecal = true;
	_TellParentChildrenExpand();
	_TellParentChildrenRecal();
}
//----------------------------------------------------------------------------
void UIItem::_TellParentChildrenExpand()
{
	UIItem *parentItem = DynamicCast<UIItem>(GetParent());
	while (parentItem)
	{
		parentItem->mIsNumAllChildExpandNeedRecal = true;

		parentItem = DynamicCast<UIItem>(parentItem->GetParent());
	}
}
//----------------------------------------------------------------------------
void UIItem::_TellParentChildrenRecal()
{
	UIItem *parentItem = DynamicCast<UIItem>(GetParent());
	while (parentItem)
	{
		parentItem->mIsNeedRecal = true;

		for (int i = 0; i < parentItem->GetNumChildren(); i++)
		{
			UIItem *parentChildItem = DynamicCast<UIItem>(
				parentItem->GetChild(i));
			if (parentChildItem)
			{
				parentChildItem->mIsNeedRecal = true;
			}
		}

		parentItem = DynamicCast<UIItem>(parentItem->GetParent());
	}
}
//----------------------------------------------------------------------------
void UIItem::SetLabel(const std::string &label)
{
	mFText->GetText()->SetText(label);
}
//----------------------------------------------------------------------------
const std::string &UIItem::GetLabel() const
{
	return mFText->GetText()->GetText();
}
//----------------------------------------------------------------------------
void UIItem::OnSizeChanged()
{
	UIFrame::OnSizeChanged();
}
//----------------------------------------------------------------------------
void UIItem::SetLevelAdjust(float adjust)
{
	mLevelAdjust = adjust;

	mIsNeedRecal = true;
}
//----------------------------------------------------------------------------
void UIItem::SetIconArrowState(IconArrowState state)
{
	mIconArrowState = state;

	mIsNeedRecal = true;
}
//----------------------------------------------------------------------------
void UIItem::Select(bool select)
{
	mIsSelected = select;
	SetActivate(mIsSelected);

	if (mButBack)
	{
		mButBack->SetActivate(mIsSelected);
	}
}
//----------------------------------------------------------------------------
void UIItem::UpdateWorldData(double applicationTime, double elapsedTime)
{
	if (mIsNumAllChildExpandNeedRecal)
	{
		_RecalNumChildExpand();
	}

	if (mIsNeedRecal)
	{
		_Recal();
	}

	UIFrame::UpdateWorldData(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void UIItem::PreCanvasPick(const CanvasInputData &inputData, Canvas *canvas)
{
	if (!IsShow())
		return;

	if (!IsDoPick())
		return;

	for (int i = 0; i < GetNumChildren(); i++)
	{
		UIFrame *childFrame = DynamicCast<UIFrame>(GetChild(i));
		UIItem *childItem = DynamicCast<UIItem>(GetChild(i));

		if (childFrame && !childItem && IsShowItem())
		{
			childFrame->PreCanvasPick(inputData, canvas);
		}

		if (IsExpand() && childItem)
		{
			childItem->PreCanvasPick(inputData, canvas);
		}
	}
}
//----------------------------------------------------------------------------
void UIItem::_RecalNumChildExpand()
{
	mIsNumAllChildExpandNeedRecal = false;

	int numChildren = GetNumChildren();
	for (int i = 0; i < numChildren; i++)
	{
		UIItem *item = DynamicCast<UIItem>(GetChild(i));
		if (item)
		{
			item->_SetLevel(_GetLevel() + 1 + item->GetLevelAdjust());
			item->_RecalNumChildExpand();
		}
	}

	mNumAllChildExpand = 1;
	if (mIsExpand)
	{
		for (int i = 0; i < GetNumChildren(); i++)
		{
			UIItem *item = DynamicCast<UIItem>(GetChild(i));
			if (item)
			{
				mNumAllChildExpand += item->GetNumAllChildsExpand();
			}
		}
	}
	else
	{
		mNumAllChildExpand = 1;
	}
}
//----------------------------------------------------------------------------
void UIItem::_Recal()
{
	mIsNeedRecal = false;

	int numLevels = 0;
	UITree *tree = GetFirstParentDerivedFromType<UITree>(&numLevels);
	if (tree)
	{
		float iconArrowSpace = tree->GetIconArrowSpace();
		float itemHeight = tree->GetItemHeight();

		float selfVerOffset = -itemHeight / 2.0f;
		if (!IsShowItem())
			selfVerOffset += itemHeight;

		SetAnchorHor(0.0f, 1.0f);
		SetAnchorVer(1.0f, 1.0f);

		float level = _GetLevel();
		UIText *text = GetFText()->GetText();
		float offsetPosX = level * iconArrowSpace;
		float textOffsetPosX = offsetPosX;
		if (mButArrow && mButArrow->IsShow())
		{
			mButArrow->Check(!IsExpand(), false);
			mButArrow->SetAnchorParamHor(
				offsetPosX + iconArrowSpace / 2.0f, 0.0f);
		}
		textOffsetPosX += iconArrowSpace;
		text->SetOffset(Float2(textOffsetPosX, 0.0f));

		int numItemExtend = 0;
		for (int i = 0; i < GetNumChildren(); i++)
		{
			UIItem *item = DynamicCast<UIItem>(GetChild(i));
			if (item)
			{
				item->Show(IsExpand());

				// -itemHeight/2.0f is this item height
				item->SetAnchorParamVer(
					selfVerOffset - itemHeight * (numItemExtend + 1), 0.0f);

				numItemExtend += item->GetNumAllChildsExpand();
			}
		}
	}
}
//----------------------------------------------------------------------------
void UIItem::_SetLevel(float level)
{
	mLevel = level;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
UIItem::UIItem(LoadConstructor value) :
UIFrame(value),
mIsRootItem(false),
mIsShowItem(false),
mIsNeedRecal(true),
mIsExpand(true),
mIsNumAllChildExpandNeedRecal(true),
mNumAllChildExpand(0),
mIconArrowState(IconArrowState::IAS_NONE),
mLevelAdjust(0),
mLevel(0),
mObject(0)
{
}
//----------------------------------------------------------------------------
void UIItem::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	UIFrame::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(UIItem, source);
}
//----------------------------------------------------------------------------
void UIItem::Link(InStream& source)
{
	UIFrame::Link(source);
}
//----------------------------------------------------------------------------
void UIItem::PostLink()
{
	UIFrame::PostLink();
}
//----------------------------------------------------------------------------
bool UIItem::Register(OutStream& target) const
{
	if (UIFrame::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void UIItem::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	UIFrame::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(UIItem, target);
}
//----------------------------------------------------------------------------
int UIItem::GetStreamingSize(Stream &stream) const
{
	int size = UIFrame::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------