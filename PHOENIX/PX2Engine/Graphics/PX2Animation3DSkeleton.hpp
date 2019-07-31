// PX2Animation3DSkeleton.hpp

#ifndef PX2ANIMATION3DSKELETON_HPP
#define PX2ANIMATION3DSKELETON_HPP

#include "PX2Animation.hpp"
#include "PX2Node.hpp"
#include "PX2KeyframeController.hpp"
#include "PX2FString.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Animation3DSkeleton : public Animation
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(Animation3DSkeleton);
		PX2_DECLARE_STREAM(Animation3DSkeleton);

	public:
		Animation3DSkeleton();
		virtual ~Animation3DSkeleton();

		virtual void SetFilename(const std::string &filename);
		virtual void SetMovable(Movable *movable);

		virtual void SetFrequency(float frequency);

		virtual void SetPlayOnce(bool once);
		virtual void OnPlay(ModelController *character);

		Node *GetAnimNode();
		std::map<FString, KeyframeControllerPtr> &GetKeyframeCtrlMap();
		KeyframeController *GetRootKeyframeCtrl();

		void SetMovingScale(const APoint &movingScale);

		void SetKeyframeRangeTime(float fromKFTime, float toKFTime);

	private:
		void _CollectKFC(Movable *mov, float &maxTime);
		void _AddKeyFrameCtrl(const std::string &keyFrameName, KeyframeController *keyFrameCtrl);

		std::string mTagName;
		NodePtr mAnimNode;
		std::map<FString, KeyframeControllerPtr> mKeyframeCtrlMap;
		KeyframeControllerPtr mKeyframeCtrlRoot;

		APoint mMovingScale;

		float mFromKeyframeTime;
		float mToKeyframeTime;
	};

	PX2_REGISTER_STREAM(Animation3DSkeleton);
#include "PX2Animation3DSkeleton.inl"
	typedef Pointer0<Animation3DSkeleton> Animation3DSkeletonPtr;

}

#endif
