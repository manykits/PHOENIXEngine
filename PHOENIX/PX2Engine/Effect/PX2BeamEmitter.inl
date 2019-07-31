// PX2BeamEmitter.inl

//----------------------------------------------------------------------------
inline void BeamEmitter::SetEmitRate (float rate)
{
	mEmitRate = rate;
}
//----------------------------------------------------------------------------
inline float BeamEmitter::GetEmitRate () const
{
	return mEmitRate;
}
//----------------------------------------------------------------------------
inline void BeamEmitter::SetEmitStartPos (const APoint &startPos)
{
	mEmitStartPos = startPos;
}
//----------------------------------------------------------------------------
inline const APoint &BeamEmitter::GetEmitStartPos () const
{
	return mEmitStartPos;
}
//----------------------------------------------------------------------------
inline void BeamEmitter::SetEmitEndPos (const APoint &endPos)
{
	mEmitEndPos = endPos;
}
//----------------------------------------------------------------------------
inline const APoint &BeamEmitter::GetEmitEndPos () const
{
	return mEmitEndPos;
}
//----------------------------------------------------------------------------
inline void BeamEmitter::SetStartPosUseLocal (bool startUseLocal)
{
	mIsStartUseLocal = startUseLocal;
}
//----------------------------------------------------------------------------
inline bool BeamEmitter::IsStartPosUseLocal () const
{
	return mIsStartUseLocal;
}
//----------------------------------------------------------------------------
inline void BeamEmitter::SetSimpleLineWave (bool simpleLineWave)
{
	mIsSimpleLineWave = simpleLineWave;
}
//----------------------------------------------------------------------------
inline bool BeamEmitter::IsSimpleLineWave () const
{
	return mIsSimpleLineWave;
}
//----------------------------------------------------------------------------
inline void BeamEmitter::SetSimpleLineDoAlphaDisAfterStopSpeed (float speed)
{
	mDoAlphaDisAfterStopSpeed = speed;
}
//----------------------------------------------------------------------------
inline float BeamEmitter::GetSimpleLineDoAlphaDisAfterStopSpeed () const
{
	return mDoAlphaDisAfterStopSpeed;
}
//----------------------------------------------------------------------------
inline void BeamEmitter::SetWaveTypeUp (WaveType type)
{
	mWaveTypeUp = type;
}
//----------------------------------------------------------------------------
inline BeamEmitter::WaveType BeamEmitter::GetWaveTypeUp () const
{
	return mWaveTypeUp;
}
//----------------------------------------------------------------------------
inline void BeamEmitter::SetWaveTypeExtend (WaveType type)
{
	mWaveTypeExtend = type;
}
//----------------------------------------------------------------------------
inline BeamEmitter::WaveType BeamEmitter::GetWaveTypeExtend () const
{
	return mWaveTypeExtend;
}
//----------------------------------------------------------------------------
inline int BeamEmitter::GetNumMaxBeams () const
{
	return mNumMaxBeams;
}
//----------------------------------------------------------------------------
inline int BeamEmitter::GetNumLowFrequency () const
{
	return mNumLowFrequency;
}
//----------------------------------------------------------------------------
inline int BeamEmitter::GetNumHighFrequency () const
{
	return mNumHighFrequency;
}
//----------------------------------------------------------------------------
inline void BeamEmitter::SetLowFrequencyRangeUp (const Float2 &range)
{
	mLowRangeUp = range;
}
//----------------------------------------------------------------------------
inline void BeamEmitter::SetLowFrequencyRangeExtend (const Float2 &range)
{
	mLowRangeExtend = range;
}
//----------------------------------------------------------------------------
inline const Float2 &BeamEmitter::GetLowFrequencyRangeUp () const
{
	return mLowRangeUp; 
}
//----------------------------------------------------------------------------
inline const Float2 &BeamEmitter::GetLowFrequencyRangeExtend () const
{
	return mLowRangeExtend;
}
//----------------------------------------------------------------------------
inline void BeamEmitter::SetHighFrequencyRangeUp (const Float2 &range)
{
	mHighRangeUp = range;
}
//----------------------------------------------------------------------------
inline void BeamEmitter::SetHighFrequencyRangeExtend (const Float2 &range)
{
	mHighRangeExtend = range;
}
//----------------------------------------------------------------------------
inline const Float2 &BeamEmitter::GetHighFrequencyRangeUp () const
{
	return mHighRangeUp;
}
//----------------------------------------------------------------------------
inline const Float2 &BeamEmitter::GetHighFrequencyRangeExtend () const
{
	return mHighRangeExtend;
}
//----------------------------------------------------------------------------