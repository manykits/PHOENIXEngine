// PX2ParticleEmitter.cpp

#include "PX2ParticleEmitter.hpp"
#include "PX2ParticleEmitterController.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2EffectModuleCallbacks_ParticleEmitter.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Effectable, ParticleEmitter);
PX2_IMPLEMENT_STREAM(ParticleEmitter);
PX2_IMPLEMENT_FACTORY(ParticleEmitter);
PX2_IMPLEMENT_DEFAULT_NAMES(Effectable, ParticleEmitter);

//----------------------------------------------------------------------------
std::vector<std::string> ParticleEmitter::msModuleNames_EA;
std::vector<std::string> ParticleEmitter::msModuleNames_EO;
//----------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter () :
mIsBlast(false),
mDrawType(DT_NORMAL),
mRotateAxisType(RAT_D),
mEmitRate(10.0f),
mEmitDirType(EST_NORMAL),
mEmitSpeed(1.0f),
mIsEmitDirDynamic(true),
mIsEmitDirLocal(true),
mAccelerate(0.0f),
mEmitRotateDegree(0.0f),
mEmitRotateSpeed(0.0f),
mPlacerType(PT_SPHERE),
mPlacerInLength(0.0f),
mPlacerOutLength(1.0f),
mPlacerInWidth(0.0f),
mPlacerOutWidth(1.0f),
mPlacerInHeight(0.0f),
mPlacerOutHeight(1.0f),
mCollisionType(CT_NONE),
mCollisionOption(CO_REFLECT),
mCollisionHeight(0.0f),
mCollisionSpeedPercent(1.0f)
{
	SetName("ParticleEmitter");

	mPivotPoint = Float2(0.5f, 0.5f);
	mEmitDir = AVector::UNIT_Z;
	mRotateAxis = AVector::UNIT_Z;
	mAccelerateDir = -AVector::UNIT_Z;

	ParticleEmitterController *pCtrl = new0 ParticleEmitterController();

	mEffectableCtrl = pCtrl;
	mEffectableCtrl->SetName("ParticleEmitterController");
	AttachController(mEffectableCtrl);

	pCtrl->SetMaxNumParticles(100);

	SetTex("Data/engine/effect/star.dds");
	SetEmitColor(Float3::RED);
}
//----------------------------------------------------------------------------
ParticleEmitter::~ParticleEmitter ()
{
}
//----------------------------------------------------------------------------
APoint ParticleEmitter::GetPlacerAPoint () const
{
	float halfOutLength = mPlacerOutLength/2.0f;
	float halfInLength = mPlacerInLength/2.0f;
	float halfOutWidth = mPlacerOutWidth/2.0f;
	float halfInWidth = mPlacerInWidth/2.0f;
	float halfOutHeight = mPlacerOutHeight/2.0f;
	float halfInHeight = mPlacerInHeight/2.0f;

	float signLength = Mathf::Sign(Mathf::SymmetricRandom());
	float signWidth = Mathf::Sign(Mathf::SymmetricRandom());
	float signHeight = Mathf::Sign(Mathf::SymmetricRandom());
	if (0 == signLength)
		signLength = 1.0f;
	if (0 == signWidth)
		signWidth = 1.0f;
	if (0 == signHeight)
		signHeight = 1.0f;

	APoint pos = APoint::ORIGIN;
	if (PT_BOX == mPlacerType)
	{
		float halfLength = Mathf::IntervalRandom(halfInLength, halfOutLength)*signLength;
		float halfWidth = Mathf::IntervalRandom(halfInWidth, halfOutWidth)*signWidth;
		float halfHeight = Mathf::IntervalRandom(halfInHeight, halfOutHeight)*signHeight;

		pos = APoint(halfLength, halfWidth, halfHeight);
	}
	else if (PT_SPHERE == mPlacerType)
	{
		AVector dir = AVector(Mathf::SymmetricRandom(), 
			Mathf::SymmetricRandom(),
			Mathf::SymmetricRandom());
		float length = dir.Normalize();
		if (0.0f == length)
		{
			dir = AVector::UNIT_X;
		}

		float halfLength = Mathf::IntervalRandom(halfInLength, halfOutLength)*signLength;
		pos = APoint::ORIGIN + dir * halfLength;
	}
	else if (PT_COLUMN == mPlacerType)
	{
		AVector dir = AVector(Mathf::SymmetricRandom(),
			Mathf::SymmetricRandom(), 0.0f);
		float length = dir.Normalize();
		if (0.0f == length)
		{
			dir = AVector::UNIT_X;
		}

		float halfLength = Mathf::IntervalRandom(halfInLength, halfOutLength)*signLength;
		float halfHeight = Mathf::IntervalRandom(halfInHeight, halfOutHeight)*signHeight;
		pos = APoint::ORIGIN + dir*halfLength + AVector::UNIT_Z*halfHeight;
	}

	if (!IsLocal())
		pos = WorldTransform * pos;

	return pos;
}
//----------------------------------------------------------------------------
void ParticleEmitter::SetMaxNumParticles (int num)
{
	ParticleEmitterController *ctrl
		= DynamicCast<ParticleEmitterController>(mEffectableCtrl);
	ctrl->SetMaxNumParticles(num);
}
//----------------------------------------------------------------------------
int ParticleEmitter::GetMaxNumParticles () const
{
	ParticleEmitterController *ctrl
		= DynamicCast<ParticleEmitterController>(mEffectableCtrl);
	return ctrl->GetMaxNumParticles();
}
//----------------------------------------------------------------------------
AVector &ParticleEmitter::GetEmitDir ()
{
	if (mIsEmitDirDynamic)
	{
		EffectModule *module = mEffectableCtrl->GetModuleByTypeName("PE_EmitDir");
		if (module)
		{
			PE_Callbacks::PE_EmitDir(module, this, mEffectableCtrl->GetPlayedTimeMinusDelay());
		}
	}

	return mEmitDir;
}
//----------------------------------------------------------------------------
const std::vector<std::string> &ParticleEmitter::GetAllModuleNames_EA () const
{
	return msModuleNames_EA;
}
//----------------------------------------------------------------------------
const std::vector<std::string> &ParticleEmitter::GetAllModuleNames_EO () const
{
	return msModuleNames_EO;
}
//----------------------------------------------------------------------------
void ParticleEmitter::GenBuffers ()
{ 
	Effectable::GenBuffers();

	ParticleEmitterController *pec = DynamicCast<ParticleEmitterController>(
		mEffectableCtrl);

	VertexBufferAccessor vba(mVFormat, mVBuffer);
	assertion(vba.HasPosition(), "Positions must exist\n");

	// 计算在粒子模型坐标系下的相机方向
	int vertexIndex = 0;
	TRecyclingArray<ParticleObject> *arr = pec->GetArray();
	for (int i=0; i<arr->GetQuantity(); i++)
	{
		if (!arr->IsUsed(i))
			continue;

		ParticleObject &parObj = arr->GetAt(i);

		float width = parObj.SizeX;
		float height = parObj.SizeY;
		float anchorWidth = mPivotPoint[0]*width;
		float anchorHeight = mPivotPoint[1]*height;

		AVector scaledUpR = parObj.UVector * height + parObj.RVector * width;
		AVector scaledUmR = parObj.UVector * height - parObj.RVector * width;

		const APoint &position = parObj.Pos;

		APoint pos0 = position 
			- parObj.RVector * anchorWidth
			- parObj.UVector * anchorHeight;

		APoint pos1 = position 
			+ parObj.RVector * (width-anchorWidth)
			- parObj.UVector * anchorHeight;

		APoint pos2 = position 
			+ parObj.RVector * (width-anchorWidth)
			+ parObj.UVector * (height-anchorHeight);

		APoint pos3 = position 
			- parObj.RVector * anchorWidth
			+ parObj.UVector * (height-anchorHeight);

		Float4 color;
		color[0] = parObj.Color[0];
		color[1] = parObj.Color[1];
		color[2] = parObj.Color[2];
		color[3] = parObj.Alpha;

		float uBegin = parObj.UV0Begin[0];
		float uEnd = parObj.UV0End[0];
		float vBegin = parObj.UV0Begin[1];
		float vEnd = parObj.UV0End[1];

		if (!IsBlast())
		{
			GetUV(parObj.StartRandomIndex, parObj.Age, uBegin, uEnd, vBegin, vEnd);
		}
		else
		{
			const std::vector<TexPackElement> &animFrames = GetTexPackAnim_Frames();
			int numAnims = (int)animFrames.size();
			if (numAnims>0 && parObj.BlastIndex<numAnims)
			{
				const TexPackElement &ele = animFrames[parObj.BlastIndex];

				uBegin = (float)ele.X/(float)ele.TexWidth;
				uEnd = (float)(ele.X+ele.W)/(float)ele.TexWidth;
				vBegin = (float)(ele.TexHeight-ele.Y-ele.H)/(float)ele.TexHeight;
				vEnd = (float)(ele.TexHeight-ele.Y)/(float)ele.TexHeight;
			}
		}

		vba.Position<Float3>(vertexIndex) = pos0;
		vba.Color<Float4>(0, vertexIndex) = color;
		vba.TCoord<Float2>(0, vertexIndex) = Float2(uEnd, vBegin);
		vertexIndex++;

		vba.Position<Float3>(vertexIndex) = pos1;
		vba.Color<Float4>(0, vertexIndex) = color;
		vba.TCoord<Float2>(0, vertexIndex) = Float2(uBegin, vBegin);
		vertexIndex++;

		vba.Position<Float3>(vertexIndex) = pos2;
		vba.Color<Float4>(0, vertexIndex) = color;
		vba.TCoord<Float2>(0, vertexIndex) = Float2(uBegin, vEnd);
		vertexIndex++;

		vba.Position<Float3>(vertexIndex) = pos3;
		vba.Color<Float4>(0, vertexIndex) = color;
		vba.TCoord<Float2>(0, vertexIndex) = Float2(uEnd, vEnd);
		vertexIndex++;
	}

	mVBuffer->SetNumElements(vertexIndex);
	mIBuffer->SetNumElements((int)(1.5f*vertexIndex));

	if (mIsFixedBound)
	{
		mModelBound.SetRadius(mFixedBoundRadius);
		mModelBound.SetCenter(mFixedCenter);
	}
	else
	{
		UpdateModelSpace(GU_MODEL_BOUND_ONLY);

		if (!mIsLocal)
		{
			APoint center = mModelBound.GetCenter();
			center -= WorldTransform.GetTranslate();
			mModelBound.SetCenter(center);
		}
	}

	Renderer::UpdateAll(mVBuffer);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void ParticleEmitter::RegistProperties ()
{
	Effectable::RegistProperties();

	AddPropertyClass("ParticleEmitter");
	AddProperty("IsBlast", PT_BOOL, IsBlast());
	AddProperty("MaxNumParticles", PT_INT, GetMaxNumParticles());

	std::vector<std::string> drawTypes;
	drawTypes.push_back("DT_NORMAL");
	drawTypes.push_back("DT_MODEL");
	AddPropertyEnum("DrawType", (int)GetDrawType(), drawTypes);
	AddProperty("ModelFilename", PT_STRINGBUTTON, GetModelFilename());

	AddProperty("AnchorPoint", PT_FLOAT2, GetPivot());

	std::vector<std::string> rotateAxisTypes;
	rotateAxisTypes.push_back("RAT_D");
	rotateAxisTypes.push_back("RAT_U");
	rotateAxisTypes.push_back("RAT_R");
	rotateAxisTypes.push_back("RAT_FREE");
	AddPropertyEnum("RotateAxisType", (int)GetRotateAxisType(), rotateAxisTypes);
	AddProperty("EmitRotateAxis", PT_AVECTOR3, GetEmitRotateAxis());
	AddProperty("EmitRotateDegree", PT_FLOAT, GetEmitRotateDegree());
	AddProperty("EmitRotateSpeed", PT_FLOAT, GetEmitRotateSpeed());

	std::vector<std::string> emitDirTypes;
	emitDirTypes.push_back("EST_NORMAL");
	emitDirTypes.push_back("EST_TOATTRACTPOINT");
	emitDirTypes.push_back("EST_LEVELATTRACTPOINT");
	AddPropertyEnum("EmitDirType", (int)GetEmitDirType(), emitDirTypes);
	
	AddProperty("IsEmitDirDynamic", PT_BOOL, IsEmitDirDynamic());
	AddProperty("EmitDir", PT_AVECTOR3, GetEmitDir());
	AddProperty("IsEmitDirLocal", PT_BOOL, IsEmitDirLocal());
	AddProperty("EmitSpeed", PT_FLOAT, GetEmitSpeed());
	AddProperty("Obstruct", PT_AVECTOR3, GetObstruct());
	AddProperty("EmitAccelerateDir", PT_AVECTOR3, GetEmitAccelerateDir());
	AddProperty("EmitAccelerate", PT_FLOAT, GetEmitAccelerate());
	AddProperty("EmitAttactPoint", PT_APOINT3, GetEmitAttactPoint());
	AddProperty("EmitAttactStrength", PT_AVECTOR3, GetEmitAttackStrength());
	AddProperty("EmitRate", PT_FLOAT, GetEmitRate());

	std::vector<std::string> placerTypes;
	placerTypes.push_back("PT_BOX");
	placerTypes.push_back("PT_SPHERE");
	placerTypes.push_back("PT_COLUMN");
	AddPropertyEnum("PlacerType", (int)GetPlacerType(), placerTypes);

	std::vector<std::string> collisionTypes;
	collisionTypes.push_back("CT_NONE");
	collisionTypes.push_back("CT_FACE_HEIGHTUP");
	AddPropertyEnum("CollisionType", (int)GetCollisionType(), collisionTypes);
	std::vector<std::string> collisionOptions;
	collisionOptions.push_back("CO_REFLECT");
	collisionOptions.push_back("CO_DEAD");
	AddPropertyEnum("CollisionOption", (int)GetCollisionOption(), collisionOptions);
	AddProperty("CollisionFaceHeight", PT_FLOAT, GetCollisionFaceHeight());
	AddProperty("CollisionSpeedPercent", PT_FLOAT, GetCollisionSpeedPercent());

	AddProperty("PlacerInLength", PT_FLOAT, GetPlacerInLength());
	AddProperty("PlacerOutLength", PT_FLOAT, GetPlacerOutLength());
	AddProperty("PlacerInWidth", PT_FLOAT, GetPlacerInWidth());
	AddProperty("PlacerOutWidth", PT_FLOAT, GetPlacerOutWidth());
	AddProperty("PlacerInHeight", PT_FLOAT, GetPlacerInHeight());
	AddProperty("PlacerOutHeight", PT_FLOAT, GetPlacerOutHeight());
}
//----------------------------------------------------------------------------
void ParticleEmitter::OnPropertyChanged (const PropertyObject &obj)
{
	Effectable::OnPropertyChanged(obj);

	if ("IsBlast" == obj.Name)
	{
		SetBlast(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("MaxNumParticles" == obj.Name)
	{
		SetMaxNumParticles(PX2_ANY_AS(obj.Data, int));
	}
	else if ("DrawType" == obj.Name)
	{
		SetDrawType((DrawType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("ModelFilename" == obj.Name)
	{
		SetModelFilename(PX2_ANY_AS(obj.Data, std::string));
	}
	else if ("AnchorPoint" == obj.Name)
	{
		SetPivot(PX2_ANY_AS(obj.Data, Float2));
	}
	else if ("RotateAxisType" == obj.Name)
	{
		SetRotateAxisType((RotateAxisType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("EmitRotateAxis" == obj.Name)
	{
		SetEmitRotateAxis(PX2_ANY_AS(obj.Data, AVector));
	}
	else if ("EmitRotateDegree" == obj.Name)
	{
		SetEmitRotateDegree(PX2_ANY_AS(obj.Data, float));
	}
	else if ("EmitRotateSpeed" == obj.Name)
	{
		SetEmitRotateSpeed(PX2_ANY_AS(obj.Data, float));
	}
	else if ("EmitDirType" == obj.Name)
	{
		SetEmitDirType((EmitDirType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("IsEmitDirDynamic" == obj.Name)
	{
		SetEmitDirDynamic(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("EmitDir" == obj.Name)
	{
		SetEmitDir(PX2_ANY_AS(obj.Data, AVector));
	}
	else if ("IsEmitDirLocal" == obj.Name)
	{
		SetEmitDirLocal(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("EmitSpeed" == obj.Name)
	{
		SetEmitSpeed(PX2_ANY_AS(obj.Data, float));
	}
	else if ("Obstruct" == obj.Name)
	{
		SetObstruct(PX2_ANY_AS(obj.Data, AVector));
	}
	else if ("EmitAccelerateDir" == obj.Name)
	{
		SetEmitAccelerateDir(PX2_ANY_AS(obj.Data, AVector));
	}
	else if ("EmitAccelerate" == obj.Name)
	{
		SetEmitAccelerate(PX2_ANY_AS(obj.Data, float));
	}
	else if ("EmitAttactPoint" == obj.Name)
	{
		SetEmitAttractPoint(PX2_ANY_AS(obj.Data, APoint));
	}
	else if ("EmitAttactStrength" == obj.Name)
	{
		SetEmitAttactStrength(PX2_ANY_AS(obj.Data, AVector));
	}
	else if ("EmitRate" == obj.Name)
	{
		SetEmitRate(PX2_ANY_AS(obj.Data, float));
	}
	else if ("CollisionType" == obj.Name)
	{
		SetCollisionType((CollisionType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("CollisionOption" == obj.Name)
	{
		SetCollisionOption((CollisionOption)PX2_ANY_AS(obj.Data, int));
	}
	else if ("CollisionFaceHeight" == obj.Name)
	{
		SetCollisionFaceHeight(PX2_ANY_AS(obj.Data, float));
	}
	else if ("CollisionSpeedPercent" == obj.Name)
	{
		SetCollisionSpeedPercent(PX2_ANY_AS(obj.Data, float));
	}
	else if ("PlacerType" == obj.Name)
	{
		SetPlacerType((PlacerType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("PlacerInLength" == obj.Name)
	{
		SetPlacerInLength(PX2_ANY_AS(obj.Data, float));
	}
	else if ("PlacerOutLength" == obj.Name)
	{
		SetPlacerOutLength(PX2_ANY_AS(obj.Data, float));
	}
	else if ("PlacerInWidth" == obj.Name)
	{
		SetPlacerInWidth(PX2_ANY_AS(obj.Data, float));
	}
	else if ("PlacerOutWidth" == obj.Name)
	{
		SetPlacerOutWidth(PX2_ANY_AS(obj.Data, float));
	}
	else if ("PlacerInHeight" == obj.Name)
	{
		SetPlacerInHeight(PX2_ANY_AS(obj.Data, float));
	}
	else if ("PlacerOutHeight" == obj.Name)
	{
		SetPlacerOutHeight(PX2_ANY_AS(obj.Data, float));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化
//----------------------------------------------------------------------------
ParticleEmitter::ParticleEmitter (LoadConstructor value)
	:
Effectable(value),
mIsBlast(false),
mDrawType(DT_NORMAL),
mRotateAxisType(RAT_D),
mEmitRate(10.0f),
mEmitSpeed(1.0f),
mEmitDirType(EST_NORMAL),
mIsEmitDirDynamic(true),
mIsEmitDirLocal(true),
mAccelerate(0.0f),
mEmitRotateDegree(0.0f),
mEmitRotateSpeed(0.0f),
mPlacerInLength(0.0f),
mPlacerOutLength(1.0f),
mPlacerInWidth(0.0f),
mPlacerOutWidth(1.0f),
mPlacerInHeight(0.0f),
mPlacerOutHeight(1.0f),
mCollisionType(CT_NONE),
mCollisionOption(CO_REFLECT),
mCollisionHeight(0.0f),
mCollisionSpeedPercent(1.0f)
{
	mPivotPoint = Float2(0.5f, 0.5f);
	mEmitDir = AVector::UNIT_Z;
	mRotateAxis = AVector::UNIT_Z;
	mAccelerateDir = -AVector::UNIT_Z;
}
//----------------------------------------------------------------------------
void ParticleEmitter::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Effectable::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadBool(mIsBlast);

	source.ReadEnum(mDrawType);
	source.ReadString(mModelFilename);

	source.ReadAggregate(mPivotPoint);

	source.ReadEnum(mRotateAxisType);
	source.ReadAggregate(mRotateAxis);
	source.Read(mEmitRotateDegree);
	source.Read(mEmitRotateSpeed);
	source.ReadAggregate(mEmitDir);

	source.ReadBool(mIsEmitDirDynamic);

	source.ReadBool(mIsEmitDirLocal);

	source.Read(mEmitSpeed);
	source.ReadAggregate(mAccelerateDir);
	source.Read(mAccelerate);
	
	source.ReadAggregate(mAttactPoint);
	source.ReadAggregate(mAttackStrength);

	source.Read(mEmitRate);

	source.ReadEnum(mPlacerType);
	source.Read(mPlacerInLength);
	source.Read(mPlacerOutLength);
	source.Read(mPlacerInWidth);
	source.Read(mPlacerOutWidth);
	source.Read(mPlacerInHeight);
	source.Read(mPlacerOutHeight);

	source.ReadEnum(mEmitDirType);

	source.ReadAggregate(mObstruct);
	source.ReadEnum(mCollisionType);
	source.ReadEnum(mCollisionOption);
	source.Read(mCollisionHeight);
	source.Read(mCollisionSpeedPercent);

	PX2_END_DEBUG_STREAM_LOAD(ParticleEmitter, source);
}
//----------------------------------------------------------------------------
void ParticleEmitter::Link (InStream& source)
{
	Effectable::Link(source);
}
//----------------------------------------------------------------------------
void ParticleEmitter::PostLink ()
{
	Effectable::PostLink();

	GetVertexBuffer()->SetNumElements(0);
}
//----------------------------------------------------------------------------
bool ParticleEmitter::Register (OutStream& target) const
{
	if (Effectable::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void ParticleEmitter::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Effectable::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteBool(mIsBlast);
	target.WriteEnum(mDrawType);
	target.WriteString(mModelFilename);
	target.WriteAggregate(mPivotPoint);
	target.WriteEnum(mRotateAxisType);
	target.WriteAggregate(mRotateAxis);
	target.Write(mEmitRotateDegree);
	target.Write(mEmitRotateSpeed);
	target.WriteAggregate(mEmitDir);
	target.WriteBool(mIsEmitDirDynamic);
	target.WriteBool(mIsEmitDirLocal);
	target.Write(mEmitSpeed);
	target.WriteAggregate(mAccelerateDir);
	target.Write(mAccelerate);
	target.WriteAggregate(mAttactPoint);
	target.WriteAggregate(mAttackStrength);
	target.Write(mEmitRate);
	target.WriteEnum(mPlacerType);
	target.Write(mPlacerInLength);
	target.Write(mPlacerOutLength);
	target.Write(mPlacerInWidth);
	target.Write(mPlacerOutWidth);
	target.Write(mPlacerInHeight);
	target.Write(mPlacerOutHeight);
	target.WriteEnum(mEmitDirType);

	target.WriteAggregate(mObstruct);
	target.WriteEnum(mCollisionType);
	target.WriteEnum(mCollisionOption);
	target.Write(mCollisionHeight);
	target.Write(mCollisionSpeedPercent);

	PX2_END_DEBUG_STREAM_SAVE(ParticleEmitter, target);
}
//----------------------------------------------------------------------------
int ParticleEmitter::GetStreamingSize (Stream &stream) const
{
	int size = Effectable::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_BOOLSIZE(mIsBlast);

	size += PX2_ENUMSIZE(mDrawType);
	size += PX2_STRINGSIZE(mModelFilename);

	size += sizeof(mPivotPoint);

	size += PX2_ENUMSIZE(mRotateAxisType);
	size += sizeof(mRotateAxis);
	size += sizeof(mEmitRotateDegree);
	size += sizeof(mEmitRotateSpeed);
	size += sizeof(mEmitDir);
	size += sizeof(mEmitSpeed);
	size += sizeof(mAccelerateDir);

	size += PX2_BOOLSIZE(mIsEmitDirDynamic);

	size += PX2_BOOLSIZE(mIsEmitDirLocal);

	size += sizeof(mAccelerate);

	size += sizeof(mAttactPoint);
	size += sizeof(mAttackStrength);

	size += sizeof(mEmitRate);

	size += PX2_ENUMSIZE(mPlacerType);
	size += sizeof(mPlacerInLength);
	size += sizeof(mPlacerOutLength);
	size += sizeof(mPlacerInWidth);
	size += sizeof(mPlacerOutWidth);
	size += sizeof(mPlacerInHeight);
	size += sizeof(mPlacerOutHeight);

	size += PX2_ENUMSIZE(mEmitDirType);

	size += sizeof(mObstruct);
	size += PX2_ENUMSIZE(mCollisionType);
	size += PX2_ENUMSIZE(mCollisionOption);
	size += sizeof(mCollisionHeight);
	size += sizeof(mCollisionSpeedPercent);

	return size;
}
//----------------------------------------------------------------------------
