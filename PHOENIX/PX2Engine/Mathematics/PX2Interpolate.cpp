// PX2IntrLine3Triangle3.cpp

#include "PX2Interpolate.hpp"
using namespace PX2;

Interpolate::Interpolate ()
{

}
//----------------------------------------------------------------------------
Interpolate::~Interpolate ()
{

}
//----------------------------------------------------------------------------
float Interpolate::LinearFloat (float &v0, float &v1, float t)
{
	return v0 + (v1-v0)*t;
}
//----------------------------------------------------------------------------
Float4 Interpolate::LinearFloat4 (Float4 &v0, Float4 &v1, float t)
{
	return Float4(v0[0] + (v1[0]-v0[0])*t,
		v0[1] + (v1[1]-v0[1])*t,
		v0[2] + (v1[2]-v0[2])*t,
		v0[3] + (v1[3]-v0[3])*t);
}
//----------------------------------------------------------------------------
AVector Interpolate::LinearAVector(AVector &v0, AVector &v1, float t)
{
	AVector vec = AVector(v0[0] + (v1[0] - v0[0])*t,
		v0[1] + (v1[1] - v0[1])*t,
		v0[2] + (v1[2] - v0[2])*t);
	return vec;
}
//----------------------------------------------------------------------------