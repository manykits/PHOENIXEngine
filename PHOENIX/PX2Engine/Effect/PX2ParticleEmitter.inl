// PX2ParticleEmitter.inl

//----------------------------------------------------------------------------
inline void ParticleEmitter::SetBlast (bool blast)
{
	mIsBlast = blast;
}
//----------------------------------------------------------------------------
inline bool ParticleEmitter::IsBlast () const
{
	return mIsBlast;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetDrawType (DrawType type)
{
	mDrawType = type;
}
//----------------------------------------------------------------------------
inline ParticleEmitter::DrawType ParticleEmitter::GetDrawType () const
{
	return mDrawType;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetPivot (const Float2 &point)
{
	mPivotPoint = point;
}
//----------------------------------------------------------------------------
inline const Float2 &ParticleEmitter::GetPivot () const
{
	return mPivotPoint;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetRotateAxisType (RotateAxisType type)
{
	mRotateAxisType = type;
}
//----------------------------------------------------------------------------
inline ParticleEmitter::RotateAxisType ParticleEmitter::GetRotateAxisType ()
	const
{
	return mRotateAxisType;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetEmitRotateAxis (const AVector &dir)
{
	mRotateAxis = dir;
}
//----------------------------------------------------------------------------
inline const AVector &ParticleEmitter::GetEmitRotateAxis () const
{
	return mRotateAxis;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetEmitRotateDegree (float degree)
{
	mEmitRotateDegree = degree;
}
//----------------------------------------------------------------------------
inline float ParticleEmitter::GetEmitRotateDegree () const
{
	return mEmitRotateDegree;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetEmitRotateSpeed (float speed)
{
	mEmitRotateSpeed = speed;
}
//----------------------------------------------------------------------------
inline float ParticleEmitter::GetEmitRotateSpeed () const
{
	return mEmitRotateSpeed;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetCollisionType (CollisionType type)
{
	mCollisionType = type;
}
//----------------------------------------------------------------------------
inline ParticleEmitter::CollisionType ParticleEmitter::GetCollisionType () const
{
	return mCollisionType;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetCollisionOption (CollisionOption type)
{
	mCollisionOption = type;
}
//----------------------------------------------------------------------------
inline ParticleEmitter::CollisionOption ParticleEmitter::GetCollisionOption () const
{
	return mCollisionOption;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetCollisionFaceHeight (float height)
{
	mCollisionHeight = height;
}
//----------------------------------------------------------------------------
inline float ParticleEmitter::GetCollisionFaceHeight () const
{
	return mCollisionHeight;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetCollisionSpeedPercent (float percent)
{
	mCollisionSpeedPercent = percent;
}
//----------------------------------------------------------------------------
inline float ParticleEmitter::GetCollisionSpeedPercent () const
{
	return mCollisionSpeedPercent;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetModelFilename (const std::string &filename)
{
	mModelFilename = filename;
}
//----------------------------------------------------------------------------
inline const std::string &ParticleEmitter::GetModelFilename () const
{
	return mModelFilename;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetEmitDirDynamic (bool dyn)
{
	mIsEmitDirDynamic = dyn;
}
//----------------------------------------------------------------------------
inline bool ParticleEmitter::IsEmitDirDynamic () const
{
	return mIsEmitDirDynamic;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetEmitDir (const AVector &dir)
{
	mEmitDir = dir;
	mEmitDir.Normalize();
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetEmitDirLocal (bool isLocal)
{
	mIsEmitDirLocal = isLocal;
}
//----------------------------------------------------------------------------
inline bool ParticleEmitter::IsEmitDirLocal () const
{
	return mIsEmitDirLocal;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetEmitDirType (EmitDirType type)
{
	mEmitDirType = type;
}
//----------------------------------------------------------------------------
inline ParticleEmitter::EmitDirType ParticleEmitter::GetEmitDirType () const
{
	return mEmitDirType;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetEmitSpeed (float speed)
{
	mEmitSpeed = speed;
}
//----------------------------------------------------------------------------
inline float ParticleEmitter::GetEmitSpeed () const
{
	return mEmitSpeed;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetObstruct (const AVector &obs)
{
	mObstruct = obs;
}
//----------------------------------------------------------------------------
inline const AVector &ParticleEmitter::GetObstruct () const
{
	return mObstruct;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetEmitAccelerateDir (const AVector &dir)
{
	mAccelerateDir = dir;
}
//----------------------------------------------------------------------------
inline const AVector &ParticleEmitter::GetEmitAccelerateDir () const
{
	return mAccelerateDir;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetEmitAccelerate (float val)
{
	mAccelerate = val;
}
//----------------------------------------------------------------------------
inline float ParticleEmitter::GetEmitAccelerate () const
{
	return mAccelerate;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetEmitAttractPoint (const APoint &pos)
{
	mAttactPoint = pos;
}
//----------------------------------------------------------------------------
inline const APoint &ParticleEmitter::GetEmitAttactPoint () const
{
	return mAttactPoint;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetEmitAttactStrength (const AVector &strength)
{
	mAttackStrength = strength;
}
//----------------------------------------------------------------------------
inline const AVector &ParticleEmitter::GetEmitAttackStrength () const
{
	return mAttackStrength;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetEmitRate (float rate)
{
	mEmitRate = rate;
}
//----------------------------------------------------------------------------
inline float ParticleEmitter::GetEmitRate () const
{
	return mEmitRate;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetPlacerType (PlacerType type)
{
	mPlacerType = type;
}
//----------------------------------------------------------------------------
inline ParticleEmitter::PlacerType ParticleEmitter::GetPlacerType () const
{
	return mPlacerType;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetPlacerInLength (float val)
{
	mPlacerInLength = val;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetPlacerOutLength (float val)
{
	mPlacerOutLength = val;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetPlacerInWidth (float val)
{
	mPlacerInWidth = val;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetPlacerOutWidth (float val)
{
	mPlacerOutWidth = val;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetPlacerInHeight (float val)
{
	mPlacerInHeight = val;
}
//----------------------------------------------------------------------------
inline void ParticleEmitter::SetPlacerOutHeight (float val)
{
	mPlacerOutHeight = val;
}
//----------------------------------------------------------------------------
inline float ParticleEmitter::GetPlacerInLength () const
{
	return mPlacerInLength;
}
//----------------------------------------------------------------------------
inline float ParticleEmitter::GetPlacerOutLength () const
{
	return mPlacerOutLength;
}
//----------------------------------------------------------------------------
inline float ParticleEmitter::GetPlacerInWidth () const
{
	return mPlacerInWidth;
}
//----------------------------------------------------------------------------
inline float ParticleEmitter::GetPlacerOutWidth () const
{
	return mPlacerOutWidth;
}
//----------------------------------------------------------------------------
inline float ParticleEmitter::GetPlacerInHeight () const
{
	return mPlacerInHeight;
}
//----------------------------------------------------------------------------
inline float ParticleEmitter::GetPlacerOutHeight () const
{
	return mPlacerOutHeight;
}
//----------------------------------------------------------------------------