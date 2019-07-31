// PX2Animation.hpp

#ifndef PX2ANIMATION_HPP
#define PX2ANIMATION_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2Node.hpp"
#include "PX2AnimationCall.hpp"

namespace PX2
{

	class ModelController;

	class PX2_ENGINE_ITEM Animation : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Animation);

	public:
		enum AnimType
		{
			AT_2DFRAMES,
			AT_2DSKELETON,
			AT_3DSKELETON,
			AT_MAX_TYPE
		};
		virtual ~Animation();

		AnimType GetAnimType() const;

		void Update(float appSeconds, float elapsedSeconds);

		virtual void SetFilename(const std::string &filename);
		const std::string &GetFilename() const;

		void SetNormalTime(float time);
		float GetNormalTime() const;

		void SetTime(float time);
		float GetTime() const;

		void SetFrequency(float frequence);
		float GetFrequency() const;

		void SetPlayOnce(bool once);
		bool IsPlayOnce() const;
		void SetPlayedKeepPlaying(bool isPlayedKeepPlaying);
		bool IsPlayedKeepPlaying() const;

		AnimationCall *AddAnimationCall(const std::string &name, float animTime,
			AnimationCallback callback);
		AnimationCall *GetAnimationCall(const std::string &name);

		// 是否使用动画融合
		void SetUseBlend(bool useBlend);
		bool IsUseBlend() const;

		void LetCharacterPlay();
		virtual void OnPlay(ModelController *modelCtrl);
		virtual bool IsPlaying() const;
		virtual void Pause();

		ModelController *GetCharacter();
		virtual void OnRemove(ModelController *character);

	public_internal:
		virtual void SetModelController(ModelController *character);

	protected:
		Animation();
		Animation(AnimType type);

		ModelController *mModelController;

		AnimType mAnimType;
		std::string mFilename;
		float mAnimNormalTime;
		float mAnimTime;
		float mFrequency;
		bool mIsPlayOnce;
		bool mIsPlayedKeepPlaying;
		bool mIsUseBlend;

		bool mIsPlaying;
		float mPlayedTime;

		std::map<std::string, AnimationCallPtr> mAnimationCalls;
	};

#include "PX2Animation.inl"
	typedef Pointer0<Animation> AnimationPtr;

}

#endif