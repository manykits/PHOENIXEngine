// PX2AnimationController.inl

//----------------------------------------------------------------------------
inline const std::string &ModelController::GetMovableFilename() const
{
	return mMovableFilename;
}
//----------------------------------------------------------------------------
inline bool ModelController::IsShareVI() const
{
	return mIsShareVI;
}
//----------------------------------------------------------------------------
inline Movable *ModelController::GetMovable()
{
	return mMovable;
}
//----------------------------------------------------------------------------
inline const std::string &ModelController::GetSkinAnimationConfig() const
{
	return mSkinAnimationConfig;
}
//----------------------------------------------------------------------------
inline const AVector &ModelController::GetHeading() const
{
	return mHeading;
}
//----------------------------------------------------------------------------
inline void ModelController::SetMovableUseAutoWorldBound(bool isUseAutoWorldBound)
{
	mIsMovableAutoWorldBound = isUseAutoWorldBound;
}
//----------------------------------------------------------------------------
inline bool ModelController::IsMovableUseAutoWorldBound() const
{
	return mIsMovableAutoWorldBound;
}
//----------------------------------------------------------------------------
inline float ModelController::GetMovableAutoWorldBoundRadius() const
{
	return mMovableAutoWorldBoundRadius;
}
//----------------------------------------------------------------------------
inline void ModelController::SetAnimType(AnimType type)
{
	mAnimType = type;
}
//----------------------------------------------------------------------------
inline ModelController::AnimType ModelController::GetAnimType() const
{
	return mAnimType;
}
//----------------------------------------------------------------------------
inline std::map<FString, AnimationPtr> &ModelController::GetAnimsMap()
{
	return mAnimsMap;
}
//----------------------------------------------------------------------------
inline const std::string &ModelController::GetDefaultAnimName() const
{
	return mDefaultAnimName;
}
//----------------------------------------------------------------------------
inline Animation *ModelController::GetDefaultAnim()
{
	return mDefaultAnim;
}
//----------------------------------------------------------------------------
inline Animation *ModelController::GetCurPlayingAnim()
{
	return mCurPlayingAnim;
}
//----------------------------------------------------------------------------