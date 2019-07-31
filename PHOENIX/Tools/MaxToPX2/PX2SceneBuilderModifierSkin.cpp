// PX2SceneBuilderSkinModifier.cpp

#include "PX2SceneBuilder.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
struct In 
{ 
	float data; 
	int index; 
};
//----------------------------------------------------------------------------
int cmp( const void *a ,const void *b)
{ 
	return (*(In *)b).data > (*(In *)a).data ? 1 : -1; 

} 
//----------------------------------------------------------------------------
void SceneBuilder::ProcessSkin(INode *node, Modifier *skinMod)
{
	// 构造皮肤控制器。如果Max的网格被按照材质细分，每一个网格都需要自己的蒙皮
	// 信息控制器。蒙皮信息中的offset，在动画起始时被计算，是骨骼的世界变换。
	//
	// node:
	//		指向蒙皮修改器指向的Max中的节点。
	// skinMod:
	//		指向蒙皮修改器

	// 1. 获得max蒙皮信息中的骨骼，对应的在Phoenix的骨骼节点列表
	// 2. 获得maxNode影响的Phoenix网格
	// 3. 获得max中每个骨骼所影响的Phoenix网格中的顶点的数量，忽略不受蒙皮信息
	//	  影响的网格
	// 4. 计算Phoenix mesh的蒙皮信息，生成SkinControl，AttachController到
	//    Phoenix mesh上。
	
	// 1

	bool needDel;
	TriObject *triObj = GetTriObject(node, &needDel);
	Mesh *maxMesh = &triObj->GetMesh();

	// Max皮肤控制器接口
	ISkin *skin = (ISkin*)skinMod->GetInterface(I_SKIN);
	ISkinContextData *skinData = skin->GetContextInterface(node);

	// max Skin Bones -> Phoenix2 Skin Bones
	int b, numSkinBone = skin->GetNumBones();
	PX2::Node **bones = new1<PX2::Node*>(numSkinBone);
	for (b=0; b<numSkinBone; b++)
	{
		INode *boneNode = skin->GetBone(b);
		const std::string &boneName = boneNode->GetName();
		if (!boneName.empty())
		{
			std::vector<PX2::Object *> nodes;
			mScene->GetAllObjectsByName(boneName, nodes);
			if (1 == nodes.size())
			{
				bones[b] = DynamicCast<Node>(nodes[0]);
			}
			else
			{
				assertion(false, "");
			}

			//PX2::Node *node = PX2::StaticCast<PX2::Node>(mScene->GetObjectByName(boneName));
			//bones[b] = node;
		}
		else
		{
			assertion(false, "");
		}
	}

	// 1

	// 获得maxNode相关联的Phoenix mesh
	std::vector<PX2::TriMesh*> meshes;
	PX2::Object *object = mScene->GetObjectByName(node->GetName());
	if (object->IsExactly(PX2::TriMesh::TYPE))
	{
		meshes.push_back(PX2::StaticCast<PX2::TriMesh>(object));
	}
	else
	{
		PX2::Node *node = PX2::StaticCast<PX2::Node>(object);
		const char *nName = node->GetName().c_str();
		for (int c=0; c<node->GetNumChildren(); c++)
		{
			PX2::Movable *child = node->GetChild(c);
			const char *cName = child->GetName().c_str();
			if (strncmp(cName, nName, strlen(nName)) == 0) // 这里必须是strlen(nName),因为子节点有_1,_2
			{
				meshes.push_back(PX2::StaticCast<PX2::TriMesh>(child));
			}
		}
	}

	// 为Phoenix2的每个网格建立相关的皮肤控制器

	int *boneInfuseNumVert = new1<int>(numSkinBone);
	for (int m=0; m<(int)meshes.size(); m++)
	{
		PX2::TriMesh *mesh = meshes[m];

		// Phoenix顶点在max顶点中的索引
		PX2::VertexBuffer *vb = mesh->GetVertexBuffer();
		int px2MeshVertexNum = vb->GetNumElements();
		std::vector<int> MaxVertexIndex; // i->max索引
		int v, i, j, k;

		PX2::VertexBufferAccessor vba(mesh->GetVertexFormat(), vb);

		// 3

		for (int v=0; v<px2MeshVertexNum; ++v)
		{
			Float3 &position = vba.Position<Float3>(v);
			for (i=0; i<maxMesh->getNumVerts(); i++)
			{
				if (position[0] == maxMesh->verts[i].x
					&& position[1] == maxMesh->verts[i].y 
					&& position[2] == maxMesh->verts[i].z)
				{
					MaxVertexIndex.push_back(i);
					break;
				}
			}
		}

		// 确定每个骨骼所影响的顶点数量
		int maxVertexSize = (int)MaxVertexIndex.size();
		memset(boneInfuseNumVert, 0, sizeof(int)*numSkinBone);
		for (i=0; i<maxVertexSize; i++)
		{
			v = MaxVertexIndex[i];
			for (j=0; j<skinData->GetNumAssignedBones(v); j++)
			{ // 这个max中的顶点受到几个骨骼影响啊？！
				b = skinData->GetAssignedBone(v, j); // 获得这个影响的骨骼索引(这个j是第几个)
				boneInfuseNumVert[b]++; // 这个骨骼影响的顶点数量++
			}
		}

		// (通过PX2中的顶点找到Max中的顶点，找到Max中影响该顶点的骨骼)

		// 如果Max的网格是被按照材质分割的，可能一些骨骼对当前Phoenix2网格没有
		// 影响
		int bQuantity = 0; // 影响当前Phoenix2网格的骨骼数量
		for (b=0; b<numSkinBone; b++)
		{
			if (boneInfuseNumVert[b] > 0)
				bQuantity++;
		}

		if (bQuantity == 0)
		{
			// Phoenix网格不被任何骨骼影响，进入下一个网格
			continue;
		}

		// 4

		PX2::Node **theBones = new1<PX2::Node*>(bQuantity);
		float **weight = new2<float>(bQuantity, maxVertexSize);
		memset(weight[0],0,bQuantity*maxVertexSize*sizeof(float));
		PX2::APoint **offset = new2<PX2::APoint>(bQuantity, maxVertexSize);
		memset(offset[0],0,bQuantity*maxVertexSize*sizeof(PX2::APoint));
		PX2::HMatrix *mats = new1<PX2::HMatrix>(bQuantity);

		// 计算max骨骼到Phoenix骨骼对应的索引(k)
		std::vector<int> bIArray(numSkinBone);
		for (b=0, k=0; b<numSkinBone; b++)
		{
			if (boneInfuseNumVert[b] > 0)
			{
				theBones[k] = bones[b]; // 获取对Mesh有影响的骨骼
				bIArray[b] = k; // max bone index -> px2 可用bone index
				
				HMatrix boneWorldMat = theBones[k]->WorldTransform.Matrix();
				HMatrix meshWorldMat =  mesh->WorldTransform.Matrix();

				mats[k] = boneWorldMat.Inverse() * meshWorldMat;
				
				k++;
			}
		}

		// 遍历顶点，计算顶点权重和offset
		for (i=0; i<maxVertexSize; i++)
		{
			v = MaxVertexIndex[i];
			for (j=0; j<skinData->GetNumAssignedBones(v); j++)
			{ // 遍历影响该Max顶点的骨骼
				b = skinData->GetAssignedBone(v, j);
				k = bIArray[b];
				float wit = skinData->GetBoneWeight(v, j); // 第j个骨骼的影响权重
				weight[i][k] = wit;

				Float3 &position = vba.Position<Float3>(i);

				APoint point = theBones[k]->WorldTransform.Inverse() 
					* (mesh->WorldTransform * APoint(position));
				offset[i][k] = Float3(point.X(), point.Y(), point.Z()); // 在所受影响骨骼中的位置
			}
		}

		PX2::SkinController *skinCtrl = new0 PX2::SkinController
			(maxVertexSize, bQuantity);
		skinCtrl->SetName("SkinController");
		for (int i=0; i<bQuantity; i++)
		{
			skinCtrl->GetBones()[i] = theBones[i];
			skinCtrl->GetTMMatrixs()[i] = mats[i];
		}

		// offset
		for (int i=0; i<maxVertexSize; i++)
		{
			for (int j=0; j<bQuantity; j++)
			{
				skinCtrl->GetWeights()[i][j] = weight[i][j];
				skinCtrl->GetOffsets()[i][j] = offset[i][j]; 
			}
		}

		// index weights
		for (int i=0; i<maxVertexSize; i++)
		{
			Float4 inds = Float4(0.0f, 0.0f, 0.0f, 0.0f);
			Float4 wights = Float4(0.0f, 0.0f, 0.0f, 0.0f);

			In *pSortBone = new In[bQuantity];
			for (int j=0; j<bQuantity; j++)
			{
				pSortBone[j].index = j;
				pSortBone[j].data = weight[i][j];
			}
			qsort(pSortBone, bQuantity, sizeof(pSortBone[0]), cmp);

			int useBoneNum = bQuantity;
			if (useBoneNum > 4)
				useBoneNum = 4;

			float allWeight = 0.0f;
			for (int useBoneIndex=0; useBoneIndex<useBoneNum; useBoneIndex++)
			{
				allWeight += pSortBone[useBoneIndex].data;
			}
			if (allWeight <= 0.0f)
				allWeight = 1.0f;

			for (int useBoneIndex=0; useBoneIndex<useBoneNum; useBoneIndex++)
			{
				inds[useBoneIndex] = (float)(pSortBone[useBoneIndex].index);
				wights[useBoneIndex] = pSortBone[useBoneIndex].data/allWeight;
			}

			vba.TCoord<Float4>(1, i) = inds;
			vba.TCoord<Float4>(2, i) = wights;

			delete [] pSortBone;
		}
		
		skinCtrl->Repeat = Controller::RT_NONE;
		skinCtrl->MinTime = 0.0f;
		skinCtrl->MaxTime = TicksToSec(mTimeEnd - mTimeStart);

		mesh->AttachController(skinCtrl);

		delete1(theBones);
		delete2(weight);
		delete2(offset);
		delete1(mats);
	}

	if (needDel)
	{
		delete0(triObj);
	}

	delete1(bones);
	delete1(boneInfuseNumVert);
}
//----------------------------------------------------------------------------