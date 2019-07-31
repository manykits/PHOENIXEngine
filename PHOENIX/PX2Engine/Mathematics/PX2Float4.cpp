// PX2Float4.cpp

#include "PX2Float4.hpp"
using namespace PX2;

const Float4 Float4::ZERO(0.0f, 0.0f, 0.0f, 0.0f);
const Float4 Float4::UNIT(1.0f, 1.0f, 1.0f, 1.0f);
const Float4 Float4::BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const Float4 Float4::WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const Float4 Float4::RED(1.0f, 0.0f, 0.0f, 1.0f);
const Float4 Float4::GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const Float4 Float4::BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const Float4 Float4::YELLOW(1.0f, 1.0f, 0.0f, 1.0f);

//----------------------------------------------------------------------------
Float4::Float4 ()
{
	mTuple[0] = 0.0f;
	mTuple[1] = 0.0f;
	mTuple[2] = 0.0f;
	mTuple[3] = 0.0f;
}
//----------------------------------------------------------------------------
Float4::~Float4 ()
{
}
//----------------------------------------------------------------------------
float Float4::X() const
{
	return mTuple[0];
}
//----------------------------------------------------------------------------
float& Float4::X()
{
	return mTuple[0];
}
//----------------------------------------------------------------------------
float Float4::Y() const
{
	return mTuple[1];
}
//----------------------------------------------------------------------------
float& Float4::Y()
{
	return mTuple[1];
}
//----------------------------------------------------------------------------
float Float4::Z() const
{
	return mTuple[2];
}
//----------------------------------------------------------------------------
float& Float4::Z()
{
	return mTuple[2];
}
//----------------------------------------------------------------------------
float Float4::W() const
{
	return mTuple[3];
}
//----------------------------------------------------------------------------
float& Float4::W()
{
	return mTuple[3];
}
//----------------------------------------------------------------------------
Float4::Float4 (float f0, float f1, float f2, float f3)
{
	mTuple[0] = f0;
	mTuple[1] = f1;
	mTuple[2] = f2;
	mTuple[3] = f3;
}
//----------------------------------------------------------------------------
Float4::Float4 (const Float4& tuple)
{
	mTuple[0] = tuple.mTuple[0];
	mTuple[1] = tuple.mTuple[1];
	mTuple[2] = tuple.mTuple[2];
	mTuple[3] = tuple.mTuple[3];
}
//----------------------------------------------------------------------------
Float4& Float4::operator= (const Float4& tuple)
{
	mTuple[0] = tuple.mTuple[0];
	mTuple[1] = tuple.mTuple[1];
	mTuple[2] = tuple.mTuple[2];
	mTuple[3] = tuple.mTuple[3];
	return *this;
}
//----------------------------------------------------------------------------
Float4 Float4::MakeColor (unsigned int r, unsigned int g, 
	unsigned int b, unsigned int a)
{
	return Float4((float)r/255.0f, (float)g/255.0f, (float)b/255.0f,
		(float)a/255.0f);
}
//----------------------------------------------------------------------------