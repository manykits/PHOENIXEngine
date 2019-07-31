// PX2UIAuiBlockFrame.inl

//----------------------------------------------------------------------------
inline bool UIAuiBlockFrame::IsCaputred() const
{
	return mIsCaptured;
}
//----------------------------------------------------------------------------
inline UIAuiBlockFrame *UIAuiBlockFrame::GetBrotherFrame()
{
	return mBrotherFrame;
}
//----------------------------------------------------------------------------
inline UILayoutPosType UIAuiBlockFrame::GetLayOutPosType() const
{
	return mPosType;
}
//----------------------------------------------------------------------------
inline bool UIAuiBlockFrame::IsAutoExpand() const
{
	return mIsAutoExpand;
}
//----------------------------------------------------------------------------
inline UITabFrame *UIAuiBlockFrame::GetUITabFrame()
{
	return mUITabFrame;
}
//----------------------------------------------------------------------------
inline const Sizef &UIAuiBlockFrame::GetMinSize() const
{
	return mMinSize;
}
//----------------------------------------------------------------------------
inline UISplitterFrame *UIAuiBlockFrame::GetSideFrameHor0()
{
	return mSideFrameHor0;
}
//----------------------------------------------------------------------------
inline UISplitterFrame *UIAuiBlockFrame::GetSideFrameHor1()
{
	return mSideFrameHor1;
}
//----------------------------------------------------------------------------
inline UISplitterFrame *UIAuiBlockFrame::GetSideFrameVer0()
{
	return mSideFrameVer0;
}
//----------------------------------------------------------------------------
inline UISplitterFrame *UIAuiBlockFrame::GetSideFrameVer1()
{
	return mSideFrameVer1;
}
//----------------------------------------------------------------------------
inline UISplitterFrame *UIAuiBlockFrame::GetSpliterFrame()
{
	return mSpliterFrame;
}
//----------------------------------------------------------------------------
inline UIAuiBlockFrame *UIAuiBlockFrame::GetParentSpBlockFrame()
{
	return mParentAuiBlockFrame;
}
//----------------------------------------------------------------------------