// PX2CameraNode.inl

//----------------------------------------------------------------------------
inline Camera* CameraNode::GetCamera ()
{
	return mCamera;
}
//----------------------------------------------------------------------------
inline const Camera* CameraNode::GetCamera () const
{
	return mCamera;
}
//----------------------------------------------------------------------------
inline void CameraNode::EnableUpdate (bool enable)
{
	mEnableUpdate = enable;
}
//----------------------------------------------------------------------------
inline bool CameraNode::IsEnableUpdate ()
{
	return mEnableUpdate;
}
//----------------------------------------------------------------------------
