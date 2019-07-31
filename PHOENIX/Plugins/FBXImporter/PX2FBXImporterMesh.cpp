// PX2FBXImporterMesh.cpp

#include "PX2FBXImporter.hpp"
#include "PX2Log.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2Float4.hpp"
#include "PX2Float3.hpp"
#include "PX2Creater.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
VertexFormat *FBXImporter::_GetVF(FbxMesh* fbxMesh)
{
	int hasColor = fbxMesh->GetElementVertexColorCount() > 1;
	int hasNormal = fbxMesh->GetElementNormalCount() > 1;
	int numUVs = fbxMesh->GetUVLayerCount();

	// VertexBuffer
	VertexFormat *vFormat = new0 VertexFormat();
	vFormat->Add(VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0);
	if (hasNormal)
	{
		vFormat->Add(VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0);
	}
	if (hasColor)
	{
		vFormat->Add(VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT4, 0);
	}

	if (1 == numUVs)
	{
		vFormat->Add(VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
	}
	else if (2 == numUVs)
	{
		vFormat->Add(VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
		vFormat->Add(VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 1);
	}
	if (HasSkeleton())
	{
		vFormat->Add(VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT4, 1);
		vFormat->Add(VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT4, 2);
	}

	vFormat->Create();

	return vFormat;
}
//----------------------------------------------------------------------------
void ReadVertex(FbxMesh* mesh, int ctrlPIndex, APoint* vertex)
{
	FbxVector4 *ctrlPoint = mesh->GetControlPoints();

	vertex->X() = (float)ctrlPoint[ctrlPIndex][0];
	vertex->Y() = (float)ctrlPoint[ctrlPIndex][1];
	vertex->Z() = (float)ctrlPoint[ctrlPIndex][2];
}
//----------------------------------------------------------------------------
void ReadColor(FbxMesh* mesh, int ctrlPIndex, int vertexCounter,
	Float4* color)
{
	if (mesh->GetElementVertexColorCount() < 1)
	{
		return;
	}

	FbxGeometryElementVertexColor* vertexColors = mesh->GetElementVertexColor(0);
	FbxLayerElement::EMappingMode mappingMode = vertexColors->GetMappingMode();
	FbxLayerElement::EReferenceMode refMode = vertexColors->GetReferenceMode();

	switch (mappingMode)
	{
	case FbxGeometryElement::eByControlPoint:
	{
												switch (refMode)
												{
												case FbxGeometryElement::eDirect:
												{
																					(*color)[0] = (float)(vertexColors->GetDirectArray().GetAt(ctrlPIndex).mRed);
																					(*color)[1] = (float)(vertexColors->GetDirectArray().GetAt(ctrlPIndex).mGreen);
																					(*color)[2] = (float)(vertexColors->GetDirectArray().GetAt(ctrlPIndex).mBlue);
																					(*color)[3] = (float)(vertexColors->GetDirectArray().GetAt(ctrlPIndex).mAlpha);
												} break;
												case FbxGeometryElement::eIndexToDirect:
												{
																						   int id = vertexColors->GetIndexArray().GetAt(ctrlPIndex);
																						   (*color)[0] = (float)vertexColors->GetDirectArray().GetAt(id).mRed;
																						   (*color)[1] = (float)vertexColors->GetDirectArray().GetAt(id).mGreen;
																						   (*color)[2] = (float)vertexColors->GetDirectArray().GetAt(id).mBlue;
																						   (*color)[3] = (float)vertexColors->GetDirectArray().GetAt(id).mAlpha;
												} break;
												default: break;
												}
	} break;
	case FbxGeometryElement::eByPolygonVertex:
	{
												 switch (refMode) {
												 case FbxGeometryElement::eDirect:
												 {
																					 (*color)[0] = (float)(vertexColors->GetDirectArray().GetAt(vertexCounter).mRed);
																					 (*color)[1] = (float)(vertexColors->GetDirectArray().GetAt(vertexCounter).mGreen);
																					 (*color)[2] = (float)(vertexColors->GetDirectArray().GetAt(vertexCounter).mBlue);
																					 (*color)[3] = (float)(vertexColors->GetDirectArray().GetAt(vertexCounter).mAlpha);
												 } break;
												 case FbxGeometryElement::eIndexToDirect:
												 {
																							int id = vertexColors->GetIndexArray().GetAt(vertexCounter);
																							(*color)[0] = (float)vertexColors->GetDirectArray().GetAt(id).mRed;
																							(*color)[1] = (float)vertexColors->GetDirectArray().GetAt(id).mGreen;
																							(*color)[2] = (float)vertexColors->GetDirectArray().GetAt(id).mBlue;
																							(*color)[3] = (float)vertexColors->GetDirectArray().GetAt(id).mAlpha;
												 } break;
												 default: break;
												 }
	} break;
	}
}
//----------------------------------------------------------------------------
void ReadUV(FbxMesh* mesh, int ctrlPIndex, int textureUVIndex,
	int uvLayer, Float2* uv)
{
	if (uvLayer >= 2 || mesh->GetElementUVCount() <= uvLayer)
	{
		return;
	}

	FbxGeometryElementUV* vertexUV = mesh->GetElementUV(uvLayer);
	FbxLayerElement::EMappingMode mappingMode = vertexUV->GetMappingMode();
	FbxLayerElement::EReferenceMode refMode = vertexUV->GetReferenceMode();

	switch (mappingMode)
	{
	case FbxGeometryElement::eByControlPoint:
	{
												switch (refMode)
												{
												case FbxGeometryElement::eDirect:
												{
																					(*uv)[0] = (float)vertexUV->GetDirectArray().GetAt(ctrlPIndex)[0];
																					(*uv)[1] = (float)vertexUV->GetDirectArray().GetAt(ctrlPIndex)[1];
												} break;
												case FbxGeometryElement::eIndexToDirect:
												{
																						   int id = vertexUV->GetIndexArray().GetAt(ctrlPIndex);
																						   (*uv)[0] = (float)vertexUV->GetDirectArray().GetAt(id)[0];
																						   (*uv)[1] = (float)vertexUV->GetDirectArray().GetAt(id)[1];
												} break;
												default: break;
												}
	}
		break;

	case FbxGeometryElement::eByPolygonVertex:
	{
												 switch (refMode)
												 {
												 case FbxGeometryElement::eDirect:
												 case FbxGeometryElement::eIndexToDirect:
												 {
																							(*uv)[0] = (float)vertexUV->GetDirectArray().GetAt(textureUVIndex)[0];
																							(*uv)[1] = (float)vertexUV->GetDirectArray().GetAt(textureUVIndex)[1];
												 } break;
												 default: break;
												 }
	}
		break;
	}
}
//----------------------------------------------------------------------------
void ReadNormal(FbxMesh* mesh, int ctrlPIndex, int vertexCounter,
	AVector* normal)
{
	if (mesh->GetElementNormalCount() < 1)
	{
		return;
	}

	FbxGeometryElementNormal* leNormal = mesh->GetElementNormal(0);
	FbxLayerElement::EMappingMode mappingMode = leNormal->GetMappingMode();
	FbxLayerElement::EReferenceMode refMode = leNormal->GetReferenceMode();

	switch (mappingMode)
	{
	case FbxGeometryElement::eByControlPoint:
	{
												switch (refMode)
												{
												case FbxGeometryElement::eDirect:
												{
																					normal->X() = (float)leNormal->GetDirectArray().GetAt(ctrlPIndex)[0];
																					normal->Y() = (float)leNormal->GetDirectArray().GetAt(ctrlPIndex)[1];
																					normal->Z() = (float)leNormal->GetDirectArray().GetAt(ctrlPIndex)[2];
												} break;
												case FbxGeometryElement::eIndexToDirect:
												{
																						   int id = leNormal->GetIndexArray().GetAt(ctrlPIndex);
																						   normal->X() = (float)leNormal->GetDirectArray().GetAt(id)[0];
																						   normal->Y() = (float)leNormal->GetDirectArray().GetAt(id)[1];
																						   normal->Z() = (float)leNormal->GetDirectArray().GetAt(id)[2];
												} break;
												default: break;
												}
	} break;
	case FbxGeometryElement::eByPolygonVertex:
	{
												 switch (leNormal->GetReferenceMode())
												 {
												 case FbxGeometryElement::eDirect:
												 {
																					 normal->X() = (float)leNormal->GetDirectArray().GetAt(vertexCounter)[0];
																					 normal->Y() = (float)leNormal->GetDirectArray().GetAt(vertexCounter)[1];
																					 normal->Z() = (float)leNormal->GetDirectArray().GetAt(vertexCounter)[2];
												 } break;
												 case FbxGeometryElement::eIndexToDirect:
												 {
																							int id = leNormal->GetIndexArray().GetAt(vertexCounter);
																							normal->X() = (float)leNormal->GetDirectArray().GetAt(id)[0];
																							normal->Y() = (float)leNormal->GetDirectArray().GetAt(id)[1];
																							normal->Z() = (float)leNormal->GetDirectArray().GetAt(id)[2];
												 } break;
												 default: break;
												 }
	} break;
	}
}
//----------------------------------------------------------------------------
void ReadTangent(FbxMesh* mesh, int ctrlPIndex, int vertecCounter,
	AVector* tangent)
{
	if (mesh->GetElementTangentCount() < 1)
	{
		return;
	}

	FbxGeometryElementTangent* leTangent = mesh->GetElementTangent(0);
	FbxLayerElement::EMappingMode mappingMode = leTangent->GetMappingMode();
	FbxLayerElement::EReferenceMode refMode = leTangent->GetReferenceMode();

	switch (mappingMode)
	{
	case FbxGeometryElement::eByControlPoint:
	{
												switch (refMode)
												{
												case FbxGeometryElement::eDirect:
												{
																					tangent->X() = (float)leTangent->GetDirectArray().GetAt(ctrlPIndex)[0];
																					tangent->Y() = (float)leTangent->GetDirectArray().GetAt(ctrlPIndex)[1];
																					tangent->Z() = (float)leTangent->GetDirectArray().GetAt(ctrlPIndex)[2];
												} break;
												case FbxGeometryElement::eIndexToDirect:
												{
																						   int id = leTangent->GetIndexArray().GetAt(ctrlPIndex);
																						   tangent->X() = (float)leTangent->GetDirectArray().GetAt(id)[0];
																						   tangent->Y() = (float)leTangent->GetDirectArray().GetAt(id)[1];
																						   tangent->Z() = (float)leTangent->GetDirectArray().GetAt(id)[2];
												} break;
												default: break;
												}
	}
		break;

	case FbxGeometryElement::eByPolygonVertex:
	{
												 switch (leTangent->GetReferenceMode())
												 {
												 case FbxGeometryElement::eDirect:
												 {
																					 tangent->X() = (float)leTangent->GetDirectArray().GetAt(vertecCounter)[0];
																					 tangent->Y() = (float)leTangent->GetDirectArray().GetAt(vertecCounter)[1];
																					 tangent->Z() = (float)leTangent->GetDirectArray().GetAt(vertecCounter)[2];
												 } break;
												 case FbxGeometryElement::eIndexToDirect:
												 {
																							int id = leTangent->GetIndexArray().GetAt(vertecCounter);
																							tangent->X() = (float)leTangent->GetDirectArray().GetAt(id)[0];
																							tangent->Y() = (float)leTangent->GetDirectArray().GetAt(id)[1];
																							tangent->Z() = (float)leTangent->GetDirectArray().GetAt(id)[2];
												 } break;
												 default: break;
												 }
	} break;
	}
}
//----------------------------------------------------------------------------
void FBXImporter::ProcessMesh(FbxNode* inNode, FbxMesh *currMesh)
{
	PX2_UNUSED(inNode);

	auto it = mMeshes.find(currMesh);
	if (it == mMeshes.end()) return;

	Mesh *mesh = it->second;

	int numTanglesCount = currMesh->GetPolygonCount();

	mesh->mFbxMeshTriangleCount = numTanglesCount;
	mesh->mTriangles.reserve(mesh->mFbxMeshTriangleCount);
	
	int vertexCounter = 0;
	for (int i = 0; i < numTanglesCount; ++i)
	{
		Float4 color[3];
		AVector normal[3];
		AVector tangent[3];
		AVector binormal[3];
		Float2 UV[3][2];
		Triangle currTriangle;

		for (unsigned int j = 0; j < 3; ++j)
		{
			int ctrlPIndex = currMesh->GetPolygonVertex(i, j);
			CtrlPoint* currCtrlPoint = mMeshes[currMesh]->mControlPoints[ctrlPIndex];

			ReadColor(currMesh, ctrlPIndex, vertexCounter, &color[j]);
			ReadNormal(currMesh, ctrlPIndex, vertexCounter, &normal[j]);
			for (int k = 0; k < 1; ++k)
			{
				ReadUV(currMesh, ctrlPIndex, currMesh->GetTextureUVIndex(i, j), k, &UV[j][k]);
			}

			PNTIWVertex temp;
			temp.mPosition = currCtrlPoint->mPosition;
			temp.mColor = color[j];
			temp.mNormal = normal[j];
			temp.mUV = UV[j][0];

			for (unsigned int i = 0; i < currCtrlPoint->mBlendingInfo.size(); ++i)
			{
				VertexBlendingInfo cbi;
				cbi.BoneIndex = currCtrlPoint->mBlendingInfo[i].BoneIndex;
				cbi.BoneWeight = currCtrlPoint->mBlendingInfo[i].BoneWeight;
				temp.mVertexBlendingInfos.push_back(cbi);
			}
			temp.SortBlendingInfoByWeight();

			mesh->mVertices.push_back(temp);
			currTriangle.mIndices.push_back(vertexCounter);

			++vertexCounter;
		}

		mesh->mTriangles.push_back(currTriangle);
	}

	// delete ctrl points
	for (auto itr = mesh->mControlPoints.begin();
		itr != mesh->mControlPoints.end(); ++itr)
	{
		delete itr->second;
	}
	mesh->mControlPoints.clear();
}
//----------------------------------------------------------------------------