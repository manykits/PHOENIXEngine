// PX2ModelControllerAnim.cpp

#include "PX2ModelController.hpp"
#include "PX2Animation3DSkeleton.hpp"
#include "PX2XMLData.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
bool ModelController::AddAnim(Animation *anim)
{
	if (HasAnim(anim)) 
		return false;

	const std::string &name = anim->GetName();
	mAnimsMap[FString(name.c_str())] = anim;
	anim->SetModelController(this);

	return true;
}
//----------------------------------------------------------------------------
bool ModelController::HasAnim(Animation *anim)
{
	return HasAnim(anim->GetName());
}
//----------------------------------------------------------------------------
bool ModelController::HasAnim(const std::string &name)
{
	std::map<FString, AnimationPtr>::iterator it = 
		mAnimsMap.find(FString(name.c_str()));
	if (it != mAnimsMap.end())
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool ModelController::RemoveAnim(Animation *anim)
{
	return RemoveAnim(anim->GetName());
}
//----------------------------------------------------------------------------
bool ModelController::RemoveAnim(const std::string &name)
{
	std::map<FString, AnimationPtr>::iterator it = 
		mAnimsMap.find(FString(name.c_str()));
	if (it != mAnimsMap.end())
	{
		it->second->SetModelController(0);

		mAnimsMap.erase(it);
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void ModelController::RemoveAllAnims()
{
	std::map<FString, AnimationPtr>::iterator it = mAnimsMap.begin();
	for (; it != mAnimsMap.end(); it++)
	{
		it->second->SetModelController(0);
	}

	mAnimsMap.clear();
}
//----------------------------------------------------------------------------
int ModelController::GetNumAnims() const
{
	return (int)mAnimsMap.size();
}
//----------------------------------------------------------------------------
Animation *ModelController::GetAnimByName(const std::string &name)
{
	std::map<FString, AnimationPtr>::iterator it = mAnimsMap.find(
		FString(name.c_str()));
	if (it != mAnimsMap.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
void ModelController::SetDefaultAnim(const std::string &name)
{
	mDefaultAnim = GetAnimByName(name);
}
//----------------------------------------------------------------------------
void ModelController::PlayAnim(Animation *anim)
{
	if (!HasAnim(anim)) return;

	bool sameAnim = false;
	if (mCurPlayingAnim == anim)
		sameAnim = true;

	if (mCurPlayingAnim)
		mCurPlayingAnim->Pause();

	if (AT_SKELETON == mAnimType)
	{
		Node *charNode = DynamicCast<Node>(GetControlledable());
		if (mCurPlayingAnim && charNode)
		{
			Animation3DSkeleton *anim3dSk = DynamicCast<Animation3DSkeleton>(mCurPlayingAnim);

			if (anim3dSk)
			{
				Node *animNode = anim3dSk->GetAnimNode();
				charNode->DetachChild(animNode);
			}

			if (!sameAnim)
			{
				mLastAnimObj = new0 AnimationObject();
				mLastAnimObj->TheAnim = mCurPlayingAnim;
				mLastAnimObj->BlendTime = 0.0f;
				mLastAnimObj->TheCharacter = this;
			}
			else
			{
				mLastAnimObj = 0;
			}
		}
	}

	mCurPlayingAnim = anim;

	if (mCurPlayingAnim)
		mCurPlayingAnim->OnPlay(this);
}
//----------------------------------------------------------------------------
Animation *ModelController::PlayAnimByName(const std::string &name)
{
	Animation *anim = GetAnimByName(name);
	if (anim) 
		PlayAnim(anim);
	return anim;
}
//----------------------------------------------------------------------------
void ModelController::StopAnim(Animation *anim)
{
	if (!HasAnim(anim))
		return;

	anim->Pause();
}
//----------------------------------------------------------------------------
void ModelController::PlayCurAnim()
{
	if (mCurPlayingAnim)
		mCurPlayingAnim->LetCharacterPlay();
}
//----------------------------------------------------------------------------
void ModelController::StopCurAnim()
{
	if (mCurPlayingAnim)
		mCurPlayingAnim->Pause();
}
//----------------------------------------------------------------------------
bool ModelController::IsHasAnimPlaying() const
{
	std::map<FString, AnimationPtr>::const_iterator it = mAnimsMap.begin();
	for (; it != mAnimsMap.end(); it++)
	{
		if (it->second->IsPlaying())
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------