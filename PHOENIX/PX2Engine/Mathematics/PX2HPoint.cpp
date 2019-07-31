// PX2HPoint.cpp

#include "PX2HPoint.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
HPoint::HPoint ()
{
}
//----------------------------------------------------------------------------
HPoint::HPoint (const HPoint& pnt)
{
	mTuple[0] = pnt.mTuple[0];
	mTuple[1] = pnt.mTuple[1];
	mTuple[2] = pnt.mTuple[2];
	mTuple[3] = pnt.mTuple[3];
}
//----------------------------------------------------------------------------
HPoint::HPoint (float x, float y, float z, float w)
{
	mTuple[0] = x;
	mTuple[1] = y;
	mTuple[2] = z;
	mTuple[3] = w;
}
//----------------------------------------------------------------------------
HPoint::~HPoint ()
{
}
//----------------------------------------------------------------------------
HPoint& HPoint::operator= (const HPoint& pnt)
{
	mTuple[0] = pnt.mTuple[0];
	mTuple[1] = pnt.mTuple[1];
	mTuple[2] = pnt.mTuple[2];
	mTuple[3] = pnt.mTuple[3];
	return *this;
}
//----------------------------------------------------------------------------
bool HPoint::operator== (const HPoint& pnt) const
{
	return memcmp(mTuple, pnt.mTuple, 4*sizeof(float)) == 0;
}
//----------------------------------------------------------------------------
bool HPoint::operator!= (const HPoint& pnt) const
{
	return memcmp(mTuple, pnt.mTuple, 4*sizeof(float)) != 0;
}
//----------------------------------------------------------------------------
bool HPoint::operator< (const HPoint& pnt) const
{
	return memcmp(mTuple, pnt.mTuple, 4*sizeof(float)) < 0;
}
//----------------------------------------------------------------------------
bool HPoint::operator<= (const HPoint& pnt) const
{
	return memcmp(mTuple, pnt.mTuple, 4*sizeof(float)) <= 0;
}
//----------------------------------------------------------------------------
bool HPoint::operator> (const HPoint& pnt) const
{
	return memcmp(mTuple, pnt.mTuple, 4*sizeof(float)) > 0;
}
//----------------------------------------------------------------------------
bool HPoint::operator>= (const HPoint& pnt) const
{
	return memcmp(mTuple, pnt.mTuple, 4*sizeof(float)) >= 0;
}
//----------------------------------------------------------------------------