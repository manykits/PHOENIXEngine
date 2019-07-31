// PX2Rtti.inl

//----------------------------------------------------------------------------
inline const char* Rtti::GetName () const
{
    return mName;
}
//----------------------------------------------------------------------------
inline unsigned short Rtti::GetVersion () const
{
	return mVersion;
}
//----------------------------------------------------------------------------
inline bool Rtti::IsExactly (const Rtti& type) const
{
    return (&type == this);
}
//----------------------------------------------------------------------------