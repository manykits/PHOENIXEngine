// PX2Line3.inl

//----------------------------------------------------------------------------
template <typename Real>
Line3<Real>::Line3 ()
{
}
//----------------------------------------------------------------------------
template <typename Real>
Line3<Real>::~Line3 ()
{
}
//----------------------------------------------------------------------------
template <typename Real>
Line3<Real>::Line3 (const Vector3<Real>& origin,
					const Vector3<Real>& direction)
					:
Origin(origin),
Direction(direction)
{
}
//----------------------------------------------------------------------------