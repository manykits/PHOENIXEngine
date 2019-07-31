// PX2SkillEffectMan.cpp

#include "PX2SkillEffectMan.hpp"
#include "PX2ResourceManager.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
SkillEffectMan::SkillEffectMan()
{
}
//----------------------------------------------------------------------------
SkillEffectMan::~SkillEffectMan()
{
}
//----------------------------------------------------------------------------
void SkillEffectMan::Clear()
{
	mEffects.clear();
}
//----------------------------------------------------------------------------
Movable *SkillEffectMan::AddEffect(FString effectName, const std::string &filename)
{
	Movable *mov = DynamicCast<Movable>(PX2_RM.BlockLoad(filename));
	if (mov)
	{
		mEffects[effectName] = mov;
	}	
	return mov;
}
//----------------------------------------------------------------------------
Movable *SkillEffectMan::GetEffect(FString effectName)
{
	auto it = mEffects.find(effectName);
	if (it != mEffects.end())
	{
		Movable *mov = it->second;
		Movable *newMov = (Movable*)(mov->Copy(""));
		
		return newMov;
	}

	return 0;
}
//----------------------------------------------------------------------------