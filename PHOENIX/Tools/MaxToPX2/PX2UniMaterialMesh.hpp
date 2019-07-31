// PX2UniMaterialMesh.hpp

#ifndef PX2UNIMATERIALMESH_HPP
#define PX2UNIMATERIALMESH_HPP

#include "PX2MaxToPX2PluginPre.hpp"

class UniMaterialMesh
{
public:
	UniMaterialMesh ();
	~UniMaterialMesh ();

	int &VQuantity ();
	PX2::Float3 *&VertexMap ();
	PX2::Float3 *&NormalMap ();
	int &CQuantity ();
	PX2::Float4 *&ColorMap ();
	int &AQuantity ();
	float *&AlphaMap ();
	int &TQuantity ();
	PX2::Float2 *&TextureCoordMap ();
	int &TQuantity1 ();
	PX2::Float2 *&TextureCoordMap1 ();
	int &FQuantity ();
	int *&Face ();
	int *&AFace ();
	int *&CFace ();
	int *&TFace ();
	int *&TFace1 ();

	void SetShineProperty (PX2::Shine *shine);
	void SetMaterialInstance (PX2::MaterialInstance *mi);

	// 一个Max顶点可以拥有多个UVW坐标。Phoenix2只支持一个UV坐标（W被忽略掉），
	// 所以Max顶点拥有的N个UVW坐标必须被复制到N个Phoenix2的顶点数组中，每个顶
	// 点只拥有一个UV坐标。
	void DuplicateGeometry ();

	void SetExportColor (bool color);
	void SetExportTargentBinormal (bool exp);
	void SetNumTexcoordToExport (int num);
	void SetExportSkin (bool skin);
	PX2::TriMesh *ToTriMesh ();

	class VertexAttr
	{
	public:
		VertexAttr ();
		bool operator== (const VertexAttr &vFormat) const;
		bool operator< (const VertexAttr &vFormat) const;
		int V, C, A, T, T1;
	};

private:

	// --
	int mVertexMapQuantity;
	PX2::Float3 *mVertexMap;
	PX2::Float3 *mNormalMap;

	int mColorMapQuantity;
	PX2::Float4 *mColorMap;

	int mAlphaMapQuantity;
	float *mAlphaMap;

	int mTMapQuantity;
	PX2::Float2 *mTextureCoordMap;

	int mTMapQuantity1;
	PX2::Float2 *mTextureCoordMap1;
	// --

	int mFQuantity;
	int *mVFace;	// 每个元素代表一个面上，一个顶点的顶点索引
	int *mCFace;	// 每个元素代表一个面上，一个顶点的颜色索引
	int *mAFace;	// 每个元素代表一个面上，一个顶点的颜色透明索引
	int *mTFace;	// 每个元素代表一个面上，一个顶点的贴图坐标索引
	int *mTFace1;

	bool mExportTargentBinormal;
	bool mExportColor;
	int mNumTexcoordToExport;
	bool mExportSkin;

	PX2::ShinePtr mShine;
	PX2::MaterialInstancePtr mMaterialInstance;
};

#endif