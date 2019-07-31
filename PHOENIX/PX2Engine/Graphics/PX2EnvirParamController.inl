// EnvirParamController.inl

//----------------------------------------------------------------------------
inline Light *EnvirParamController::GetLight_Dir()
{
	return mLight_Dir;
}
//----------------------------------------------------------------------------
inline Projector *EnvirParamController::GetLight_Dir_Projector()
{
	return mLight_Dir_Projector;
}
//----------------------------------------------------------------------------
inline Texture *EnvirParamController::GetLight_Dir_DepthTexture()
{
	return mLight_Dir_DepthTexture;
}
//----------------------------------------------------------------------------
inline void EnvirParamController::SetShadowMap_OffsetPropertyScale(float scale)
{
	mShadowOffsetProperty_Scale = scale;
}
//----------------------------------------------------------------------------
inline float EnvirParamController::GetShadowMap_OffsetPropertyScale() const
{
	return mShadowOffsetProperty_Scale;
}
//----------------------------------------------------------------------------
inline void EnvirParamController::SetShadowMap_OffsetPropertyBias(float bias)
{
	mShadowOffsetProperty_Bias = bias;
}
//----------------------------------------------------------------------------
inline float EnvirParamController::GetShadowMap_OffsetPropertyBias() const
{
	return mShadowOffsetProperty_Bias;
}
//----------------------------------------------------------------------------
inline void EnvirParamController::SetFogParam(const Float4 &param)
{
	mFogParam = param;
}
//----------------------------------------------------------------------------
inline const Float4 &EnvirParamController::GetFogParam() const
{
	return mFogParam;
}
//----------------------------------------------------------------------------
inline void EnvirParamController::SetFogColorHeight(const Float4 &fogColor)
{
	mFogColorHeightLast = fogColor;
}
//----------------------------------------------------------------------------
inline const Float4 &EnvirParamController::GetFogColorHeight() const
{
	return mFogColorHeightLast;
}
//----------------------------------------------------------------------------
inline void EnvirParamController::SetFogColorDist(const Float4 &fogColor)
{
	mFogColorDistLast = fogColor;
}
//----------------------------------------------------------------------------
inline const Float4 &EnvirParamController::GetFogColorDist() const
{
	return mFogColorDistLast;
}
//----------------------------------------------------------------------------
inline bool EnvirParamController::IsUseShadowMap() const
{
	return mIsUseShadowMap;
}
//----------------------------------------------------------------------------
inline const Sizef &EnvirParamController::GetShadowRenderTargetSize() const
{
	return mShadowRenderTargetSize;
}
//----------------------------------------------------------------------------
inline bool EnvirParamController::IsShadowRenderTargetSizeSameWithCanvas() const
{
	return mIsShadowRenderTargetSizeSameWithCanvas;
}
//----------------------------------------------------------------------------
inline bool EnvirParamController::IsUseBloom() const
{
	return mIsUseBloom;
}
//----------------------------------------------------------------------------
inline float EnvirParamController::GetBloomBrightWeight() const
{
	return mBloomBrightWeight;
}
//----------------------------------------------------------------------------
inline float EnvirParamController::GetBloomBlurDeviation() const
{
	return mBloomBlurDeviation;
}
//----------------------------------------------------------------------------
inline float EnvirParamController::GetBloomBlurWeight() const
{
	return mBloomBlurWeight;
}
//----------------------------------------------------------------------------
inline float EnvirParamController::GetBloomWeight() const
{
	return mBloomWeight;
}
//----------------------------------------------------------------------------
inline const Float4 &EnvirParamController::GetBloomBrightParam() const
{
	return mBloomBrightParam;
}
//----------------------------------------------------------------------------
inline const Float4 &EnvirParamController::GetBloomParam() const
{
	return mBloomParam;
}
//----------------------------------------------------------------------------