// PX2UIFrameGridAlignControl.inl

//----------------------------------------------------------------------------
inline const Sizef &UIFrameGridAlignControl::GetCellSize() const
{
	return mCellSize;
}
//----------------------------------------------------------------------------
inline const Float2 &UIFrameGridAlignControl::GetSpacing() const
{
	return mSpacing;
}
//----------------------------------------------------------------------------
inline UIFrameGridAlignControl::StartAxisType UIFrameGridAlignControl::
GetStartAxis() const
{
	return mStartAxisType;
}
//----------------------------------------------------------------------------
inline UIFrameGridAlignControl::ChildAlignmentType UIFrameGridAlignControl::
GetChildAlignment() const
{
	return mChildAlignmentType;
}
//----------------------------------------------------------------------------
inline UIFrameGridAlignControl::ConstraintType UIFrameGridAlignControl::
GetConstraintType() const
{
	return mConstraintType;
}
//----------------------------------------------------------------------------
inline int UIFrameGridAlignControl::GetConstraintValue() const
{
	return mConstraintValue;
}
//----------------------------------------------------------------------------
inline bool UIFrameGridAlignControl::IsConstraintExtend() const
{
	return mIsConstraintExtend;
}
//----------------------------------------------------------------------------
inline bool UIFrameGridAlignControl::IsLayoutChanged() const
{
	return mIsLayoutChanged;
}
//----------------------------------------------------------------------------
inline void UIFrameGridAlignControl::SetAutoAdjustContentSize(bool adjust)
{
	mIsAutoAdjustContentSize = adjust;
}
//----------------------------------------------------------------------------
inline bool UIFrameGridAlignControl::IsAutoAdjustContentSize() const
{
	return mIsAutoAdjustContentSize;
}
//----------------------------------------------------------------------------
inline Sizef UIFrameGridAlignControl::GetContentSize() const
{
	return mContentSize;
}
//----------------------------------------------------------------------------