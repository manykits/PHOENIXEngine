// PX2RibbonObject.cpp

#include "PX2RibbonObject.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
RibbonObject::RibbonObject () :
Length(0.0f),
LengthOnCompleteOneSegment(0.0f)
{
}
//----------------------------------------------------------------------------
RibbonObject::~RibbonObject ()
{
}
//----------------------------------------------------------------------------
bool RibbonObject::Update (Effectable *able, float elapsedTime)
{
	if (!EffectObject::Update(able, elapsedTime))
		return false;

	return true;
}
//----------------------------------------------------------------------------