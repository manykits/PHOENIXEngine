// PX2UIInputText.inl

//----------------------------------------------------------------------------
inline bool UIInputText::IsPassword() const
{
	return mIsPassword;
}
//----------------------------------------------------------------------------
inline const std::string &UIInputText::GetRealText() const
{
	return mRealText;
}
//----------------------------------------------------------------------------
inline void UIInputText::SetCallback(UIInputTextCallback callback)
{
	mCallback = callback;
}
//----------------------------------------------------------------------------
inline UIInputTextCallback UIInputText::GetCallback() const
{
	return mCallback;
}
//----------------------------------------------------------------------------
inline float UIInputText::GetFixedWidth() const
{
	return mFixedWidth;
}
//----------------------------------------------------------------------------