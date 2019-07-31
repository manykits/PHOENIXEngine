// PX2PushTransformController.inl

//----------------------------------------------------------------------------
inline void PushTransformController::SetTransType (TransType transType)
{
	mTransType = transType;
}
//----------------------------------------------------------------------------
inline PushTransformController::TransType PushTransformController::
	GetTransType ()
{
	return mTransType;
}
//----------------------------------------------------------------------------
inline void PushTransformController::SetMaxVelocity (float forceMax)
{
	mMaxVelocity = forceMax;
}
//----------------------------------------------------------------------------
inline float PushTransformController::GetMaxVelocity ()
{
	return mMaxVelocity;
}
//----------------------------------------------------------------------------
inline const AVector &PushTransformController::GetVelocity () const
{
	return mVelocity;
}
//----------------------------------------------------------------------------
inline void PushTransformController::SetFriction (float fricton)
{
	mFriction = fricton;
}
//----------------------------------------------------------------------------
inline float PushTransformController::GetFriction ()
{
	return mFriction;
}
//----------------------------------------------------------------------------
inline void PushTransformController::SetXY (bool isXY)
{
	mIsXY = isXY;
}
//----------------------------------------------------------------------------
inline bool PushTransformController::IsXY () const
{
	return mIsXY;
}
//----------------------------------------------------------------------------