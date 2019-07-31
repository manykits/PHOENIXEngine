// PX2CameraShake.inl

//----------------------------------------------------------------------------
inline void CameraShake::SetUseX (bool use)
{
	mIsUseX = use;
}
//----------------------------------------------------------------------------
inline bool CameraShake::IsUseX () const
{
	return mIsUseX;
}
//----------------------------------------------------------------------------
inline void CameraShake::SetUseY (bool use)
{
	mIsUseY = use;
}
//----------------------------------------------------------------------------
inline bool CameraShake::IsUseY () const
{
	return mIsUseY;
}
//----------------------------------------------------------------------------
inline void CameraShake::SetUseZ (bool use)
{
	mIsUseZ = use;
}
//----------------------------------------------------------------------------
inline bool CameraShake::IsUseZ () const
{
	return mIsUseZ;
}
//----------------------------------------------------------------------------
inline float CameraShake::GetMaxDistance () const
{
	return mMaxDistance;
}
//----------------------------------------------------------------------------
inline float CameraShake::GetEmitStrength () const
{
	return mEmitStrength;
}
//----------------------------------------------------------------------------
inline int CameraShake::GetShakeChannel () const
{
	return mShakeChannel;
}
//----------------------------------------------------------------------------