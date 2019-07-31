// PX2Culler.inl

//----------------------------------------------------------------------------
inline void Culler::SetFlag_CastShadow(int cast)
{
	mFlag_CastShadow = cast;
}
//----------------------------------------------------------------------------
inline int Culler::GetFlag_CastShadow() const
{
	return mFlag_CastShadow;
}
//----------------------------------------------------------------------------
inline void Culler::SetCamera (const Camera* camera)
{
	mCamera = camera;
}
//----------------------------------------------------------------------------
inline const Camera* Culler::GetCamera () const
{
	return mCamera;
}
//----------------------------------------------------------------------------
inline const float* Culler::GetFrustum () const
{
	return mFrustum;
}
//----------------------------------------------------------------------------
inline VisibleSet& Culler::GetVisibleSet()
{
	return mVisibleSet;
}
//----------------------------------------------------------------------------
inline int Culler::GetPlaneQuantity () const
{
	return mPlaneQuantity;
}
//----------------------------------------------------------------------------
inline const HPlane* Culler::GetPlanes () const
{
	return mPlane;
}
//----------------------------------------------------------------------------
inline void Culler::SetPlaneState (unsigned int planeState)
{
	mPlaneState = planeState;
}
//----------------------------------------------------------------------------
inline unsigned int Culler::GetPlaneState () const
{
	return mPlaneState;
}
//----------------------------------------------------------------------------
inline void Culler::PushPlane (const HPlane& plane)
{
	if (mPlaneQuantity < MAX_PLANE_QUANTITY)
	{
		mPlane[mPlaneQuantity] = plane;
		++mPlaneQuantity;
	}
}
//----------------------------------------------------------------------------
inline void Culler::PopPlane ()
{
	if (mPlaneQuantity > Camera::VF_QUANTITY)
	{
		--mPlaneQuantity;
	}
}
//----------------------------------------------------------------------------
inline Movable *Culler::GetCurCullingScene()
{
	return mCurCullingScene;
}
//----------------------------------------------------------------------------