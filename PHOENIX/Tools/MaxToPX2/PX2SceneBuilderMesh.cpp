// PX2SceneBuilderMesh.cpp

#include "PX2SceneBuilder.hpp"
#include "PX2UniMaterialMesh.hpp"
#include <set>

//----------------------------------------------------------------------------
TriObject *SceneBuilder::GetTriObject(INode* maxNode, bool *needDel)
{
	// 确定Max的节点是否有Mesh物体。
	//
	// pkNode:
	//		要检测的节点。
	// pbNeedDel:
	//		true，在使用完之后，调用者需要删除返回的TriObject*；false，不需要。
	// 返回mesh物体指针，如果没有与节点相关的mesh，返回0。

	Object *obj = maxNode->EvalWorldState(mTimeStart).obj;
	if (!obj)
		return 0;

	if (!obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
		return 0;

	TriObject *triObject = (TriObject*)obj->ConvertToType(mTimeStart,
		Class_ID(TRIOBJ_CLASS_ID,0));

	if (triObject == 0)
		return 0;
	
	*needDel = (triObject!=obj ? true : false);

	return triObject;
}
//----------------------------------------------------------------------------
PX2::Movable *SceneBuilder::BuildMesh(INode *maxNode,
									 PX2::Node *relatParentOrEqualNode, bool hasSkin)
{
	// 将Max的三角形网格数据转换到一个或者更多的等价的Phoenix2三角形网格。
	//
	// maxNode：
	//		Max场景图中的Mesh节点。
	// relatParentOrEqualNode:
	//		在Phoenix2场景图系统中最新创建的父亲节点。
	// 返回在Phoenix2场景中指向新的孩子节点的指针，这个指针直接指向TriMesh物体；
	// 或者是一个“link”节点，“link”的多个孩子TriMesh代表Max中的多个孩子mesh。

	bool needDel = false;
	TriObject *triObject = GetTriObject(maxNode, &needDel);
	if (!triObject)
	{
		return 0;
	}

	Mesh *maxMesh = &triObject->GetMesh();

	Mtl *mtl = maxNode->GetMtl();
	int mtlIndex = mMtls.GetIndex(mtl);

	// 判断这个Max的几何图形节点是否有“子几何图形节点”，如果有子几何图形节点
	// isEqualNode为真，反之为假。
	// 如果名称相等，就不是relatParentOrEqualNode了，而是equalNode
	PX2::Movable *link = 0;
	bool isEqualNode = (relatParentOrEqualNode->GetName().length()>0 &&
		strcmp(maxNode->GetName(), relatParentOrEqualNode->GetName().c_str()) == 0);

	// maxName
	const char *maxName = maxNode->GetName();

	// 如果只需要一个Phoenix的Mesh表示Max的Mesh，直接将Phoenix的Mesh链接到
	// Phoenix的场景图中；否则，创建一个"link"节点，将按照材质分割的子Mesh
	// 放在"link"下。
	int i;
	std::vector<UniMaterialMesh*> uMeshs;
	SplitGeometry(maxMesh, mtlIndex, uMeshs);
	if ((int)uMeshs.size() > 1)
	{
		if (!isEqualNode)
		{
			link = BuildNode(maxNode, relatParentOrEqualNode);
		}
		else
		{
			link = relatParentOrEqualNode;
		}

		assertion(link->IsDerived(PX2::Node::TYPE), "link must be a Node.");

		for (i=0; i<(int)uMeshs.size(); i++)
		{
			uMeshs[i]->SetExportSkin(hasSkin);

			PX2::TriMesh *triMesh = uMeshs[i]->ToTriMesh();

			if (triMesh)
			{
				char meshNumber[6];
				sprintf_s(meshNumber, 6, "_%d", i+1);
				size_t size = strlen(maxName) + strlen(meshNumber) + 1;
				char *tdName = new1<char>((int)size);
				strcpy_s(tdName, size, maxName);
				strcat_s(tdName, size, meshNumber);
				triMesh->SetName(tdName);
				delete1(tdName);
				((PX2::Node*)link)->AttachChild(triMesh);
			}
		}
	}
	else if ((int)uMeshs.size() == 1)
	{
		uMeshs[0]->SetExportSkin(hasSkin);

		PX2::TriMesh *triMesh = uMeshs[0]->ToTriMesh();
		if (triMesh)
		{
			if (!isEqualNode)
			{
				triMesh->SetName(maxName);
				triMesh->LocalTransform = GetLocalTransform(maxNode, mTimeStart);
			}
			else
			{
				size_t size = strlen(maxName) + 3;
				char *tdName = new1<char>((int)size);
				strcpy_s(tdName, size, maxName);
				strcat_s(tdName, size, "_1");
				triMesh->SetName(tdName);
				delete1(tdName);
			}

			assertion(relatParentOrEqualNode->IsDerived(PX2::Node::TYPE), 
				"relatParentOrEqualNode must be a Node.");

			relatParentOrEqualNode->AttachChild(triMesh);
			link = triMesh;
		}
	}

	for (i=0; i<(int)uMeshs.size(); i++)
	{
		delete0(uMeshs[i]);
	}

	if (needDel)
	{
		delete0(triObject);
	}

	return link;
}
//----------------------------------------------------------------------------
void SceneBuilder::SplitGeometry(Mesh *maxMesh, int mtlIndex, 
								 std::vector<UniMaterialMesh*> &uMeshes)
{
	// 如果这个Mesh有多个material并且使用两个或者多个material。这个网格需要
	// 被拆分，因为Phoenix2是一个物体只对应一个material。
	//
	// maxMesh:
	//		要分割的Max网格。
	// mtlIndex:
	//		材质ID
	// uMeshes:
	//		UniMaterialMesh集合，Max的mesh被拆分后放里面。

	int i, j;
	PX2::Float3 *normalsAll = 0;
	if (mSettings->IncludeNormals)
	{
		maxMesh->buildNormals();
		normalsAll = new1<PX2::Float3>(maxMesh->numVerts);
		for (i=0; i<maxMesh->numFaces; i++)
		{
			Face &face = maxMesh->faces[i];
			for (j=0; j<3; j++)
			{
				int vertexIndex = face.getVert(j);
				normalsAll[vertexIndex] = GetVertexNormal(maxMesh, i, vertexIndex);
			}
		}
	}

	// 没有材质
	if (mtlIndex < 0)
	{
		UniMaterialMesh *triMesh = new0 UniMaterialMesh;
		triMesh->SetMaterialInstance(PX2::VertexColor4Material::CreateUniqueInstance());

		std::vector<int> faceIndexs;
		int faceIndex = -1;
		for (faceIndex=0; faceIndex<maxMesh->numFaces; faceIndex++)
		{
			faceIndexs.push_back(faceIndex);
		}

		PackVertices(triMesh, maxMesh, faceIndexs, normalsAll);

		if (mSettings->IncludeVertexColors && maxMesh->numCVerts>0)
		{
			triMesh->SetExportColor(true);
			PackColors(triMesh, maxMesh, faceIndexs);
		}

		if (mSettings->IncludeTargentBiNormal && maxMesh->numTVerts>0)
		{
			triMesh->SetExportTargentBinormal(true);
		}
		else
		{
			triMesh->SetExportTargentBinormal(false);
		}

		if (mSettings->IncludeTexCoords && maxMesh->numTVerts>0)
		{
			triMesh->SetNumTexcoordToExport(mSettings->NumTexCoords);
			PackTextureCoords(triMesh, maxMesh, faceIndexs);
		}

		uMeshes.push_back(triMesh);

		triMesh->DuplicateGeometry();
	}
	else
	{
		// 获得Mtl的子材质数量
		MtlTree &tree = mMtlTreeList[mtlIndex];
		int subQuantity = 0; // 子材质数量
		if (mtlIndex >= 0)
		{
			subQuantity = tree.GetMChildQuantity();
		}

		// 计算几何图形所使用的最大附加材质索引
		int faceIndex, subID, maxSubID = -1;
		for (faceIndex=0; faceIndex<maxMesh->numFaces; faceIndex++)
		{
			subID = maxMesh->faces[faceIndex].getMatID();
			if (subID >= subQuantity)
			{
				if (subQuantity > 0)
				{
					subID = subID % subQuantity;
				}
				else
				{
					subID = 0;
				}
			}

			if (subID > maxSubID)
			{
				maxSubID = subID;
			}
		}

		if (-1 != maxSubID)
		{
			// 根据material ID，将每个三角形面分类
			std::vector<int> *faceIndexPartByMtl = new1<std::vector<int> >(maxSubID+1);
			for (faceIndex=0; faceIndex<maxMesh->numFaces; faceIndex++)
			{
				subID = maxMesh->faces[faceIndex].getMatID();
				if (subID >= subQuantity)
				{
					if (subQuantity > 0)
					{
						subID = subID % subQuantity;
					}
					else
					{
						subID = 0;
					}
				}
				//  将每个面按照材质分类
				faceIndexPartByMtl[subID].push_back(faceIndex);
			}

			// 对每种材质类型，分配网格
			for (subID=0; subID<=maxSubID; subID++)
			{
				if (faceIndexPartByMtl[subID].size() == 0)
				{
					// 这种材质没有三角形面
					continue;
				}

				// 为每种材质新建一个mesh
				UniMaterialMesh *triMesh = new0 UniMaterialMesh;
				if (mtlIndex >= 0)
				{
					PX2::MaterialInstance *mi = 0;

					if (subQuantity > 0)
					{
						MtlTree &subtree = tree.GetMChild(subID);
						triMesh->SetShineProperty(subtree.GetShine());

						mi = subtree.GetMaterialInstance();
					}
					else
					{
						triMesh->SetShineProperty(tree.GetShine());

						mi = tree.GetMaterialInstance();
					}

					triMesh->SetMaterialInstance(mi);
				}

				PackVertices(triMesh, maxMesh, faceIndexPartByMtl[subID], normalsAll);

				if (mSettings->IncludeVertexColors && maxMesh->numCVerts>0)
				{
					PackColors(triMesh, maxMesh, faceIndexPartByMtl[subID]);
					PackAlphas(triMesh, maxMesh, faceIndexPartByMtl[subID]);
				}

				if (mSettings->IncludeTargentBiNormal && maxMesh->numTVerts>0)
				{
					triMesh->SetExportTargentBinormal(true);
				}
				else
				{
					triMesh->SetExportTargentBinormal(false);
				}

				if (mSettings->IncludeTexCoords && maxMesh->numTVerts>0)
				{
					triMesh->SetNumTexcoordToExport(mSettings->NumTexCoords);
					PackTextureCoords(triMesh, maxMesh, faceIndexPartByMtl[subID]);
				}

				uMeshes.push_back(triMesh);
			}

			delete1(faceIndexPartByMtl);
		}

		for (i=0; i<(int)uMeshes.size(); i++)
		{
			uMeshes[i]->DuplicateGeometry();
		}
	}

	delete1(normalsAll);
}
//----------------------------------------------------------------------------
void SceneBuilder::PackVertices (UniMaterialMesh* uniMesh, Mesh* maxMesh,
									std::vector<int>& faceIndexParts,
									PX2::Float3 *normalsAll)
{
	// 对Max的网格进行分割，(根据三角形顶点面的索引)，将顶点数据打包进uniMesh。
	// 
	// uniMesh
	//		到打包到的UniMaterialMesh
	// maxMesh
	//		顶点数据来源
	// faceIndexParts
	//		从此获得面，再根据面获得顶点索引
	// normalsAll
	//		法线数据来源

	// 通过Set获取max中的顶点索引
	// Stl的set按照递增顺序插入元素
	std::set<int> vertexIndexs; // Set
	int i, j;
	for (i=0; i<(int)faceIndexParts.size(); i++)
	{
		Face &face = maxMesh->faces[faceIndexParts[i]];
		for (j = 0; j < 3; j++)
		{
			vertexIndexs.insert(face.v[j]);
		}
	}

	if (vertexIndexs.size() == 0)
		return;

	// vMap要足够大，去容纳可能的j，这依赖于Stl::set递增插入元素
	// vMap[k] == -1 表示max中的k顶点不在这个Phoenix网格中
	int indexMax = *vertexIndexs.rbegin();
	int *vMap = new1<int>(indexMax+1);// max vertex index -> Phoenix vertex index
	memset(vMap, 0xFF, (indexMax+1)*sizeof(int));

	uniMesh->VQuantity() = (int)vertexIndexs.size();
	uniMesh->VertexMap() = new1<PX2::Float3>(uniMesh->VQuantity());
	uniMesh->NormalMap() = new1<PX2::Float3>(uniMesh->VQuantity());

	std::set<int>::iterator iter = vertexIndexs.begin();
	for (i=0; i<(int)vertexIndexs.size(); i++, iter++)
	{
		j = *iter; // max vertex index
		vMap[j] = i; // max vertex index -> Phoenix vertex index

		(uniMesh->VertexMap()[i])[0] = maxMesh->verts[j].x;
		(uniMesh->VertexMap()[i])[1] = maxMesh->verts[j].y;
		(uniMesh->VertexMap()[i])[2] = maxMesh->verts[j].z;

		if (normalsAll)
			uniMesh->NormalMap()[i] = normalsAll[j];
	}

	// 建立Phoenix2网格，面的顶点索引
	uniMesh->FQuantity() = (int)faceIndexParts.size();
	uniMesh->Face() = new1<int>(3*uniMesh->FQuantity());
	for (i = 0; i < (int)faceIndexParts.size(); i++)
	{
		Face &face = maxMesh->faces[faceIndexParts[i]];
		for (j = 0; j < 3; j++)
		{
			uniMesh->Face()[3*i+j] = vMap[face.v[j]];
		}
	}
	delete1(vMap);
}
//----------------------------------------------------------------------------
void SceneBuilder::PackColors (UniMaterialMesh* uniMesh, Mesh* maxMesh, 
								  std::vector<int>& faceIndexParts)
{
	// 对Max的网格进行分割，(根据三角形颜色面的索引)，将颜色数据打包进uniMesh。
	// 
	// uniMesh
	//		到打包到的UniMaterialMesh
	// maxMesh
	//		顶点数据来源
	// faceIndexParts
	//		从此获得颜色面，再根据颜色面获得颜色索引

	// 获得此网格顶点颜色数据的索引
	std::set<int> colorIndexs;
	int i, j;
	for (i = 0; i < (int)faceIndexParts.size(); i++)
	{
		TVFace &face = maxMesh->vcFace[faceIndexParts[i]];
		for (j = 0; j < 3; j++)
		{
			colorIndexs.insert(face.t[j]);
		}
	}

	int colorIndexMax = *colorIndexs.rbegin();
	int *colorMap = new1<int>(colorIndexMax+1);
	memset(colorMap, 0xFF, (colorIndexMax+1)*sizeof(int));

	uniMesh->CQuantity() = (int)colorIndexs.size();
	uniMesh->ColorMap() = new1<PX2::Float4>(uniMesh->CQuantity());

	std::set<int>::iterator iter = colorIndexs.begin();
	for (i = 0; i < (int)colorIndexs.size(); i++, iter++)
	{
		j = *iter;
		colorMap[j] = i;

		(uniMesh->ColorMap()[i])[0] = maxMesh->vertCol[j].x;
		(uniMesh->ColorMap()[i])[1] = maxMesh->vertCol[j].y;
		(uniMesh->ColorMap()[i])[2] = maxMesh->vertCol[j].z;
		(uniMesh->ColorMap()[i])[3] = 1.0f;
	}

	uniMesh->CFace() = new1<int>(3*uniMesh->FQuantity());
	for (i = 0; i < (int)faceIndexParts.size(); i++)
	{
		TVFace &colorFace = maxMesh->vcFace[faceIndexParts[i]];
		for (j = 0; j < 3; j++)
		{
			uniMesh->CFace()[3*i+j] = colorMap[colorFace.t[j]];
		}
	}	
	delete1(colorMap);
}
//----------------------------------------------------------------------------
void SceneBuilder::PackAlphas(UniMaterialMesh* uniMesh, Mesh* maxMesh, 
	std::vector<int>& faceIndexParts)
{
	// 是否支持Alpha
	bool isVertexAlpha = (TRUE == maxMesh->mapSupport(-VDATA_ALPHA));
	if (!isVertexAlpha) isVertexAlpha = (TRUE == maxMesh->mapSupport(MAP_ALPHA));

	if (!isVertexAlpha)
		return;

	TVFace* _TvFaceArray = maxMesh->mapFaces(MAP_ALPHA);
	UVVert* _TabTVertArray = maxMesh->mapVerts(MAP_ALPHA);

	std::set<int> alphaIndexs;
	for (int i = 0; i < (int)faceIndexParts.size(); i++)
	{
		int faceIndex = faceIndexParts[i];

		const TVFace& tvface = _TvFaceArray[faceIndex];

		for (int j=0; j<3; j++)
		{
			alphaIndexs.insert(tvface.t[j]);
		}
	}

	int alpahIndexMax = *alphaIndexs.rbegin();
	int *alphaMap = new1<int>(alpahIndexMax+1);
	memset(alphaMap, 0xFF, (alpahIndexMax+1)*sizeof(int));

	uniMesh->AQuantity() = (int)alphaIndexs.size();
	uniMesh->AlphaMap() = new1<float>(uniMesh->AQuantity());

	if (_TvFaceArray && _TabTVertArray)
	{
		std::set<int>::iterator kIter = alphaIndexs.begin();
		for (int i = 0; i < (int)alphaIndexs.size(); i++, kIter++)
		{
			int j = *kIter; // max中的索引
			alphaMap[j] = i; // max坐标索引(j)->我们自己的字典缩影(i)

			Color pickCol(0, 0, 0);
			pickCol = _TabTVertArray[j];
			uniMesh->AlphaMap()[i] = pickCol.r;
		}
	}

	uniMesh->AFace() = new1<int>(3*uniMesh->FQuantity());

	for (int i = 0; i<(int)faceIndexParts.size(); i++)
	{
		TVFace& rkTFace = _TvFaceArray[faceIndexParts[i]];

		for (int j=0; j<3; j++)
		{
			uniMesh->AFace()[3*i+j] = alphaMap[rkTFace.t[j]];
		}
	}

	delete1(alphaMap);
}
//----------------------------------------------------------------------------
void SceneBuilder::PackTextureCoords (UniMaterialMesh* uniMesh,	 
									  Mesh* maxMesh,
									  std::vector<int>& faceIndexParts)
{
	// 对Max的网格进行分割，(根据三角形颜色面的索引)，将贴图纹理坐标数据打包进
	// UniMaterialMesh。
	//
	// uniMesh
	//		到打包到的UniMaterialMesh
	// maxMesh
	//		顶点数据来源
	// faceIndexParts
	//		从此获得贴图坐标面，再通过贴图做表面获得贴图坐标索引
	
	// 将纹理所用到的纹理坐标放到一起（形成一个独一无二的字典）
	std::set<int> texIndexs;
	std::set<int> texIndexs1;

	bool isHasTwoUVs = false;
	int iNumberOfMaxChannels = maxMesh->getNumMaps();
	for(int iMaxChannelID=1; iMaxChannelID<iNumberOfMaxChannels; iMaxChannelID++)
	{
		if (maxMesh->mapSupport(iMaxChannelID) && maxMesh->getNumMapVerts(iMaxChannelID)>0)
		{
			if (iMaxChannelID >= 2) isHasTwoUVs = true;

			TVFace* _TvFaceArray = maxMesh->mapFaces(iMaxChannelID);
			UVVert* _TabTVertArray = maxMesh->mapVerts(iMaxChannelID);

			if (_TvFaceArray && _TabTVertArray)
			{
				for (int i=0; i<(int)faceIndexParts.size(); i++)
				{
					int faceIndex = faceIndexParts[i];

					const TVFace& tvface = _TvFaceArray[faceIndex];

					for (int j=0; j<3; j++)
					{
						if (1 == iMaxChannelID)
							texIndexs.insert(tvface.t[j]);
						else if (2 == iMaxChannelID)
							texIndexs1.insert(tvface.t[j]);
					}
				}
			}
		}
	}

	int texIndexMax = *texIndexs.rbegin();
	int *texMap = new1<int>(texIndexMax+1);
	memset(texMap, 0xFF, (texIndexMax + 1)*sizeof(int));

	int texIndexMax1 = 0;
	int *texMap1 = 0;
	if (isHasTwoUVs)
	{
		texIndexMax1 = *texIndexs1.rbegin();
		texMap1 = new1<int>(texIndexMax1+1);
		memset(texMap1, 0xFF, (texIndexMax1 + 1)*sizeof(int));
	}

	uniMesh->TQuantity() = (int)texIndexs.size();
	uniMesh->TextureCoordMap() = new1<PX2::Float2>(uniMesh->TQuantity());

	if (isHasTwoUVs)
	{
		uniMesh->TQuantity1() = (int)texIndexs1.size();
		uniMesh->TextureCoordMap1() = new1<PX2::Float2>(uniMesh->TQuantity1());
	}

	for(int iMaxChannelID=1; iMaxChannelID<iNumberOfMaxChannels; iMaxChannelID++)
	{
		if (maxMesh->mapSupport(iMaxChannelID) && maxMesh->getNumMapVerts(iMaxChannelID)>0)
		{		
			TVFace* _TvFaceArray = maxMesh->mapFaces(iMaxChannelID);
			UVVert* _TabTVertArray = maxMesh->mapVerts(iMaxChannelID);

			if (_TvFaceArray && _TabTVertArray)
			{
				if (1 == iMaxChannelID)
				{
					std::set<int>::iterator kIter = texIndexs.begin();
					for (int i = 0; i < (int)texIndexs.size(); i++, kIter++)
					{
						int j = *kIter; // max中的索引
						texMap[j] = i; // max坐标索引(j)->我们自己的字典缩影(i)

						uniMesh->TextureCoordMap()[i][0] = _TabTVertArray[j].x;
						uniMesh->TextureCoordMap()[i][1] = 1.0f - _TabTVertArray[j].y;
					}
				}
				else if (2 == iMaxChannelID)
				{
					std::set<int>::iterator kIter = texIndexs1.begin();
					for (int i = 0; i < (int)texIndexs1.size(); i++, kIter++)
					{
						int j = *kIter; // max中的索引
						texMap1[j] = i;

						uniMesh->TextureCoordMap1()[i][0] = _TabTVertArray[j].x;
						uniMesh->TextureCoordMap1()[i][1] = 1.0f - _TabTVertArray[j].y;
					}
				}
			}
		}
	}

	// face
	for(int iMaxChannelID=1; iMaxChannelID<iNumberOfMaxChannels; iMaxChannelID++)
	{
		if (maxMesh->mapSupport(iMaxChannelID) && maxMesh->getNumMapVerts(iMaxChannelID)>0)
		{		
			TVFace* _TvFaceArray = maxMesh->mapFaces(iMaxChannelID);
			UVVert* _TabTVertArray = maxMesh->mapVerts(iMaxChannelID);

			if (_TvFaceArray && _TabTVertArray)
			{
				if (1 == iMaxChannelID)
				{
					uniMesh->TFace() = new1<int>(3*uniMesh->FQuantity());

					for (int i = 0; i<(int)faceIndexParts.size(); i++)
					{
						TVFace& rkTFace = _TvFaceArray[faceIndexParts[i]];

						for (int j=0; j<3; j++)
						{
							uniMesh->TFace()[3*i+j] = texMap[rkTFace.t[j]];
						}
					}
				}
				else if (2 == iMaxChannelID)
				{
					uniMesh->TFace1() = new1<int>(3*uniMesh->FQuantity());

					for (int i = 0; i<(int)faceIndexParts.size(); i++)
					{
						TVFace& rkTFace = _TvFaceArray[faceIndexParts[i]];

						for (int j=0; j<3; j++)
						{
							uniMesh->TFace1()[3*i+j] = texMap1[rkTFace.t[j]];
						}
					}
				}
			}
		}
	}

	delete1(texMap);
	delete1(texMap1);
}
//----------------------------------------------------------------------------
PX2::Float3 SceneBuilder::GetVertexNormal (Mesh* maxMesh, int faceIndex,
										  int vertexIndex)
{
	// 获得网格中一个特定顶点的法线
	//
	// maxMesh：
	//		包含这个顶点的网格
	// faceIndex:
	//		顶点属于的网格面
	// vertexIndex:
	//		顶点的顶点索引
	// 返回法线向量

	Point3 normal;

	RVertex* pkRV = maxMesh->getRVertPtr(vertexIndex);

	if (pkRV->rFlags & SPECIFIED_NORMAL)
	{
		normal = pkRV->rn.getNormal();
	}
	else
	{
		// 如果RVertex不包含特定的法向量，如果存在smoothing group，从
		// smoothing group计算；如果不存在smoothing group，使用面的法线。
		Face& rkFace = maxMesh->faces[faceIndex];
		DWORD dwSmoothingGroup = rkFace.smGroup;
		int numNormals = (pkRV->rFlags & NORCT_MASK);
		if (numNormals && dwSmoothingGroup)
		{
			if (numNormals == 1)
			{
				// smoothing group里只有一个法线，rkRV已经存储它了。
				normal = pkRV->rn.getNormal();
			}
			else
			{
				// 从包含这个面的smoothing group里查找法线
				for (int i = 0; i < numNormals; i++)
				{
					if (pkRV->ern[i].getSmGroup() & dwSmoothingGroup)
					{
						normal = pkRV->ern[i].getNormal();
						break;
					}
				}
			}
		}
		else
		{
			// 当smoothing group不存在时，使用面的法线，作为顶点的法线。
			normal = maxMesh->FaceNormal(faceIndex, true);
		}
	}

	PX2::AVector nor = PX2::AVector(normal.x, normal.y, normal.z);
	nor.Normalize();

	return PX2::Float3(nor.X(), nor.Y(), nor.Z());
}
//----------------------------------------------------------------------------
