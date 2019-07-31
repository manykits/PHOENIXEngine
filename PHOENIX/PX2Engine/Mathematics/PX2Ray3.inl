// PX2Ray3.inl

//----------------------------------------------------------------------------
template <typename Real>
Ray3<Real>::Ray3 ()
{
}
//----------------------------------------------------------------------------
template <typename Real>
Ray3<Real>::~Ray3 ()
{
}
//----------------------------------------------------------------------------
template <typename Real>
Ray3<Real>::Ray3 (const Vector3<Real>& origin,
				  const Vector3<Real>& direction)
				  :
Origin(origin),
Direction(direction)
{
}
//----------------------------------------------------------------------------
