// PX2Renderable.cpp

#include "PX2Renderable.hpp"
#include "PX2Culler.hpp"
#include "PX2VertexBufferAccessor.hpp"
#include "PX2ShaderParameters.hpp"
#include "PX2VertexColor4Material.hpp"
#include "PX2ShineDiffuseConstant.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2MathHelp.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Movable, Renderable);
PX2_IMPLEMENT_STREAM(Renderable);
PX2_IMPLEMENT_ABSTRACT_FACTORY(Renderable);

//----------------------------------------------------------------------------
Renderable::Renderable (PrimitiveType type) :
mEyeDistance(0.0f),
mType(type),
mVFormat(0),
mVBuffer(0),
mIBuffer(0),
mMaterialInstance(0),
mSortIndex(0),
mSubLayer(0),
mIsUseBoundPick(false),
mIsUseShareBuffers(false),
mDBObject_V(0),
mDBObject_I(0),
mIBOffset(0),
mIBNumElements(0),
mIsBackObject(true),
mIsBackTarget(false),
mIsUseLightTexture(false),
mBakeSizeType(BST_NORMAL),
mPhysicsType(PHYS_NONE),
mFogIP_Height(1.0f),
mFogIP_Distance(1.0f),
mIsFakeTransparent(false),
mIsEnableShareDraw(false),
mOnDrawCallback(0)
{
	SetRenderLayer(RL_SCENE);
	SetUpdatePriority(-1);

	mDefaultShine = new0 Shine();
	mDefaultShine->SetName("DefaultShine");
	mDefaultShine->Emissive = Float4::BLACK;
	mDefaultShine->Ambient = Float4::MakeColor(150, 150, 150, 255);
	mDefaultShine->Diffuse = mDefaultShine->Ambient;
	mDefaultShine->Specular = Float4::MakeColor(230, 230, 230, 51);
	mDefaultShine->ReCalTemp();

	mBakeShine = new0 Shine ();
	mBakeShine->SetName("BakeShine");
	mBakeShine->Ambient = Float4::MakeColor(150, 150, 150, 255);
	mBakeShine->Diffuse = mBakeShine->Ambient;
	mBakeShine->Specular = Float4::ZERO;

	mPhysicsParam = Float3::UNIT;
}
//----------------------------------------------------------------------------
Renderable::Renderable (PrimitiveType type, VertexFormat* vformat,
				VertexBuffer* vbuffer, IndexBuffer* ibuffer) :
