// PX2Circle3.inl

//----------------------------------------------------------------------------
template <typename Real>
Circle3<Real>::Circle3 ()
{
}
//----------------------------------------------------------------------------
template <typename Real>
Circle3<Real>::~Circle3 ()
{
}
//----------------------------------------------------------------------------
template <typename Real>
Circle3<Real>::Circle3 (const Vector3<Real>& center,
						const Vector3<Real>& direction0, const Vector3<Real>& direction1,
						const Vector3<Real>& normal, const Real radius)
						:
Center(center),
Direction0(direction0),
Direction1(direction1),
Normal(normal),
Radius(radius)
{
}
//----------------------------------------------------------------------------
