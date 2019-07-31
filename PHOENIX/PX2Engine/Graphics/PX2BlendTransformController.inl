// PX2BlendTransformController.inl

//----------------------------------------------------------------------------
inline TransformController* BlendTransformController::GetController0 () const
{
	return mController0;
}
//----------------------------------------------------------------------------
inline TransformController* BlendTransformController::GetController1 () const
{
	return mController1;
}
//----------------------------------------------------------------------------
inline bool BlendTransformController::GetRSMatrices () const
{
	return mRSMatrices;
}
//----------------------------------------------------------------------------
inline void BlendTransformController::SetWeight (float weight)
{
	mWeight = weight;
}
//----------------------------------------------------------------------------
inline float BlendTransformController::GetWeight () const
{
	return mWeight;
}
//----------------------------------------------------------------------------
