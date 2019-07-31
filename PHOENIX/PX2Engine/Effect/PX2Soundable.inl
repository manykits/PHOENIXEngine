// PX2Soundable.inl

//----------------------------------------------------------------------------
inline Soundable::SourceType Soundable::GetSourceType () const
{
	return mSourceType;
}
//----------------------------------------------------------------------------
inline void Soundable::SetMusicChannel (int channel)
{
	mMusicChannel = channel;
}
//----------------------------------------------------------------------------
inline int Soundable::GetMusicChannel () const
{
	return mMusicChannel;
}
//----------------------------------------------------------------------------
inline void Soundable::SetIngoreStopSound (bool stop)
{
	mIngoreStopSound = stop;
}
//----------------------------------------------------------------------------
inline bool Soundable::IsIngoreStopSound () const
{
	return mIngoreStopSound;
}
//----------------------------------------------------------------------------
inline const std::string &Soundable::GetSoundFilename () const
{
	return mFilename;
}
//----------------------------------------------------------------------------
inline bool Soundable::Is3D () const
{
	return mIs3D;
}
//----------------------------------------------------------------------------
inline float Soundable::GetVolume () const
{
	return mVolume;
}
//----------------------------------------------------------------------------
inline const AVector &Soundable::GetVelocity () const
{
	return mVelocity;
}
//----------------------------------------------------------------------------
inline float Soundable::GetMinDistance () const
{
	return mMinDistance;
}
//----------------------------------------------------------------------------
inline float Soundable::GetMaxDistance () const
{
	return mMaxDistance;
}
//----------------------------------------------------------------------------
inline bool Soundable::IsLoop () const
{
	return mIsLoop;
}
//----------------------------------------------------------------------------
inline void Soundable::SetDistanceUseX (bool use)
{
	mDistanceUseX = use;
}
//----------------------------------------------------------------------------
inline bool Soundable::IsDistanceUseX () const
{
	return mDistanceUseX;
}
//----------------------------------------------------------------------------
inline void Soundable::SetDistanceUseY (bool use)
{
	mDistanceUseY = use;
}
//----------------------------------------------------------------------------
inline bool Soundable::IsDistanceUseY () const
{
	return mDistanceUseY;
}
//----------------------------------------------------------------------------
inline void Soundable::SetDistanceUseZ (bool use)
{
	mDistanceUseZ = use;
}
//----------------------------------------------------------------------------
inline bool Soundable::IsDistanceUseZ () const
{
	return mDistanceUseZ;
}
//----------------------------------------------------------------------------