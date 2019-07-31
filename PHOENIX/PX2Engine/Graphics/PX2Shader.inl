// PX2Shader.inl

//----------------------------------------------------------------------------
inline int Shader::GetNumInputs () const
{
	return mNumInputs;
}
//----------------------------------------------------------------------------
inline int Shader::GetNumOutputs () const
{
	return mNumOutputs;
}
//----------------------------------------------------------------------------
inline int Shader::GetNumConstants () const
{
	return mNumConstants;
}
//----------------------------------------------------------------------------
inline int Shader::GetNumSamplers () const
{
	return mNumSamplers;
}
//----------------------------------------------------------------------------
inline void Shader::SetShaderKey (int key)
{
	mShaderKey = key;
}
//----------------------------------------------------------------------------
inline int Shader::GetShaderKey () const
{
	return mShaderKey;
}
//----------------------------------------------------------------------------