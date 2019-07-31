// PX2Camera.inl

//----------------------------------------------------------------------------
inline APoint Camera::GetPosition () const
{
	return mPosition;
}
//----------------------------------------------------------------------------
inline AVector Camera::GetDVector () const
{
	return mDVector;
}
//----------------------------------------------------------------------------
inline AVector Camera::GetUVector () const
{
	return mUVector;
}
//----------------------------------------------------------------------------
inline AVector Camera::GetRVector () const
{
	return mRVector;
}
//----------------------------------------------------------------------------
inline const HMatrix& Camera::GetViewMatrix () const
{
	return mViewMatrix;
}
//----------------------------------------------------------------------------
inline bool Camera::IsPerspective () const
{
	return mIsPerspective;
}
//----------------------------------------------------------------------------
inline const float* Camera::GetFrustum () const
{
	return mFrustum;
}
//----------------------------------------------------------------------------
inline float Camera::GetDMin () const
{
	return mFrustum[VF_DMIN];
}
//----------------------------------------------------------------------------
inline float Camera::GetDMax () const
{
	return mFrustum[VF_DMAX];
}
//----------------------------------------------------------------------------
inline float Camera::GetUMin () const
{
	return mFrustum[VF_UMIN];
}
//----------------------------------------------------------------------------
inline float Camera::GetUMax () const
{
	return mFrustum[VF_UMAX];
}
//----------------------------------------------------------------------------
inline float Camera::GetRMin () const
{
	return mFrustum[VF_RMIN];
}
//----------------------------------------------------------------------------
inline float Camera::GetRMax () const
{
	return mFrustum[VF_RMAX];
}
//----------------------------------------------------------------------------
inline Camera::DepthType Camera::GetDepthType () const
{
	return mDepthType;
}
//----------------------------------------------------------------------------
inline const HMatrix& Camera::GetProjectionMatrix () const
{
	return mProjectionMatrix[mDepthType];
}
//----------------------------------------------------------------------------
inline const HMatrix& Camera::GetProjectionViewMatrix () const
{
	return mProjectionViewMatrix[mDepthType];
}
//----------------------------------------------------------------------------
inline const HMatrix& Camera::GetPreViewMatrix () const
{
	return mPreViewMatrix;
}
//----------------------------------------------------------------------------
inline bool Camera::PreViewIsIdentity () const
{
	return mPreViewIsIdentity;
}
//----------------------------------------------------------------------------
inline const HMatrix& Camera::GetPostProjectionMatrix () const
{
	return mPostProjectionMatrix;
}
//----------------------------------------------------------------------------
inline bool Camera::PostProjectionIsIdentity () const
{
	return mPostProjectionIsIdentity;
}
//----------------------------------------------------------------------------
inline void Camera::SetDefaultDepthType (DepthType type)
{
	msDefaultDepthType = type;
}
//----------------------------------------------------------------------------
inline Camera::DepthType Camera::GetDefaultDepthType ()
{
	return msDefaultDepthType;
}
//----------------------------------------------------------------------------
inline const Float4 &Camera::GetClearColor() const
{
	return mClearColor;
}
//----------------------------------------------------------------------------
inline float Camera::GetClearDepth() const
{
	return mClearDepth;
}
//----------------------------------------------------------------------------
inline unsigned int Camera::GetClearStencil() const
{
	return mClearStencil;
}
//----------------------------------------------------------------------------
inline void Camera::GetClearFlag(bool &color, bool &depth, bool &stencil)
{
	color = mClearFlagColor;
	depth = mClearFlagDepth;
	stencil = mClearFlagStencil;
}
//----------------------------------------------------------------------------
