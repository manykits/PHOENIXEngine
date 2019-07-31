// PX2SkillAffect.cpp

#include "PX2SkillAffect.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
GenActorObj::GenActorObj()
{
	ID = 0;
	TypeID = 0;
	MonsID = 0;
	GroupID = 0;
}
//----------------------------------------------------------------------------
GenActorObj::~GenActorObj()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
SkillAffect::SkillAffect()
{
	IsAffectValue = false;
	IsAffectEffectBlood = false;

	FromSkillInst = 0;

	TheAffectType = AT_HP_MINUS;

	Val0 = 0.0f;
	Val1 = 0.0f;
	Val2 = 0.0f;
	Val3 = 0.0f;
}
//----------------------------------------------------------------------------
SkillAffect::~SkillAffect()
{
}
//----------------------------------------------------------------------------