// PX2ModelControllerMovable.cpp

#include "PX2ModelController.hpp"
#include "PX2StringHelp.hpp"
#include "PX2Animation3DSkeleton.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2XMLData.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void ModelController::SetMovableFilename(const std::string &filename, 
	bool shareVI)
{
	mMovableFilename = filename;

	mIsShareVI = shareVI;
	ObjectLoadFun1 loadFun = PX2_GR.msUserLoadFun1;

	Object *obj = 0;
	if (mIsShareVI)
	{
		obj = loadFun(filename.c_str(), true, true, false);
	}
	else
	{
		obj = loadFun(filename.c_str(), false, false, false);
	}

	Movable *mov = DynamicCast<Movable>(obj);
	SetMovable(mov);
}
//----------------------------------------------------------------------------
void ModelController::SetMovable(Movable *mov)
{
	Node *node = DynamicCast<Node>(GetControlledable());

	if (mMovable && node)
		node->DetachChild(mMovable);

	mMovable = mov;

	if (mMovable)
	{
		mMovable->SetSaveWriteIngore(true);
		mMovable->SetReceiveShadow(true);
	}

	if (mMovable && node)
		node->AttachChild(mMovable);

	CollectAnchors();

	if (AT_SKELETON == mAnimType)
	{
		_CalSkins();

		// 为动作树每个Node加一个BlendTransformController
		if (mModelAnimMovable)
			_CalAnimNode(mModelAnimMovable);

		SetMovableAutoWorldBoundRadius(mMovableAutoWorldBoundRadius);
	}
}
//----------------------------------------------------------------------------
void ModelController::SetSkinAnimationConfig(const std::string &filename)
{
	mSkinAnimationConfig = filename;

	if (mModelAnimMovable)
		_DetachKeyframeCtrl(mModelAnimMovable);

	int bufSize = 0;
	char *buf = 0;

	BufferLoadFun loadFun = PX2_GR.msBufferLoadFun;
	if (loadFun(filename.c_str(), bufSize, buf))
	{
		XMLData xd;
		if (xd.LoadBuffer(buf, bufSize))
		{
			XMLNode rootNode = xd.GetRootNode();

			std::string resStr = rootNode.AttributeToString("res");
			std::string outPath;
			std::string outBaseFilename;
			StringHelp::SplitFilename(filename, outPath, outBaseFilename);

			std::string animResFilename = outPath + resStr;

			XMLNode childNode = rootNode.IterateChild();
			while (!childNode.IsNull())
			{
				std::string nStr = childNode.AttributeToString("n");
				std::string vStr = childNode.AttributeToString("v");
				bool isLoop = childNode.AttributeToBool("loop");
				int key0 = -1;
				if (childNode.HasAttribute("key0"))
					key0 = childNode.AttributeToInt("key0");
				int key1 = -1;
				if (childNode.HasAttribute("key1"))
					key1 = childNode.AttributeToInt("key1");
				int actKey = -1;
				if (childNode.HasAttribute("act"))
					actKey = childNode.AttributeToInt("act");
				int val0 = 0;
				int val1 = 0;
				StringHelp::StringToVal01(vStr, val0, val1);

				float timeVal = 1.0f / 30.0f;

				float fromTime = val0*timeVal;
				float toTime = val1*timeVal;

				Animation3DSkeletonPtr anim = new0 Animation3DSkeleton();
				anim->SetName(nStr);
				anim->SetFilename(animResFilename);
				anim->SetKeyframeRangeTime(fromTime, toTime);
				anim->SetPlayOnce(!isLoop);

				anim->AddAnimationCall("begin", 0.0f, 0);

				if (key0 > 0)
				{
					float time = key0 * timeVal - fromTime;
					anim->AddAnimationCall("key0", time, 0);
				}

				if (key1 > 0)
				{
					float time = key1 * timeVal - fromTime;
					anim->AddAnimationCall("key1", time, 0);
				}

				if (actKey > 0)
				{
					float time = actKey * timeVal - fromTime;
					anim->AddAnimationCall("act", time, 0);
				}

				float endTime = toTime - fromTime;
				anim->AddAnimationCall("end", endTime, 0);

				AddAnim(anim);

				childNode = childNode.IterateChild(childNode);
			}
		}
	}

}
//----------------------------------------------------------------------------
void ModelController::SetSkinAnimationConfig(const std::string &filename,
	Movable *animMovable)
{
	mSkinAnimationConfig = filename;

	if (mModelAnimMovable)
		_DetachKeyframeCtrl(mModelAnimMovable);

	int bufSize = 0;
	char *buf = 0;

	BufferLoadFun loadFun = PX2_GR.msBufferLoadFun;
	if (loadFun(filename.c_str(), bufSize, buf))
	{
		XMLData xd;
		if (xd.LoadBuffer(buf, bufSize))
		{
			XMLNode rootNode = xd.GetRootNode();

			std::string resStr = rootNode.AttributeToString("res");
			std::string outPath;
			std::string outBaseFilename;
			StringHelp::SplitFilename(filename, outPath, outBaseFilename);

			std::string animResFilename = outPath + resStr;

			XMLNode childNode = rootNode.IterateChild();
			while (!childNode.IsNull())
			{
				std::string nStr = childNode.AttributeToString("n");
				std::string vStr = childNode.AttributeToString("v");
				bool isLoop = childNode.AttributeToBool("loop");
				int key0 = -1;
				if (childNode.HasAttribute("key0"))
					key0 = childNode.AttributeToInt("key0");
				int key1 = -1;
				if (childNode.HasAttribute("key1"))
					key1 = childNode.AttributeToInt("key1");
				int actKey = -1;
				if (childNode.HasAttribute("act"))
					actKey = childNode.AttributeToInt("act");
				int val0 = 0;
				int val1 = 0;
				StringHelp::StringToVal01(vStr, val0, val1);

				float timeVal = 1.0f / 30.0f;

				float fromTime = val0*timeVal;
				float toTime = val1*timeVal;

				Animation3DSkeleton *anim = new0 Animation3DSkeleton();
				anim->SetName(nStr);
				anim->SetMovable(animMovable);
				anim->SetKeyframeRangeTime(fromTime, toTime);
				anim->SetPlayOnce(!isLoop);

				anim->AddAnimationCall("begin", 0.0f, 0);

				if (key0 > 0)
				{
					float time = key0 * timeVal - fromTime;
					anim->AddAnimationCall("key0", time, 0);
				}

				if (key1 > 0)
				{
					float time = key1 * timeVal - fromTime;
					anim->AddAnimationCall("key1", time, 0);
				}

				if (actKey > 0)
				{
					float time = actKey * timeVal - fromTime;
					anim->AddAnimationCall("act", time, 0);
				}

				float endTime = toTime - fromTime;
				anim->AddAnimationCall("end", endTime, 0);

				AddAnim(anim);

				childNode = childNode.IterateChild(childNode);
			}
		}
	}
}
//----------------------------------------------------------------------------
void ModelController::SetHeading(const AVector &heading)
{
	mHeading = heading;
}
//----------------------------------------------------------------------------
void ModelController::CollectAnchors()
{
	mAnchorMap.clear();

	Node *node = DynamicCast<Node>(GetControlledable());
	if (node)
		_CollectAnchor(node);
}
//----------------------------------------------------------------------------
APoint ModelController::GetAnchorPos(int id)
{
	if (!mMovable) return APoint::ORIGIN;

	Node *node = GetAnchor(id);

	if (node)
		return node->WorldTransform.GetTranslate();

	return APoint::ORIGIN;
}
//----------------------------------------------------------------------------
Node *ModelController::GetAnchor(int id)
{
	if (!mMovable) return 0;

	std::map<int, Node*>::iterator it = mAnchorMap.find(id);
	if (it != mAnchorMap.end())
	{
		return it->second;
	}

	return 0;
}
//----------------------------------------------------------------------------
void ModelController::_CollectAnchor(Movable *mov)
{
	if (!mov) return;

	Node *node = DynamicCast<Node>(mov);
	if (node)
	{
		int anchorId = node->GetAnchorID();
		if (0 != anchorId)
		{
			mAnchorMap[anchorId] = node;
		}

		for (int i = 0; i < node->GetNumChildren(); i++)
		{
			Node *childNode = DynamicCast<Node>(node->GetChild(i));
			if (childNode)
			{
				_CollectAnchor(childNode);
			}
		}
	}
}
//----------------------------------------------------------------------------