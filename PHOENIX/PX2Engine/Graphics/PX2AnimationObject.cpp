// PX2AnimationObject.cpp

#include "PX2AnimationObject.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
AnimationObject::AnimationObject() :
BlendTime(0.0f),
BlendWeight(0.0f),
TheCharacter(0)
{
}
//----------------------------------------------------------------------------
AnimationObject::~AnimationObject()
{
}
//----------------------------------------------------------------------------
void AnimationObject::Update(double appTime, double elapsedTime)
{
	PX2_UNUSED(appTime);
	BlendTime += (float)elapsedTime;
}
//----------------------------------------------------------------------------
void AnimationObject::BeforeRemove()
{
	TheCharacter = 0;
}
//----------------------------------------------------------------------------