// PX2Intersector.cpp

#include "PX2Intersector.hpp"

namespace PX2
{
//----------------------------------------------------------------------------
template <typename Real, typename TVector>
Intersector<Real,TVector>::Intersector ()
{
    mContactTime = (Real)0;
    mIntersectionType = IT_EMPTY;
}
//----------------------------------------------------------------------------
template <typename Real, typename TVector>
Intersector<Real,TVector>::~Intersector ()
{
}
//----------------------------------------------------------------------------
template <typename Real, typename TVector>
Real Intersector<Real,TVector>::GetContactTime () const
{
    return mContactTime;
}
//----------------------------------------------------------------------------
template <typename Real, typename TVector>
int Intersector<Real,TVector>::GetIntersectionType () const
{
    return mIntersectionType;
}
//----------------------------------------------------------------------------
template <typename Real, typename TVector>
bool Intersector<Real,TVector>::Test ()
{
    // 派生类实现
    assertion(false, "Function not yet implemented\n");
    return false;
}
//----------------------------------------------------------------------------
template <typename Real, typename TVector>
bool Intersector<Real,TVector>::Find ()
{
    // 派生类实现
    assertion(false, "Function not yet implemented\n");
    return false;
}
//----------------------------------------------------------------------------
template <typename Real, typename TVector>
bool Intersector<Real,TVector>::Test (Real, const TVector&, const TVector&)
{
    // 派生类实现
    assertion(false, "Function not yet implemented\n");
    return false;
}
//----------------------------------------------------------------------------
template <typename Real, typename TVector>
bool Intersector<Real,TVector>::Find (Real, const TVector&, const TVector&)
{
    // 派生类实现
    assertion(false, "Function not yet implemented\n");
    return false;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 实例
//----------------------------------------------------------------------------
template 
class Intersector<float,Vector2f>;

template 
class Intersector<float,Vector3f>;

template 
class Intersector<double,Vector2d>;

template 
class Intersector<double,Vector3d>;
//----------------------------------------------------------------------------
}
