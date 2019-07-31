// PX2FBXImporter.cpp

#include "PX2FBXImporter.hpp"
#include "PX2Log.hpp"
#include "PX2StringHelp.hpp"
#include "PX2HQuaternion.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
FBXImporter::FBXImporter() :
mFbxManager(0),
mFbxScene(0),
mFbxImporter(0),
mFbxIOSettings(0)
{
	mFbxManager = FbxManager::Create();

	mFbxIOSettings = FbxIOSettings::Create(mFbxManager, IOSROOT);
	mFbxManager->SetIOSettings(mFbxIOSettings);

	mFbxIOSettings->SetBoolProp(IMP_FBX_MATERIAL, true);
	mFbxIOSettings->SetBoolProp(IMP_FBX_TEXTURE, true);
	mFbxIOSettings->SetBoolProp(IMP_FBX_LINK, true);
	mFbxIOSettings->SetBoolProp(IMP_FBX_SHAPE, true);
	mFbxIOSettings->SetBoolProp(IMP_FBX_GOBO, true);
	mFbxIOSettings->SetBoolProp(IMP_FBX_ANIMATION, true);
	mFbxIOSettings->SetBoolProp(IMP_SKINS, true);
	mFbxIOSettings->SetBoolProp(IMP_DEFORMATION, true);
	mFbxIOSettings->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	mFbxIOSettings->SetBoolProp(IMP_TAKE, true);
}
//----------------------------------------------------------------------------
FBXImporter::~FBXImporter()
{
	Clear();

	if (mFbxManager)
	{
		mFbxManager->Destroy();
		mFbxManager = 0;
	}
}
//----------------------------------------------------------------------------
FBXImporter* FBXImporter::New()
{
	return new0 FBXImporter();
}
//----------------------------------------------------------------------------
void FBXImporter::Delete(FBXImporter *importer)
{
	delete0(importer);
}
//----------------------------------------------------------------------------
void FBXImporter::Clear()
{
	for (auto it = mMeshes.begin(); it != mMeshes.end(); it++)
	{
		delete0(it->second);
	}
	mMeshes.clear();
	mSkeleton.mJoints.clear();

	if (mFbxImporter)
	{
		mFbxImporter->Destroy();
		mFbxImporter = 0;
	}

	if (mFbxScene)
	{
		mFbxScene->Destroy();
		mFbxScene = 0;
	}

	mPX2Scene = 0;
	mFbxNode2Nodes.clear();
	mFbxMesh2Meshes.clear();
	mTriMesh2Mtls.clear();
}
//----------------------------------------------------------------------------
Node *FBXImporter::GetPX2Node()
{
	return mPX2Scene;
}
//----------------------------------------------------------------------------
#define PX2_IOSET_REF (*(mFbxManager->GetIOSettings()))
//----------------------------------------------------------------------------
bool FBXImporter::Import(const std::string &filename)
{
	Clear();

	mFbxScene = FbxScene::Create(mFbxManager, "myScene");

	mModelFilename = filename;
	StringHelp::SplitFilename(mModelFilename, mOutPathFilename, mBaseFilename);

	mFbxImporter = FbxImporter::Create(mFbxManager, "MyImporter");
	if (!mFbxImporter)
		return false;

	if (!mFbxImporter->Initialize(filename.c_str(), -1, mFbxManager->GetIOSettings()))
		return false;

	if (!mFbxImporter->Import(mFbxScene)) 
		return false;
	
	mFbxImporter->Destroy();
	mFbxImporter = 0;

	ProcessSkeletonHierarchy(mFbxScene->GetRootNode());

	ProcessGeometry(mFbxScene->GetRootNode());

	Optimize();

	mPX2Scene = ConvertToPX2();

	return true;
}
//----------------------------------------------------------------------------
void FBXImporter::Optimize()
{
	for (auto it = mMeshes.begin(); it != mMeshes.end(); it++)
	{
		(*it).second->Optimize();
	}
}
//----------------------------------------------------------------------------
void FBXImporter::ProcessSkeletonHierarchy(FbxNode* fbxNode)
{
	std::string fbxNodeName = fbxNode->GetName();
	Node *node = new0 Node();
	node->SetName(fbxNodeName);
	mFbxNode2Nodes[fbxNode] = node;

	APoint scale;
	HQuaternion rotate;
	HMatrix rotMat;
	APoint translate;

	//FbxDouble3 fbxScale = fbxNode->LclScaling;
	//FbxDouble3 fbxRot = fbxNode->LclRotation;
	//FbxDouble3 fbxTrans = fbxNode->LclTranslation;
	//scale = _CovertP(fbxScale);
	//rotMat = Matrix3f().MakeEulerZYX(fbxRot[2] * Mathf::DEG_TO_RAD, fbxRot[1] * Mathf::DEG_TO_RAD, fbxRot[0] * Mathf::DEG_TO_RAD);
	//translate = _CovertP(fbxTrans);


	FbxAMatrix &m = fbxNode->EvaluateLocalTransform(FBXSDK_TIME_INFINITE);
	FbxVector4 v = m.GetT();
	translate[0] = (float)v.mData[0];
	translate[1] = (float)v.mData[1];
	translate[2] = (float)v.mData[2];
	FbxQuaternion q = m.GetQ();
	rotate.X() = (float)q.mData[0];
	rotate.Y() = (float)q.mData[1];
	rotate.Z() = (float)q.mData[2];
	rotate.W() = (float)q.mData[3];
	rotate.Normalize();
	FbxVector4 s = m.GetS();
	scale[0] = (float)s.mData[0];
	scale[1] = (float)s.mData[1];
	scale[2] = (float)s.mData[2];

	rotate.ToRotationMatrix(rotMat);

	node->LocalTransform.SetScale(scale);
	node->LocalTransform.SetRotate(rotMat);
	node->LocalTransform.SetTranslate(translate);

	for (int cIndex = 0; cIndex < fbxNode->GetChildCount(); ++cIndex)
	{
		FbxNode* currNode = fbxNode->GetChild(cIndex);
		ProcessSkeletonHierarchyRecursively(currNode, 0, 0, -1);
	}
}
//----------------------------------------------------------------------------
void FBXImporter::ProcessSkeletonHierarchyRecursively(FbxNode* inNode,
	int inDepth, int myIndex, int inParentIndex)
{
	std::string fbxNodeName = inNode->GetName();
	Node *node = new0 Node();
	node->SetName(fbxNodeName);
	mFbxNode2Nodes[inNode] = node;

	APoint scale;
	HQuaternion rotate;
	APoint translate;

	FbxAMatrix &m = inNode->EvaluateLocalTransform();
	FbxVector4 v = m.GetT();
	translate[0] = (float)v.mData[0];
	translate[1] = (float)v.mData[1];
	translate[2] = (float)v.mData[2];
	FbxQuaternion q = m.GetQ();
	rotate.X() = (float)q.mData[0];
	rotate.Y() = (float)q.mData[1];
	rotate.Z() = (float)q.mData[2];
	rotate.W() = (float)q.mData[3];
	rotate.Normalize();
	FbxVector4 s = m.GetS();
	scale[0] = (float)s.mData[0];
	scale[1] = (float)s.mData[1];
	scale[2] = (float)s.mData[2];

	HMatrix rotMat;
	rotate.ToRotationMatrix(rotMat);

	node->LocalTransform.SetScale(scale);
	node->LocalTransform.SetRotate(rotMat);
	node->LocalTransform.SetTranslate(translate);

	if (inNode->GetNodeAttribute() &&
		inNode->GetNodeAttribute()->GetAttributeType() &&
		inNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		if (!mSkeleton.HasJoint(inNode))
		{
			Joint currJoint;
			currJoint.mParentIndex = inParentIndex;
			currJoint.mNode = inNode;
			mSkeleton.mJoints.push_back(currJoint);
		}
		else
		{
			assertion(false, "Joint already exist!");
		}
	}
	for (int i = 0; i < inNode->GetChildCount(); i++)
	{
		ProcessSkeletonHierarchyRecursively(inNode->GetChild(i), inDepth + 1,
			mSkeleton.mJoints.size(), myIndex);
	}
}
//----------------------------------------------------------------------------
void FBXImporter::ProcessGeometry(FbxNode* inNode)
{
	if (inNode->GetNodeAttribute())
	{
		switch (inNode->GetNodeAttribute()->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
		{
										FbxMesh* currMesh = inNode->GetMesh();
										mMeshes[currMesh] = new0 Mesh();

										ProcessControlPoints(inNode);
										if (HasSkeleton()) ProcessJointsAndAnimations(inNode, currMesh);
										ProcessMesh(inNode, currMesh);
										AssociateMaterialToMesh(inNode, currMesh);
										ProcessMaterials(inNode, currMesh);
		}
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < inNode->GetChildCount(); ++i)
	{
		ProcessGeometry(inNode->GetChild(i));
	}
}
//----------------------------------------------------------------------------
void FBXImporter::ProcessControlPoints(FbxNode* inNode)
{
	FbxMesh* currMesh = inNode->GetMesh();
	unsigned int ctrlPointCount = currMesh->GetControlPointsCount();
	for (unsigned int i = 0; i < ctrlPointCount; ++i)
	{
		APoint curPoint;
		curPoint.X() = static_cast<float>(currMesh->GetControlPointAt(i).mData[0]);
		curPoint.Y() = static_cast<float>(currMesh->GetControlPointAt(i).mData[1]);
		curPoint.Z() = static_cast<float>(currMesh->GetControlPointAt(i).mData[2]);

		CtrlPoint* currCtrlPoint = new CtrlPoint();
		currCtrlPoint->mPosition = curPoint;

		mMeshes[currMesh]->mControlPoints[i] = currCtrlPoint;
	}
}
//----------------------------------------------------------------------------
void FBXImporter::ProcessJointsAndAnimations(FbxNode* inNode, FbxMesh *currMesh)
{
	FbxAMatrix geometryTransform = Utilities::GetGeometryTransformation(inNode);

	unsigned int numOfDeformers = currMesh->GetDeformerCount();
	for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
	{
		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex,
			FbxDeformer::eSkin));
		if (!currSkin) continue;

		// 计算mesh所受影响的骨骼
		unsigned int numOfClusters = currSkin->GetClusterCount();
		for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		{
			FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
			FbxNode *boneNode = currCluster->GetLink();
			unsigned int currJointIndex = FindJointIndex(boneNode);
		
			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			currCluster->GetTransformMatrix(transformMatrix);
			currCluster->GetTransformLinkMatrix(transformLinkMatrix);
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * (transformMatrix * geometryTransform);

			//// Update the information in mSkeleton 
			mSkeleton.mJoints[currJointIndex].TMMatrix = Utilities::FbxMatrixToPX2(globalBindposeInverseMatrix);

			// Associate each joint with the control points it affects
			unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
			for (unsigned int i = 0; i < numOfIndices; ++i)
			{
				int ctrlPIndex = currCluster->GetControlPointIndices()[i];

				BlendingPair bPair;
				bPair.BoneIndex = currJointIndex;
				bPair.BoneWeight = currCluster->GetControlPointWeights()[i];
				mMeshes[currMesh]->mControlPoints[ctrlPIndex]->mBlendingInfo.push_back(bPair);
			}
		}
	}

	// add empty
	BlendingPair bPair;
	bPair.BoneIndex = 0;
	bPair.BoneWeight = 0;
	for (auto itr = mMeshes[currMesh]->mControlPoints.begin(); 
		itr != mMeshes[currMesh]->mControlPoints.end(); ++itr)
	{
		for (unsigned int i = itr->second->mBlendingInfo.size(); i < 4; ++i)
		{
			itr->second->mBlendingInfo.push_back(bPair);
		}
	}
}
//----------------------------------------------------------------------------
unsigned int FBXImporter::FindJointIndex(FbxNode *fbxNode)
{
	for (int i = 0; i < (int)mSkeleton.mJoints.size(); i++)
	{
		if (fbxNode == mSkeleton.mJoints[i].mNode)
			return i;
	}

	return 0;
}
//----------------------------------------------------------------------------
bool FBXImporter::HasSkeleton()
{
	return !mSkeleton.mJoints.empty();
}
//----------------------------------------------------------------------------
APoint FBXImporter::_CovertP(FbxDouble3 &val)
{
	return APoint((float)val[0], (float)val[1], (float)val[2]);
}
//----------------------------------------------------------------------------
AVector FBXImporter::_CovertV(FbxDouble3 &val)
{
	return AVector((float)val[0], (float)val[1], (float)val[2]);
}
//----------------------------------------------------------------------------
APoint FBXImporter::_ConvertP(FbxVector4 &val)
{
	return APoint((float)val[0], (float)val[1], (float)val[2]);
}
//----------------------------------------------------------------------------
AVector FBXImporter::_ConvertV(FbxVector4 &val)
{
	return AVector((float)val[0], (float)val[1], (float)val[2]);
}
//----------------------------------------------------------------------------