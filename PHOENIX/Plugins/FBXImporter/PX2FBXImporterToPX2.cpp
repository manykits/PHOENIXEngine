// PX2FBXImporterToPX2.cpp

#include "PX2FBXImporter.hpp"
#include "PX2VertexColor4Material.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2StringHelp.hpp"
#include "PX2MathHelp.hpp"
#include "PX2SkinController.hpp"
#include "PX2KeyframeController.hpp"
#include "PX2Creater.hpp"
#include "PX2Time.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Node *FBXImporter::ConvertToPX2()
{
	FbxNode *rootNode = mFbxScene->GetRootNode();

	Node *node = PX2ProcessNode(rootNode);
	PX2ProcessMesh(rootNode);

	node->Update(Time::GetTimeInSeconds(), 0.1f);

	PX2ProcessMtl(rootNode);
	PX2ProcessAnimFrames();

	return node;
}
//----------------------------------------------------------------------------
Node *FBXImporter::PX2ProcessNode(FbxNode* fbxNode)
{
	auto it = mFbxNode2Nodes.find(fbxNode);
	if (it == mFbxNode2Nodes.end()) return 0;

	Node *node = it->second;

	for (int i = 0; i < fbxNode->GetChildCount(); ++i)
	{
		Node *newNode = PX2ProcessNode(fbxNode->GetChild(i));
		node->AttachChild(newNode);
	}

	return node;
}
//----------------------------------------------------------------------------
void FBXImporter::PX2ProcessMesh(FbxNode* fbxNode)
{
	auto it = mFbxNode2Nodes.find(fbxNode);
	if (it == mFbxNode2Nodes.end()) return;

	Node *node = mFbxNode2Nodes[fbxNode];

	if (fbxNode->GetNodeAttribute())
	{
		switch (fbxNode->GetNodeAttribute()->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
		{
										Movable *mov = PX2ConvertMeshs(fbxNode->GetMesh());
										if (mov) node->AttachChild(mov);
		}
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < fbxNode->GetChildCount(); ++i)
	{
		FbxNode *childNode = fbxNode->GetChild(i);
		if (childNode)
		{
			PX2ProcessMesh(childNode);
		}
	}
}
//----------------------------------------------------------------------------
void FBXImporter::PX2ProcessMtl(FbxNode* fbxNode)
{
	if (fbxNode->GetNodeAttribute())
	{
		switch (fbxNode->GetNodeAttribute()->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
		{
										PX2ProcessMtl(fbxNode->GetMesh());
		}
			break;
		default:
			break;
		}
	}

	for (int i = 0; i < fbxNode->GetChildCount(); ++i)
	{
		PX2ProcessMtl(fbxNode->GetChild(i));
	}
}
//----------------------------------------------------------------------------
inline void updateAnimTime(FbxAnimCurve *const &curve, AnimInfo &ts, 
	const float &animStart, const float &animStop) 
{
	FbxTimeSpan fts;
	curve->GetTimeInterval(fts);
	const FbxTime start = fts.GetStart();
	const FbxTime stop = fts.GetStop();
	ts.start = std::max(animStart, std::min(ts.start, (float)(start.GetMilliSeconds())));
	ts.stop = std::min(animStop, std::max(ts.stop, (float)stop.GetMilliSeconds()));
	// Could check the number and type of keys (ie curve->KeyGetInterpolation) to lower the framerate
	ts.framerate = std::max(ts.framerate, (float)stop.GetFrameRate(FbxTime::eDefaultMode));
}
//----------------------------------------------------------------------------
inline void collectKeyFrames(FbxAnimCurve *const &curve,
	std::list<float>& keyframesTime)
{
	int key_cout = curve->KeyGetCount();
	for (int i = 0; i < key_cout; ++i)
	{
		FbxAnimCurveKey curveKey = curve->KeyGet(i);
		FbxTime fbx_time = curveKey.GetTime();
		float keytime = (float)fbx_time.GetMilliSeconds();

		if (keytime < 0) 
			continue;

		std::list<float>::iterator iter = std::find(keyframesTime.begin(), keyframesTime.end(), keytime);
		if (iter == keyframesTime.end())
		{
			keyframesTime.push_back(keytime);
		}
	}
}
//----------------------------------------------------------------------------
void FBXImporter::PX2ProcessAnimFrames()
{
	for (int i = 0; i < mFbxScene->GetSrcObjectCount<FbxAnimStack>(); i++)
	{
		FbxAnimStack* lAnimStack = mFbxScene->GetSrcObject<FbxAnimStack>(i);
		FbxString trackName = lAnimStack->GetName();
		FbxTimeSpan animTimeSpan = lAnimStack->GetLocalTimeSpan();
		float animStart = (float)(animTimeSpan.GetStart().GetMilliSeconds());
		float animStop = (float)(animTimeSpan.GetStop().GetMilliSeconds());

		const int layerCount = lAnimStack->GetMemberCount<FbxAnimLayer>();

		static std::map<FbxNode *, std::list<float> > keyframesTimeMap;
		static std::map<FbxNode *, AnimInfo> affectedNodes;

		for (int l = 0; l < layerCount; l++)
		{
			FbxAnimLayer *layer = lAnimStack->GetMember<FbxAnimLayer>(l);

			const int curveNodeCount = layer->GetSrcObjectCount<FbxAnimCurveNode>();
			for (int n = 0; n < curveNodeCount; n++)
			{
				FbxAnimCurveNode *curveNode = layer->GetSrcObject<FbxAnimCurveNode>(n);

				const int nc = curveNode->GetDstPropertyCount();
				for (int o = 0; o < nc; o++)
				{
					FbxProperty prop = curveNode->GetDstProperty(o);
					FbxNode *node = static_cast<FbxNode *>(prop.GetFbxObject());
					if (node)
					{
						FbxString propName = prop.GetName();

						if ((!node->LclTranslation.IsValid() || propName != node->LclTranslation.GetName()) &&
							(!node->LclScaling.IsValid() || propName != node->LclScaling.GetName()) &&
							(!node->LclRotation.IsValid() || propName != node->LclRotation.GetName()))
							continue;

						FbxAnimCurve *curve;

						AnimInfo ts;
						ts.translate = propName == node->LclTranslation.GetName();
						ts.rotate = propName == node->LclRotation.GetName();
						ts.scale = propName == node->LclScaling.GetName();

						bool find = false;
						std::list<float> keyframesTimeList;
						std::map<FbxNode *, std::list<float> >::iterator iter = keyframesTimeMap.find(node);
						if (iter != keyframesTimeMap.end())
							find = true;

						if (curve = prop.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X))
						{
							updateAnimTime(curve, ts, animStart, animStop);

							//if (settings->compressLevel >= COMPRESS_LEVEL_1)
							{
								if (find)
									collectKeyFrames(curve, keyframesTimeMap[node]);
								else
									collectKeyFrames(curve, keyframesTimeList);
							}
						}

						if (curve = prop.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y))
						{
							updateAnimTime(curve, ts, animStart, animStop);

							//if (settings->compressLevel >= COMPRESS_LEVEL_1)
							{
								if (find)
									collectKeyFrames(curve, keyframesTimeMap[node]);
								else
									collectKeyFrames(curve, keyframesTimeList);
							}
						}

						if (curve = prop.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z))
						{
							updateAnimTime(curve, ts, animStart, animStop);

							//if (settings->compressLevel >= COMPRESS_LEVEL_1)
							{
								if (find)
									collectKeyFrames(curve, keyframesTimeMap[node]);
								else
									collectKeyFrames(curve, keyframesTimeList);
							}
						}

						//if (ts.start < ts.stop)
						affectedNodes[node] += ts;

						//if (settings->compressLevel >= COMPRESS_LEVEL_1)
						{
							if (!find)
								keyframesTimeMap[node] = keyframesTimeList;

							keyframesTimeMap[node].sort();
						}
					}
				}
			}
		}

		if (affectedNodes.empty())
			return;

		lAnimStack->GetScene()->SetCurrentAnimationStack(lAnimStack);

		for (auto itr = affectedNodes.begin(); itr != affectedNodes.end(); itr++) 
		{
			auto itPX = mFbxNode2Nodes.find(itr->first);
			if (itPX == mFbxNode2Nodes.end())
				return;

			Node *pxNode = itPX->second;

			FbxTime fbxTime;
			// Calculate all keyframes upfront

			//std::list<float> keytimeList = keyframesTimeMap[(*itr).first];
			//int numFrames = keytimeList.size();
			//if (!keytimeList.empty())
			//{
			//	KeyframeController *kfc = new0 KeyframeController(numFrames,
			//		numFrames, numFrames, numFrames, pxNode->LocalTransform);
			//	pxNode->AttachController(kfc);
			//	kfc->SetName(trackName.Buffer());
			//	kfc->Repeat = PX2::Controller::RT_WRAP;
			//	kfc->MinTime = 0.0f;
			//	kfc->Active = true;
			//	kfc->Play();

			//	float maxTime = 0.0f;

			//	int kfcIndex = 0;
			//	for (const auto& val : keytimeList)
			//	{
			//		float time = val;
			//		time = std::min(time, (*itr).second.stop);

			//		fbxTime.SetMilliSeconds((FbxLongLong)time);
			//	
			//		APoint scale;
			//		HQuaternion rotate;
			//		APoint translate;
			//		FbxAMatrix &m = (*itr).first->EvaluateLocalTransform(fbxTime);
			//		FbxVector4 tran = m.GetT();
			//		translate[0] = (float)tran.mData[0];
			//		translate[1] = (float)tran.mData[1];
			//		translate[2] = (float)tran.mData[2];
			//		FbxQuaternion q;
			//		q = m.GetQ();
			//		rotate.X() = (float)q.mData[0];
			//		rotate.Y() = (float)q.mData[1];
			//		rotate.Z() = (float)q.mData[2];
			//		rotate.W() = (float)q.mData[3];
			//		FbxVector4 s = m.GetS();
			//		scale[0] = (float)s.mData[0];
			//		scale[1] = (float)s.mData[1];
			//		scale[2] = (float)s.mData[2];

			//		kfc->GetCommonTimes()[kfcIndex] = time * 0.01f;
			//		kfc->GetScales()[kfcIndex] = Float3(scale[0], scale[1], scale[2]);
			//		kfc->GetTranslations()[kfcIndex] = translate;
			//		kfc->GetRotations()[kfcIndex] = rotate;

			//		maxTime = time * 0.01f;

			//		kfcIndex++;
			//	}
			//	kfc->MaxTime = maxTime;
			//}
			{
				float stepSize = (*itr).second.framerate <= 0.f ?
					(*itr).second.stop - (*itr).second.start : 1000.f / (*itr).second.framerate;
				const float last = (*itr).second.stop + stepSize * 0.5f;

				int numFrames = (int)((last - (*itr).second.start) / stepSize)+1;
				KeyframeController *kfc = new0 KeyframeController(numFrames,
					numFrames, numFrames, numFrames, pxNode->LocalTransform);
				pxNode->AttachController(kfc);
				kfc->SetName(trackName.Buffer());
				kfc->Repeat = PX2::Controller::RT_WRAP;
				kfc->MinTime = 0.0f;
				kfc->Active = true;
				kfc->Play();

				int kfcIndex = 0;
				float maxTime = 0.0f;
				HQuaternion lastQ;
				for (float time = (*itr).second.start; time <= last; time += stepSize)
				{
					time = std::min(time, (*itr).second.stop);

					fbxTime.SetMilliSeconds((FbxLongLong)time);

					APoint scale;
					HQuaternion rotate;
					APoint translate;

					FbxAMatrix &m = (*itr).first->EvaluateLocalTransform(fbxTime);

					FbxVector4 tran = m.GetT();
					translate[0] = (float)tran.mData[0];
					translate[1] = (float)tran.mData[1];
					translate[2] = (float)tran.mData[2];
					FbxQuaternion q;
					q = m.GetQ();
					rotate.X() = (float)q.mData[0];
					rotate.Y() = (float)q.mData[1];
					rotate.Z() = (float)q.mData[2];
					rotate.W() = (float)q.mData[3];						
					float dotVal = lastQ.Dot(rotate);
					if (dotVal < 0.0f)
						rotate = -rotate;
					lastQ = rotate;
					FbxVector4 s = m.GetS();
					scale[0] = (float)s.mData[0];
					scale[1] = (float)s.mData[1];
					scale[2] = (float)s.mData[2];

					kfc->GetCommonTimes()[kfcIndex] = time * 0.001f;
					kfc->GetScales()[kfcIndex] = Float3(scale[0], scale[1], scale[2]);
					kfc->GetTranslations()[kfcIndex] = translate;
					kfc->GetRotations()[kfcIndex] = rotate;
					kfcIndex++;
					
					maxTime = time * 0.001f;
				}

				kfc->MaxTime = maxTime;
			}
		}
	}
}
//----------------------------------------------------------------------------
Movable *FBXImporter::PX2ConvertMeshs(FbxMesh* fbxMesh)
{
	auto it = mMeshes.find(fbxMesh);
	if (it == mMeshes.end()) return 0;

	Mesh *mesh = (*it).second;

	int numVertices = (int)mesh->mVertices.size();
	VertexFormatPtr vf = _GetVF(fbxMesh);
	VertexBufferPtr vb = new0 VertexBuffer(numVertices, vf->GetStride());
	VertexBufferAccessor vba(vf, vb);

	for (int i = 0; i < (int)mesh->mVertices.size(); i++)
	{
		PNTIWVertex &v = mesh->mVertices[i];

		vba.Position<Float3>(i) = v.mPosition;
		if (vba.HasColor(0))
			vba.Color<Float4>(i, 0) = v.mColor;
		if (vba.HasNormal())
			vba.Normal<AVector>(i) = v.mNormal;
		if (vba.HasTCoord(0))
			vba.TCoord<Float2>(0, i) = v.mUV;

		if (HasSkeleton())
		{
			Float4 inds;
			inds[0] = (float)v.mVertexBlendingInfos[0].BoneIndex;
			inds[1] = (float)v.mVertexBlendingInfos[1].BoneIndex;
			inds[2] = (float)v.mVertexBlendingInfos[2].BoneIndex;
			inds[3] = (float)v.mVertexBlendingInfos[3].BoneIndex;
			vba.TCoord<Float4>(1, i) = inds;

			Float4 weights;
			weights[0] = (float)v.mVertexBlendingInfos[0].BoneWeight;
			weights[1] = (float)v.mVertexBlendingInfos[1].BoneWeight;
			weights[2] = (float)v.mVertexBlendingInfos[2].BoneWeight;
			weights[3] = (float)v.mVertexBlendingInfos[3].BoneWeight;
			vba.TCoord<Float4>(2, i) = weights;
		}
	}
	
	Movable *mov = 0;
	int numMeshes = (int)mesh->mMaterialLookUp.size();
	if (numMeshes > 1)
		mov = new0 Node();

	for (auto it = mesh->mMaterialLookUp.begin(); it != mesh->mMaterialLookUp.end(); it++)
	{
		unsigned int mtlIndex = (*it).first;
		Mtl *mtl = (*it).second;
		mtl->mNumTriangles = 0;

		for (int i = 0; i < (int)mesh->mTriangles.size(); i++)
		{
			if (mtlIndex == mesh->mTriangles[i].mMaterialIndex)
			{
				mtl->mNumTriangles++;
			}
		}
	}

	for (auto it = mesh->mMaterialLookUp.begin(); it != mesh->mMaterialLookUp.end(); it++)
	{
		unsigned int mtlIndex = (*it).first;
		Mtl *mtl = (*it).second;

		if (mtl->mNumTriangles > 0)
		{
			IndexBufferPtr ib = new0 IndexBuffer((int)mtl->mNumTriangles * 3, 2);
			int tangleIndex = 0;

			for (int i = 0; i < (int)mesh->mTriangles.size(); i++)
			{
				if (mtlIndex == mesh->mTriangles[i].mMaterialIndex)
				{
					((unsigned short*)ib->GetData())[tangleIndex * 3 + 0] = (unsigned short)mesh->mTriangles[i].mIndices[0];
					((unsigned short*)ib->GetData())[tangleIndex * 3 + 1] = (unsigned short)mesh->mTriangles[i].mIndices[1];
					((unsigned short*)ib->GetData())[tangleIndex * 3 + 2] = (unsigned short)mesh->mTriangles[i].mIndices[2];

					tangleIndex++;
				}
			}

			PX2::TriMesh *triMesh = new0 PX2::TriMesh(vf, vb, ib);
			triMesh->UpdateModelSpace(Renderable::GU_MODEL_BOUND_ONLY);
			mFbxMesh2Meshes[fbxMesh].push_back(triMesh);
			mTriMesh2Mtls[triMesh] = mtl;

			if (1 == numMeshes)
			{
				mov = triMesh;
			}
			else
			{
				((Node*)mov)->AttachChild(triMesh);
			}
		}
	}

	return mov;
}
//----------------------------------------------------------------------------
void FBXImporter::PX2ProcessMtl(FbxMesh *fbxmesh)
{
	auto it = mFbxMesh2Meshes.find(fbxmesh);
	if (it == mFbxMesh2Meshes.end()) return;

	std::vector<TriMesh*> &meshVec = it->second;

	for (int i = 0; i < (int)meshVec.size(); i++)
	{
		TriMesh *triMesh = (*it).second[i];

		MaterialInstance *mi = 0;
		if (false)
		{
			int numVertices = triMesh->GetVertexBuffer()->GetNumElements();
			int bQuantity = (int)mSkeleton.mJoints.size();

			SkinController *skinCtrl = new0 SkinController(numVertices, bQuantity);
			skinCtrl->SetName("SkinController");
			for (int i = 0; i < bQuantity; i++)
			{
				Joint jt = mSkeleton.mJoints[i];

				auto it = mFbxNode2Nodes.find(jt.mNode);
				if (it != mFbxNode2Nodes.end())
				{
					Node *node = mFbxNode2Nodes[jt.mNode];
					skinCtrl->GetBones()[i] = node;
					
					HMatrix boneWorldMat = node->WorldTransform.Matrix();
					HMatrix meshWorldMat = triMesh->WorldTransform.Matrix();
					skinCtrl->GetTMMatrixs()[i] = mSkeleton.mJoints[i].TMMatrix;// boneWorldMat.Inverse() * meshWorldMat;
				}
				else
				{
					assertion(false, "No node");
				}
			}

			skinCtrl->Repeat = Controller::RT_NONE;
			skinCtrl->MinTime = 0.0f;
			skinCtrl->MaxTime = 20.0f;

			triMesh->AttachController(skinCtrl);

			mi = new0 MaterialInstance("Data/engine_mtls/skinskeleton/skinskeleton.px2obj", "skinskeleton", false);
		}
		else
		{
			mi = new0 MaterialInstance("Data/engine_mtls/std/std.px2obj", "std_light", false);
		}

		Mtl * mtl = mTriMesh2Mtls[triMesh];
		if (!mtl->mDiffuseMapName.empty())
		{
			std::string outPath;
			std::string outBaseFileName;
			StringHelp::SplitFilename(mtl->mDiffuseMapName, outPath, outBaseFileName);

			std::string beforeDirStr;
			size_t pos = outPath.find(mOutPathFilename);
			if (pos != std::string::npos)
			{
				beforeDirStr = outPath.substr(pos + mOutPathFilename.length(), outPath.length() - pos - outPath.length());
			}
			std::string texFilename = mOutPathFilename + beforeDirStr + outBaseFileName;
			Texture *texture = DynamicCast<Texture>(PX2_RM.BlockLoad(texFilename));
			if (texture)
			{
				mi->SetPixelTexture(0, "SampleBase", texture);
			}
		}
		else
		{
			mi = new0 MaterialInstance("Data/engine_mtls/materialcolor/materialcolor.px2obj", "materialcolor", false);
		}

		triMesh->SetMaterialInstance(mi);

		Shine *shine = new0 Shine();
		shine->Emissive = MathHelp::Float3ToFloat4(mtl->mEmissive, 1.0f);
		shine->Ambient = MathHelp::Float3ToFloat4(mtl->mAmbient, 1.0f);
		shine->Diffuse = MathHelp::Float3ToFloat4(mtl->mDiffuse, 1.0f);
		triMesh->SetShine(shine);
	}
}
//----------------------------------------------------------------------------