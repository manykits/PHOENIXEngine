// PX2Sphere3.inl

//----------------------------------------------------------------------------
template <typename Real>
Sphere3<Real>::Sphere3 ()
{
}
//----------------------------------------------------------------------------
template <typename Real>
Sphere3<Real>::~Sphere3 ()
{
}
//----------------------------------------------------------------------------
template <typename Real>
Sphere3<Real>::Sphere3 (const Vector3<Real>& center, Real radius)
:
Center(center),
Radius(radius)
{
}
//----------------------------------------------------------------------------
