// PX2VertexFormat.inl

//----------------------------------------------------------------------------
inline int VertexFormat::GetNumAttributes () const
{
	return mNumAttributes;
}
//----------------------------------------------------------------------------
inline unsigned int VertexFormat::GetStreamIndex (int attribute) const
{
	if (0 <= attribute && attribute < mNumAttributes)
	{
		return mElements[attribute].StreamIndex;
	}
	else
	{
		assertion(false, "Invalid index in GetStreamIndex\n");
		return 0;
	}
}
//----------------------------------------------------------------------------
inline unsigned int VertexFormat::GetOffset (int attribute) const
{
	if (0 <= attribute && attribute < mNumAttributes)
	{
		return mElements[attribute].Offset;
	}
	else
	{
		assertion(false, "Invalid index in GetOffset\n");
		return 0;
	}
}
//----------------------------------------------------------------------------
inline VertexFormat::AttributeType VertexFormat::GetAttributeType (
	int attribute) const
{
	if (0 <= attribute && attribute < mNumAttributes)
	{
		return mElements[attribute].Type;
	}
	else
	{
		assertion(false, "Invalid index in GetAttributeType\n");
		return AT_NONE;
	}
}
//----------------------------------------------------------------------------
inline VertexFormat::AttributeUsage VertexFormat::GetAttributeUsage (
	int attribute) const
{
	if (0 <= attribute && attribute < mNumAttributes)
	{
		return mElements[attribute].Usage;
	}
	else
	{
		assertion(false, "Invalid index in GetAttributeUsage\n");
		return AU_NONE;
	}
}
//----------------------------------------------------------------------------
inline unsigned int VertexFormat::GetUsageIndex (int attribute) const
{
	if (0 <= attribute && attribute < mNumAttributes)
	{
		return mElements[attribute].UsageIndex;
	}
	else
	{
		assertion(false, "Invalid index in GetUsageIndex\n");
		return 0;
	}
}
//----------------------------------------------------------------------------
inline int VertexFormat::GetStride () const
{
	return mStride;
}
//----------------------------------------------------------------------------
inline int VertexFormat::GetComponentSize (AttributeType type)
{
	return msComponentSize[type];
}
//----------------------------------------------------------------------------
inline int VertexFormat::GetNumComponents (AttributeType type)
{
	return msNumComponents[type];
}
//----------------------------------------------------------------------------
inline int VertexFormat::GetTypeSize (AttributeType type)
{
	return msTypeSize[type];
}
//----------------------------------------------------------------------------