// PX2EffectModelNode.inl

//----------------------------------------------------------------------------
inline const std::string &EffectModelNode::GetModelFilename () const
{
	return mModelFilename;
}
//----------------------------------------------------------------------------
inline EffectModelNode::CoordinateType EffectModelNode::GetCoordinateType0 () const
{
	return mCT0;
}
//----------------------------------------------------------------------------
inline EffectModelNode::CoordinateType EffectModelNode::GetCoordinateType1 () const
{
	return mCT1;
}
//----------------------------------------------------------------------------
inline void EffectModelNode::SetUVSpeed (const Float2 &uv)
{
	mUVSpeed = uv;
}
//----------------------------------------------------------------------------
inline const Float2 &EffectModelNode::GetUVSpeed () const
{
	return mUVSpeed;
}
//----------------------------------------------------------------------------
inline const std::string &EffectModelNode::GetTextureFilename () const
{
	return mTextureFileName;
}
//----------------------------------------------------------------------------