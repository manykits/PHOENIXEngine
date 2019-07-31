// PX2SceneBuilder.cpp

#include "PX2SceneBuilder.hpp"
#include "PX2Time.hpp"

const float SceneBuilder::ATTENUATION = 1e+03f;
const float SceneBuilder::MIN_DIFFERENCE = 1e-03f;
//----------------------------------------------------------------------------
SceneBuilder::SceneBuilder (const TCHAR *filename, BOOL exportSelected, 
							ExportSettings *settings, ExpInterface *ept,
							Interface *max, INode *exportNode)
{
	float time = (float)PX2::Time::GetTimeInSeconds();
	PX2_UNUSED(time);

	const size_t size = _tcslen(filename)+1;
	mFileName = new1<TCHAR>((int)size);
	_tcscpy_s(mFileName, size, filename);

	mExportSelected = exportSelected;
	mSettings = settings;
	mExport = ept;
	mMax = max;
	mExportMaxNode = exportNode;
	mTicksPerFrame = GetTicksPerFrame();

	if (mSettings->IncludeCurrentFrame)
	{
		mTimeStart = mMax->GetAnimRange().Start();
		mTimeEnd = mTimeStart;
	}
	else
	{
		mTimeStart = mSettings->StartFrame * GetTicksPerFrame();
		mTimeEnd = mSettings->EndFrame * GetTicksPerFrame();

		if (mSettings->UseLocalTime)
		{
			mTimeOffset = mTimeStart;
		}
		else
		{
			mTimeOffset = 0;
		}
	}

	// 遍历场景3次。第1次处理materials，第2次遍历节点继承关系，第3次应用 modifiers

	mScene = new0 PX2::Node();
	mScene->SetName("RootNode");

	// 1
	CollectMaterials(mExportMaxNode);
	ConvertMaterials();

	// 2
	if (!mExportSelected)
	{
		// entire scene

		if (mSettings->IncludeLights)
		{
			BuildAmbientLight();
		}

		int numChildren = mExportMaxNode->NumberOfChildren();
		for (int i=0; i<numChildren; i++)
		{
			Traverse(mExportMaxNode->GetChildNode(i), mScene);
		}
	}
	else
	{
		// selection only

		Traverse(mExportMaxNode, mScene);
	}
	
	mScene->Update(0.0f);

	// 3
	if (mSettings->IncludeModifiers)
	{
		ApplyModifiers();
	}

	PX2::OutStream stream;

	if (mExportSelected)
	{
		stream.Insert(mScene->GetChild(0));
	}
	else
	{
		stream.Insert(mScene);
	}

	stream.Save(filename);

	mScene = 0;
}
//----------------------------------------------------------------------------
SceneBuilder::~SceneBuilder ()
{
	delete1(mFileName);
	mScene = 0;
	std::vector<ModifierInfo*>::iterator iter;
	for (iter=mModifierList.begin(); iter<mModifierList.end(); iter++)
	{
		delete0(*iter);
	}
}
//---------------------------------------------------------------------------
static bool IsNodeRenderable( INode *pnode, Object *pobj )
{
	if(!pobj || !pobj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
		return false;

	if (pobj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0)) 
		return false;
	
	if (pobj->ClassID() == Class_ID(BONE_CLASS_ID, 0)) 
		return false;

#if MAX_RELEASE >= 4000
	if( pobj->ClassID() == BONE_OBJ_CLASSID )
		return false;
#endif

	// check for biped node
	Control *pControl = pnode->GetTMController();
	if( (pControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) || 
		(pControl->ClassID() == BIPBODY_CONTROL_CLASS_ID) || 
		(pControl->ClassID() == FOOTPRINT_CLASS_ID) )
	{
		return false;
	}

	return true;
}
//----------------------------------------------------------------------------
bool SceneBuilder::Traverse(INode *maxNode, PX2::Node *relatParent)
{
	// movbale对应maxNode的父节点

	bool isHasSkin = false;

	// Modifier
	if (mSettings->IncludeModifiers)
	{
		ModifierInfo *modInfo = new0 ModifierInfo;
		CollectModifiers(maxNode, modInfo->Modifiers);
		if (modInfo->Modifiers.empty())
		{
			delete0(modInfo);
		}
		else
		{
			modInfo->Node = maxNode;
			mModifierList.push_back(modInfo);

			for (int i=0; i<(int)modInfo->Modifiers.size(); i++)
			{
				Modifier *modifier = modInfo->Modifiers[i];
				Class_ID id = modifier->ClassID();
				if (id == SKIN_CLASSID ||
					id == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
				{
					isHasSkin = true;
				}
			}
		}
	}

	// Node
	const char *nodeName = maxNode->GetName();

	if(stricmp(nodeName, "PHYSICSDATA") == 0)
	{
	}
	else if(stricmp(nodeName, "PORTALDATA") == 0)
	{
	}

	PX2::Movable *child = 0;

	ObjectState objectState = maxNode->EvalWorldState(mTimeStart);
	bool supported = true;

	// Object
	if (objectState.obj)
	{
		switch (objectState.obj->SuperClassID())
		{
		case GEOMOBJECT_CLASS_ID:
			if (IsNodeRenderable(maxNode, objectState.obj))
				child = BuildGeometry(maxNode, relatParent, isHasSkin);
			else
				child = BuildNode(maxNode, relatParent);
			break;
		case CAMERA_CLASS_ID:
			// ToDo
			break;
		case LIGHT_CLASS_ID:
			if (mSettings->IncludeLights && !maxNode->IsHidden())
			{
				BuildLight(maxNode, relatParent);
			}
			break;
		case HELPER_CLASS_ID:
			//supported = false;
			child = BuildNode(maxNode, relatParent);
			break;
		default:
			assertion(false, "Some object type not supportted.");
			supported = false;
			break;
		}
	}

	// Keyframe
	if (child)
	{
		if (mSettings->IncludeKeyFrames)
			BuildKeyFrameController(maxNode, child);
		else
			BuildFrameController(maxNode, child);
	}

	// Child
	int numChildren = maxNode->NumberOfChildren();
	if (numChildren == 0)
		return true;

	PX2::Node *childNode = 0;
	childNode = PX2::DynamicCast<PX2::Node>(child);
	if (childNode == 0)
		return true;

	for (int i=0; i<numChildren; i++)
	{
		if (!Traverse(maxNode->GetChildNode(i), childNode) || mMax->GetCancel())
			return false;
	}

	return true;
}
//----------------------------------------------------------------------------
PX2::Movable *SceneBuilder::BuildGeometry (INode *maxNode, PX2::Node *relatParent, bool hasSkin)
{
	// 建立并且链接几何图形物体到场景
	// maxNode：
	//		Max场景中节点的指针
	// relatParent:
	//		当前新节点要绑定到的父亲节点

	// 如果Max“几何图形”节点没有孩子节点，直接在relatParent下添加几何图形。
	// 如果Max“几何图形”节点有孩子节点，会新建一个Node节点，将孩子几何图形节
	// 点添加到这个新建的Node节点下。

	PX2::Movable *child = 0;
	PX2::Node *link = 0;

	int numMaxChildren = maxNode->NumberOfChildren();
	for (int i=0; i<numMaxChildren; i++)
	{
		ObjectState objectState = maxNode->GetChildNode(i)
			->EvalWorldState(mTimeStart);
		if (objectState.obj->SuperClassID() == GEOMOBJECT_CLASS_ID ||
			objectState.obj->SuperClassID() == HELPER_CLASS_ID)
		{
			link = BuildNode(maxNode, relatParent);
			break;
		}
	}

	if (link == 0)
	{
		if (mSettings->IncludeMeshes && !maxNode->IsHidden())
		{
			child = BuildMesh(maxNode, relatParent, hasSkin);
		}
		else
		{
			child = BuildNode(maxNode, relatParent);
		}

		return child;
	}
	else
	{
		if (mSettings->IncludeMeshes && !maxNode->IsHidden())
		{
			child = BuildMesh(maxNode, link, hasSkin);
		}
		else
		{
			child = BuildNode(maxNode, link);
		}

		return link;
	}
}
//----------------------------------------------------------------------------
PX2::Node *SceneBuilder::BuildNode(INode *maxNode, PX2::Node *relatParent)
{
	PX2::Node *child = new0 PX2::Node;
	child->SetName(maxNode->GetName());
	child->LocalTransform = GetLocalTransform(maxNode, mTimeStart);

	relatParent->AttachChild(child);

	TSTR strUserData;
	maxNode->GetUserPropBuffer(strUserData);
	if (strstr(strUserData, "a"))
	{
		int anchorID = 0;
		maxNode->GetUserPropInt("a", anchorID);

		child->SetAnchorID(anchorID);
	}

	return child;
}
//----------------------------------------------------------------------------
PX2::Transform SceneBuilder::GetLocalTransform (INode *node, TimeValue time)
{
	// 计算节点的本地变换。Max节点的变换方法提供的节点的世界变换，所以我们
	// 必须做一些操纵去获得节点的本地变换。

	Matrix3 maxLocal = node->GetObjTMAfterWSM(time) *
		Inverse(node->GetParentNode()->GetObjTMAfterWSM(time));

	// 分解变换
	AffineParts affParts;
	decomp_affine(maxLocal, &affParts);

	// Position
	bool isTranslationZero = 
		fabsf(affParts.t.x) < MIN_DIFFERENCE &&
		fabsf(affParts.t.y) < MIN_DIFFERENCE &&
		fabsf(affParts.t.z) < MIN_DIFFERENCE;

	// Rotation
	float qSign = (affParts.q.w >= 0.0f ? 1.0f : -1.0f);
	bool isRotationIndentity = 
		fabsf(qSign*affParts.q.w - 1.0f) < MIN_DIFFERENCE &&
		fabsf(affParts.q.x) < MIN_DIFFERENCE &&
		fabsf(affParts.q.y) < MIN_DIFFERENCE &&
		fabsf(affParts.q.z) < MIN_DIFFERENCE;

	// Reflect
	bool hasReflection = (affParts.f < 0.0f);

	// Uniform scale
	bool isScaleUniform = (fabsf(affParts.k.x - affParts.k.y)<MIN_DIFFERENCE &&
		fabsf(affParts.k.y - affParts.k.z)<MIN_DIFFERENCE);

	// Unity scale
	bool isScaleUnity = isScaleUniform &&
		fabsf(affParts.k.x - 1.0f) < MIN_DIFFERENCE;

	// Scale orientation is identity?
	float uSign = (affParts.u.w >= 0.0f ? 1.0f : -1.0f);
	bool isOrientIndentity = isScaleUniform || (
		fabsf(uSign*affParts.u.w - 1.0f) < MIN_DIFFERENCE &&
		fabsf(affParts.u.x) < MIN_DIFFERENCE &&
		fabsf(affParts.u.y) < MIN_DIFFERENCE &&
		fabsf(affParts.u.z) < MIN_DIFFERENCE);

	// 计算Phoenix2等价变换
	PX2::Transform local;

	if (!isTranslationZero)
	{
		local.SetTranslate(PX2::APoint(affParts.t.x, affParts.t.y,
			affParts.t.z));
	}

	if (hasReflection)
	{
		affParts.k *= -1.0f;
	}
	
	if (isScaleUniform)
	{
		// 矩阵的形式为R*(s*I)，s是统一缩放矩阵。
		if (!isRotationIndentity)
		{
			PX2::HMatrix rot;
			PX2::HQuaternion(affParts.q.w, -affParts.q.x, -affParts.q.y,
				-affParts.q.z).ToRotationMatrix(rot);
			local.SetRotate(rot);
		}

		if (!isScaleUnity)
		{
			local.SetUniformScale(affParts.k.x);
		}
	}
	else if (isOrientIndentity)
	{
		if (!isRotationIndentity)
		{
			PX2::HMatrix rot;
			PX2::HQuaternion(affParts.q.w, -affParts.q.x, -affParts.q.y,
				-affParts.q.z).ToRotationMatrix(rot);
			local.SetRotate(rot);
		}

		local.SetScale(PX2::APoint(affParts.k.x, affParts.k.y, affParts.k.z));
	}
	else
	{
		PX2::Matrix3f mat(
			maxLocal.GetAddr()[0][0],
			maxLocal.GetAddr()[1][0],
			maxLocal.GetAddr()[2][0],
			maxLocal.GetAddr()[0][1],
			maxLocal.GetAddr()[1][1],
			maxLocal.GetAddr()[2][1],
			maxLocal.GetAddr()[0][2],
			maxLocal.GetAddr()[1][2],
			maxLocal.GetAddr()[2][2]);

		local.SetMatrix(PX2::HMatrix(mat));
	}

	return local;
}
//----------------------------------------------------------------------------