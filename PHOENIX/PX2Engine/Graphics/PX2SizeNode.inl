// PX2SizeNode.inl

//----------------------------------------------------------------------------
inline const Sizef &SizeNode::GetSize() const
{
	return mSize;
}
//----------------------------------------------------------------------------
inline float SizeNode::GetWidth() const
{
	return mSize.Width;
}
//----------------------------------------------------------------------------
inline float SizeNode::GetHeight() const
{
	return mSize.Height;
}
//----------------------------------------------------------------------------
inline const Float2 &SizeNode::GetPivot() const
{
	return mPivot;
}
//----------------------------------------------------------------------------
inline bool SizeNode::IsAnchorLayoutEnable() const
{
	return mIsAnchorLayoutEnable;
}
//----------------------------------------------------------------------------
inline const Float2 &SizeNode::GetAnchorHor() const
{
	return mAnchorHor;
}
//----------------------------------------------------------------------------
inline const Float2 &SizeNode::GetAnchorVer() const
{
	return mAnchorVer;
}
//----------------------------------------------------------------------------
inline const Float2 &SizeNode::GetAnchorParamHor() const
{
	return mAnchorParamHor;
}
//----------------------------------------------------------------------------
inline const Float2 &SizeNode::GetAnchorParamVer() const
{
	return mAnchorParamVer;
}
//----------------------------------------------------------------------------
inline bool SizeNode::IsEnableScreenRectLayout() const
{
	return mIsEnableScreenRectLayout;
}
//----------------------------------------------------------------------------
inline const Rectf &SizeNode::GetScreenRect() const
{
	return mScreenRect;
}
//----------------------------------------------------------------------------
inline bool SizeNode::IsWidget() const
{
	return mIsWidget;
}
//----------------------------------------------------------------------------