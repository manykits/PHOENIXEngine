// PX2AxisAlignedBox3.inl

//----------------------------------------------------------------------------
template <typename Real>
AxisAlignedBox3<Real>::AxisAlignedBox3 ()
{
}
//----------------------------------------------------------------------------
template <typename Real>
AxisAlignedBox3<Real>::~AxisAlignedBox3 ()
{
}
//----------------------------------------------------------------------------
template <typename Real>
AxisAlignedBox3<Real>::AxisAlignedBox3 (Real xmin, Real xmax, Real ymin,
    Real ymax, Real zmin, Real zmax)
{
    Min[0] = xmin;
    Max[0] = xmax;
    Min[1] = ymin;
    Max[1] = ymax;
    Min[2] = zmin;
    Max[2] = zmax;
}
//----------------------------------------------------------------------------
template <typename Real>
AxisAlignedBox3<Real>::AxisAlignedBox3 (const Vector3<Real> &min, 
	const Vector3<Real> &max)
{
	Min[0] = min[0];
	Min[1] = min[1];
	Min[2] = min[2];

	Max[0] = max[0];
	Max[1] = max[1];
	Max[2] = max[2];
}
//----------------------------------------------------------------------------
template <typename Real>
void AxisAlignedBox3<Real>::GetCenterExtents (Vector3<Real>& center,
    Real extent[3])
{
    center[0] = ((Real)0.5)*(Max[0] + Min[0]);
    center[1] = ((Real)0.5)*(Max[1] + Min[1]);
    center[2] = ((Real)0.5)*(Max[2] + Min[2]);
    extent[0] = ((Real)0.5)*(Max[0] - Min[0]);
    extent[1] = ((Real)0.5)*(Max[1] - Min[1]);
    extent[2] = ((Real)0.5)*(Max[2] - Min[2]);
}
//----------------------------------------------------------------------------
template <typename Real>
bool AxisAlignedBox3<Real>::HasXOverlap (const AxisAlignedBox3& box) const
{
    return (Max[0] >= box.Min[0] && Min[0] <= box.Max[0]);
}
//----------------------------------------------------------------------------
template <typename Real>
bool AxisAlignedBox3<Real>::HasYOverlap (const AxisAlignedBox3& box) const
{
    return (Max[1] >= box.Min[1] && Min[1] <= box.Max[1]);
}
//----------------------------------------------------------------------------
template <typename Real>
bool AxisAlignedBox3<Real>::HasZOverlap (const AxisAlignedBox3& box) const
{
    return (Max[2] >= box.Min[2] && Min[2] <= box.Max[2]);
}
//----------------------------------------------------------------------------
template <typename Real>
bool AxisAlignedBox3<Real>::TestIntersection (const AxisAlignedBox3& box)
    const
{
    for (int i = 0; i < 3; i++)
    {
        if (Max[i] < box.Min[i] || Min[i] > box.Max[i])
        {
            return false;
        }
    }
    return true;
}
//----------------------------------------------------------------------------
template <typename Real>
bool AxisAlignedBox3<Real>::Contain (const AxisAlignedBox3& box) const
{
	for (int i=0; i<3; i++)
	{
		if (Min[i]>box.Min[i] || Max[i]<box.Max[i])
		{
			return false;
		}
	}

	return true;
}
//----------------------------------------------------------------------------
template <typename Real>
bool AxisAlignedBox3<Real>::Contain(const Vector3f &pos)
{
	return Min[0] <= pos.X() && pos.X() <= Max[0] &&
		Min[1] <= pos.Y() && pos.Y() <= Max[1] &&
		Min[2] <= pos.Z() && pos.Z() <= Max[2];
}
//----------------------------------------------------------------------------
template <typename Real>
bool AxisAlignedBox3<Real>::FindIntersection (const AxisAlignedBox3& box,
    AxisAlignedBox3& intersection) const
{
    int i;
    for (i = 0; i < 3; i++)
    {
        if (Max[i] < box.Min[i] || Min[i] > box.Max[i])
        {
            return false;
        }
    }

    for (i = 0; i < 3; i++)
    {
        if (Max[i] <= box.Max[i])
        {
            intersection.Max[i] = Max[i];
        }
        else
        {
            intersection.Max[i] = box.Max[i];
        }

        if (Min[i] <= box.Min[i])
        {
            intersection.Min[i] = box.Min[i];
        }
        else
        {
            intersection.Min[i] = Min[i];
        }
    }
    return true;
}
//----------------------------------------------------------------------------
template <typename Real>
Vector3<Real> AxisAlignedBox3<Real>::GetMin()
{
	return Vector3<Real>(Min[0], Min[1], Min[2]);
}
//----------------------------------------------------------------------------
template <typename Real>
Vector3<Real> AxisAlignedBox3<Real>::GetMax()
{
	return Vector3<Real>(Max[0], Max[1], Max[2]);
}
//----------------------------------------------------------------------------
