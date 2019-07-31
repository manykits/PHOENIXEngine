// PX2EffectModel.cpp

#include "PX2EffectModel.hpp"
#include "PX2EffectModelController.hpp"
#include "PX2StandardMesh.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Effectable, EffectModel);
PX2_IMPLEMENT_STREAM(EffectModel);
PX2_IMPLEMENT_FACTORY(EffectModel);
PX2_IMPLEMENT_DEFAULT_NAMES(Effectable, EffectModel);

//----------------------------------------------------------------------------
std::vector<std::string> EffectModel::msModuleNames_EA;
std::vector<std::string> EffectModel::msModuleNames_EO;
//----------------------------------------------------------------------------
EffectModel::EffectModel ()
	:
mModelType(MT_SPHERE),
mRadiusSample(16),
mZSample(16),
mHeightAlphaType(HAT_NORMAL),
mIsNeedReGenMesh(true)
{
	SetName("EffectModel");

	mEffectableCtrl = new0 EffectModelController();
	mEffectableCtrl->SetName("EffectModelController");
	AttachController(mEffectableCtrl);

	SetLocal(true);
	SetBlendMode(BM_NONE);
}
//----------------------------------------------------------------------------
EffectModel::~EffectModel ()
{
}
//----------------------------------------------------------------------------
void EffectModel::Reset ()
{
	Effectable::Reset();

	mIsNeedReGenMesh = true;
}
//----------------------------------------------------------------------------
void EffectModel::SetModelType (ModelType modelType)
{
	if (mModelType == modelType)
		return;

	mModelType = modelType;

	mIsNeedReGenMesh = true;
}
//----------------------------------------------------------------------------
void EffectModel::SetZSample (int zSample)
{
	if (zSample < 2)
		return;

	mZSample = zSample;

	mIsNeedReGenMesh = true;
}
//----------------------------------------------------------------------------
void EffectModel::SetRadiusSample (int radiusSample)
{
	if (radiusSample < 2)
		return;

	mRadiusSample = radiusSample;

	mIsNeedReGenMesh = true;
}
//----------------------------------------------------------------------------
void EffectModel::SetHeightAlphaType (HeightAlphaType type)
{
	mHeightAlphaType = type;
}
//----------------------------------------------------------------------------
void EffectModel::SetModelFilename (const std::string &filename)
{
	mModelFilename = filename;

	mIsNeedReGenMesh = true;
}
//----------------------------------------------------------------------------
void EffectModel::GenBuffers ()
{
	Effectable::GenBuffers();

	EffectModelController *emController =
		(EffectModelController*)GetEffectableController();

	if (mIsNeedReGenMesh)
	{
		GenMesh();
		mIsNeedReGenMesh = false;
	}

	const EffectObject *eo = emController->GetEffectModelObject();

	if (eo)
	{
		float uBegin = eo->UV0Begin[0];
		float uEnd =  eo->UV0End[0];
		float vBegin =  eo->UV0Begin[1];
		float vEnd =  eo->UV0End[1];
		GetUV(eo->StartRandomIndex,	eo->Age, uBegin, uEnd, vBegin, vEnd);

		float zLegth = 1.0f;
		float zStart = 0.0f;
		if (MT_SPHERE == mModelType)
		{
			zLegth = GetEmitSizeX()*2.0f;
			zStart = -GetEmitSizeX();
		}
		else if (MT_CYLINDEROPEN == mModelType)
		{
			zLegth = GetEmitSizeZ();
			zStart = -zLegth/2.0f;
		}

		VertexBufferAccessor vba(GetVertexFormat(), GetVertexBuffer());

		for (int i=0; i<vba.GetNumVertices(); i++)
		{
			Float2 uvInit = mInitUVs[i];
			float blend = 1.0f;

			APoint pos = vba.Position<Float3>(i);

			if (HAT_H2L_APPEAR == mHeightAlphaType)
			{
				blend = 1.0f - Mathf::FAbs(pos.Z()-zStart)/zLegth;
			}
			else if (HAT_H2L_DISAPPEAR == mHeightAlphaType)
			{
				blend = Mathf::FAbs(pos.Z()-zStart)/zLegth;
			}
			else if (HAT_H2L_APPEAR_DISAPPEAR == mHeightAlphaType)
			{
				blend = 1.0f - Mathf::FAbs(pos.Z())/(zLegth/2.0f);
			}
			else if (HAT_H2L_DISAPPEAR_APPEAR == mHeightAlphaType)
			{
				blend = Mathf::FAbs(pos.Z())/(zLegth/2.0f);
			}

			float alpha = eo->Alpha * blend;
			vba.Color<Float4>(0, i) = Float4(eo->Color[0], eo->Color[1], 
				eo->Color[2], alpha);
			vba.TCoord<Float2>(0, i) = Float2(uBegin+(uEnd-uBegin)*uvInit[0],
				vBegin+(vEnd-vBegin)*uvInit[1]);
		}
		
		Renderer::UpdateAll(GetVertexBuffer());
	}
}
//----------------------------------------------------------------------------
void EffectModel::GenMesh ()
{
	StandardMesh stdMesh(GetVertexFormat(), false);

	TriMeshPtr mesh;

	if (MT_SPHERE == mModelType)
	{
		mesh = stdMesh.Sphere(mZSample, mRadiusSample, GetEmitSizeX());
	}
	else if (MT_CYLINDEROPEN == mModelType)
	{
		mesh = stdMesh.Cylinder(mZSample, mRadiusSample, GetEmitSizeX(), GetEmitSizeZ(), true);
	}
	else if (MT_MODEL == mModelType)
	{
		if (!mModelFilename.empty())
			mesh = DynamicCast<TriMesh>(PX2_RM.BlockLoadCopy(mModelFilename));
	}

	if (mesh)
	{
		SetVertexBuffer(mesh->GetVertexBuffer());
		SetIndexBuffer(mesh->GetIndexBuffer());

		mInitUVs.clear();
		VertexBufferAccessor vba(GetVertexFormat(), GetVertexBuffer());
		for (int i=0; i<vba.GetNumVertices(); i++)
		{
			Float2 uv = vba.TCoord<Float2>(0, i);
			mInitUVs.push_back(uv);
		}
	}
}
//----------------------------------------------------------------------------
const std::vector<std::string> &EffectModel::GetAllModuleNames_EA () const
{
	return msModuleNames_EA;
}
//----------------------------------------------------------------------------
const std::vector<std::string> &EffectModel::GetAllModuleNames_EO () const
{
	return msModuleNames_EO;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void EffectModel::RegistProperties ()
{
	Effectable::RegistProperties();

	AddPropertyClass("EffectModel");

	std::vector<std::string> modelTypes;
	modelTypes.push_back("MT_SPHERE");
	modelTypes.push_back("MT_CYLINDEROPEN");
	modelTypes.push_back("MT_MODEL");
	AddPropertyEnum("ModelType", (int)GetModelType(), modelTypes);

	AddProperty("RadiusSample", PT_INT, GetRadiusSample());
	AddProperty("ZSample", PT_INT, GetZSample());
	AddProperty("ModelFilename", PT_STRING, GetModelFilename());

	std::vector<std::string> heightAlphaTypes;
	heightAlphaTypes.push_back("HAT_NORMAL");
	heightAlphaTypes.push_back("HAT_H2L_APPEAR");
	heightAlphaTypes.push_back("HAT_H2L_DISAPPEAR");
	heightAlphaTypes.push_back("HAT_H2L_APPEAR_DISAPPEAR");
	heightAlphaTypes.push_back("HAT_H2L_DISAPPEAR_APPEAR");
	AddPropertyEnum("HeightAlphaType", (int)GetHeightAlphaType(), heightAlphaTypes);
}
//----------------------------------------------------------------------------
void EffectModel::OnPropertyChanged (const PropertyObject &obj)
{
	Effectable::OnPropertyChanged(obj);

	if ("ModelType" == obj.Name)
	{
		SetModelType((ModelType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("RadiusSample" == obj.Name)
	{
		SetRadiusSample(PX2_ANY_AS(obj.Data, int));
	}
	else if ("ZSample" == obj.Name)
	{
		SetZSample(PX2_ANY_AS(obj.Data, int));
	}
	else if ("ModelFilename" == obj.Name)
	{
		SetModelFilename(PX2_ANY_AS(obj.Data, std::string));
	}
	else if ("HeightAlphaType" == obj.Name)
	{
		SetHeightAlphaType((HeightAlphaType)PX2_ANY_AS(obj.Data, int));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
EffectModel::EffectModel (LoadConstructor value) :
Effectable(value),
mModelType(MT_SPHERE),
mRadiusSample(16),
mZSample(16),
mHeightAlphaType(HAT_NORMAL),
mIsNeedReGenMesh(false) // must be false
{
}
//----------------------------------------------------------------------------
void EffectModel::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Effectable::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mModelType);
	source.Read(mRadiusSample);
	source.Read(mZSample);
	source.ReadEnum(mHeightAlphaType);

	source.ReadString(mModelFilename);

	PX2_END_DEBUG_STREAM_LOAD(EffectModel, source);
}
//----------------------------------------------------------------------------
void EffectModel::Link (InStream& source)
{
	Effectable::Link(source);
}
//----------------------------------------------------------------------------
void EffectModel::PostLink ()
{
	Effectable::PostLink();
}
//----------------------------------------------------------------------------
bool EffectModel::Register (OutStream& target) const
{
	if (Effectable::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void EffectModel::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Effectable::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mModelType);
	target.Write(mRadiusSample);
	target.Write(mZSample);
	target.WriteEnum(mHeightAlphaType);

	target.WriteString(mModelFilename);

	PX2_END_DEBUG_STREAM_SAVE(EffectModel, target);
}
//----------------------------------------------------------------------------
int EffectModel::GetStreamingSize (Stream &stream) const
{
	int size = Effectable::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_ENUMSIZE(mModelType);
	size += sizeof(mRadiusSample);
	size += sizeof(mZSample);
	size += PX2_ENUMSIZE(mHeightAlphaType);
	
	size += PX2_STRINGSIZE(mModelFilename);

	return size;
}
//----------------------------------------------------------------------------