// PX2TargetTransformController.inl

//----------------------------------------------------------------------------
inline APoint &TargetTransformController::GetStartPos ()
{
	return mStartPos;
}
//----------------------------------------------------------------------------
inline APoint &TargetTransformController::GetTargetPos ()
{
	return mTargetPos;
}
//----------------------------------------------------------------------------
inline Movable *TargetTransformController::GetTarget () const
{
	return mTarget;
}
//----------------------------------------------------------------------------
inline void TargetTransformController::SetSpeed (float speed)
{
	mSpeed = speed;
}
//----------------------------------------------------------------------------
inline float TargetTransformController::GetSpeed () const
{
	return mSpeed;
}
//----------------------------------------------------------------------------
inline void TargetTransformController::SetTolerance (float tolerance)
{
	mTolerance = tolerance;
}
//----------------------------------------------------------------------------
inline float TargetTransformController::GetTolerance () const
{
	return mTolerance;
}
//----------------------------------------------------------------------------
inline bool TargetTransformController::IsPlaying () const
{
	return mIsPlaying;
}
//----------------------------------------------------------------------------
inline TTCArriveAtTargetCallback TargetTransformController::GetArriveAtTargetCallback ()
{
	return mArriveAtTargetCallback;
}
//----------------------------------------------------------------------------