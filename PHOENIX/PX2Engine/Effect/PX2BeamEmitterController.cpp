// PX2BeamEmitterController.cpp

#include "PX2BeamEmitterController.hpp"
#include "PX2BeamEmitter.hpp"
#include "PX2Renderer.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, EffectableController, BeamEmitterController);
PX2_IMPLEMENT_STREAM(BeamEmitterController);
PX2_IMPLEMENT_FACTORY(BeamEmitterController);
PX2_IMPLEMENT_DEFAULT_NAMES(EffectableController, BeamEmitterController);

//----------------------------------------------------------------------------
BeamEmitterController::BeamEmitterController () :
mNumNewBeamExcess(0)
{
}
//----------------------------------------------------------------------------
BeamEmitterController::~BeamEmitterController ()
{
}
//----------------------------------------------------------------------------
void BeamEmitterController::Reset ()
{
	EffectableController::Reset();
	
	mBeamObjects.clear();
}
//----------------------------------------------------------------------------
bool BeamEmitterController::Update(double applicationTime, 
	double elapsedTime1)
{
	BeamEmitter *emitter = DynamicCast<BeamEmitter>(mObject);

	// module update
	if (!EffectableController::Update(applicationTime, elapsedTime1))
		return false;

	float elapsedTime = (float)elapsedTime1;

	float emitRate = emitter->GetEmitRate();
	int numMaxBeams = emitter->GetNumMaxBeams();

	std::vector<BeamObject>::iterator it=mBeamObjects.begin();
	for (; it!=mBeamObjects.end();)
	{
		BeamObject &beam = *it;

		if (!beam.Update(emitter, elapsedTime))
		{
			it = mBeamObjects.erase(it);
		}
		else
		{
			ModulesUpdateEffectObject(&beam);
			it++;
		}
	}

	if (IsPlaying())
	{
		float emitNum = emitRate * (float)elapsedTime;
		int iNewNum= (int)emitNum;
		mNumNewBeamExcess += (emitNum - iNewNum);

		if (mNumNewBeamExcess > 1.0f)
		{
			iNewNum += (int)mNumNewBeamExcess;
			mNumNewBeamExcess -= (int)mNumNewBeamExcess;
		}

		int numBeams = (int)mBeamObjects.size();
		while (iNewNum>0 && numBeams<numMaxBeams)
		{
			EmitABeam(mPlayedTime);
			numBeams = (int)mBeamObjects.size();
			iNewNum--;
		}
	}
	else
	{
		if (emitter->IsSimpleLineWave())
		{
			for (int i=0; i<(int)mBeamObjects.size(); i++)
			{
				BeamObject &beamObj = mBeamObjects[i];

				beamObj.Alpha -= emitter->GetSimpleLineDoAlphaDisAfterStopSpeed()*elapsedTime;

				if (beamObj.Alpha < 0)
					beamObj.Alpha = 0.0f;
			}
		}
	}

	if (emitter->IsSimpleLineWave())
	{
		for (int i=0; i<(int)mBeamObjects.size(); i++)
		{
			BeamObject &beamObj = mBeamObjects[i];

			APoint emitStartPos = emitter->GetEmitStartPos();
			const APoint &emitEndPos = emitter->GetEmitEndPos();

			if (emitter->IsStartPosUseLocal() && !emitter->IsLocal())
			{
				emitStartPos = emitter->WorldTransform * emitStartPos;
			}

			beamObj.StartPos = emitStartPos;
			beamObj.EndPos = emitEndPos;
			AVector seVec = emitEndPos - emitStartPos;

			int numPoints = (int)beamObj.Points.size();
			for (int j=0; j<numPoints; j++)
			{
				beamObj.Points[j] = beamObj.StartPos + 
					seVec*(float)j/(float)(numPoints-1);
			}
		}
	}

	emitter->GenBuffers();

	return true;
}
//----------------------------------------------------------------------------
void BeamEmitterController::GernerateLFPoints (std::vector<float>& points, 
	int frequency, int type, float param0, float param1)
{
	if (BeamEmitter::WT_RANDOM == (BeamEmitter::WaveType)type)
	{
		float rangeMin = param0;
		float rangeMax = param1;

		for (int i=0; i<frequency; i++)
		{
			if ((0==i) || (frequency-1)==i)
			{
				points.push_back(0.0f);
			}
			else
			{
				points.push_back(Mathf::IntervalRandom(rangeMin, rangeMax));
			}
		}
	}
	else if (BeamEmitter::WT_LINE == (BeamEmitter::WaveType)type)
	{
		for (int i=0; i<frequency; i++)
		{
			points.push_back(0.0f);
		}
	}
}
//----------------------------------------------------------------------------
void BeamEmitterController::GernerateLFPointsSin (std::vector<float>& points,
	int frequency, float param0, float param1, float param2)
{
	float range = param0;
	float offset = param1;
	float cycles = param2;

	for (int i=0; i<frequency; i++)
	{
		float var = (cycles*360.0f) * (float)i/(float)frequency;
		float value = range * Mathf::Sin(var + offset);
		points.push_back(value);
	}
}
//----------------------------------------------------------------------------
void BeamEmitterController::EmitABeam (float ctrlTime)
{
	BeamEmitter *emitter = (BeamEmitter*)mObject;
	ModulesUpdateEffectable(ctrlTime);

	Camera *cam = PX2_GR.GetCurUpdateCamera();
	if (!cam)
		return;

	const AVector &camD = cam->GetDVector();

	Effectable::FaceType faceType = emitter->GetFaceType();
	BeamEmitter::WaveType waveTypeUp = emitter->GetWaveTypeUp();
	BeamEmitter::WaveType waveTypeExtend = emitter->GetWaveTypeExtend();
	int numLowFre = emitter->GetNumLowFrequency();
	int numHighFre = emitter->GetNumHighFrequency();
	const Float2 &lowFreRangeUp = emitter->GetLowFrequencyRangeUp();
	const Float2 &lowFreRangeExtend = emitter->GetLowFrequencyRangeExtend();
	const Float2 &hightFreRangeUp = emitter->GetHighFrequencyRangeUp();
	const Float2 &hightFreRangeExtend = emitter->GetHighFrequencyRangeExtend();
	APoint emitStartPos = emitter->GetEmitStartPos();
	const APoint &emitEndPos = emitter->GetEmitEndPos();

	if (emitter->IsStartPosUseLocal() && !emitter->IsLocal())
	{
		emitStartPos = emitter->WorldTransform * emitStartPos;
	}

	BeamObject obj;
	obj.StartPos = emitStartPos;
	obj.EndPos = emitEndPos;

	AVector dirVec = emitEndPos - emitStartPos;
	if (dirVec == AVector::ZERO)
	{
		dirVec.X() = 0.01f;
	}
	AVector dir = dirVec;
	dir.Normalize();

	AVector toMeDir;
	if (Effectable::FT_X == faceType)
	{
		toMeDir = AVector::UNIT_X;
	}
	else if (Effectable::FT_NX == faceType)
	{
		toMeDir = -AVector::UNIT_X;
	}
	else if (Effectable::FT_Y == faceType)
	{
		toMeDir = AVector::UNIT_Y;
	}
	else if (Effectable::FT_NY == faceType)
	{
		toMeDir = -AVector::UNIT_Y;
	}
	else if (Effectable::FT_Z == faceType)
	{
		toMeDir = AVector::UNIT_Z;
	}
	else if (Effectable::FT_NZ == faceType)
	{
		toMeDir = -AVector::UNIT_Z;
	}
	else if (Effectable::FT_CAMERA == faceType || 
		Effectable::FT_SPEEDDIR==faceType || 
		Effectable::FT_FREE==faceType)
	{
		toMeDir = -camD;
	}

	AVector upDir = toMeDir.Cross(dir);
	upDir.Normalize();
	AVector toMeExtendDir = dir.Cross(upDir);
	toMeExtendDir.Normalize();

	int numLowFreNumPoints = numLowFre+1;

	std::vector<float> upLFs, toMeLFs;

	if (BeamEmitter::WT_RANDOM==waveTypeUp || BeamEmitter::WT_LINE==waveTypeUp)
	{
		GernerateLFPoints(upLFs, numLowFreNumPoints, waveTypeUp, lowFreRangeUp[0], lowFreRangeUp[1]);
	}
	else
	{
	}

	if (BeamEmitter::WT_RANDOM==waveTypeExtend || BeamEmitter::WT_LINE==waveTypeExtend)
	{
		GernerateLFPoints(toMeLFs, numLowFreNumPoints, waveTypeExtend, lowFreRangeExtend[0], lowFreRangeExtend[1]);
	}
	else
	{
	}

	APoint lastPoint;
	for (int i=0; i<numLowFreNumPoints; i++)
	{
		APoint lPos = emitStartPos + dirVec * (i/(float)(numLowFreNumPoints-1));
		APoint lPoint = lPos + upDir*upLFs[i] + toMeExtendDir * toMeLFs[i];

		if (0 != i)
		{
			for (int k=1; k<numHighFre; k++)
			{
				AVector hVec = lPoint - lastPoint;
				APoint hPoint = lastPoint + hVec * (k/(float)numHighFre);
				hPoint += upDir * Mathf::IntervalRandom(hightFreRangeUp[0], hightFreRangeUp[1]);
				hPoint += toMeExtendDir * Mathf::IntervalRandom(hightFreRangeExtend[0], hightFreRangeExtend[1]);

				obj.Points.push_back(hPoint);
			}
		}

		obj.Points.push_back(lPoint);

		lastPoint = lPoint;
	}

	OnNewAEffectObject(&obj);

	ModulesUpdateEffectObject(&obj);

	mBeamObjects.push_back(obj);
}
//----------------------------------------------------------------------------
void BeamEmitterController::OnNewAEffectObject (EffectObject *obj)
{
	EffectableController::OnNewAEffectObject(obj);

	BeamEmitter *emitter = (BeamEmitter*)GetControlledable();
	BeamObject *bObj = (BeamObject*)obj;

	bObj->Update(emitter, 0.0f);
}
//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
BeamEmitterController::BeamEmitterController (LoadConstructor value)
	:
EffectableController(value),
mNumNewBeamExcess(0)
{
}
//----------------------------------------------------------------------------
void BeamEmitterController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	EffectableController::Load(source);
	PX2_VERSION_LOAD(source);

	PX2_END_DEBUG_STREAM_LOAD(BeamEmitterController, source);
}
//----------------------------------------------------------------------------
void BeamEmitterController::Link (InStream& source)
{
	EffectableController::Link(source);
}
//----------------------------------------------------------------------------
void BeamEmitterController::PostLink ()
{
	EffectableController::PostLink();
}
//----------------------------------------------------------------------------
bool BeamEmitterController::Register (OutStream& target) const
{
	if (EffectableController::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void BeamEmitterController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	EffectableController::Save(target);
	PX2_VERSION_SAVE(target);

	PX2_END_DEBUG_STREAM_SAVE(BeamEmitterController, target);
}
//----------------------------------------------------------------------------
int BeamEmitterController::GetStreamingSize (Stream &stream) const
{
	int size = EffectableController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	return size;
}
//----------------------------------------------------------------------------
