// PX2UIPicBox.inl

//----------------------------------------------------------------------------
inline UIPicBox::PicBoxType UIPicBox::GetPicBoxType()
{
	return mPicBoxType;
}
//----------------------------------------------------------------------------
inline const Float2 &UIPicBox::GetPivot() const
{
	return mPivotPoint;
}
//----------------------------------------------------------------------------
inline Float2 &UIPicBox::GetPivot()
{
	return mPivotPoint;
}
//----------------------------------------------------------------------------
inline const Sizef &UIPicBox::GetSize() const
{
	return mSize;
}
//----------------------------------------------------------------------------
inline float UIPicBox::GetWidth() const
{
	return mSize.Width;
}
//----------------------------------------------------------------------------
inline float UIPicBox::GetHeight() const
{
	return mSize.Height;
}
//----------------------------------------------------------------------------
inline UIPicBox::PicBoxTexMode UIPicBox::GetPicBoxTexMode() const
{
	return mPBTexMode;
}
//----------------------------------------------------------------------------
inline const std::string &UIPicBox::GetTextureFilename() const
{
	return mTexturePathname;
}
//----------------------------------------------------------------------------
inline const std::string &UIPicBox::GetTexturePackName() const
{
	return mTexturePackName;
}
//----------------------------------------------------------------------------
inline const std::string &UIPicBox::GetElementName() const
{
	return mElementName;
}
//----------------------------------------------------------------------------
inline const Sizef &UIPicBox::GetTexCornerSizeLB() const
{
	return mCornerSizeLB;
}
//----------------------------------------------------------------------------
inline Sizef &UIPicBox::GetTexCornerSizeLB()
{
	return mCornerSizeLB;
}
//----------------------------------------------------------------------------
inline const Sizef &UIPicBox::GetTexCornerSizeRT() const
{
	return mCornerSizeRT;
}
//----------------------------------------------------------------------------
inline Sizef &UIPicBox::GetTexCornerSizeRT()
{
	return mCornerSizeRT;
}
//----------------------------------------------------------------------------