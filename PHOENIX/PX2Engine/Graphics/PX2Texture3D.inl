// PX2Texture3D.inl

//----------------------------------------------------------------------------
inline int Texture3D::GetWidth () const
{
	return GetDimension(0, 0);
}
//----------------------------------------------------------------------------
inline int Texture3D::GetHeight () const
{
	return GetDimension(1, 0);
}
//----------------------------------------------------------------------------
inline int Texture3D::GetThickness () const
{
	return GetDimension(2, 0);
}
//----------------------------------------------------------------------------
