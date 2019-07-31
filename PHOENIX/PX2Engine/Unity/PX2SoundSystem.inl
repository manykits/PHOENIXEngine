// PX2SoundSystem.inl

//----------------------------------------------------------------------------
inline SoundSystem::SystemType SoundSystem::GetSystemType() const
{
	return mSystemType;
}
//----------------------------------------------------------------------------
inline const APoint &SoundSystem::GetListenerPos() const
{
	return mListenPos;
}
//----------------------------------------------------------------------------
inline bool SoundSystem::IsMusicEnable() const
{
	return mIsMusicEnable;
}
//----------------------------------------------------------------------------
inline bool SoundSystem::IsSoundEnable() const
{
	return mIsSoundEnable;
}
//----------------------------------------------------------------------------