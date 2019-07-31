// PX2EffectModel.inl

//----------------------------------------------------------------------------
inline EffectModel::ModelType EffectModel::GetModelType () const
{
	return mModelType;
}
//----------------------------------------------------------------------------
inline int EffectModel::GetZSample () const
{
	return mZSample;
}
//----------------------------------------------------------------------------
inline int EffectModel::GetRadiusSample () const
{
	return mRadiusSample;
}
//----------------------------------------------------------------------------
inline EffectModel::HeightAlphaType EffectModel::GetHeightAlphaType () const
{
	return mHeightAlphaType;
}
//----------------------------------------------------------------------------
inline const std::string &EffectModel::GetModelFilename () const
{
	return mModelFilename;
}
//----------------------------------------------------------------------------