mEyeDistance(0.0f),
mType(type),
mVFormat(vformat),
mVBuffer(vbuffer),
mIBuffer(ibuffer),
mMaterialInstance(0),
mSortIndex(0),
mSubLayer(0),
mIsUseBoundPick(false),
mIsUseShareBuffers(false),
mDBObject_V(0),
mDBObject_I(0),
mIBOffset(0),
mIBNumElements(0),
mIsBackObject(true),
mIsBackTarget(false),
mIsUseLightTexture(false),
mBakeSizeType(BST_NORMAL),
mPhysicsType(PHYS_NONE),
mFogIP_Height(1.0f),
mFogIP_Distance(1.0f),
mIsEnableShareDraw(false),
mOnDrawCallback(0)
{
	SetRenderLayer(RL_SCENE);
	UpdateModelSpace(GU_MODEL_BOUND_ONLY);
	SetUpdatePriority(-1);

	mDefaultShine = new0 Shine();
	mDefaultShine->SetName("DefaultShine");
	mDefaultShine->Emissive = MathHelp::Float3ToFloat4(Float3::BLACK, 1.0f);
	mDefaultShine->Ambient = Float4::MakeColor(150, 150, 150, 255);
	mDefaultShine->Diffuse = mDefaultShine->Ambient;
	mDefaultShine->Specular = Float4::MakeColor(230, 230, 230, 51);
	mDefaultShine->ReCalTemp();

	mBakeShine = new0 Shine ();
	mBakeShine->SetName("BakeShine");
	mBakeShine->Ambient = Float4::MakeColor(150, 150, 150, 255);
	mBakeShine->Diffuse = mBakeShine->Ambient;
	mBakeShine->Specular = Float4::ZERO;
	mBakeShine->ReCalTemp();

	mPhysicsParam = Float3::UNIT;
}
//----------------------------------------------------------------------------
Renderable::~Renderable ()
{
}
//----------------------------------------------------------------------------
void Renderable::SetRenderLayer (RenderLayer layer, int sublayer)
{
	mLayer = layer;
	mSubLayer = sublayer;
	mSortIndex = (layer<<24)|(sublayer<<16)|(mSortIndex&0xffff);
}
//----------------------------------------------------------------------------
bool Renderable::AddLight (Light *light)
{
	bool bIn = false;

	for (int i=0; i<(int)mInfulencedLights.size(); i++)
	{
		if (light == mInfulencedLights[i])
			bIn = true; 
	}

	if (!bIn)
	{
		mInfulencedLights.push_back(light);
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool Renderable::RemoveLight (Light *light)
{
	std::vector<LightPtr>::iterator it = mInfulencedLights.begin();
	for (; it!=mInfulencedLights.end(); it++)
	{
		if (light == *it)
		{
			mInfulencedLights.erase(it);
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void Renderable::ClearLights ()
{
	mInfulencedLights.clear();
}
//----------------------------------------------------------------------------
int Renderable::GetNumLights() const
{
	return (int)mInfulencedLights.size();
}
//----------------------------------------------------------------------------
Light *Renderable::GetLight(int i)
{
	return mInfulencedLights[i];
}
//----------------------------------------------------------------------------
const Light *Renderable::GetLight(int i) const
{
	return mInfulencedLights[i];
}
//----------------------------------------------------------------------------
void Renderable::SetLightTexture (Texture2D *tex)
{
	PX2_UNUSED(tex);
}
//----------------------------------------------------------------------------
void Renderable::SetUseLightTexture (bool use, Texture2D *lightTex)
{
	PX2_UNUSED(use);
	PX2_UNUSED(lightTex);
}
//----------------------------------------------------------------------------
void Renderable::SetCastShadow(bool castShadow)
{
	Movable::SetCastShadow(castShadow);
}
//----------------------------------------------------------------------------
void Renderable::SetReceiveShadow(bool reciveShadow)
{
	Movable::SetReceiveShadow(reciveShadow);

	if (mMaterialInstance)
	{
		Material *mtl = mMaterialInstance->GetMaterial();
		const std::string &mtlName = mtl->GetName();

		if ("std" == mtlName)
		{
			std::string techName = "std_light";

			if (!reciveShadow)
				techName = "std_light";
			else
				techName = "std_lightshadow";

			mMaterialInstance->SetUseMaterialTechnique(techName);
		}
		if ("terrain" == mtlName)
		{
			std::string techName = "terrain_light";

			if (!reciveShadow)
				techName = "terrain_light";
			else
				techName = "terrain_lightshadow";

			mMaterialInstance->SetUseMaterialTechnique(techName);
		}
	}
}
//----------------------------------------------------------------------------
void Renderable::SetFogInfulenceParam_Height (float param)
{
	mFogIP_Height = param;
}
//----------------------------------------------------------------------------
void Renderable::SetFogInfulenceParam_Distance (float param)
{
	mFogIP_Distance = param;
}
//----------------------------------------------------------------------------
Float4 Renderable::UpdateFogParam(const Float4 &envFogParam) const
{
	Float4 lastFogParam;

	lastFogParam = envFogParam;

	lastFogParam[0] = envFogParam[1] - (envFogParam[1] - envFogParam[0]) * 1.0f
		/ mFogIP_Height;
	lastFogParam[1] = envFogParam[1];

	lastFogParam[2] = envFogParam[2];
	lastFogParam[3] = envFogParam[2] + (envFogParam[3] - envFogParam[2]) * 1.0f
		/ mFogIP_Distance;

	return lastFogParam;
}
//----------------------------------------------------------------------------
Renderable::PreDrawRetType Renderable::OnPreShareDraw(Renderable *shareMesh)
{
	MaterialInstance *shareMI = shareMesh->GetMaterialInstance();
	int shareNumVB = shareMesh->GetVertexBuffer()->GetNumElements();
	int shareNumIB = shareMesh->GetIndexBuffer()->GetNumElements();

	MaterialInstance *mi = GetMaterialInstance();
	VertexBuffer *vb = GetVertexBuffer();
	IndexBuffer *ib = GetIndexBuffer();
	if (!vb || !ib)
		return PBRT_NONE;

	int numVB = vb->GetNumElements();
	int numIB = ib->GetNumElements();

	if (0 == shareNumVB || 0 == shareNumIB)
		return PBRT_NONE;

	if (shareNumVB + numVB > shareMesh->GetVertexBuffer()->GetNumMaxElements())
		return PDRT_VB_FULL;

	if (shareNumIB + numIB > shareMesh->GetIndexBuffer()->GetNumMaxElements())
		return PDRT_IB_FULL;

	if (*mi == *shareMI)
	{
		return PBRT_NONE;
	}
	else
	{
		return PDRT_MTL_NOTEQUAL;
	}
}
//----------------------------------------------------------------------------
void Renderable::OnShareDraw(Renderable *shareMesh)
{
	int shareNumVB = shareMesh->GetVertexBuffer()->GetNumElements();
	int shareNumIB = shareMesh->GetIndexBuffer()->GetNumElements();

	if (0 == shareNumVB || 0 == shareNumIB)
	{
		shareMesh->SetMaterialInstance(GetMaterialInstance());
	}

	VertexBuffer *vb = GetVertexBuffer();
	IndexBuffer *ib = GetIndexBuffer();
	if (!vb || !ib)
		return;

	int numVB = GetVertexBuffer()->GetNumElements();
	int numIB = GetIndexBuffer()->GetNumElements();

	VertexBufferAccessor vbaShare;
	vbaShare.ApplyTo(shareMesh);

	VertexBufferAccessor vba;
	vba.ApplyTo(this);
	for (int i = 0; i < numVB; i++)
	{
		vbaShare.Position<Float3>(shareNumVB + i) = WorldTransform * APoint(vba.Position<Float3>(i));
		vbaShare.Color<Float4>(0, shareNumVB + i) = vba.Color<Float4>(0, i);
		vbaShare.TCoord<Float2>(0, shareNumVB + i) = vba.TCoord<Float2>(0, i);
	}

	for (int i = 0; i < numIB; i++)
	{
		IndexBuffer *ib = shareMesh->GetIndexBuffer();
		if (ib)
		{
			unsigned short *shareIndexs = (unsigned short*)shareMesh->GetIndexBuffer()->GetData();
			if (shareIndexs)
			{
				IndexBuffer *myIB = GetIndexBuffer();
				if (myIB)
				{
					unsigned short *indexs = (unsigned short*)myIB->GetData();
					shareIndexs[shareNumIB + i] = (unsigned short)(shareNumVB + indexs[i]);
				}
			}
		}
	}

	shareNumVB += numVB;
	shareNumIB += numIB;
	shareMesh->GetVertexBuffer()->SetNumElements(shareNumVB);
	shareMesh->GetIndexBuffer()->SetNumElements(shareNumIB);
}
//----------------------------------------------------------------------------
void Renderable::UpdateModelSpace (UpdateType)
{
	UpdateModelBound();
}
//----------------------------------------------------------------------------
void Renderable::OnForceBind ()
{
}
//----------------------------------------------------------------------------
void Renderable::SetOnDrawCallback(OnDrawCallback dcb)
{
	mOnDrawCallback = dcb;
}
//----------------------------------------------------------------------------
OnDrawCallback Renderable::GetOnDrawCallback()
{
	return mOnDrawCallback;
}
//----------------------------------------------------------------------------
void Renderable::OnDraw(Renderer *renderer)
{
	PX2_UNUSED(renderer);

	if (mOnDrawCallback)
	{
		mOnDrawCallback(renderer, this);
	}
}
//----------------------------------------------------------------------------
void Renderable::UpdateWorldData(double applicationTime, double elapsedTime)
{
	if (mMaterialInstance)
		mMaterialInstance->Update(applicationTime, elapsedTime);

	bool cc = mIsAlphaColorBrightnessChanged;
	Movable::UpdateWorldData(applicationTime, elapsedTime);
	if (cc)
	{
		mDefaultShine->Emissive[0] = mLastColor[0] * mLastBrightness;
		mDefaultShine->Emissive[1] = mLastColor[1] * mLastBrightness;
		mDefaultShine->Emissive[2] = mLastColor[2] * mLastBrightness;
		mDefaultShine->Emissive[3] = mLastAlpha;
	}
}
//----------------------------------------------------------------------------
void Renderable::UpdateWorldBound ()
{
	mModelBound.TransformBy(BoundWorldTransform, WorldBound);
}
//----------------------------------------------------------------------------
void Renderable::UpdateModelBound ()
{
	if (!mVBuffer) return;

	int numVertices = mVBuffer->GetNumElements();
	int stride = mVFormat->GetStride();

	int posIndex = mVFormat->GetIndex(VertexFormat::AU_POSITION);
	if (posIndex == -1)
	{
		assertion(false, "Update requires vertex positions\n");
		return;
	}

	VertexFormat::AttributeType posType =
		mVFormat->GetAttributeType(posIndex);
	if (posType != VertexFormat::AT_FLOAT3
		&&  posType != VertexFormat::AT_FLOAT4)
	{
		assertion(false, "Positions must be 3-tuples or 4-tuples\n");
		return;
	}

	char* data = mVBuffer->GetData();
	int posOffset = mVFormat->GetOffset(posIndex);
	mModelBound.ComputeFromData(numVertices, stride, data + posOffset);
}
//----------------------------------------------------------------------------
void Renderable::OnGetVisibleSet (Culler& culler, bool)
{
	int flag_CastShadow = culler.GetFlag_CastShadow();
	if (0 == flag_CastShadow)
	{
	}
	else if (1 == flag_CastShadow)
	{
		if (!IsCastShadow()) return;
	}
	else if (2 == flag_CastShadow)
	{
		if (IsCastShadow()) return;
	}

	AdjustTransparent();

	const Camera *camera = culler.GetCamera();
	assertion(camera!=0, "camera must not be 0.");

	AVector cameraDir = camera->GetDVector();
	AVector diff = WorldBound.GetCenter() - camera->GetPosition();

	mEyeDistance = cameraDir.Dot(diff);

	culler.Insert(this);
}
//----------------------------------------------------------------------------
void Renderable::SetFakeTransparent(bool fakeTrasparent)
{
	mIsFakeTransparent = fakeTrasparent;
}
//----------------------------------------------------------------------------
void Renderable::AdjustTransparent ()
{
	if (mIsFakeTransparent)
	{
		_SetTransparent(true);
		return;
	}

	if (!mMaterialInstance)
	{
		_SetTransparent(false);
		return;
	}

	for (int i=0; i<mMaterialInstance->GetNumPasses(); i++)
	{
		AlphaProperty *alphaProperty =
			mMaterialInstance->GetPass(i)->GetAlphaProperty();
		if (alphaProperty)
		{
			if (alphaProperty->BlendEnabled)
			{
				_SetTransparent(true);
				return;
			}
			else
			{
				_SetTransparent(false);
				return;
			}
		}
	}

	_SetTransparent(false);
}
//----------------------------------------------------------------------------
bool Renderable::LessThan (const Renderable *renderable0,
						   const Renderable *renderable1)
{
	if (renderable0->mSortIndex == renderable1->mSortIndex)
	{
		if (renderable0->IsTransparent())
			return renderable0->mEyeDistance > renderable1->mEyeDistance;
		else
		{
			return renderable0 < renderable1;
		}
	}

	return renderable0->mSortIndex < renderable1->mSortIndex;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Renderable::RegistProperties ()
{
	Movable::RegistProperties();

	AddPropertyClass("Renderable");

	std::vector<std::string> primitiveTypes;
	primitiveTypes.push_back("PT_NONE");
	primitiveTypes.push_back("PT_POLYPOINT");
	primitiveTypes.push_back("PT_POLYSEGMENTS_DISJOINT");
	primitiveTypes.push_back("PT_POLYSEGMENTS_CONTIGUOUS");
	primitiveTypes.push_back("PT_TRIANGLES");
	primitiveTypes.push_back("PT_TRIMESH");
	primitiveTypes.push_back("PT_TRISTRIP");
	primitiveTypes.push_back("PT_TRIFAN");
	AddPropertyEnum("PrimitiveType", (int)GetPrimitiveType(), primitiveTypes, false);
	AddProperty("ModelBoundCenter", PT_APOINT3, GetModelBound().GetCenter(), false);
	AddProperty("ModelBoundRadius", PT_FLOAT, GetModelBound().GetRadius(), false);

	std::vector<std::string> renderLayers;
	renderLayers.push_back("RL_BACKGROUND");
	renderLayers.push_back("RL_SKY");
	renderLayers.push_back("RL_TERRAIN");
	renderLayers.push_back("RL_SCENE");
	renderLayers.push_back("RL_UI");
	renderLayers.push_back("RL_UI_RANGE");
	renderLayers.push_back("RL_UI_RANGE_ELEMENT");
	AddPropertyEnum("RenderLayer", (int)GetRenderLayer(), renderLayers);
	AddProperty("SubLayer", PT_INT, GetSubLayer());

	AddProperty("Shine_Emissive", PT_COLOR3FLOAT3, MathHelp::Float4ToFloat3(mDefaultShine->Emissive));
	AddProperty("Shine_Ambient", PT_COLOR3FLOAT3, MathHelp::Float4ToFloat3(mDefaultShine->Ambient));
	AddProperty("Shine_Diffuse", PT_COLOR3FLOAT3, MathHelp::Float4ToFloat3(mDefaultShine->Diffuse));
	AddProperty("Shine_Specular", PT_COLOR3FLOAT3, MathHelp::Float4ToFloat3(mDefaultShine->Specular));

	AddProperty("Material", PT_STRINGBUTTON, mMaterialInstance->GetMaterialFilename());
	AddProperty("TechniqueName", PT_STRING, mMaterialInstance->GetTechniqueName());
	AddProperty("TechniqueIndex", PT_INT, mMaterialInstance->GetTechniqueIndex());

	AddProperty("SortIndex", PT_INT, (int)GetSortIndex(), false);
	AddProperty("NumLights", PT_INT, GetNumLights(), false);
	AddProperty("EyeDistance", PT_FLOAT, mEyeDistance, false);
	AddProperty("IsTransparent", PT_BOOL, IsTransparent(), false);

	AddProperty("IsBakeObject", PT_BOOL, IsBakeObject());
	AddProperty("IsBakeTarget", PT_BOOL, IsBakeTarget());

	std::vector<std::string> bakeSizeTypes;
	bakeSizeTypes.push_back("BST_NORMAL");
	bakeSizeTypes.push_back("BST_64");
	bakeSizeTypes.push_back("BST_128");
	bakeSizeTypes.push_back("BST_256");
	bakeSizeTypes.push_back("BST_512");
	bakeSizeTypes.push_back("BST_1024");
	AddPropertyEnum("BakeSizeType", (int)GetBakeSizeType(), bakeSizeTypes);

	std::vector<std::string> physTypes;
	physTypes.push_back("PHYS_NONE");
	physTypes.push_back("PHYS_BOX");
	physTypes.push_back("PHYS_SPHERE");
	physTypes.push_back("PHYS_CONVEXHULL");
	physTypes.push_back("PHYS_CONVEXTRIMESH");
	physTypes.push_back("PHYS_HEIGHTFIELD");
	AddPropertyEnum("PhysicsType", (int)GetPhysicsType(), physTypes);

	AddProperty("PhysicsParam", Object::PT_FLOAT3, mPhysicsParam);
}
//----------------------------------------------------------------------------
void Renderable::OnPropertyChanged (const PropertyObject &obj)
{
	Movable::OnPropertyChanged(obj);

	if ("RenderLayer" == obj.Name)
	{
		SetRenderLayer((RenderLayer)PX2_ANY_AS(obj.Data, int), mSubLayer);
	}
	else if ("SubLayer" == obj.Name)
	{
		SetRenderLayer(mLayer, PX2_ANY_AS(obj.Data, int));
	}
	else if ("Shine_Emissive" == obj.Name)
	{
		mDefaultShine->Emissive = MathHelp::Float3ToFloat4(PX2_ANY_AS(obj.Data, Float3), GetAlpha());
	}
	else if ("Shine_Ambient" == obj.Name)
	{
		mDefaultShine->Ambient = MathHelp::Float3ToFloat4(PX2_ANY_AS(obj.Data, Float3), 1.0f);
	}
	else if ("Shine_Diffuse" == obj.Name)
	{
		mDefaultShine->Diffuse = MathHelp::Float3ToFloat4(PX2_ANY_AS(obj.Data, Float3), 1.0f);
	}
	else if ("Shine_Specular" == obj.Name)
	{
		mDefaultShine->Specular = MathHelp::Float3ToFloat4(PX2_ANY_AS(obj.Data, Float3), 1.0f);
	}
	else if ("IsBakeObject" == obj.Name)
	{
		SetBakeObject(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("IsBakeTarget" == obj.Name)
	{
		SetBakeTarget(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("BakeSizeType" == obj.Name)
	{
		SetBakeSizeType((BakeSizeType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("PhysicsType" == obj.Name)
	{
		SetPhysicsType((PhysicsType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("PhysicsParam" == obj.Name)
	{
		SetPhysicsParam(PX2_ANY_AS(obj.Data, Float3));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 名称支持
//----------------------------------------------------------------------------
Object* Renderable::GetObjectByName (const std::string& name)
{
	Object* found = Movable::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	PX2_GET_OBJECT_BY_NAME(mVFormat, name, found);
	PX2_GET_OBJECT_BY_NAME(mVBuffer, name, found);
	PX2_GET_OBJECT_BY_NAME(mIBuffer, name, found);
	PX2_GET_OBJECT_BY_NAME(mMaterialInstance, name, found);

	return 0;
}
//----------------------------------------------------------------------------
void Renderable::GetAllObjectsByName (const std::string& name,
								  std::vector<Object*>& objects)
{
	Movable::GetAllObjectsByName(name, objects);

	PX2_GET_ALL_OBJECTS_BY_NAME(mVFormat, name, objects);
	PX2_GET_ALL_OBJECTS_BY_NAME(mVBuffer, name, objects);
	PX2_GET_ALL_OBJECTS_BY_NAME(mIBuffer, name, objects);
	PX2_GET_ALL_OBJECTS_BY_NAME(mMaterialInstance, name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化
//----------------------------------------------------------------------------
Renderable::Renderable (LoadConstructor value) :
Movable(value),
mEyeDistance(0.0f),
mType(PT_NONE),
mVFormat(0),
mVBuffer(0),
mIBuffer(0),
mMaterialInstance(0),
mSortIndex(0),
mSubLayer(0),
mIsUseBoundPick(false),
mIsUseShareBuffers(false),
mDBObject_V(0),
mDBObject_I(0),
mIBOffset(0),
mIBNumElements(0),
mIsBackObject(true),
mIsBackTarget(false),
mIsUseLightTexture(false),
mBakeSizeType(BST_NORMAL),
mPhysicsType(PHYS_NONE),
mFogIP_Height(1.0f),
mFogIP_Distance(1.0f),
mIsFakeTransparent(false),
mIsEnableShareDraw(false),
mOnDrawCallback(0)
{
	SetUpdatePriority(-1);

	mPhysicsParam = Float3::UNIT;
}
//----------------------------------------------------------------------------
void Renderable::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Movable::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mType);
	source.ReadAggregate(mModelBound);
	source.ReadPointer(mVFormat);
	source.ReadPointer(mVBuffer);
	source.ReadPointer(mIBuffer);
	source.ReadPointer(mMaterialInstance);
	source.ReadEnum(mLayer);
	SetRenderLayer(mLayer);

	source.ReadPointer(mDefaultShine);
	source.ReadBool(mIsUseLightTexture);
	source.ReadString(mLightTexPath);
	source.ReadString(mNormalTexPath);
	source.ReadEnum(mBakeSizeType);
	source.ReadBool(mIsBackObject);
	source.ReadBool(mIsBackTarget);
	source.ReadPointer(mNormalMaterialInstance);
	source.Read(mSubLayer);
	source.ReadPointer(mBakeShine);
	source.ReadEnum(mPhysicsType);
	source.ReadAggregate(mPhysicsParam);
	source.Read(mFogIP_Height);
	source.Read(mFogIP_Distance);

	PX2_END_DEBUG_STREAM_LOAD(Renderable, source);
}
//----------------------------------------------------------------------------
void Renderable::Link (InStream& source)
{
	Movable::Link(source);

	source.ResolveLink(mVFormat);
	source.ResolveLink(mVBuffer);
	source.ResolveLink(mIBuffer);
	source.ResolveLink(mMaterialInstance);
	source.ResolveLink(mDefaultShine);
	source.ResolveLink(mNormalMaterialInstance);
	source.ResolveLink(mBakeShine);
}
//----------------------------------------------------------------------------
void Renderable::PostLink ()
{
	Movable::PostLink();

	if (!mDefaultShine)
	{
		mDefaultShine = new0 Shine();
		mDefaultShine->SetName("DefaultShine");
	}

	if (!mBakeShine)
	{
		mBakeShine = new0 Shine();
		mBakeShine->SetName("BakeShine");
	}
}
//----------------------------------------------------------------------------
bool Renderable::Register (OutStream& target) const
{
	if (Movable::Register(target))
	{
		target.Register(mVFormat);
		target.Register(mVBuffer);
		target.Register(mIBuffer);
		target.Register(mMaterialInstance);
		target.Register(mDefaultShine);
		target.Register(mNormalMaterialInstance);
		target.Register(mBakeShine);

		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void Renderable::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Movable::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mType);
	target.WriteAggregate(mModelBound);
	target.WritePointer(mVFormat);
	target.WritePointer(mVBuffer);
	target.WritePointer(mIBuffer);
	target.WritePointer(mMaterialInstance);
	target.WriteEnum(mLayer);

	target.WritePointer(mDefaultShine);
	target.WriteBool(mIsUseLightTexture);
	target.WriteString(mLightTexPath);
	target.WriteString(mNormalTexPath);
	target.WriteEnum(mBakeSizeType);
	target.WriteBool(mIsBackObject);
	target.WriteBool(mIsBackTarget);
	target.WritePointer(mNormalMaterialInstance);
	target.Write(mSubLayer);
	target.WritePointer(mBakeShine);
	target.WriteEnum(mPhysicsType);
	target.WriteAggregate(mPhysicsParam);
	target.Write(mFogIP_Height);
	target.Write(mFogIP_Distance);

	PX2_END_DEBUG_STREAM_SAVE(Renderable, target);
}
//----------------------------------------------------------------------------
int Renderable::GetStreamingSize (Stream &stream) const
{
	int size = Movable::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += PX2_ENUMSIZE(mType);
	size += sizeof(mModelBound);
	size += PX2_POINTERSIZE(mVFormat);
	size += PX2_POINTERSIZE(mVBuffer);
	size += PX2_POINTERSIZE(mIBuffer);
	size += PX2_POINTERSIZE(mMaterialInstance);
	size += PX2_ENUMSIZE(mLayer);

	size += PX2_POINTERSIZE(mDefaultShine);
	size += PX2_BOOLSIZE(mIsUseLightTexture);
	size += PX2_STRINGSIZE(mLightTexPath);
	size += PX2_STRINGSIZE(mNormalTexPath);
	size += PX2_BOOLSIZE(mBakeSizeType);
	size += PX2_BOOLSIZE(mIsBackObject);
	size += PX2_BOOLSIZE(mIsBackTarget);
	size += PX2_BOOLSIZE(mNormalMaterialInstance);
	size += sizeof(mSubLayer);
	size += PX2_POINTERSIZE(mBakeShine);
	size += PX2_ENUMSIZE(mPhysicsType);
	size += sizeof(mPhysicsParam);
	size += sizeof(mFogIP_Height);
	size += sizeof(mFogIP_Distance);

	return size;
}
//----------------------------------------------------------------------------