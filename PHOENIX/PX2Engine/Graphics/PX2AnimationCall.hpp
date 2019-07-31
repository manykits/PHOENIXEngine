// PX2AnimationCall.hpp

#ifndef PX2ANIMATIONCALL_HPP
#define PX2ANIMATIONCALL_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	class Animation;
	class AnimationCall;

	typedef void(*AnimationCallback) (AnimationCall *call);

	class PX2_ENGINE_ITEM AnimationCall : public Object
	{
	public:
		AnimationCall(Animation *anim);
		~AnimationCall();

		Animation *GetAnimation();

		float CallTime;
		AnimationCallback CallBack;

		float CallTimeFreq;

		enum CallType
		{
			CT_NONE,
			CT_ONCE,
			CT_MAX_TYPE
		};
		CallType TheCallType;
		bool IsCalled;

		void Call();

	protected:
		Animation *mAnimation;
	};
	typedef Pointer0<AnimationCall> AnimationCallPtr;

}

#endif