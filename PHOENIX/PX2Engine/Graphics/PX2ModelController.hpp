// PX2ModelController.hpp

#ifndef PX2MODELCONTROLLER_HPP
#define PX2MODELCONTROLLER_HPP

#include "PX2Controller.hpp"
#include "PX2Animation.hpp"
#include "PX2FString.hpp"
#include "PX2BlendTransformController.hpp"
#include "PX2AnimationObject.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM ModelController : public Controller
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_NEW(ModelController);
		PX2_DECLARE_STREAM(ModelController);

	public:
		ModelController();
		virtual ~ModelController();

	protected:
		virtual void _Update(double applicationTime, double elapsedTime);
		void _AnimationBlend();

		// Movable
	public:
		virtual void SetMovableFilename(const std::string &filename,
			bool shareVI = false);
		const std::string &GetMovableFilename() const;
		bool IsShareVI() const;

		virtual void SetMovable(Movable *mov);
		Movable *GetMovable();

		// 所有动作集中在一个文件中，通过配置文件加载进行配置
		void SetSkinAnimationConfig(const std::string &filename);
		void SetSkinAnimationConfig(const std::string &filename, Movable *animMovable);
		const std::string &GetSkinAnimationConfig() const;

		virtual void SetHeading(const AVector &heading);
		const AVector &GetHeading() const;

		void SetMovableUseAutoWorldBound(bool isUseAutoWorldBound);
		bool IsMovableUseAutoWorldBound() const;
		void SetMovableAutoWorldBoundRadius(float radius);
		float GetMovableAutoWorldBoundRadius() const;

	protected:
		void _CalSkins();
		void _CalAnimNode(Movable *mov);
		void _DetachKeyframeCtrl(Movable *mov);
		virtual void SetControlledable(Controlledable* object);

		bool mIsShareVI;
		std::string mMovableFilename;
		MovablePtr mMovable;

		std::string mSkinAnimationConfig;

		bool mIsMovableAutoWorldBound;
		float mMovableAutoWorldBoundRadius;

		AVector mHeading;

		// Anchor
	public:
		void CollectAnchors();

		APoint GetAnchorPos(int id);
		Node *GetAnchor(int id);

	protected:
		void _CollectAnchor(Movable *mov);

		std::map<int, Node*> mAnchorMap;

		// anim
	public:
		// set it before add anim
		enum AnimType
		{
			AT_NONE,
			AT_FRAMES,
			AT_SKELETON,
			AT_MAX_TYPE
		};
		void SetAnimType(AnimType type);
		AnimType GetAnimType() const;

		bool AddAnim(Animation *anim);
		bool HasAnim(Animation *anim);
		bool HasAnim(const std::string &name);
		bool RemoveAnim(Animation *anim);
		bool RemoveAnim(const std::string &name);
		void RemoveAllAnims();
		int GetNumAnims() const;
		std::map<FString, AnimationPtr> &GetAnimsMap();
		Animation *GetAnimByName(const std::string &name);

		void SetDefaultAnim(const std::string &name);
		const std::string &GetDefaultAnimName() const;
		Animation *GetDefaultAnim();

		virtual void PlayAnim(Animation *anim);
		virtual Animation *PlayAnimByName(const std::string &name);
		virtual void StopAnim(Animation *anim);
		virtual void StopCurAnim();
		virtual void PlayCurAnim();

		Animation *GetCurPlayingAnim();
		bool IsHasAnimPlaying() const;

	protected:
		AnimType mAnimType;

		// Frames

		// Skeleton
		std::map<FString, AnimationPtr> mAnimsMap;
		AnimationPtr mCurPlayingAnim;
		std::string mDefaultAnimName;
		AnimationPtr mDefaultAnim;

		std::vector<MovablePtr> mSkinMovables;
		MovablePtr mModelAnimMovable;
		std::map<FString, BlendTransformController*> mBTCMap;
		AnimationObjectPtr mLastAnimObj;
	};

	PX2_REGISTER_STREAM(ModelController);
	typedef Pointer0<ModelController> ModelControllerPtr;
#include "PX2ModelController.inl"

}

#endif