// PX2ShaderParameters.inl

//----------------------------------------------------------------------------
inline int ShaderParameters::GetNumConstants () const
{
    return mNumConstants;
}
//----------------------------------------------------------------------------
inline int ShaderParameters::GetNumTextures () const
{
    return mNumTextures;
}
//----------------------------------------------------------------------------
inline ShaderFloatPtr*ShaderParameters:: GetConstants () const
{
    return mConstants;
}
//----------------------------------------------------------------------------
inline TexturePtr* ShaderParameters::GetTextures () const
{
    return mTextures;
}
//----------------------------------------------------------------------------