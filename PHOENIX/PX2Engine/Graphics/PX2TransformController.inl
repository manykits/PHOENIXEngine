// PX2TransformController.inl

//----------------------------------------------------------------------------
inline void TransformController::SetWorld (bool isWorld)
{
	mIsWorld = isWorld;
}
//----------------------------------------------------------------------------
inline bool TransformController::IsWorld () const
{
	return mIsWorld;
}
//----------------------------------------------------------------------------
inline void TransformController::SetTransform (const Transform& trans)
{
	mTransform = trans;
}
//----------------------------------------------------------------------------
inline const Transform& TransformController::GetTransform () const
{
	return mTransform;
}
//----------------------------------------------------------------------------