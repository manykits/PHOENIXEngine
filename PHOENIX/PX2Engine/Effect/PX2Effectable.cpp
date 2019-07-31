// PX2Effectable.cpp

#include "PX2Effectable.hpp"
#include "PX2PVWMatrixConstant.hpp"
#include "PX2PVMatrixConstant.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2EffectObject.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, TriMesh, Effectable);
PX2_IMPLEMENT_STREAM(Effectable);
PX2_IMPLEMENT_ABSTRACT_FACTORY(Effectable);
PX2_IMPLEMENT_DEFAULT_NAMES(TriMesh, Effectable);

//----------------------------------------------------------------------------
Effectable::Effectable () :
mIsNeedUpdate(false),
mIsFixedBound(true),
mFixedBoundRadius(2.0f),
mIsLocal(false),
mEmitSizeX(1.0f),
mEmitSizeY(1.0f),
mEmitSizeZ(1.0f),
mEmitColor(Float3::WHITE),
mEmitAlpha(1.0f),
mEmitLife(1.0f),
mFaceType(FT_CAMERA),
mMtlType(MT_TEX),
mBlendMode(BM_MAX_MODE),
mTexMode(TM_TEX),
mAnimInterval(1.0f),
mIsAnimStartDoRandom(false),
mUserNumAnimFrames(-1),
mIsAnimFramesPlayOnce(false),
mCoordinateType0(CT_CLAMP),
mCoordinateType1(CT_CLAMP),
mIsBackCull(false),
mIsBufferEverGenerated(false)
{
	mTexAnim_Repeat = Float2(1.0f, 1.0f);
	mEmitUV0_Offset = Float2::ZERO;
	mEmitUV0_Repeat = Float2::UNIT;
	mEmitUV0_Speed = Float2::ZERO;
	mUV1_Offset = Float2::ZERO;
	mUV1_Repeat = Float2::UNIT;
	mUV1_Speed = Float2::ZERO;

	SetFixedBound(mIsFixedBound);
	SetFaceType(mFaceType);

	SetMaterialType(mMtlType);

	SetBlendMode(BM_BLEND_SCRCALPHA_DSTONEMIUSSRCALPHA);
	SetBackCull(mIsBackCull);
	
	SetTex("Data/engine/default.png");
}
//----------------------------------------------------------------------------
Effectable::~Effectable ()
{
}
//----------------------------------------------------------------------------
void Effectable::SetFixedBound (bool fixed)
{
	mIsFixedBound = fixed;

	if (mIsFixedBound)
	{
		mModelBound.SetRadius(mFixedBoundRadius);
	}
}
//----------------------------------------------------------------------------
void Effectable::SetFixedBoundCenter (const APoint &center)
{
	mFixedCenter = center;

	if (mIsFixedBound)
	{
		mModelBound.SetCenter(mFixedCenter);
	}
}
//----------------------------------------------------------------------------
void Effectable::SetFixedBoundRadius (float radius)
{
	mFixedBoundRadius = radius;

	if (mIsFixedBound)
	{
		mModelBound.SetRadius(mFixedBoundRadius);
	}
}
//----------------------------------------------------------------------------
void Effectable::SetLocal (bool l)
{
	if (!mMaterialInstance) return;

	mIsLocal = l;

	if (mIsLocal)
	{
		mMaterialInstance->SetVertexConstant(0, "PVWMatrix", new0 PVWMatrixConstant());
	}
	else
	{
		mMaterialInstance->SetVertexConstant(0, "PVWMatrix", new0 PVMatrixConstant());
	}
}
//----------------------------------------------------------------------------
void Effectable::SetFaceType (FaceType type)
{
	mFaceType = type;
}
//----------------------------------------------------------------------------
void Effectable::SetMaterialType (MaterialType type)
{
	mMtlType = type;

	if (MT_TEX == mMtlType)
	{
		SetVertexFormat(PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PCT1));
	}
	else if(MT_TEX_MASK_MULTIPLY == mMtlType)
	{
		SetVertexFormat(PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PCT2));
	}
	else if (MT_TEX_MASK_ADD == mMtlType)
	{
		SetVertexFormat(PX2_GR.GetVertexFormat(GraphicsRoot::VFT_PCT2));
	}

	MaterialInstance *mi = new0 MaterialInstance("Data/engine_mtls/effect/effect.px2obj", "default", false);
	SetMaterialInstance(mi);
}
//----------------------------------------------------------------------------
void Effectable::SetBackCull (bool isBackCull)
{
	mIsBackCull = isBackCull;

	if (mMaterialInstance)
	{
		Material *mtl = mMaterialInstance->GetMaterial();
		mtl->GetCullProperty(0, 0)->Enabled = isBackCull;
	}
}
//----------------------------------------------------------------------------
void Effectable::SetBlendMode (BlendMode mode)
{
	mBlendMode = mode;

	_SetBlendMode(mBlendMode);

	SetLocal(mIsLocal);
	SetCoordinateType0(mCoordinateType0);
	SetCoordinateType1(mCoordinateType1);

	if (TM_TEX == mTexMode)
	{
		SetTex(mTexFilename);
	}
	else if (TM_TEXPACK_ELE == mTexMode)
	{
		SetTexPack_Ele(mTexFilename, mTexPackEleName);
	}
	else if (TM_TEX_ANIM == mTexMode)
	{
		SetTex(mTexFilename);
	}
	else if (TM_TEXS_ANIM == mTexMode)
	{
	}
	else if (TM_TEXPACK_ANIM == mTexMode)
	{
		SetTexPackAnim(mTexFilename);
	}
}
//----------------------------------------------------------------------------
void Effectable::_SetBlendMode(BlendMode mode)
{
	bool BlendEnabled = false;
	AlphaProperty::SrcBlendMode SrcBlend = AlphaProperty::SBM_SRC_ALPHA;
	AlphaProperty::DstBlendMode DstBlend = AlphaProperty::DBM_ONE_MINUS_SRC_ALPHA;
	bool CompareEnabled = false;
	AlphaProperty::CompareMode Compare = AlphaProperty::CM_ALWAYS;
	float Reference = 0.0f;

	if (0 == mode)
	{
		BlendEnabled = false;
		CompareEnabled = false;
	}
	else if (1 == mode)
	{
		BlendEnabled = false;
		CompareEnabled = true;
		Reference = 0.33f;
		Compare = AlphaProperty::CM_GEQUAL;
	}
	else if (2 == mode)
	{
		BlendEnabled = true;
		SrcBlend = AlphaProperty::SBM_SRC_ALPHA;
		DstBlend = AlphaProperty::DBM_ONE_MINUS_SRC_ALPHA;
		CompareEnabled = true;
		Reference = 0.0f;
		Compare = AlphaProperty::CM_GREATER;
	}
	else if (3 == mode)
	{
		BlendEnabled = true;
		SrcBlend = AlphaProperty::SBM_SRC_ALPHA;
		DstBlend = AlphaProperty::DBM_ONE;
		CompareEnabled = false;
	}
	else if (4 == mode)
	{
		BlendEnabled = true;
		SrcBlend = AlphaProperty::SBM_SRC_COLOR;
		DstBlend = AlphaProperty::DBM_ONE_MINUS_SRC_COLOR;
		CompareEnabled = false;
	}
	else if (5 == mode)
	{
		BlendEnabled = true;
		SrcBlend = AlphaProperty::SBM_SRC_COLOR;
		DstBlend = AlphaProperty::DBM_ONE;
		CompareEnabled = false;
	}
	else if (6 == mode)
	{
		BlendEnabled = true;
		SrcBlend = AlphaProperty::SBM_ONE;
		DstBlend = AlphaProperty::DBM_ONE;
		CompareEnabled = false;
	}
	else if (7 == mode)
	{
		BlendEnabled = true;
		SrcBlend = AlphaProperty::SBM_ONE;
		DstBlend = AlphaProperty::DBM_ONE_MINUS_SRC_ALPHA;
		CompareEnabled = false;
	}
	else if (8 == mode)
	{
		BlendEnabled = true;
		SrcBlend = AlphaProperty::SBM_ONE;
		DstBlend = AlphaProperty::DBM_ONE_MINUS_SRC_COLOR;
		CompareEnabled = false;
	}

	Material *mtl = mMaterialInstance->GetMaterial();

	AlphaProperty *aProp = mtl->GetAlphaProperty(0, 0);
	aProp->BlendEnabled = BlendEnabled;
	aProp->SrcBlend = SrcBlend;
	aProp->DstBlend = DstBlend;
	aProp->CompareEnabled = CompareEnabled;
	aProp->Compare = Compare;
	aProp->Reference = Reference;

	if (0 == mode || 1 == mode)
	{
		mtl->GetDepthProperty(0, 0)->Writable = true;
		mtl->GetDepthProperty(0, 0)->Enabled = true;
	}
	else
	{
		mtl->GetDepthProperty(0, 0)->Writable = false;
		mtl->GetDepthProperty(0, 0)->Enabled = true;
	}
}
//----------------------------------------------------------------------------
void Effectable::SetTexMode (TexMode mode)
{
	mTexMode = mode;
}
//----------------------------------------------------------------------------
bool Effectable::SetTex (const std::string &filename)
{
	if ("" == filename)
		return false;

	if (TM_TEX != mTexMode)
		return false;

	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(filename));
	if (tex)
	{
		mTexFilename = filename;

		mMaterialInstance->SetPixelTexture(0, "SampleBase", tex);
		return true;
	}
	else
	{
		assertion(false, "Load texture %s failed.", filename.c_str());
		return false;
	}
}
//----------------------------------------------------------------------------
bool Effectable::SetTexPack_Ele (const std::string &packFileName, 
	const std::string &eleName)
{
	if (TM_TEXPACK_ELE != mTexMode)
		return false;

	PX2_RM.AddTexPack(packFileName);
	mTexPackEle = PX2_RM.GetTexPackElement(packFileName, eleName);

	if (!mTexPackEle.IsValid())
		return false;

	mTexFilename = packFileName;
	mTexPackEleName = eleName;

	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(
		mTexPackEle.ImagePathFull));

	if (tex)
	{
		mMaterialInstance->SetPixelTexture(0, "SampleBase", tex);
		
		return true;
	}
	else
	{
		assertion(false, "Load texture %s failed.", 
			mTexPackEle.ImagePathFull.c_str());
		
		return false;
	}
}
//----------------------------------------------------------------------------
bool Effectable::SetTexAnim (const std::string &filename)
{
	if (TM_TEX_ANIM != mTexMode)
		return false;

	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(filename));
	if (tex)
	{
		mTexFilename = filename;

		if (mMaterialInstance)
			mMaterialInstance->SetPixelTexture(0, "SampleBase", tex);
		
		return true;
	}
	else
	{
		assertion(false, "Load texture %s failed.", filename.c_str());
		return false;
	}
}
//----------------------------------------------------------------------------
void Effectable::SetTexAnim_Repeat (const Float2 &repeat)
{
	mTexAnim_Repeat = repeat;
}
//----------------------------------------------------------------------------
bool Effectable::AddTexsAnim_Frame (const std::string &filename)
{
	if (TM_TEXS_ANIM != mTexMode)
		return false;

	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(filename));

	if (tex)
	{
		TexPackElement ele;
		ele.W = tex->GetWidth();
		ele.H = tex->GetHeight();
		ele.ImagePathFull = filename;

		mTexPackAnim_Frames.push_back(ele);

		return true;
	}
	else
	{
		assertion(false, "Load texture %s failed.", filename.c_str());
		return false;
	}
}
//----------------------------------------------------------------------------
bool Effectable::SetTexPackAnim (const std::string &texPackFilename)
{
	if (TM_TEXPACK_ANIM != mTexMode)
		return false;

	PX2_RM.AddTexPack(texPackFilename);
	const TexPack &texPack = ResourceManager::GetSingleton().GetTexPack(
		texPackFilename);
	if (!texPack.IsValid())
		return false;

	mTexFilename = texPackFilename;

	mTexPackAnim_Frames.clear();

	for (int i=0; i<(int)texPack.Elements.size(); i++)
	{
		const TexPackElement &ele = texPack.Elements[i];
		mTexPackAnim_Frames.push_back(ele);
	}

	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(
		texPack.ImageFullPath));

	if (tex)
	{
		if (mMaterialInstance)
			mMaterialInstance->SetPixelTexture(0, "SampleBase", tex);
		
		return true;
	}
	else
	{
		return false;
	}
}
//----------------------------------------------------------------------------
bool Effectable::AddTexPackAnim_Frame (const std::string &texPackFilename, 
	const std::string &eleName)
{
	if (TM_TEXPACK_ANIM != mTexMode)
		return false;

	PX2_RM.AddTexPack(texPackFilename);
	const TexPack &texPack = ResourceManager::GetSingleton().GetTexPack(
		texPackFilename);
	if (!texPack.IsValid())
		return false;

	const TexPackElement &ele = PX2_RM.GetTexPackElement(texPackFilename, 
		eleName);

	if (!ele.IsValid())
		return false;

	mTexPackAnim_Frames.push_back(ele);

	Texture2D *tex = DynamicCast<Texture2D>(PX2_RM.BlockLoad(
		texPack.ImageFullPath));

	if (tex)
	{
		if (mMaterialInstance)
			mMaterialInstance->SetPixelTexture(0, "SampleBase", tex);
		
		return true;
	}
	else
	{
		return false;
	}
}
//----------------------------------------------------------------------------
int Effectable::GetNumAnimFrames () const
{
	if (mUserNumAnimFrames > -1)
	{
		return mUserNumAnimFrames;
	}

	if (TM_TEX == mTexMode)
	{
		return 1;
	}
	else if (TM_TEXPACK_ELE == mTexMode)
	{
		return 1;
	}
	else if (TM_TEX_ANIM == mTexMode)
	{
		return (int)(mTexAnim_Repeat[0]*mTexAnim_Repeat[1]);
	}
	else if (TM_TEXS_ANIM==mTexMode || TM_TEXPACK_ANIM==mTexMode)
	{
		return (int)mTexPackAnim_Frames.size();
	}

	return 0;
}
//----------------------------------------------------------------------------
void Effectable::ClearTexAnimTexPackAnimFrames ()
{
	mTexPackAnim_Frames.clear();
}
//----------------------------------------------------------------------------
void Effectable::SetMaskTex (const std::string &filename)
{
	mTexMaskFilename = filename;
}
//----------------------------------------------------------------------------
void Effectable::SetEmitUV0_Offset (const Float2 &offset)
{
	mEmitUV0_Offset = offset;
}
//----------------------------------------------------------------------------
void Effectable::SetEmitUV0_Repeat (const Float2 &repeat)
{
	mEmitUV0_Repeat = repeat;
}
//----------------------------------------------------------------------------
void Effectable::SetEmitUV0_Speed (const Float2 &speed)
{
	mEmitUV0_Speed = speed;
}
//----------------------------------------------------------------------------
void Effectable::SetUV1_Offset (const Float2 &offset)
{
	mUV1_Offset = offset;
}
//----------------------------------------------------------------------------
void Effectable::SetUV1_Repeat (const Float2 &repeat)
{
	mUV1_Repeat = repeat;
}
//----------------------------------------------------------------------------
void Effectable::SetUV1_Speed (const Float2 &speed)
{
	mUV1_Speed = speed;
}
//----------------------------------------------------------------------------
void Effectable::SetCoordinateType0 (CoordinateType type)
{
	mCoordinateType0 = type;

	if (mMaterialInstance)
	{
		Material *mtl = mMaterialInstance->GetMaterial();

		if (CT_CLAMP == type)
		{
			mtl->GetPixelShader(0, 0)->SetCoordinate(0, 0, Shader::SC_CLAMP);
		}
		else
		{
			mtl->GetPixelShader(0, 0)->SetCoordinate(0, 0, Shader::SC_REPEAT);
			mtl->GetPixelShader(0, 0)->SetCoordinate(0, 1, Shader::SC_REPEAT);
		}
	}
}
//----------------------------------------------------------------------------
void Effectable::SetCoordinateType1 (CoordinateType type)
{
	mCoordinateType1 = type;

	if (mMaterialInstance)
	{
		Material *mtl = mMaterialInstance->GetMaterial();

		if (CT_CLAMP == type)
		{
			mtl->GetPixelShader(0, 0)->SetCoordinate(0, 1, Shader::SC_CLAMP);
		}
		else
		{
			mtl->GetPixelShader(0, 0)->SetCoordinate(0, 1, Shader::SC_REPEAT);
		}
	}
}
//----------------------------------------------------------------------------
int Effectable::GetAnimStartRandomIndex () const
{
	TexMode mode = GetTexMode();
	if (TM_TEX==mode || TM_TEXPACK_ELE==mode)
		return 0;

	int numRepeats = GetNumAnimFrames();

	return Mathf::IntRandom(0, numRepeats);
}
//----------------------------------------------------------------------------
void Effectable::GenBuffers ()
{
	mIsBufferEverGenerated = true;
}
//----------------------------------------------------------------------------
void Effectable::UpdateWorldData(double applicationTime, double elapsedTime)
{
	TriMesh::UpdateWorldData(applicationTime, elapsedTime);

	if (mIsNeedUpdate)
	{
		SetLocal(IsLocal());

		mIsNeedUpdate = false;
	}
}
//----------------------------------------------------------------------------
int Effectable::GetUV (int startRandomIndex, float age, 
	float &uBegin, float &uEnd, float &vBegin, float &vEnd)
{
	TexMode texMode = GetTexMode();
	float interval = mAnimInterval;
	if (0.0f >= interval)
		interval = 1.0f;

	if (TM_TEX == texMode)
	{
		/*_*/
		return 0;
	}
	else if (TM_TEXPACK_ELE == texMode)
	{
		const TexPackElement &texPackEle = GetTexPackElement();

		if (texPackEle.IsValid())
		{
			uBegin = (float)texPackEle.X/(float)texPackEle.TexWidth;
			uEnd = (float)(texPackEle.X+texPackEle.W)/(float)texPackEle.TexWidth;
			vBegin = (float)(texPackEle.TexHeight-texPackEle.Y-texPackEle.H)/(float)texPackEle.TexHeight;
			vEnd = (float)(texPackEle.TexHeight-texPackEle.Y)/(float)texPackEle.TexHeight;
		}

		return 0;
	}
	else if (TM_TEX_ANIM == texMode)
	{
		float uInterval = 1.0f/mTexAnim_Repeat[0];
		float vInterval = 1.0f/mTexAnim_Repeat[1];

		int index = startRandomIndex + (int)(age/interval);
		int numAnimFrames = GetNumAnimFrames();
		
		if (IsAnimFramesPlayOnce())
		{
			if (index >= (numAnimFrames-1))
				index = numAnimFrames-1;
		}
		index %= numAnimFrames;

		int xIndex = index%(int)mTexAnim_Repeat[0];
		int yIndex = index/(int)mTexAnim_Repeat[0];

		uBegin = (float)xIndex/mTexAnim_Repeat[0];
		uEnd = uBegin + uInterval;

		uBegin = Mathf::Clamp(uBegin, 0.0f, 1.0f);
		uEnd = Mathf::Clamp(uEnd, 0.0f, 1.0f);

		vBegin = 1.0f - (yIndex+1)*vInterval;
		vEnd = vBegin + vInterval;

		vBegin = Mathf::Clamp(vBegin, 0.0f, 1.0f);
		vEnd = Mathf::Clamp(vEnd, 0.0f, 1.0f);
		
		return index;
	}
	else if (TM_TEXPACK_ANIM == texMode)
	{
		const std::vector<TexPackElement> &animFrames = GetTexPackAnim_Frames();
		int numAnims = GetNumAnimFrames();
		if (numAnims > 0)
		{
			int index = startRandomIndex + (int)(age/interval);

			if (IsAnimFramesPlayOnce())
			{
				if (index >= (numAnims-1))
					index = numAnims-1;
			}

			index = index%numAnims;

			const TexPackElement &ele = animFrames[index];

			uBegin = (float)ele.X/(float)ele.TexWidth;
			uEnd = (float)(ele.X+ele.W)/(float)ele.TexWidth;
			vBegin = (float)(ele.TexHeight-ele.Y-ele.H)/(float)ele.TexHeight;
			vEnd = (float)(ele.TexHeight-ele.Y)/(float)ele.TexHeight;

			return index;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Effectable::RegistProperties ()
{
	TriMesh::RegistProperties();

	AddPropertyClass("Effectable");
	AddProperty("IsLocal", PT_BOOL, IsLocal());
	AddProperty("IsFixedBound", PT_BOOL, IsFixedBound());
	AddProperty("FixedBoundCenter", PT_APOINT3, GetFixedBoundCenter());
	AddProperty("FixedBoundRadius", PT_FLOAT, GetFixedBoundRadius());
	AddProperty("EmitSizeX", PT_FLOAT, GetEmitSizeX());
	AddProperty("EmitSizeY", PT_FLOAT, GetEmitSizeY());
	AddProperty("EmitSizeZ", PT_FLOAT, GetEmitSizeZ());
	AddProperty("EmitSizeXYZ", PT_FLOAT, GetEmitSizeX());
	AddProperty("EmitColor", PT_COLOR3FLOAT3, GetEmitColor());
	AddProperty("EmitAlpha", PT_FLOAT, GetEmitAlpha());
	AddProperty("EmitLife", PT_FLOAT, GetEmitLife());

	std::vector<std::string> fts;
	fts.push_back("FT_X");
	fts.push_back("FT_NX");
	fts.push_back("FT_Y");
	fts.push_back("FT_NY");
	fts.push_back("FT_Z");
	fts.push_back("FT_NZ");
	fts.push_back("FT_CAMERA");
	fts.push_back("FT_SPEEDDIR");
	fts.push_back("FT_FREE");
	fts.push_back("FT_CAMERA_ZUP");
	fts.push_back("FT_CAMERA_ALIGN_EMITDIR");
	AddPropertyEnum("FaceType", (int)GetFaceType(), fts);

	std::vector<std::string> mts;
	mts.push_back("MT_TEX1");
	mts.push_back("MT_TEX2");
	AddPropertyEnum("MaterialType", (int)GetMaterialType(), mts);

	AddProperty("IsBackCull", PT_BOOL, IsBackCull());

	std::vector<std::string> bms;
	bms.push_back("BM_NONE");
	bms.push_back("BM_ALPHATEST85");
	bms.push_back("BM_BLEND_SCRCALPHA_DSTONEMIUSSRCALPHA");
	bms.push_back("BM_BLEND_SRCALPHA_DSTONE");
	bms.push_back("BM_BLEND_SRCCOLOR_DSTONEMIUSSRCCOLOR");
	bms.push_back("BM_BLEND_SRCCOLOR_DSTONE");
	bms.push_back("BM_BLEND_SRCONE_DSTONE");
	bms.push_back("BM_BLEND_SRCONE_DSTONEMIUSSRCALPHA");
	bms.push_back("BM_BLEND_SRCONE_DSTONEMIUSSRCCOLOR");
	AddPropertyEnum("BlendMode", (int)GetBlendMode(), bms);

	std::vector<std::string> coordinateTypes;
	coordinateTypes.push_back("CT_CLAMP");
	coordinateTypes.push_back("CT_REPEAT");
	AddPropertyEnum("CoordinateType0", GetCoordinateType0(), coordinateTypes);
	AddPropertyEnum("CoordinateType1", GetCoordinateType1(), coordinateTypes);

	std::vector<std::string> texModes;
	texModes.push_back("TM_TEX");
	texModes.push_back("TM_TEXPACK_ELE");
	texModes.push_back("TM_TEX_ANIM");
	texModes.push_back("TM_TEXS_ANIM");
	texModes.push_back("TM_TEXPACK_ANIM");
	AddPropertyEnum("TexMode", (int)GetTexMode(), texModes);

	AddProperty("Tex", PT_STRINGBUTTON, GetTex());

	std::string packFilename;
	std::string eleName;
	GetTexPack_Ele(packFilename, eleName);
	AddProperty("TexPack_Ele", PT_STRINGBUTTON, eleName);

	AddProperty("TexAnim", PT_STRINGBUTTON, GetTexAnim());
	AddProperty("TexAnim_Repeat", PT_FLOAT2, GetTexAnim_Repeat());

	AddProperty("TexPackAnim", PT_STRINGBUTTON, GetTexPackAnim());

	AddProperty("AnimInterval", PT_FLOAT, GetAnimInterval());
	AddProperty("IsAnimStartDoRandom", PT_BOOL, IsAnimStartDoRandom());
	AddProperty("UserNumAnimFrames", PT_INT, GetUserNumAnimFrames());
	AddProperty("IsAnimFramesPlayOnce", PT_BOOL, IsAnimFramesPlayOnce());

	AddProperty("EmitUV0_Offset", PT_FLOAT2, GetEmitUV0_Offset());
	AddProperty("EmitUV0_Repeat", PT_FLOAT2, GetEmitUV0_Repeat());
	AddProperty("EmitUV0_Speed", PT_FLOAT2, GetEmitUV0_Speed());
}
//----------------------------------------------------------------------------
void Effectable::OnPropertyChanged (const PropertyObject &obj)
{
	TriMesh::OnPropertyChanged(obj);

	if ("IsLocal" == obj.Name)
	{
		SetLocal(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("IsFixedBound" == obj.Name)
	{
		SetFixedBound(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("FixedBoundCenter" == obj.Name)
	{
		SetFixedBoundCenter(PX2_ANY_AS(obj.Data, APoint));
	}
	else if ("FixedBoundRadius" == obj.Name)
	{
		SetFixedBoundRadius(PX2_ANY_AS(obj.Data, float));
	}
	else if ("EmitSizeX" == obj.Name)
	{
		SetEmitSizeX(PX2_ANY_AS(obj.Data, float));
	}
	else if ("EmitSizeY" == obj.Name)
	{
		SetEmitSizeY(PX2_ANY_AS(obj.Data, float));
	}
	else if ("EmitSizeZ" == obj.Name)
	{
		SetEmitSizeZ(PX2_ANY_AS(obj.Data, float));
	}
	else if ("EmitSizeXYZ" == obj.Name)
	{
		SetEmitSizeXYZ(PX2_ANY_AS(obj.Data, float));
	}
	else if ("EmitColor" == obj.Name)
	{
		SetEmitColor(PX2_ANY_AS(obj.Data, Float3));
	}
	else if ("EmitAlpha" == obj.Name)
	{
		SetEmitAlpha(PX2_ANY_AS(obj.Data, float));
	}
	else if ("EmitLife" == obj.Name)
	{
		SetEmitLife(PX2_ANY_AS(obj.Data, float));
	}
	else if ("FaceType" == obj.Name)
	{
		SetFaceType((FaceType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("MaterialType" == obj.Name)
	{
		SetMaterialType((MaterialType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("IsBackCull" == obj.Name)
	{
		SetBackCull(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("BlendMode" == obj.Name)
	{
		SetBlendMode((BlendMode)PX2_ANY_AS(obj.Data, int));
	}
	else if ("CoordinateType0" == obj.Name)
	{
		SetCoordinateType0((CoordinateType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("CoordinateType1" == obj.Name)
	{
		SetCoordinateType1((CoordinateType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("TexMode" == obj.Name)
	{
		SetTexMode((TexMode)PX2_ANY_AS(obj.Data, int));
	}
	else if ("Tex" == obj.Name)
	{
		SetTex(PX2_ANY_AS(obj.Data, std::string));
	}
	else if ("TexPack_Ele" == obj.Name)
	{
		SetTexPack_Ele(PX2_ANY_AS(obj.Data1, std::string), 
			PX2_ANY_AS(obj.Data, std::string));
	}
	else if ("TexAnim" == obj.Name)
	{
		SetTexAnim(PX2_ANY_AS(obj.Data, std::string));
	}
	else if ("TexAnim_Repeat" == obj.Name)
	{
		SetTexAnim_Repeat(PX2_ANY_AS(obj.Data, Float2));
	}
	else if ("TexPackAnim" == obj.Name)
	{
		SetTexPackAnim(PX2_ANY_AS(obj.Data, std::string));
	}
	else if ("AnimInterval" == obj.Name)
	{
		SetAnimInterval(PX2_ANY_AS(obj.Data, float));
	}
	else if ("IsAnimStartDoRandom" == obj.Name)
	{
		SetAnimStartDoRandom(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("UserNumAnimFrames" == obj.Name)
	{
		SetUserNumAnimFrames(PX2_ANY_AS(obj.Data, int));
	}
	else if ("IsAnimFramesPlayOnce" == obj.Name)
	{
		SetAnimFramesPlayOnce(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("EmitUV0_Offset" == obj.Name)
	{
		SetEmitUV0_Offset(PX2_ANY_AS(obj.Data, Float2));
	}
	else if ("EmitUV0_Repeat" == obj.Name)
	{
		SetEmitUV0_Repeat(PX2_ANY_AS(obj.Data, Float2));
	}
	else if ("EmitUV0_Speed" == obj.Name)
	{
		SetEmitUV0_Speed(PX2_ANY_AS(obj.Data, Float2));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
Effectable::Effectable (LoadConstructor value) :
TriMesh(value),
mIsNeedUpdate(true),
mIsFixedBound(true),
mFixedBoundRadius(2.0f),
mIsLocal(false),
mEmitSizeX(1.0f),
mEmitSizeY(1.0f),
mEmitSizeZ(1.0f),
mEmitColor(Float3::WHITE),
mEmitAlpha(1.0f),
mEmitLife(1.0f),
mFaceType(FT_CAMERA),
mMtlType(MT_TEX),
mBlendMode(BM_MAX_MODE),
mTexMode(TM_TEX),
mAnimInterval(1.0f),
mIsAnimStartDoRandom(false),
mUserNumAnimFrames(-1),
mIsAnimFramesPlayOnce(false),
mCoordinateType0(CT_CLAMP),
mCoordinateType1(CT_CLAMP),
mIsBackCull(false),
mIsBufferEverGenerated(false)
{
	mTexAnim_Repeat = Float2(1.0f, 1.0f);
	mEmitUV0_Offset = Float2::ZERO;
	mEmitUV0_Repeat = Float2::UNIT;
	mEmitUV0_Speed = Float2::ZERO;
	mUV1_Offset = Float2::ZERO;
	mUV1_Repeat = Float2::UNIT;
	mUV1_Speed = Float2::ZERO;
}
//----------------------------------------------------------------------------
void Effectable::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	TriMesh::Load(source);
	PX2_VERSION_LOAD(source);
	
	source.ReadBool(mIsFixedBound);
	source.ReadAggregate(mFixedCenter);
	source.Read(mFixedBoundRadius);
	source.ReadBool(mIsLocal);
	source.Read(mEmitSizeX);
	source.Read(mEmitSizeY);
	source.Read(mEmitSizeZ);
	source.ReadAggregate(mEmitColor);
	source.Read(mEmitAlpha);
	source.Read(mEmitLife);
	source.ReadEnum(mFaceType);
	source.ReadEnum(mMtlType);
	source.ReadEnum(mBlendMode);

	source.ReadEnum(mTexMode);
	source.ReadString(mTexFilename);
	source.ReadString(mTexPackEleName);
	source.ReadAggregate(mTexAnim_Repeat);
	source.Read(mAnimInterval);
	source.ReadBool(mIsAnimStartDoRandom);

	source.Read(mUserNumAnimFrames);

	int numFrames = (int)mTexPackAnim_Frames.size();
	source.Read(numFrames);
	mTexPackAnim_Frames.resize(numFrames);
	for (int i = 0; i < numFrames; i++)
	{
		source.Read(mTexPackAnim_Frames[i].X);
		source.Read(mTexPackAnim_Frames[i].Y);
		source.Read(mTexPackAnim_Frames[i].W);
		source.Read(mTexPackAnim_Frames[i].H);
		source.Read(mTexPackAnim_Frames[i].OX);
		source.Read(mTexPackAnim_Frames[i].OY);
		source.Read(mTexPackAnim_Frames[i].OW);
		source.Read(mTexPackAnim_Frames[i].OH);
		source.ReadBool(mTexPackAnim_Frames[i].Rolated);
		source.Read(mTexPackAnim_Frames[i].TexWidth);
		source.Read(mTexPackAnim_Frames[i].TexHeight);
		source.ReadString(mTexPackAnim_Frames[i].ElementName);
		source.ReadString(mTexPackAnim_Frames[i].ImagePathFull);
	}

	source.ReadAggregate(mEmitUV0_Offset);
	source.ReadAggregate(mEmitUV0_Repeat);
	source.ReadAggregate(mEmitUV0_Speed);
	source.ReadAggregate(mUV1_Offset);
	source.ReadAggregate(mUV1_Repeat);
	source.ReadAggregate(mUV1_Speed);

	source.ReadPointer(mEffectableCtrl);

	source.ReadBool(mIsAnimFramesPlayOnce);
	source.ReadEnum(mCoordinateType0);

	source.ReadEnum(mCoordinateType1);
	source.ReadBool(mIsBackCull);

	PX2_END_DEBUG_STREAM_LOAD(Effectable, source);
}
//----------------------------------------------------------------------------
void Effectable::Link (InStream& source)
{
	TriMesh::Link(source);

	source.ResolveLink(mEffectableCtrl);
}
//----------------------------------------------------------------------------
void Effectable::PostLink ()
{
	TriMesh::PostLink();
}
//----------------------------------------------------------------------------
bool Effectable::Register (OutStream& target) const
{
	if (TriMesh::Register(target))
	{
		target.Register(mEffectableCtrl);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Effectable::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	TriMesh::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteBool(mIsFixedBound);
	target.WriteAggregate(mFixedCenter);
	target.Write(mFixedBoundRadius);
	target.WriteBool(mIsLocal);
	target.Write(mEmitSizeX);
	target.Write(mEmitSizeY);
	target.Write(mEmitSizeZ);
	target.WriteAggregate(mEmitColor);
	target.Write(mEmitAlpha);
	target.Write(mEmitLife);
	target.WriteEnum(mFaceType);
	target.WriteEnum(mMtlType);
	target.WriteEnum(mBlendMode);

	target.WriteEnum(mTexMode);
	target.WriteString(mTexFilename);
	target.WriteString(mTexPackEleName);
	target.WriteAggregate(mTexAnim_Repeat);
	target.Write(mAnimInterval);
	target.WriteBool(mIsAnimStartDoRandom);
	target.Write(mUserNumAnimFrames);

	int numFrames = (int)mTexPackAnim_Frames.size();
	target.Write(numFrames);
	for (int i=0; i<numFrames; i++)
	{
		target.Write(mTexPackAnim_Frames[i].X);
		target.Write(mTexPackAnim_Frames[i].Y);
		target.Write(mTexPackAnim_Frames[i].W);
		target.Write(mTexPackAnim_Frames[i].H);
		target.Write(mTexPackAnim_Frames[i].OX);
		target.Write(mTexPackAnim_Frames[i].OY);
		target.Write(mTexPackAnim_Frames[i].OW);
		target.Write(mTexPackAnim_Frames[i].OH);
		target.WriteBool(mTexPackAnim_Frames[i].Rolated);
		target.Write(mTexPackAnim_Frames[i].TexWidth);
		target.Write(mTexPackAnim_Frames[i].TexHeight);
		target.WriteString(mTexPackAnim_Frames[i].ElementName);
		target.WriteString(mTexPackAnim_Frames[i].ImagePathFull);
	}

	target.WriteAggregate(mEmitUV0_Offset);
	target.WriteAggregate(mEmitUV0_Repeat);
	target.WriteAggregate(mEmitUV0_Speed);
	target.WriteAggregate(mUV1_Offset);
	target.WriteAggregate(mUV1_Repeat);
	target.WriteAggregate(mUV1_Speed);

	target.WritePointer(mEffectableCtrl);

	target.WriteBool(mIsAnimFramesPlayOnce);
	target.WriteEnum(mCoordinateType0);
	target.WriteEnum(mCoordinateType1);

	target.WriteBool(mIsBackCull);

	PX2_END_DEBUG_STREAM_SAVE(Effectable, target);
}
//----------------------------------------------------------------------------
int Effectable::GetStreamingSize (Stream &stream) const
{
	int size = TriMesh::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_BOOLSIZE(mIsFixedBound);
	size += sizeof(mFixedCenter);
	size += sizeof(mFixedBoundRadius);
	size += PX2_BOOLSIZE(mIsLocal);
	size += sizeof(mEmitSizeX);
	size += sizeof(mEmitSizeY);
	size += sizeof(mEmitSizeZ);
	size += sizeof(mEmitColor);
	size += sizeof(mEmitAlpha);
	size += sizeof(mEmitLife);
	size += PX2_ENUMSIZE(mFaceType);
	size += PX2_ENUMSIZE(mMtlType);
	size += PX2_ENUMSIZE(mBlendMode);

	size += PX2_ENUMSIZE(mTexMode);
	size += PX2_STRINGSIZE(mTexFilename);
	size += PX2_STRINGSIZE(mTexPackEleName);
	size += sizeof(mTexAnim_Repeat);
	size += sizeof(mAnimInterval);
	size += PX2_BOOLSIZE(mIsAnimStartDoRandom);

	size += sizeof(mUserNumAnimFrames);

	int numFrames = (int)mTexPackAnim_Frames.size();
	size += sizeof(numFrames);
	for (int i = 0; i < numFrames; i++)
	{
		size += sizeof(mTexPackAnim_Frames[i].X);
		size += sizeof(mTexPackAnim_Frames[i].Y);
		size += sizeof(mTexPackAnim_Frames[i].W);
		size += sizeof(mTexPackAnim_Frames[i].H);
		size += sizeof(mTexPackAnim_Frames[i].OX);
		size += sizeof(mTexPackAnim_Frames[i].OY);
		size += sizeof(mTexPackAnim_Frames[i].OW);
		size += sizeof(mTexPackAnim_Frames[i].OH);
		size += PX2_BOOLSIZE(mTexPackAnim_Frames[i].Rolated);
		size += sizeof(mTexPackAnim_Frames[i].TexWidth);
		size += sizeof(mTexPackAnim_Frames[i].TexHeight);
		size += PX2_STRINGSIZE(mTexPackAnim_Frames[i].ElementName);
		size += PX2_STRINGSIZE(mTexPackAnim_Frames[i].ImagePathFull);
	}

	size += sizeof(mEmitUV0_Offset);
	size += sizeof(mEmitUV0_Repeat);
	size += sizeof(mEmitUV0_Speed);
	size += sizeof(mUV1_Offset);
	size += sizeof(mUV1_Repeat);
	size += sizeof(mUV1_Speed);

	size += PX2_POINTERSIZE(mEffectableCtrl);

	size += PX2_BOOLSIZE(mIsAnimFramesPlayOnce);
	size += PX2_ENUMSIZE(mCoordinateType0);
	size += PX2_ENUMSIZE(mCoordinateType1);
	size += PX2_BOOLSIZE(mIsBackCull);

	return size;
}
//----------------------------------------------------------------------------