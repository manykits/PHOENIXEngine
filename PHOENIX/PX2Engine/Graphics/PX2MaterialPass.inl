// PX2MaterialPass.inl

//----------------------------------------------------------------------------
inline void MaterialPass::SetVertexShader (VertexShader* vshader)
{
	mVShader = vshader;
}
//----------------------------------------------------------------------------
inline void MaterialPass::SetPixelShader (PixelShader* pshader)
{
	mPShader = pshader;
}
//----------------------------------------------------------------------------
inline void MaterialPass::SetAlphaProperty (AlphaProperty* alphaState)
{
	mAlphaProperty = alphaState;
}
//----------------------------------------------------------------------------
inline void MaterialPass::SetCullProperty (CullProperty* cullState)
{
	mCullProperty = cullState;
}
//----------------------------------------------------------------------------
inline void MaterialPass::SetDepthProperty (DepthProperty* depthState)
{
	mDepthProperty = depthState;
}
//----------------------------------------------------------------------------
inline void MaterialPass::SetOffsetProperty (OffsetProperty* offsetState)
{
	mOffsetProperty = offsetState;
}
//----------------------------------------------------------------------------
inline void MaterialPass::SetStencilProperty (StencilProperty* stencilState)
{
	mStencilProperty = stencilState;
}
//----------------------------------------------------------------------------
inline void MaterialPass::SetWireProperty (WireProperty* wireState)
{
	mWireProperty = wireState;
}
//----------------------------------------------------------------------------
inline VertexShader* MaterialPass::GetVertexShader () const
{
	return mVShader;
}
//----------------------------------------------------------------------------
inline PixelShader* MaterialPass::GetPixelShader () const
{
	return mPShader;
}
//----------------------------------------------------------------------------
inline AlphaProperty* MaterialPass::GetAlphaProperty () const
{
	return mAlphaProperty;
}
//----------------------------------------------------------------------------
inline CullProperty* MaterialPass::GetCullProperty () const
{
	return mCullProperty;
}
//----------------------------------------------------------------------------
inline DepthProperty* MaterialPass::GetDepthProperty () const
{
	return mDepthProperty;
}
//----------------------------------------------------------------------------
inline OffsetProperty* MaterialPass::GetOffsetProperty () const
{
	return mOffsetProperty;
}
//----------------------------------------------------------------------------
inline StencilProperty* MaterialPass::GetStencilProperty () const
{
	return mStencilProperty;
}
//----------------------------------------------------------------------------
inline WireProperty* MaterialPass::GetWireProperty () const
{
	return mWireProperty;
}
//----------------------------------------------------------------------------
