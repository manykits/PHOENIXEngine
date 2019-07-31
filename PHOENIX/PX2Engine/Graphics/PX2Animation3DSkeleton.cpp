// PX2Animation3DSkeleton.cpp

#include "PX2Animation3DSkeleton.hpp"
#include "PX2Movable.hpp"
#include "PX2TriMesh.hpp"
#include "PX2ModelController.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Animation, Animation3DSkeleton);
PX2_IMPLEMENT_STREAM(Animation3DSkeleton);
PX2_IMPLEMENT_FACTORY(Animation3DSkeleton);
PX2_IMPLEMENT_DEFAULT_NAMES(Animation, Animation3DSkeleton);

//----------------------------------------------------------------------------
Animation3DSkeleton::Animation3DSkeleton() :
Animation(Animation::AT_3DSKELETON)
{
	mMovingScale = APoint(1.0f, 1.0f, 1.0f);
	mFromKeyframeTime = 0.0f;
	mToKeyframeTime = 0.0f;
}
//----------------------------------------------------------------------------
Animation3DSkeleton::~Animation3DSkeleton()
{
}
//----------------------------------------------------------------------------
void Animation3DSkeleton::SetFrequency(float frequency)
{
	Animation::SetFrequency(frequency);

	std::map<FString, KeyframeControllerPtr>::iterator it = 
		mKeyframeCtrlMap.begin();
	for (; it != mKeyframeCtrlMap.end(); it++)
	{
		KeyframeController *kfc = it->second;
		if (kfc)
		{
			kfc->Frequency = frequency;
		}
	}
}
//----------------------------------------------------------------------------
bool _IsNodeHasMesh(Node *node)
{
	for (int i = 0; i < node->GetNumChildren(); i++)
	{
		TriMesh *mesh = DynamicCast<TriMesh>(node->GetChild(i));
		if (mesh)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Animation3DSkeleton::SetFilename(const std::string &filename)
{
	if (filename.empty()) return;

	Animation::SetFilename(filename);

	ObjectLoadFun1 olf = PX2_GR.msUserLoadFun1;
	NodePtr animObject = DynamicCast<Node>(olf(filename.c_str(), false, false, false));
	if (!animObject) return;

	SetMovable(animObject);
}
//----------------------------------------------------------------------------
void Animation3DSkeleton::SetMovable(Movable *animObject)
{
	Node *animNode = DynamicCast<Node>(animObject);
	if (!animNode)
		return;

	for (int i = 0; i < animNode->GetNumChildren(); i++)
	{
		Movable *mov = animNode->GetChild(i);
		Node *node = DynamicCast<Node>(mov);
		Renderable *renderable = DynamicCast<Renderable>(mov);
		if (node && !renderable && !_IsNodeHasMesh(node) &&
			node->GetNumValidChildren()>0)
		{
			mAnimNode = node;
		}
	}
	animNode->DetachChild(mAnimNode);

	mKeyframeCtrlMap.clear();
	mAnimNormalTime = 0.0f;
	_CollectKFC(mAnimNode, mAnimNormalTime);
	mAnimTime = mAnimNormalTime;

	if (mModelController && mModelController->GetCurPlayingAnim() == this)
	{
		LetCharacterPlay();
	}
}
//----------------------------------------------------------------------------
void Animation3DSkeleton::SetPlayOnce(bool once)
{
	Animation::SetPlayOnce(once);

	std::map<FString, KeyframeControllerPtr>::iterator it = 
		mKeyframeCtrlMap.begin();
	for (; it != mKeyframeCtrlMap.end(); it++)
	{
		KeyframeController *kfc = it->second;
		if (kfc)
		{
			kfc->Repeat = once ? Controller::RT_CLAMP : Controller::RT_WRAP;
		}
	}
}
//----------------------------------------------------------------------------
void Animation3DSkeleton::OnPlay(ModelController *character)
{
	if (mKeyframeCtrlMap.empty())
		return;

	Animation::OnPlay(character);

	if (mAnimNode)
	{
		SetFrequency(GetFrequency());
		SetPlayOnce(IsPlayOnce());
	}

	std::map<FString, KeyframeControllerPtr>::iterator it = 
		mKeyframeCtrlMap.begin();
	for (; it != mKeyframeCtrlMap.end(); it++)
	{
		KeyframeController *kfc = it->second;
		if (kfc)
		{
			kfc->ResetPlay();
		}
	}
}
//----------------------------------------------------------------------------
void Animation3DSkeleton::SetMovingScale(const APoint &movingScale)
{
	mMovingScale = movingScale;

	if (mKeyframeCtrlRoot)
	{
		mKeyframeCtrlRoot->SetTransScale(movingScale);
	}
}
//----------------------------------------------------------------------------
void Animation3DSkeleton::_CollectKFC(Movable *mov, float &maxTime)
{
	if (!mov) return;

	std::string nodeName = mov->GetName();

	int numContrls = mov->GetNumControllers();
	if (1 == numContrls)
	{
		KeyframeController *kfc = DynamicCast<KeyframeController>(
			mov->GetController(0));
		if (kfc)
		{
			_AddKeyFrameCtrl(nodeName, kfc);

			if (maxTime < kfc->MaxTime)
			{
				maxTime = (float)kfc->MaxTime;
			}
		}
		else
		{
			assertion(false, "");
		}
	}
	else if (0 == numContrls)
	{
		KeyframeController *kfc = new0 KeyframeController(0, 0, 0, 0,
			mov->LocalTransform);
		mov->AttachController(kfc);

		_AddKeyFrameCtrl(nodeName, kfc);
		kfc->SetName("KeyframeController");
	}
	else
	{
		assertion(false, "");
	}

	Node *node = DynamicCast<Node>(mov);
	if (node)
	{
		for (int i = 0; i < node->GetNumChildren(); i++)
		{
			_CollectKFC(node->GetChild(i), maxTime);
		}
	}
}
//----------------------------------------------------------------------------
void Animation3DSkeleton::_AddKeyFrameCtrl(const std::string &keyFrameName,
	KeyframeController *keyFrameCtrl)
{
	if (mKeyframeCtrlMap.empty())
	{
		mKeyframeCtrlRoot = keyFrameCtrl;
		mKeyframeCtrlRoot->SetTransScale(mMovingScale);
	}

	mKeyframeCtrlMap[FString(keyFrameName.c_str())] = keyFrameCtrl;
}
//----------------------------------------------------------------------------
void Animation3DSkeleton::SetKeyframeRangeTime(float fromKFTime,
	float toKFTime)
{
	mFromKeyframeTime = fromKFTime;
	mToKeyframeTime = toKFTime;

	float rangeTime = mToKeyframeTime - mFromKeyframeTime;
	SetTime(rangeTime);
	SetNormalTime(rangeTime);

	auto it = mKeyframeCtrlMap.begin();
	for (; it != mKeyframeCtrlMap.end(); it++)
	{
		KeyframeController *kfCtrl = it->second;
		kfCtrl->Phase = fromKFTime;
		kfCtrl->MinTime = fromKFTime;
		kfCtrl->MaxTime = toKFTime;
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Animation3DSkeleton::RegistProperties()
{
	Animation::RegistProperties();

	AddPropertyClass("Animation3DSkeleton");
}
//----------------------------------------------------------------------------
void Animation3DSkeleton::OnPropertyChanged(const PropertyObject &obj)
{
	Animation::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Animation3DSkeleton::Animation3DSkeleton(LoadConstructor value)
:
Animation(value)
{
	mMovingScale = APoint(1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------
void Animation3DSkeleton::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Animation::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadString(mTagName);

	PX2_END_DEBUG_STREAM_LOAD(Animation3DSkeleton, source);
}
//----------------------------------------------------------------------------
void Animation3DSkeleton::Link(InStream& source)
{
	Animation::Link(source);
}
//----------------------------------------------------------------------------
void Animation3DSkeleton::PostLink()
{
	Animation::PostLink();
}
//----------------------------------------------------------------------------
bool Animation3DSkeleton::Register(OutStream& target) const
{
	if (Animation::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Animation3DSkeleton::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Animation::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteString(mTagName);

	PX2_END_DEBUG_STREAM_SAVE(Animation3DSkeleton, target);
}
//----------------------------------------------------------------------------
int Animation3DSkeleton::GetStreamingSize(Stream &stream) const
{
	int size = Animation::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += PX2_STRINGSIZE(mTagName);

	return size;
}
//----------------------------------------------------------------------------