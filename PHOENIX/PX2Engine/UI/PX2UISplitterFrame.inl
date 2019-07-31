// PX2UISplitterFrame.inl

//----------------------------------------------------------------------------
inline bool UISplitterFrame::IsHor() const
{
	return mIsHor;
}
//----------------------------------------------------------------------------
inline UISplitterFrame::DragType UISplitterFrame::GetDragType() const
{
	return mDragType;
}
//----------------------------------------------------------------------------
inline const Float2 &UISplitterFrame::GetDragRange() const
{
	return mDragRange;
}
//----------------------------------------------------------------------------
inline UISplitterFrame::PosType UISplitterFrame::GetPosType() const
{
	return mPosType;
}
//----------------------------------------------------------------------------
inline bool UISplitterFrame::IsDragable() const
{
	return mIsDragable;
}
//----------------------------------------------------------------------------
inline bool UISplitterFrame::IsDraging() const
{
	return mIsDraging;
}
//----------------------------------------------------------------------------