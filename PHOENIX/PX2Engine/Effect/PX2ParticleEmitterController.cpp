// PX2ParticleEmitterController.cpp

#include "PX2ParticleEmitterController.hpp"
#include "PX2ParticleEmitter.hpp"
#include "PX2Log.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, EffectableController, ParticleEmitterController);
PX2_IMPLEMENT_STREAM(ParticleEmitterController);
PX2_IMPLEMENT_FACTORY(ParticleEmitterController);
PX2_IMPLEMENT_DEFAULT_NAMES(EffectableController, ParticleEmitterController);

//----------------------------------------------------------------------------
ParticleEmitterController::ParticleEmitterController () :
mParticleArray(0),
mCreatedParticlesQuantity(0),
mNumNewParticlesExcess(0.0f)
{
}
//----------------------------------------------------------------------------
ParticleEmitterController::~ParticleEmitterController ()
{
	if (mParticleArray)
	{
		delete0(mParticleArray);
	}
}
//----------------------------------------------------------------------------
void ParticleEmitterController::Reset ()
{
	EffectableController::Reset();

	mCreatedParticlesQuantity = 0;

	if (mParticleArray)
	{
		mParticleArray->DeleteAll();
	}

	ParticleEmitter *emitter = DynamicCast<ParticleEmitter>(mObject);
	VertexBuffer *vBuffer = emitter->GetVertexBuffer();
	IndexBuffer *iBuffer = emitter->GetIndexBuffer();
	vBuffer->SetNumElements(0);
	iBuffer->SetNumElements(0);
}
//----------------------------------------------------------------------------
void ParticleEmitterController::SetMaxNumParticles (int num)
{
	if (num > 10000)
	{
		PX2_LOG_INFO("ParticleEmitter %s num > 10000", GetName().c_str());
	}

	mMaxNumParticles = num;

	if (mParticleArray)
	{
		delete0(mParticleArray);
		mParticleArray = 0;
	}
	mParticleArray = new0 TRecyclingArray<ParticleObject>(mMaxNumParticles);

	ParticleEmitter *emitter = DynamicCast<ParticleEmitter>(mObject);

	VertexBufferPtr newVBuffer = new0 VertexBuffer(mMaxNumParticles*4, 
		emitter->GetVertexFormat()->GetStride(), Buffer::BU_DYNAMIC);
	VertexBufferAccessor vba(emitter->GetVertexFormat(), newVBuffer);
	assertion(vba.HasTCoord(0),	"Texture coordinates must exist and use channel 0\n");
	int i, j;
	for (i = 0, j = 0; i < mMaxNumParticles; ++i)
	{
		vba.Position<Float3>(j)	   = Float3(0.0f, 0.0f, 0.0f);
		vba.TCoord<Float2>(0, j++) = Float2(0.0f, 0.0f);
		vba.Position<Float3>(j)	   = Float3(1.0f, 0.0f, 0.0f);
		vba.TCoord<Float2>(0, j++) = Float2(1.0f, 0.0f);
		vba.Position<Float3>(j)	   = Float3(1.0f, 0.0f, 1.0f);
		vba.TCoord<Float2>(0, j++) = Float2(1.0f, 1.0f);
		vba.Position<Float3>(j)	   = Float3(0.0f, 0.0f, 1.0f);
		vba.TCoord<Float2>(0, j++) = Float2(0.0f, 1.0f);
	}
	emitter->SetVertexBuffer(newVBuffer);
	newVBuffer->SetNumElements(0);

	IndexBufferPtr newIBuffer = new0 IndexBuffer(mMaxNumParticles*6, 2, 
		Buffer::BU_STATIC);
	unsigned short iFI, iFIp1, iFIp2, iFIp3;
	unsigned short* indices = (unsigned short*)newIBuffer->GetData();
	for (i = 0; i < mMaxNumParticles; ++i)
	{
		iFI = (unsigned short)(4*i);
		iFIp1 = iFI+1;
		iFIp2 = iFI+2;
		iFIp3 = iFI+3;
		*indices++ = iFI;
		*indices++ = iFIp1;
		*indices++ = iFIp2;
		*indices++ = iFI;
		*indices++ = iFIp2;
		*indices++ = iFIp3;
	}
	emitter->SetIndexBuffer(newIBuffer);
	newIBuffer->SetNumElements(0);

	if (IsPlaying()) ResetPlay();
}
//----------------------------------------------------------------------------
bool ParticleEmitterController::Update(double applicationTime,
	double elapsedTime1)
{
	ParticleEmitter *emitter = DynamicCast<ParticleEmitter>(mObject);

	// module update
	if (!EffectableController::Update(applicationTime, elapsedTime1))
		return false;

	float elapsedTime = (float)elapsedTime1;
	float playedTimeMinusDelay = GetPlayedTimeMinusDelay();

	float emitRate = emitter->GetEmitRate();

	// particle update
	for (int i=0; i<mParticleArray->GetQuantity(); i++)
	{
		if (mParticleArray->IsUsed(i))
		{
			ParticleObject &parObj = mParticleArray->GetAt(i);

			ModulesUpdateEffectObject(&parObj);

			if (!parObj.Update(emitter, elapsedTime))
			{
				mParticleArray->Delete(&parObj);
			}
		}
	}

	// new particles
	if (IsPlaying())
	{
		float emitNum = emitRate * (float)elapsedTime;
		int iNewNum= (int)emitNum;
		mNumNewParticlesExcess += (emitNum - iNewNum);

		if (mNumNewParticlesExcess > 1.0f)
		{
			iNewNum += (int)mNumNewParticlesExcess;
			mNumNewParticlesExcess -= (int)mNumNewParticlesExcess;
		}

		for (int i=0; i<iNewNum && mParticleArray->GetFreeQuantity()>0; i++)
		{
			NewAParticle(playedTimeMinusDelay);
		}
	}

	// generate
	emitter->GenBuffers();

	return true;
}
//----------------------------------------------------------------------------
void ParticleEmitterController::NewAParticle (float ctrlTime)
{
	ModulesUpdateEffectable(ctrlTime);

	ParticleObject *particle = 0;
	particle = mParticleArray->New();

	if (!particle)
		return;

	OnNewAEffectObject(particle);

	ModulesUpdateEffectObject(particle);

	mCreatedParticlesQuantity++;
}
//----------------------------------------------------------------------------
void ParticleEmitterController::OnNewAEffectObject (EffectObject *obj)
{
	EffectableController::OnNewAEffectObject(obj);

	ParticleEmitter *emitter = (ParticleEmitter*)GetControlledable();
	ParticleObject *pObj = (ParticleObject*)obj;

	if (emitter->IsBlast())
	{
		pObj->BlastIndex = mCreatedParticlesQuantity;
	}

	pObj->Pos = emitter->GetPlacerAPoint();
	pObj->PrePos = pObj->Pos;

	pObj->RotAxis = emitter->GetEmitRotateAxis();
	pObj->RotAxisInit = pObj->RotAxis;
	pObj->RotSpeed = emitter->GetEmitRotateSpeed();
	pObj->RotSpeedInit = pObj->RotSpeed;
	pObj->RotDegree = emitter->GetEmitRotateDegree();
	pObj->RotDegreeInit = pObj->RotDegree;

	pObj->Speed = emitter->GetEmitSpeed();
	pObj->SpeedInit = pObj->Speed;

	AVector emitDir;
	ParticleEmitter::EmitDirType dirType = emitter->GetEmitDirType();
	if (ParticleEmitter::EST_NORMAL == dirType)
	{
		emitDir = emitter->GetEmitDir();
	}
	else if (ParticleEmitter::EST_TOATTRACTPOINT == dirType)
	{
		emitDir = emitter->GetEmitAttactPoint() -  pObj->Pos;
	}
	else if (ParticleEmitter::EST_LEVELATTRACTPOINT == dirType)
	{
		emitDir = pObj->Pos - emitter->GetEmitAttactPoint();
	}

	if (emitter->IsEmitDirLocal())
	{
		pObj->SpeedDir = emitter->WorldTransform * emitDir;
	}
	else
	{
		pObj->SpeedDir = emitDir;
	}
	pObj->SpeedDir.Normalize();

	if (emitter->IsBlast())
	{
		APoint pos[10];
		pos[0] = APoint(-1.0f, 0.0f, 1.0f);
		pos[1] = APoint(1.0f, 0.0f, 1.0f);
		pos[2] = APoint(1.0f, 0.0f, 0.0f);

		pos[3] = APoint(1.0f, 0.0f, -1.0f);
		pos[4] = APoint(1.0f, 0.0f, 1.0f);
		pos[5] = APoint(1.0f, 0.0f, -1.0f);
		pos[6] = APoint(0.0f, 0.0f, -1.0f);
		
		pos[7] = APoint(-1.0f, 0.0f, -1.0f);
		pos[8] = APoint(-1.0f, 0.0f, -0.5f);
		pos[9] = APoint(-1.0f, 0.0f, 0.2f);

		if (pObj->BlastIndex > 9)
			pObj->BlastIndex = 9;

		pObj->SpeedDir = emitter->GetEmitDir();
		pObj->SpeedDir.X() *= Mathf::UnitRandom();
		pObj->SpeedDir.Y() *= Mathf::UnitRandom();
		pObj->SpeedDir.Z() *= Mathf::UnitRandom();

		pObj->SpeedDir.Normalize();
	}

	pObj->SpeedDirInit = pObj->SpeedDir;

	pObj->AccelerateDir = emitter->GetEmitAccelerateDir();
	pObj->AccelerateDirInit = pObj->AccelerateDir;
	pObj->Accelerate = emitter->GetEmitAccelerate();
	pObj->AccelerateInit = pObj->Accelerate;
	pObj->Obstruct = emitter->GetObstruct();

	if (!emitter->IsLocal())
	{
		APoint scale = emitter->WorldTransform.GetScale();

		obj->SizeXInit = obj->SizeXInit * scale.X();
		obj->SizeX = obj->SizeXInit;
		obj->SizeYInit = obj->SizeYInit * scale.Y();
		obj->SizeY = obj->SizeYInit;
		obj->SizeZInit = obj->SizeZInit * scale.Z();
		obj->SizeZ = obj->SizeZInit;
	}

	pObj->Update(emitter, 0.0f);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
ParticleEmitterController::ParticleEmitterController (LoadConstructor value) :
EffectableController(value),
mParticleArray(0),
mMaxNumParticles(0),
mCreatedParticlesQuantity(0),
mNumNewParticlesExcess(0.0f)
{
}
//----------------------------------------------------------------------------
void ParticleEmitterController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	EffectableController::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mMaxNumParticles);

	PX2_END_DEBUG_STREAM_LOAD(ParticleEmitterController, source);
}
//----------------------------------------------------------------------------
void ParticleEmitterController::Link (InStream& source)
{
	EffectableController::Link(source);

	mParticleArray = new0 TRecyclingArray<ParticleObject>(mMaxNumParticles);
}
//----------------------------------------------------------------------------
void ParticleEmitterController::PostLink ()
{
	EffectableController::PostLink();
}
//----------------------------------------------------------------------------
bool ParticleEmitterController::Register (OutStream& target) const
{
	if (EffectableController::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void ParticleEmitterController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	EffectableController::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mMaxNumParticles);

	PX2_END_DEBUG_STREAM_SAVE(ParticleEmitterController, target);
}
//----------------------------------------------------------------------------
int ParticleEmitterController::GetStreamingSize (Stream &stream) const
{
	int size = EffectableController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += sizeof(mMaxNumParticles);

	return size;
}
//----------------------------------------------------------------------------
