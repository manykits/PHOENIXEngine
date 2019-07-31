// PX2FBXImporterMaterial.cpp

#include "PX2FBXImporter.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
void FBXImporter::AssociateMaterialToMesh(FbxNode* inNode, FbxMesh *fbxMesh)
{
	PX2_UNUSED(inNode);

	auto it = mMeshes.find(fbxMesh);
	if (it == mMeshes.end()) return;

	Mesh *mesh = it->second;

	FbxLayerElementArrayTemplate<int>* mtlIndices;
	FbxGeometryElement::EMappingMode mtlMapMode = FbxGeometryElement::eNone;

	int numMtls = fbxMesh->GetElementMaterialCount();
	for (int i = 0; i < numMtls; i++)
	{
		const FbxGeometryElementMaterial *fbxEleMtl = fbxMesh->GetElementMaterial(i);
		if (fbxEleMtl)
		{
			mtlIndices = &(fbxEleMtl->GetIndexArray());
			mtlMapMode = fbxEleMtl->GetMappingMode();

			if (mtlIndices)
			{
				switch (mtlMapMode)
				{
				case FbxGeometryElement::eByPolygon:
				{
					if (mtlIndices->GetCount() == mesh->mFbxMeshTriangleCount)
					{
						for (int i = 0; i < mesh->mFbxMeshTriangleCount; ++i)
						{
							unsigned int materialIndex = mtlIndices->GetAt(i);
							mesh->mTriangles[i].mMaterialIndex = materialIndex;
						}
					}
				}
				break;

				case FbxGeometryElement::eAllSame:
				{
					unsigned int materialIndex = mtlIndices->GetAt(0);
					for (int i = 0; i < mesh->mFbxMeshTriangleCount; ++i)
					{
						mesh->mTriangles[i].mMaterialIndex = materialIndex;
					}
				}
				break;

				default:
					throw std::exception("Invalid mapping mode for material\n");
				}
			}
		}
	}
}
//----------------------------------------------------------------------------
void FBXImporter::ProcessMaterials(FbxNode* inNode, FbxMesh *fbxMesh)
{
	auto it = mMeshes.find(fbxMesh);
	if (it == mMeshes.end()) return;

	Mesh *mesh = it->second;

	unsigned int mtlCount = inNode->GetMaterialCount();

	for (unsigned int i = 0; i < mtlCount; ++i)
	{
		FbxSurfaceMaterial* surfaceMaterial = inNode->GetMaterial(i);

		ProcessMaterialAttribute(surfaceMaterial, i, fbxMesh);
		ProcessMaterialTexture(surfaceMaterial, mesh->mMaterialLookUp[i]);
	}
}
//----------------------------------------------------------------------------
void FBXImporter::ProcessMaterialAttribute(FbxSurfaceMaterial* inMaterial, 
	unsigned int inMaterialIndex, FbxMesh *fbxMesh)
{
	auto it = mMeshes.find(fbxMesh);
	if (it == mMeshes.end()) return;

	Mesh *mesh = it->second;

	FbxDouble3 double3;
	FbxDouble double1;
	if (inMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		PhongMtl* currMaterial = new PhongMtl();

		// Amibent Color
		double3 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->Ambient;
		currMaterial->mAmbient[0] = static_cast<float>(double3.mData[0]);
		currMaterial->mAmbient[1] = static_cast<float>(double3.mData[1]);
		currMaterial->mAmbient[2] = static_cast<float>(double3.mData[2]);

		// Diffuse Color
		double3 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->Diffuse;
		currMaterial->mDiffuse[0] = static_cast<float>(double3.mData[0]);
		currMaterial->mDiffuse[1] = static_cast<float>(double3.mData[1]);
		currMaterial->mDiffuse[2] = static_cast<float>(double3.mData[2]);

		// Specular Color
		double3 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->Specular;
		currMaterial->mSpecular[0] = static_cast<float>(double3.mData[0]);
		currMaterial->mSpecular[1] = static_cast<float>(double3.mData[1]);
		currMaterial->mSpecular[2] = static_cast<float>(double3.mData[2]);

		// Emissive Color
		double3 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->Emissive;
		currMaterial->mEmissive[0] = static_cast<float>(double3.mData[0]);
		currMaterial->mEmissive[1] = static_cast<float>(double3.mData[1]);
		currMaterial->mEmissive[2] = static_cast<float>(double3.mData[2]);

		// Reflection
		double3 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->Reflection;
		currMaterial->mReflection[0] = static_cast<float>(double3.mData[0]);
		currMaterial->mReflection[1] = static_cast<float>(double3.mData[1]);
		currMaterial->mReflection[2] = static_cast<float>(double3.mData[2]);

		// Transparency Factor
		double1 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->TransparencyFactor;
		currMaterial->mTransparencyFactor = (float)(double)double1;

		// Shininess
		double1 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->Shininess;
		currMaterial->mShininess = double1;

		// Specular Factor
		double1 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->SpecularFactor;
		currMaterial->mSpecularPower = double1;


		// Reflection Factor
		double1 = reinterpret_cast<FbxSurfacePhong *>(inMaterial)->ReflectionFactor;
		currMaterial->mReflectionFactor = double1;

		mesh->mMaterialLookUp[inMaterialIndex] = currMaterial;
	}
	else if (inMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		LambertMtl* currMaterial = new LambertMtl();

		// Amibent Color
		double3 = reinterpret_cast<FbxSurfaceLambert *>(inMaterial)->Ambient;
		currMaterial->mAmbient[0] = static_cast<float>(double3.mData[0]);
		currMaterial->mAmbient[1] = static_cast<float>(double3.mData[1]);
		currMaterial->mAmbient[2] = static_cast<float>(double3.mData[2]);

		// Diffuse Color
		double3 = reinterpret_cast<FbxSurfaceLambert *>(inMaterial)->Diffuse;
		currMaterial->mDiffuse[0] = static_cast<float>(double3.mData[0]);
		currMaterial->mDiffuse[1] = static_cast<float>(double3.mData[1]);
		currMaterial->mDiffuse[2] = static_cast<float>(double3.mData[2]);

		// Emissive Color
		double3 = reinterpret_cast<FbxSurfaceLambert *>(inMaterial)->Emissive;
		currMaterial->mEmissive[0] = static_cast<float>(double3.mData[0]);
		currMaterial->mEmissive[1] = static_cast<float>(double3.mData[1]);
		currMaterial->mEmissive[2] = static_cast<float>(double3.mData[2]);

		// Transparency Factor
		double1 = reinterpret_cast<FbxSurfaceLambert *>(inMaterial)->TransparencyFactor;
		currMaterial->mTransparencyFactor = (float)(double)double1;

		mesh->mMaterialLookUp[inMaterialIndex] = currMaterial;
	}
}
//----------------------------------------------------------------------------
void FBXImporter::ProcessMaterialTexture(FbxSurfaceMaterial* inMaterial, 
	Mtl* ioMaterial)
{
	unsigned int textureIndex = 0;
	FbxProperty property;

	FBXSDK_FOR_EACH_TEXTURE(textureIndex)
	{
		property = inMaterial->FindProperty(
			FbxLayerElement::sTextureChannelNames[textureIndex]);
		if (property.IsValid())
		{
			unsigned int textureCount = property.GetSrcObjectCount<FbxTexture>();
			for (unsigned int i = 0; i < textureCount; ++i)
			{
				FbxLayeredTexture* layeredTexture = property.GetSrcObject<FbxLayeredTexture>(i);
				if (layeredTexture)
				{
					throw std::exception("Layered Texture is currently unsupported\n");
				}
				else
				{
					FbxTexture* texture = property.GetSrcObject<FbxTexture>(i);
					if (texture)
					{
						std::string textureType = property.GetNameAsCStr();
						FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(texture);

						if (fileTexture)
						{
							std::string filename = fileTexture->GetFileName();

							if (textureType == "DiffuseColor")
							{	
								ioMaterial->mDiffuseMapName = filename;
							}
							else if (textureType == "SpecularColor")
							{
								ioMaterial->mSpecularMapName = filename;
							}
							else if (textureType == "Bump")
							{
								ioMaterial->mNormalMapName = filename;
							}
						}
					}
				}
			}
		}
	}
}
//----------------------------------------------------------------------------