// PX2AnimationFrames.cpp

#include "PX2AnimationFrames.hpp"
#include "PX2ModelController.hpp"
#include "PX2FramesMesh.hpp"
#include "PX2Log.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Animation, AnimationFrames);
PX2_IMPLEMENT_STREAM(AnimationFrames);
PX2_IMPLEMENT_FACTORY(AnimationFrames);
PX2_IMPLEMENT_DEFAULT_NAMES(Animation, AnimationFrames);

//----------------------------------------------------------------------------
AnimationFrames::AnimationFrames() :
mNumFramePerDir(0),
mAnimNode(0)
{
}
//----------------------------------------------------------------------------
AnimationFrames::~AnimationFrames()
{
}
//----------------------------------------------------------------------------
void AnimationFrames::SetFilename(const std::string &filename)
{
	Animation::SetFilename(filename);

	if (filename.empty()) return;

	mFramesMesh = new0 FramesMesh();
	mNumFramePerDir = mFramesMesh->SetTexPack(filename);

	SetPlayOnce(mIsPlayOnce);

	if (mModelController && mModelController->GetCurPlayingAnim() == this)
	{
		LetCharacterPlay();
	}
}
//----------------------------------------------------------------------------
void AnimationFrames::SetPlayOnce(bool once)
{
	Animation::SetPlayOnce(once);

	if (mFramesMesh)
		mFramesMesh->SetPlayOnce(once);
}
//----------------------------------------------------------------------------
void AnimationFrames::OnPlay(ModelController *modelCtrl)
{
	Animation::OnPlay(modelCtrl);

	Controlledable *ctrlable = modelCtrl->GetControlledable();

	mAnimNode = DynamicCast<Node>(ctrlable->GetObjectByName("AnimNode"));
	if (mAnimNode)
	{
		const AVector &dir = modelCtrl->GetHeading();
		mAnimNode->DetachAllChildren();

		if (mFramesMesh)
			mAnimNode->AttachChild(mFramesMesh);

		SetWorldRunDir(dir);

		if (mFramesMesh)
			mFramesMesh->ResetPlay();
	}
	else
	{
		assertion(false, "must be a node.");
		PX2_LOG_INFO("must be a node.");
	}
}
//----------------------------------------------------------------------------
bool AnimationFrames::IsPlaying() const
{
	if (Animation::IsPlaying())
		return true;

	if (mFramesMesh)
		return mFramesMesh->IsPlaying();

	return false;
}
//----------------------------------------------------------------------------
void AnimationFrames::Pause()
{
	Animation::Pause();

	if (mFramesMesh)
		mFramesMesh->Pause();
}
//----------------------------------------------------------------------------
void AnimationFrames::SetWorldRunDir(const AVector &vec)
{
	mWorldRunDir = vec;

	if (mFramesMesh)
		mFramesMesh->SetWorldRunDir(mWorldRunDir);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
AnimationFrames::AnimationFrames(LoadConstructor value) :
Animation(value),
mAnimNode(0)
{
}
//----------------------------------------------------------------------------
void AnimationFrames::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Animation::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(AnimationFrames, source);
}
//----------------------------------------------------------------------------
void AnimationFrames::Link(InStream& source)
{
	Animation::Link(source);
}
//----------------------------------------------------------------------------
void AnimationFrames::PostLink()
{
	Animation::PostLink();
}
//----------------------------------------------------------------------------
bool AnimationFrames::Register(OutStream& target) const
{
	if (Animation::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void AnimationFrames::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Animation::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(AnimationFrames, target);
}
//----------------------------------------------------------------------------
int AnimationFrames::GetStreamingSize(Stream &stream) const
{
	int size = Animation::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------