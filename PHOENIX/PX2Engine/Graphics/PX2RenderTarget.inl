// PX2RenderTarget.inl

//----------------------------------------------------------------------------
inline int RenderTarget::GetNumTargets () const
{
	return mNumTargets;
}
//----------------------------------------------------------------------------
inline Texture::Format RenderTarget::GetFormat () const
{
	return mFormat;
}
//----------------------------------------------------------------------------
inline int RenderTarget::GetWidth () const
{
	return mWidth;
}
//----------------------------------------------------------------------------
inline int RenderTarget::GetHeight () const
{
	return mHeight;
}
//----------------------------------------------------------------------------
inline Texture2D* RenderTarget::GetColorTexture (int i) const
{
	return mColorTextures[i];
}
//----------------------------------------------------------------------------
inline Texture2D* RenderTarget::GetDepthStencilTexture () const
{
	return mDepthStencilTexture;
}
//----------------------------------------------------------------------------
inline bool RenderTarget::HasMipmaps () const
{
	return mHasMipmaps;
}
//----------------------------------------------------------------------------
inline bool RenderTarget::HasDepthStencil () const
{
	return mHasDepthStencil;
}
//----------------------------------------------------------------------------
inline bool RenderTarget::IsOnlyDepth16NoStencil() const
{
	return mIsOnlyDepth16NoStencil;
}
//----------------------------------------------------------------------------