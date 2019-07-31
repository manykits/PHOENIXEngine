// PX2AnimationCall.hpp

#include "PX2AnimationCall.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
AnimationCall::AnimationCall(Animation *anim) :
mAnimation(anim)
{
	CallTime = 0.0f;
	CallBack = 0;
	CallTimeFreq = 0.0f;

	TheCallType = CT_ONCE;
	IsCalled = false;
}
//----------------------------------------------------------------------------
AnimationCall::~AnimationCall()
{
}
//----------------------------------------------------------------------------
Animation *AnimationCall::GetAnimation()
{
	return mAnimation;
}
//----------------------------------------------------------------------------
void AnimationCall::Call()
{
	IsCalled = true;

	if (CallBack)
	{
		(*CallBack)(this);
	}
}
//----------------------------------------------------------------------------