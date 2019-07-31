// PX2UIItem.inl

//----------------------------------------------------------------------------
inline bool UIItem::IsRootItem() const
{
	return mIsRootItem;
}
//----------------------------------------------------------------------------
inline bool UIItem::IsShowItem() const
{
	return mIsShowItem;
}
//----------------------------------------------------------------------------
inline int UIItem::GetNumChildItem() const
{
	return (int)mChildItems.size();
}
//----------------------------------------------------------------------------
inline Object *UIItem::GetItemObject()
{
	return mObject;
}
//----------------------------------------------------------------------------
inline bool UIItem::IsExpand() const
{
	return mIsExpand;
}
//----------------------------------------------------------------------------
inline int UIItem::GetNumAllChildsExpand() const
{
	return mNumAllChildExpand;
}
//----------------------------------------------------------------------------
inline UIItem::IconArrowState UIItem::GetIconArrowState() const
{
	return mIconArrowState;
}
//----------------------------------------------------------------------------
inline UIButton *UIItem::GetButBack()
{
	return mButBack;
}
//----------------------------------------------------------------------------
inline UIFPicBox *UIItem::GetIcon()
{
	return mIcon;
}
//----------------------------------------------------------------------------
inline UIFText *UIItem::GetFText()
{
	return mFText;
}
//----------------------------------------------------------------------------
inline bool UIItem::IsSelected() const
{
	return mIsSelected;
}
//----------------------------------------------------------------------------
inline float UIItem::GetLevelAdjust() const
{
	return mLevelAdjust;
}
//----------------------------------------------------------------------------
inline float UIItem::_GetLevel() const
{
	return mLevel;
}
//----------------------------------------------------------------------------
inline bool UIItem::_IsNeedRecal() const
{
	return mIsNeedRecal;
}
//----------------------------------------------------------------------------
inline UICheckButton *UIItem::GetButArrow()
{
	return mButArrow;
}
//----------------------------------------------------------------------------
inline void UIItem::SetValue(const Any &val)
{
	mValue = val;
}
//----------------------------------------------------------------------------
inline const Any &UIItem::GetValue() const
{
	return mValue;
}
//----------------------------------------------------------------------------