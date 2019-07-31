// PX2Renderer.inl

//----------------------------------------------------------------------------
inline const std::string &Renderer::GetName() const
{
	return mName;
}
//----------------------------------------------------------------------------
inline int Renderer::GetWidth () const
{
	return mWidth;
}
//----------------------------------------------------------------------------
inline int Renderer::GetHeight () const
{
	return mHeight;
}
//----------------------------------------------------------------------------
inline Texture::Format Renderer::GetColorFormat () const
{
	return mColorFormat;
}
//----------------------------------------------------------------------------
inline Texture::Format Renderer::GetDepthStencilFormat () const
{
	return mDepthStencilFormat;
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumMultisamples () const
{
	return mNumMultisamples;
}
//----------------------------------------------------------------------------
inline const AlphaProperty* Renderer::GetAlphaProperty () const
{
	return mAlphaProperty;
}
//----------------------------------------------------------------------------
inline const CullProperty* Renderer::GetCullProperty () const
{
	return mCullProperty;
}
//----------------------------------------------------------------------------
inline const DepthProperty* Renderer::GetDepthProperty () const
{
	return mDepthProperty;
}
//----------------------------------------------------------------------------
inline const OffsetProperty* Renderer::GetOffsetProperty () const
{
	return mOffsetProperty;
}
//----------------------------------------------------------------------------
inline const StencilProperty* Renderer::GetStencilProperty () const
{
	return mStencilProperty;
}
//----------------------------------------------------------------------------
inline const WireProperty* Renderer::GetWireProperty () const
{
	return mWireProperty;
}
//----------------------------------------------------------------------------
inline void Renderer::SetOverrideAlphaProperty (const AlphaProperty* alphaState)
{
	mOverrideAlphaProperty = alphaState;
	if (alphaState)
	{
		SetAlphaProperty(alphaState);
	}
	else
	{
		SetAlphaProperty(mDefaultAlphaProperty);
	}
}
//----------------------------------------------------------------------------
inline void Renderer::SetOverrideCullProperty (const CullProperty* cullState)
{
	mOverrideCullProperty = cullState;
	if (cullState)
	{
		SetCullProperty(cullState);
	}
	else
	{
		SetCullProperty(mDefaultCullProperty);
	}
}
//----------------------------------------------------------------------------
inline void Renderer::SetOverrideDepthProperty (const DepthProperty* depthState)
{
	mOverrideDepthProperty = depthState;
	if (depthState)
	{
		SetDepthProperty(depthState);
	}
	else
	{
		SetDepthProperty(mDefaultDepthProperty);
	}
}
//----------------------------------------------------------------------------
inline void Renderer::SetOverrideOffsetProperty (const OffsetProperty* offsetState)
{
	mOverrideOffsetProperty = offsetState;
	if (offsetState)
	{
		SetOffsetProperty(offsetState);
	}
	else
	{
		SetOffsetProperty(mDefaultOffsetProperty);
	}
}
//----------------------------------------------------------------------------
inline void Renderer::SetOverrideStencilProperty (const StencilProperty* stencilState)
{
	mOverrideStencilProperty = stencilState;
	if (stencilState)
	{
		SetStencilProperty(stencilState);
	}
	else
	{
		SetStencilProperty(mDefaultStencilProperty);
	}
}
//----------------------------------------------------------------------------
inline void Renderer::SetOverrideWireProperty (const WireProperty* wireState)
{
	mOverrideWireProperty = wireState;
	if (wireState)
	{
		SetWireProperty(wireState);
	}
	else
	{
		SetWireProperty(mDefaultWireProperty);
	}
}
//----------------------------------------------------------------------------
inline const AlphaProperty* Renderer::GetOverrideAlphaProperty () const
{
	return mOverrideAlphaProperty;
}
//----------------------------------------------------------------------------
inline const CullProperty* Renderer::GetOverrideCullProperty () const
{
	return mOverrideCullProperty;
}
//----------------------------------------------------------------------------
inline const DepthProperty* Renderer::GetOverrideDepthProperty () const
{
	return mOverrideDepthProperty;
}
//----------------------------------------------------------------------------
inline const OffsetProperty* Renderer::GetOverrideOffsetProperty () const
{
	return mOverrideOffsetProperty;
}
//----------------------------------------------------------------------------
inline const StencilProperty* Renderer::GetOverrideStencilProperty () const
{
	return mOverrideStencilProperty;
}
//----------------------------------------------------------------------------
inline const WireProperty* Renderer::GetOverrideWireProperty () const
{
	return mOverrideWireProperty;
}
//----------------------------------------------------------------------------
inline void Renderer::SetReverseCullOrder (bool reverseCullOrder)
{
	mReverseCullOrder = reverseCullOrder;
}
//----------------------------------------------------------------------------
inline bool Renderer::GetReverseCullOrder () const
{
	return mReverseCullOrder;
}
//----------------------------------------------------------------------------
inline void Renderer::SetCamera (Camera* camera)
{
	mCamera = camera;
}
//----------------------------------------------------------------------------
inline Camera* Renderer::GetCamera ()
{
	return mCamera;
}
//----------------------------------------------------------------------------
inline const Camera* Renderer::GetCamera () const
{
	return mCamera;
}
//----------------------------------------------------------------------------
inline const HMatrix& Renderer::GetViewMatrix () const
{
	return mCamera->GetViewMatrix();
}
//----------------------------------------------------------------------------
inline const HMatrix& Renderer::GetProjectionMatrix () const
{
	return mCamera->GetProjectionMatrix();
}
//----------------------------------------------------------------------------
inline const HMatrix& Renderer::GetPostProjectionMatrix () const
{
	return mCamera->GetPostProjectionMatrix();
}
//----------------------------------------------------------------------------
inline const Rectf &Renderer::GetViewPort() const
{
	return mViewPort;
}
//----------------------------------------------------------------------------
inline void Renderer::SetClearColor (const Float4& clearColor)
{
	mClearColor = clearColor;
}
//----------------------------------------------------------------------------
inline const Float4& Renderer::GetClearColor () const
{
	return mClearColor;
}
//----------------------------------------------------------------------------
inline void Renderer::SetClearDepth (float clearDepth)
{
	mClearDepth = clearDepth;
}
//----------------------------------------------------------------------------
inline float Renderer::GetClearDepth () const
{
	return mClearDepth;
}
//----------------------------------------------------------------------------
inline void Renderer::SetClearStencil (unsigned int clearStencil)
{
	mClearStencil = clearStencil;
}
//----------------------------------------------------------------------------
inline unsigned int Renderer::GetClearStencil () const
{
	return mClearStencil;
}
//----------------------------------------------------------------------------
inline void Renderer::GetColorMask (bool& allowRed, bool& allowGreen,
									bool& allowBlue, bool& allowAlpha) const
{
	allowRed = mAllowRed;
	allowGreen = mAllowGreen;
	allowBlue = mAllowBlue;
	allowAlpha = mAllowAlpha;
}
//----------------------------------------------------------------------------
inline bool Renderer::InTexture2DMap (const Texture2D* texture)
{
	return mTexture2Ds.find(texture) != mTexture2Ds.end();
}
//----------------------------------------------------------------------------
inline void Renderer::InsertInTexture2DMap (const Texture2D* texture,
											PdrTexture2D* platformTexture)
{
	mTexture2Ds[texture] = platformTexture;
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumPdrVertexFormat () const
{
	return (int)mVertexFormats.size();
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumPdrVertexBuffer () const
{
	return (int)mVertexBuffers.size();
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumPdrIndexBuffer () const
{
	return (int)mIndexBuffers.size();
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumPdrTexture1D () const
{
	return (int)mTexture1Ds.size();
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumPdrTexture2D () const
{
	return (int)mTexture2Ds.size();
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumPdrTexture3D () const
{
	return (int)mTexture3Ds.size();
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumPdrTextureCube () const
{
	return (int)mTextureCubes.size();
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumPdrRenderTarget () const
{
	return (int)mRenderTargets.size();
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumPdrVertexShader () const
{
	return (int)mVertexShaders.size();
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumPdrPixelShader () const
{
	return (int)mPixelShaders.size();
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumPdrMaterialPass () const
{
	return (int)mMaterialPasses.size();
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumGPUVertexShader () const
{
	return (int)mSharePdrVertexShaders.size();
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumGPUPixelShader () const
{
	return (int)mSharePdrPixelShaders.size();
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumGPUMaterialPass () const
{
	return (int)mSharePdrMaterialPasses.size();
}
//----------------------------------------------------------------------------
inline int Renderer::GetNumDrawPrimitivePerFrame () const
{
	return mNumDrawPrimitivePerFrame;
}
//----------------------------------------------------------------------------