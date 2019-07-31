// PX2ShaderFloat.inl

//----------------------------------------------------------------------------
inline int ShaderFloat::GetNumRegisters () const
{
	return mNumElements/4;
}
//----------------------------------------------------------------------------
inline const float* ShaderFloat::GetData () const
{
	return mData;
}
//----------------------------------------------------------------------------
inline float* ShaderFloat::GetData ()
{
	return mData;
}
//----------------------------------------------------------------------------
inline float ShaderFloat::operator[] (int i) const
{
	assertion(0 <= i && i < mNumElements, "Invalid index\n");
	return mData[i];
}
//----------------------------------------------------------------------------
inline float& ShaderFloat::operator[] (int i)
{
	assertion(0 <= i && i < mNumElements, "Invalid index\n");
	return mData[i];
}
//----------------------------------------------------------------------------
inline void ShaderFloat::EnableUpdater ()
{
	mAllowUpdater = true;
}
//----------------------------------------------------------------------------
inline void ShaderFloat::DisableUpdater ()
{
	mAllowUpdater = false;
}
//----------------------------------------------------------------------------
inline bool ShaderFloat::AllowUpdater () const
{
	return mAllowUpdater;
}
//----------------------------------------------------------------------------
