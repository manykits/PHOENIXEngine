// PX2UniMaterialMesh.cpp

#include "PX2UniMaterialMesh.hpp"
#include "PX2MaterialInstance.hpp"
#include <algorithm>
#include <vector>
using namespace PX2;

UniMaterialMesh::UniMaterialMesh ()
{
	mVertexMapQuantity = 0;
	mVertexMap = 0;
	mNormalMap = 0;
	mColorMapQuantity = 0;
	mColorMap = 0;
	mAlphaMapQuantity = 0;
	mAlphaMap = 0;
	mTMapQuantity = 0;
	mTMapQuantity1 = 0;
	mTextureCoordMap = 0;
	mTextureCoordMap1 = 0;
	mFQuantity = 0;
	mVFace = 0;
	mCFace = 0;
	mAFace = 0;
	mTFace = 0;
	mTFace1 = 0;
	mExportColor = false;
	mNumTexcoordToExport = 1;
	mExportTargentBinormal = false;
	mExportSkin = false;
}
//----------------------------------------------------------------------------
UniMaterialMesh::~UniMaterialMesh ()
{
	delete1(mVertexMap);
	delete1(mNormalMap);
	delete1(mColorMap);
	delete1(mAlphaMap);
	delete1(mTextureCoordMap);
	delete1(mTextureCoordMap1);
	delete1(mVFace);
	delete1(mCFace);
	delete1(mAFace);
	delete1(mTFace);
	delete1(mTFace1);
}
//----------------------------------------------------------------------------
int &UniMaterialMesh::VQuantity()
{
	return mVertexMapQuantity;
}
//----------------------------------------------------------------------------
PX2::Float3 *&UniMaterialMesh::VertexMap ()
{
	return mVertexMap;
}
//----------------------------------------------------------------------------
PX2::Float3 *&UniMaterialMesh::NormalMap ()
{
	return mNormalMap;
}
//----------------------------------------------------------------------------
int &UniMaterialMesh::CQuantity ()
{
	return mColorMapQuantity;
}
//----------------------------------------------------------------------------
PX2::Float4 *&UniMaterialMesh::ColorMap ()
{
	return mColorMap;
}
//----------------------------------------------------------------------------
int &UniMaterialMesh::AQuantity ()
{
	return mAlphaMapQuantity;
}
//----------------------------------------------------------------------------
float *&UniMaterialMesh::AlphaMap ()
{
	return mAlphaMap;
}
//----------------------------------------------------------------------------
int &UniMaterialMesh::TQuantity ()
{
	return mTMapQuantity;
}
//----------------------------------------------------------------------------
int &UniMaterialMesh::TQuantity1 ()
{
	return mTMapQuantity1;
}
//----------------------------------------------------------------------------
PX2::Float2 *&UniMaterialMesh::TextureCoordMap ()
{
	return mTextureCoordMap;
}
//----------------------------------------------------------------------------
PX2::Float2 *&UniMaterialMesh::TextureCoordMap1 ()
{
	return mTextureCoordMap1;
}
//----------------------------------------------------------------------------
int &UniMaterialMesh::FQuantity ()
{
	return mFQuantity;
}
//----------------------------------------------------------------------------
int *&UniMaterialMesh::Face ()
{
	return mVFace;
}
//----------------------------------------------------------------------------
int *&UniMaterialMesh::CFace ()
{
	return mCFace;
}
//----------------------------------------------------------------------------
int *&UniMaterialMesh::AFace ()
{
	return mAFace;
}
//----------------------------------------------------------------------------
int *&UniMaterialMesh::TFace ()
{
	return mTFace;
}
//----------------------------------------------------------------------------
int *&UniMaterialMesh::TFace1 ()
{
	return mTFace1;
}
//----------------------------------------------------------------------------
void UniMaterialMesh::SetShineProperty (PX2::Shine *shine)
{
	mShine = shine;
}
//----------------------------------------------------------------------------
void UniMaterialMesh::SetMaterialInstance (MaterialInstance *mi)
{
	mMaterialInstance = mi;
}
//----------------------------------------------------------------------------
void UniMaterialMesh::DuplicateGeometry()
{
	std::vector<VertexAttr> *vArray = new1<std::vector<VertexAttr> >(mVertexMapQuantity);

	// 每一个三角形面包含三个顶点，相邻的三角形共两个顶点。
	// 将所有组合按照面的顶点索引分类，去除完全一样的顶点。

	int i;
	for (i=0; i<3*mFQuantity; i++)
	{
		// 对每个面进行操作

		VertexAttr vertexAttr;

		// 顶点索引
		vertexAttr.V = mVFace[i];

		// 颜色索引
		if (mColorMapQuantity > 0)
		{
			vertexAttr.C = mCFace[i];
		}

		if (mAlphaMapQuantity > 0)
		{
			vertexAttr.A = mAFace[i];
		}

		// 纹理坐标索引
		if (mTMapQuantity > 0)
		{
			vertexAttr.T = mTFace[i];
		}

		if (mTMapQuantity1 > 0)
		{
			vertexAttr.T1 = mTFace1[i];
		}

		vArray[mVFace[i]].push_back(vertexAttr);
	}

	int newVQuantity = 0;
	for (i=0; i<mVertexMapQuantity; i++)
	{
		// 去掉顶点索引，颜色索引，贴图坐标索引完全一样的顶点
		sort(vArray[i].begin(), vArray[i].end());
		std::vector<VertexAttr>::iterator end = unique(vArray[i].begin(), vArray[i].end());
		vArray[i].erase(end, vArray[i].end());
		newVQuantity += (int)vArray[i].size();
	}

	Float3 *newVertex = new1<Float3>(newVQuantity);
	Float3 *newNormal = new1<Float3>(newVQuantity);

	Float4 *newColor = 0;
	if (mColorMapQuantity > 0)
	{
		newColor = new1<Float4>(newVQuantity);
	}

	float *newAlpha = 0;
	if (mAlphaMapQuantity > 0)
	{
		newAlpha = new1<float>(newVQuantity);
	}

	Float2 *newTexture = 0;
	if (mTMapQuantity > 0)
	{
		newTexture = new1<Float2>(newVQuantity);
	}

	Float2 *newTexture1 = 0;
	if (mTMapQuantity1 > 0)
	{
		newTexture1 = new1<Float2>(newVQuantity);
	}

	// 在这一步,vArray被double了一下
	int j, k;
	for (i=0, k=0; i<mVertexMapQuantity; i++)
	{
		// 让每个顶点中，不同的颜色，纹理坐标的每个组合成为一个顶点
		std::vector<VertexAttr> &rvArray = vArray[i];
		int rvArraySize = (int)rvArray.size();
		for (j=0; j<rvArraySize; j++,k++)
		{
			newVertex[k] = mVertexMap[i];
			newNormal[k] = mNormalMap[i];

			VertexAttr vertexAttr = rvArray[j];

			if (newColor)
			{
				newColor[k] = mColorMap[vertexAttr.C];
			}

			if (newAlpha)
			{
				newAlpha[k] = mAlphaMap[vertexAttr.A];
			}

			if (newTexture)
			{
				newTexture[k] = mTextureCoordMap[vertexAttr.T];
			}

			if (newTexture1)
			{
				newTexture1[k] = mTextureCoordMap1[vertexAttr.T1];
			}

			vertexAttr.V = k;
			rvArray.push_back(vertexAttr);
		}
	}

	// 计算顶点索引
	for (i=0; i<mFQuantity; i++)
	{
		int threeI = 3*i;
		int *vIndex = mVFace + threeI;

		int *cIndex = (mColorMapQuantity>0 ? mCFace+threeI : 0);
		int *aIndex = (mAlphaMapQuantity>0 ? mAFace+threeI : 0);
		int *tIndex = (mTMapQuantity>0 ? mTFace+threeI : 0);
		int *tIndex1 = (mTMapQuantity1>0 ? mTFace1+threeI : 0);

		for (j=0; j<3; j++)
		{
			VertexAttr vertexAttr;
			vertexAttr.V = vIndex[j];

			if (cIndex)
			{
				vertexAttr.C = cIndex[j];
			}

			if (aIndex)
			{
				vertexAttr.A = aIndex[j];
			}

			if (tIndex)
			{
				vertexAttr.T = tIndex[j];
			}

			if (tIndex1)
			{
				vertexAttr.T1 = tIndex1[j];
			}

			std::vector<VertexAttr> &rvArray = vArray[vIndex[j]]; // 共有同一空间位置的点集
			int halfSize = (int)rvArray.size()/2;
			for (k=0; k<halfSize; k++)
			{
				if (rvArray[k] == vertexAttr)
				{
					vIndex[j] = rvArray[halfSize+k].V;
					break;
				}
			}
		}
	}

	delete1(mVertexMap);
	delete1(mNormalMap);
	delete1(mColorMap);
	delete1(mAlphaMap);
	delete1(mTextureCoordMap);
	delete1(mTextureCoordMap1);

	mVertexMapQuantity = newVQuantity;
	mVertexMap = newVertex;
	mNormalMap = newNormal;
	mColorMap = newColor;
	mAlphaMap = newAlpha;
	mTextureCoordMap = newTexture;
	mTextureCoordMap1 = newTexture1;

	delete1(vArray);
}
//----------------------------------------------------------------------------
void UniMaterialMesh::SetExportColor (bool color)
{
	mExportColor = color;
}
//----------------------------------------------------------------------------
void UniMaterialMesh::SetExportTargentBinormal (bool exp)
{
	mExportTargentBinormal = exp;
}
//----------------------------------------------------------------------------
void UniMaterialMesh::SetNumTexcoordToExport (int num)
{
	mNumTexcoordToExport = num;
}
//----------------------------------------------------------------------------
void UniMaterialMesh::SetExportSkin (bool skin)
{
	mExportSkin = skin;
}
//----------------------------------------------------------------------------
TriMesh *UniMaterialMesh::ToTriMesh()
{
	if (mVertexMapQuantity == 0)
		return 0;

	// VertexBuffer
	VertexFormat *vFormat = new0 VertexFormat();
	vFormat->Add(VertexFormat::AU_POSITION, VertexFormat::AT_FLOAT3, 0);
	if (mNormalMap)
	{
		vFormat->Add(VertexFormat::AU_NORMAL, VertexFormat::AT_FLOAT3, 0);
	}
	if (mExportColor)
	{
		vFormat->Add(VertexFormat::AU_COLOR, VertexFormat::AT_FLOAT4, 0);
	}
	if (mTextureCoordMap)
	{
		if (1 == mNumTexcoordToExport)
		{
			vFormat->Add(VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
		}
		else if (2 == mNumTexcoordToExport)
		{
			vFormat->Add(VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 0);
			vFormat->Add(VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT2, 1);
		}
	}
	if (mExportTargentBinormal)
	{
		vFormat->Add(VertexFormat::AU_TANGENT, VertexFormat::AT_FLOAT3, 0);
		vFormat->Add(VertexFormat::AU_BINORMAL, VertexFormat::AT_FLOAT3, 0);
	}
	if (mExportSkin)
	{
		vFormat->Add(VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT4, 1);
		vFormat->Add(VertexFormat::AU_TEXCOORD, VertexFormat::AT_FLOAT4, 2);
	}

	vFormat->Create();

	PX2::VertexBuffer *vBuffer = new0 PX2::VertexBuffer(mVertexMapQuantity, 
		vFormat->GetStride());

	VertexBufferAccessor vBA(vFormat, vBuffer);

	for (int i=0; i<mVertexMapQuantity; i++)
	{
		vBA.Position<Float3>(i) = mVertexMap[i];

		if (mNormalMap)
		{
			vBA.Normal<Float3>(i) = mNormalMap[i];
		}

		if (mColorMap)
		{
			vBA.Color<Float4>(0, i) = Float4(mColorMap[i][0], mColorMap[i][1],
				mColorMap[i][2], mColorMap[i][3]);
		}
		else
		{
			if (mExportColor)
				vBA.Color<Float4>(0, i) = Float4::WHITE;
		}

		if (mAlphaMap)
		{
			vBA.Color<Float4>(0, i) = Float4(mColorMap[i][0], mColorMap[i][1],
				mColorMap[i][2], mAlphaMap[i]);
		}

		if (mTextureCoordMap)
		{
			if (mNumTexcoordToExport == 1)
			{
				vBA.TCoord<Float2>(0, i) = Float2(mTextureCoordMap[i][0], 1.0f-mTextureCoordMap[i][1]);
			}
			else if (mNumTexcoordToExport == 2)
			{
				Float2 texCoord0 = Float2(mTextureCoordMap[i][0], 1.0f-mTextureCoordMap[i][1]);

				vBA.TCoord<Float2>(0, i) = texCoord0;

				if (mTextureCoordMap1)
				{
					vBA.TCoord<Float2>(1, i) = Float2(mTextureCoordMap1[i][0], 1.0f-mTextureCoordMap1[i][1]);
				}
				else
				{
					vBA.TCoord<Float2>(1, i) = texCoord0;
				}
			}
		}
	}
	
	// IndexBuffer
	IndexBuffer *iBuffer = new0 IndexBuffer(3*mFQuantity, 2);
	unsigned short *iData = (unsigned short*)iBuffer->GetData();
	for (int i=0; i<(int)3*mFQuantity; i++)
	{
		iData[i] = (unsigned short)mVFace[i];
	}

	// 创建Mesh
	TriMesh *triMesh = new0 TriMesh(vFormat, vBuffer, iBuffer);
	triMesh->UpdateModelSpace(Renderable::GU_MODEL_BOUND_ONLY);
	if (mExportTargentBinormal)
	{
		triMesh->UpdateModelSpace(Renderable::GU_USE_TCOORD_CHANNEL);
	}

	MaterialInstance *mi = 0;
	mi = mMaterialInstance;

	triMesh->SetMaterialInstance(mi);
	triMesh->SetShine(mShine);

	return triMesh;
}
//----------------------------------------------------------------------------
UniMaterialMesh::VertexAttr::VertexAttr()
{
	V = -1;
	C = -1;
	A = -1;
	T = -1;
	T1 = -1;
}
//----------------------------------------------------------------------------
bool UniMaterialMesh::VertexAttr::operator == (const VertexAttr &vFormat)
const
{
	return V==vFormat.V && C==vFormat.C && A==vFormat.A && T==vFormat.T && T1==vFormat.T1;
}
//----------------------------------------------------------------------------
bool UniMaterialMesh::VertexAttr::operator < (const VertexAttr &vFormat) const
{
	if (V < vFormat.V)
		return true;

	if (V > vFormat.V)
		return false;

	if (C < vFormat.C)
		return true;

	if (C > vFormat.C)
		return false;

	if (A < vFormat.A)
		return true;

	if (A > vFormat.A)
		return false;

	if (T < vFormat.T)
		return true;

	if (T > vFormat.T)
		return false;

	return T1 < vFormat.T1;
}
//----------------------------------------------------------------------------