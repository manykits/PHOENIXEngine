// PX2SkillRange.cpp

#include "PX2SkillRange.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
SkillRange::SkillRange()
{
	TheD23Type = DT_2D;
	TheRangeType = RT_CIRCLE;

	RadiusLength = 0.0f;
	Degree = 0.0f;
	Width = 0.0f;
}
//----------------------------------------------------------------------------
SkillRange::~SkillRange()
{
}
//----------------------------------------------------------------------------