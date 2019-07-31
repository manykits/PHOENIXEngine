// PX2AmbientRegionController.inl

//----------------------------------------------------------------------------
inline float AmbientRegionController::GetHorAngle() const
{
	return mHorAngle;
}
//----------------------------------------------------------------------------
inline float AmbientRegionController::GetVerAngle() const
{
	return mVerAngle;
}
//----------------------------------------------------------------------------
inline const Float3 &AmbientRegionController::GetAmbientColor() const
{
	return mAmbientColor;
}
//----------------------------------------------------------------------------
inline const Float3 &AmbientRegionController::GetDiffuseColor() const
{
	return mDiffuseColor;
}
//----------------------------------------------------------------------------
inline const Float3 &AmbientRegionController::GetSpecularColor() const
{
	return mSpecularColor;
}
//----------------------------------------------------------------------------
inline float AmbientRegionController::GetSpecularPow() const
{
	return mSpecularPow;
}
//----------------------------------------------------------------------------
inline float AmbientRegionController::GetIntensity() const
{
	return mIntensity;
}
//----------------------------------------------------------------------------
inline const Float3 &AmbientRegionController::GetFogColorHeight() const
{
	return mFogColorHeight;
}
//----------------------------------------------------------------------------
inline const Float2 &AmbientRegionController::GetFogParamHeight() const
{
	return mFogParamHeight;
}
//----------------------------------------------------------------------------
inline const Float3 &AmbientRegionController::GetFogColorDistance() const
{
	return mFogColorDist;
}
//----------------------------------------------------------------------------
inline const Float2 &AmbientRegionController::GetFogParamDistance() const
{
	return mFogParamDist;
}
//----------------------------------------------------------------------------