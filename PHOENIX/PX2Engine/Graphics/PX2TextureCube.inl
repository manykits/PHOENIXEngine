// PX2TextureCube.inl

//----------------------------------------------------------------------------
inline int TextureCube::GetWidth () const
{
    return GetDimension(0, 0);
}
//----------------------------------------------------------------------------
inline int TextureCube::GetHeight () const
{
    return GetDimension(1, 0);
}
//----------------------------------------------------------------------------