// PX2UIFrame.inl

//----------------------------------------------------------------------------
inline UIPicBox *UIFrame::GetBackgroundPicBox()
{
	return mBackgroundPicBox;
}
//----------------------------------------------------------------------------
inline const Float3 &UIFrame::GetActivateColor() const
{
	return mActivatedColor;
}
//----------------------------------------------------------------------------
inline float UIFrame::GetActivateAlpha() const
{
	return mActivatedAlpha;
}
//----------------------------------------------------------------------------
inline float UIFrame::GetActivateBrightness() const
{
	return mActivatedBrightness;
}
//----------------------------------------------------------------------------
inline float UIFrame::GetRoundRadius() const
{
	return mRoundRadius;
}
//----------------------------------------------------------------------------
inline const std::vector<UICallback> &UIFrame::GetUICallbacks() const
{
	return mUICallbacks;
}
//----------------------------------------------------------------------------
inline void UIFrame::SetMemUICallback(UIFrame *object, MemUICallback callback)
{
	mMemObject = object;
	mMemUICallback = callback;
}
//----------------------------------------------------------------------------
inline UIFrame::MemUICallback UIFrame::GetMemUICallback() const
{
	return mMemUICallback;
}
//----------------------------------------------------------------------------
inline void UIFrame::SetScriptHandler(const std::string &scriptHandler)
{
	mUIScriptHandler = scriptHandler;
}
//----------------------------------------------------------------------------
inline const std::string &UIFrame::GetScriptHandler() const
{
	return mUIScriptHandler;
}
//----------------------------------------------------------------------------
inline int UIFrame::GetMaskVal() const
{
	return mMaskVal;
}
//----------------------------------------------------------------------------
inline UIPicBox *UIFrame::GetMask()
{
	return mMaskPicBox;
}
//----------------------------------------------------------------------------
inline UIFrameGridAlignControl *UIFrame::GetGridAlignCtrl()
{
	return mGridAlignCtrl;
}
//----------------------------------------------------------------------------