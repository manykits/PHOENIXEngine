// PX2Triangles.inl

//----------------------------------------------------------------------------
inline int Triangles::GetNumVertices () const
{
	return mVBuffer->GetNumElements();
}
//----------------------------------------------------------------------------
inline const Transform& Triangles::GetWorldTransform () const
{
	return WorldTransform;
}
//----------------------------------------------------------------------------
