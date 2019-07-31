// PX2UITabFrame.inl

//----------------------------------------------------------------------------
inline UITabFrame::LayoutPosType UITabFrame::GetLayoutPos() const
{
	return mLayoutPosType;
}
//----------------------------------------------------------------------------
inline float UITabFrame::GetTabBarHeight() const
{
	return mTabBarHeight;
}
//----------------------------------------------------------------------------
inline float UITabFrame::GetTabWidth() const
{
	return mTabWidth;
}
//----------------------------------------------------------------------------
inline float UITabFrame::GetTabHeight() const
{
	return mTabHeight;
}
//----------------------------------------------------------------------------
inline UITabFrame::TabLayoutType UITabFrame::GetTabLayoutType() const
{
	return mTabLayoutType;
}
//----------------------------------------------------------------------------
inline UIAuiBlockFrame *UITabFrame::GetAuiBlockFrame()
{
	return mAuiBlockFrame;
}
//----------------------------------------------------------------------------