// PX2UIRound.inl
// PX2UIRound.inl

//----------------------------------------------------------------------------
inline UIFPicBox *UIRound::GetBack()
{
	return mBack;
}
//----------------------------------------------------------------------------
inline UIButton *UIRound::GetButton()
{
	return mBut;
}
//----------------------------------------------------------------------------
inline bool UIRound::IsDragable() const
{
	return mIsDragable;
}
//----------------------------------------------------------------------------
inline const Vector2f &UIRound::GetDraggingDir() const
{
	return mDraggingDir;
}
//----------------------------------------------------------------------------
inline float UIRound::GetDraggingPercent() const
{
	return mDraggingPercent;
}
//----------------------------------------------------------------------------
inline const Vector2f &UIRound::GetDraggedDir() const
{
	return mDraggedDir;
}
//----------------------------------------------------------------------------
inline float UIRound::GetDraggedPercent() const
{
	return mDraggedPercent;
}
//----------------------------------------------------------------------------
inline const APoint &UIRound::GetCurPickWorldPos() const
{
	return mCurPickWorldPos;
}
//----------------------------------------------------------------------------