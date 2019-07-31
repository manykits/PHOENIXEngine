// PX2Movable.inl

//----------------------------------------------------------------------------
inline Transform &Movable::GetLocalTransform()
{
	return LocalTransform;
}
//----------------------------------------------------------------------------
inline Transform &Movable::GetWorldTransform()
{
	return WorldTransform;
}
//----------------------------------------------------------------------------
inline void Movable::SetAlphaSelfCtrled (bool selfCtrled)
{
	mIsAlphaSelfCtrled = selfCtrled;
}
//----------------------------------------------------------------------------
inline void Movable::SetColorSelfCtrled (bool selfCtrled)
{
	mIsColorSelfCtrled = selfCtrled;
}
//----------------------------------------------------------------------------
inline bool Movable::IsAlphaSelfCtrled () const
{
	return mIsAlphaSelfCtrled;
}
//----------------------------------------------------------------------------
inline float Movable::GetAlpha () const
{
	return mAlpha;
}
//----------------------------------------------------------------------------
inline bool Movable::IsColorSelfCtrled () const
{
	return mIsColorSelfCtrled;
}
//----------------------------------------------------------------------------
inline const Float3 &Movable::GetColor () const
{
	return mColor;
}
//----------------------------------------------------------------------------
inline void Movable::SetBrightnessSelfCtrled (bool selfCtrled)
{
	mIsBrightnessSelfCtrled = selfCtrled;
}
//----------------------------------------------------------------------------
inline bool Movable::IsBrightnessSelfCtrled () const
{
	return mIsBrightnessSelfCtrled;
}
//----------------------------------------------------------------------------
inline float Movable::GetBrightness () const
{
	return mBrightness;
}
//----------------------------------------------------------------------------
inline const Float3 &Movable::GetLastColor() const
{
	return mLastColor;
}
//----------------------------------------------------------------------------
inline float Movable::GetLastAlpha() const
{
	return mLastAlpha;
}
//----------------------------------------------------------------------------
inline float Movable::GetLastBrightness() const
{
	return mLastBrightness;
}
//----------------------------------------------------------------------------
inline void Movable::SetEnableSelfCtrled(bool selfCtrled)
{
	mIsEnableSelfCtrled = selfCtrled;
}
//----------------------------------------------------------------------------
inline bool Movable::IsEnableSelfCtrled() const
{
	return mIsEnableSelfCtrled;
}
//----------------------------------------------------------------------------
inline void Movable::SetActivateSelfCtrled(bool selfCtrled)
{
	mIsActivateSelfCtrled = selfCtrled;
}
//----------------------------------------------------------------------------
inline bool Movable::IsActivateSelfCtrled() const
{
	return mIsActivateSelfCtrled;
}
//----------------------------------------------------------------------------
inline Movable* Movable::GetParent ()
{
    return mParent;
}
//----------------------------------------------------------------------------
inline void Movable::SetFixUpdateTime (float time)
{
	mUpdateTime = time;
	mUpdateTiming = mUpdateTime;
}
//----------------------------------------------------------------------------
inline float Movable::GetFixUpdateTime () const
{
	return mUpdateTime;
}
//----------------------------------------------------------------------------
inline void Movable::ResetFixUpdateTiming()
{
	mUpdateTiming = mUpdateTime;
}
//----------------------------------------------------------------------------
inline void Movable::SetOnlyShowUpdate(bool onlyShowUpdate)
{
	mIsOnlyShowUpdate = onlyShowUpdate;
}
//----------------------------------------------------------------------------
inline bool Movable::IsOnlyShowUpdate() const
{
	return mIsOnlyShowUpdate;
}
//----------------------------------------------------------------------------
inline int Movable::GetUpdatePriority () const
{
	return mUpdatePriority;
}
//----------------------------------------------------------------------------
inline bool Movable::IsDoPick () const
{
	return mIsDoPick;
}
//----------------------------------------------------------------------------
inline void Movable::SetPickIngoreCullingMode (bool pickIngoreCullingMode)
{
	mIsPickIngoreCullingMode = pickIngoreCullingMode;
}
//----------------------------------------------------------------------------
inline bool Movable::IsPickIngoreCullingMode () const
{
	return mIsPickIngoreCullingMode;
}
//----------------------------------------------------------------------------
inline void Movable::SetNotPickedParentChildrenNotPicked (bool use)
{
	mIsNotPickedParentChildrenNotPicked = use;
}
//----------------------------------------------------------------------------
inline bool Movable::IsNotPickedParentChildrenNotPicked () const
{
	return mIsNotPickedParentChildrenNotPicked;
}
//----------------------------------------------------------------------------
inline void Movable::SetSaveWriteIngore(bool doSaveWrite)
{
	mIsSaveWriteIngore = doSaveWrite;
}
//----------------------------------------------------------------------------
inline bool Movable::IsSaveWriteIngore() const
{
	return mIsSaveWriteIngore;
}
//----------------------------------------------------------------------------
inline bool Movable::IsCastShadow() const
{
	return mIsCastShadow;
}
//----------------------------------------------------------------------------
inline bool Movable::IsReceiveShadow() const
{
	return mIsReceiveShadow;
}
//----------------------------------------------------------------------------
template <typename CLASSTYPE>
CLASSTYPE *Movable::GetFirstParentDerivedFromType(int *numLevels)
{
	int numLev = 0;
	Movable *topestParentTemp = mParent;
	Movable *topestParent = mParent;

	while (topestParentTemp)
	{
		topestParent = topestParentTemp;
		numLev++;

		if (topestParent->IsDerived(CLASSTYPE::TYPE))
		{
			if (numLevels)
				*numLevels = numLev;

			return (CLASSTYPE*)topestParent;
		}

		topestParentTemp = topestParentTemp->GetParent();
	}

	if (numLevels)
		*numLevels = 0;

	return 0;
}
//----------------------------------------------------------------------------