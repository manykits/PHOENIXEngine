// PX2BeamEmitter.cpp

#include "PX2BeamEmitter.hpp"
#include "PX2BeamEmitterController.hpp"
#include "PX2Renderer.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Effectable, BeamEmitter);
PX2_IMPLEMENT_STREAM(BeamEmitter);
PX2_IMPLEMENT_FACTORY(BeamEmitter);
PX2_IMPLEMENT_DEFAULT_NAMES(Effectable, BeamEmitter);

//----------------------------------------------------------------------------
std::vector<std::string> BeamEmitter::msModuleNames_EA;
std::vector<std::string> BeamEmitter::msModuleNames_EO;
//----------------------------------------------------------------------------
BeamEmitter::BeamEmitter () :
mNumMaxBeams(10),
mEmitRate(10),
mIsStartUseLocal(true),
mIsSimpleLineWave(false),
mDoAlphaDisAfterStopSpeed(1.0f),
mWaveTypeUp(WT_RANDOM),
mWaveTypeExtend(WT_RANDOM),
mNumLowFrequency(5),
mNumHighFrequency(3)
{
	SetName("BeamEmitter");

	SetTex("Data/engine/default.png");

	mEmitEndPos = APoint(10.0f, 0.0f, 0.0f);
	mLowRangeUp = Float2(-1.0f, 1.0f);
	mLowRangeExtend = Float2::ZERO;
	mHighRangeUp =  Float2(-0.5f, 0.5f);
	mHighRangeExtend = Float2::ZERO;

	UpdateMaxNumVertexs();
	mEffectableCtrl = new0 BeamEmitterController();
	mEffectableCtrl->SetName("BeamEmitterController");
	AttachController(mEffectableCtrl);
}
//----------------------------------------------------------------------------
BeamEmitter::~BeamEmitter ()
{
}
//----------------------------------------------------------------------------
void BeamEmitter::SetNumMaxBeams (int num)
{
	mNumMaxBeams = num;
	UpdateMaxNumVertexs();

	ResetPlay();
}
//----------------------------------------------------------------------------
void BeamEmitter::SetNumLowFrequency (int numLow)
{
	if (numLow <= 0)
		numLow = 1;

	mNumLowFrequency = numLow;
	UpdateMaxNumVertexs();

	ResetPlay();
}
//----------------------------------------------------------------------------
void BeamEmitter::SetNumHighFrequency (int numHigh)
{
	if (numHigh <= 0)
		numHigh = 1;

	mNumHighFrequency = numHigh;
	UpdateMaxNumVertexs();

	ResetPlay();
}
//----------------------------------------------------------------------------
void BeamEmitter::RegistProperties ()
{
	Effectable::RegistProperties();

	AddPropertyClass("BeamEmitter");

	AddProperty("NumMaxBeams", PT_INT, GetNumMaxBeams());
	AddProperty("EmitRate", PT_FLOAT, GetEmitRate());
	AddProperty("EmitStartPos", PT_APOINT3, GetEmitStartPos());
	AddProperty("EmitEndPos", PT_APOINT3, GetEmitEndPos());
	AddProperty("IsStartPosUseLocal", PT_BOOL, IsStartPosUseLocal());
	AddProperty("IsUseSimpleLine", PT_BOOL, IsSimpleLineWave());
	AddProperty("SimpleLineDoAlphaDisAfterStopSpeed", PT_FLOAT, 
		GetSimpleLineDoAlphaDisAfterStopSpeed());

	std::vector<std::string> waveTypes;
	waveTypes.push_back("WT_RANDOM");
	waveTypes.push_back("WT_SIN");
	waveTypes.push_back("WT_LINE");
	AddPropertyEnum("WaveTypeUp", (int)GetWaveTypeUp(), waveTypes);
	AddPropertyEnum("WaveTypeExtend", (int)GetWaveTypeExtend(), waveTypes);
	AddProperty("NumLowFrequency", PT_INT, GetNumLowFrequency());
	AddProperty("NumHighFrequency", PT_INT, GetNumHighFrequency());
	AddProperty("LowFrequencyRangeUp", PT_FLOAT2, GetLowFrequencyRangeUp());
	AddProperty("LowFrequencyRangeExtend", PT_FLOAT2, GetLowFrequencyRangeExtend());

	AddProperty("HighFrequencyRangeUp", PT_FLOAT2, GetHighFrequencyRangeUp());
	AddProperty("HighFrequencyRangeExtend", PT_FLOAT2, GetHighFrequencyRangeExtend());
}
//----------------------------------------------------------------------------
void BeamEmitter::OnPropertyChanged (const PropertyObject &obj)
{
	Effectable::OnPropertyChanged(obj);

	if ("NumMaxBeams" == obj.Name)
	{
		SetNumMaxBeams(PX2_ANY_AS(obj.Data, int));
	}
	else if ("EmitRate" == obj.Name)
	{
		SetEmitRate(PX2_ANY_AS(obj.Data, float));
	}
	else if ("EmitStartPos" == obj.Name)
	{
		SetEmitStartPos(PX2_ANY_AS(obj.Data, APoint));
	}
	else if ("EmitEndPos" == obj.Name)
	{
		SetEmitEndPos(PX2_ANY_AS(obj.Data, APoint));
	}
	else if ("IsStartPosUseLocal" == obj.Name)
	{
		SetStartPosUseLocal(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("IsUseSimpleLine" == obj.Name)
	{
		SetSimpleLineWave(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("SimpleLineDoAlphaDisAfterStopSpeed" == obj.Name)
	{
		SetSimpleLineDoAlphaDisAfterStopSpeed(PX2_ANY_AS(obj.Data, float));
	}
	else if ("WaveTypeUp" == obj.Name)
	{
		SetWaveTypeUp((WaveType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("WaveTypeExtend" == obj.Name)
	{
		SetWaveTypeExtend((WaveType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("NumLowFrequency" == obj.Name)
	{
		SetNumLowFrequency(PX2_ANY_AS(obj.Data, int));
	}
	else if ("NumHighFrequency" == obj.Name)
	{
		SetNumHighFrequency(PX2_ANY_AS(obj.Data, int));
	}
	else if ("LowFrequencyRangeUp" == obj.Name)
	{
		SetLowFrequencyRangeUp(PX2_ANY_AS(obj.Data, Float2));
	}
	else if ("LowFrequencyRangeExtend" == obj.Name)
	{
		SetLowFrequencyRangeExtend(PX2_ANY_AS(obj.Data, Float2));
	}
	else if ("HighFrequencyRangeUp" == obj.Name)
	{
		SetHighFrequencyRangeUp(PX2_ANY_AS(obj.Data, Float2));
	}
	else if ("HighFrequencyRangeExtend" == obj.Name)
	{
		SetHighFrequencyRangeExtend(PX2_ANY_AS(obj.Data, Float2));
	}
}
//----------------------------------------------------------------------------
void BeamEmitter::UpdateMaxNumVertexs ()
{
	int numMaxVertex = (((mNumLowFrequency+1)*2) +
		(mNumHighFrequency-1)*mNumLowFrequency*2)*mNumMaxBeams;

	int numMaxIndex = mNumLowFrequency*mNumHighFrequency*6*
		mNumMaxBeams;

	VertexBufferPtr newVBuffer = new0 VertexBuffer(numMaxVertex, 
		GetVertexFormat()->GetStride(), Buffer::BU_DYNAMIC);
	SetVertexBuffer(newVBuffer);
	newVBuffer->SetNumElements(0);

	IndexBufferPtr newIBuffer = new0 IndexBuffer(numMaxIndex, 2,
		Buffer::BU_DYNAMIC);
	SetIndexBuffer(newIBuffer);
	newIBuffer->SetNumElements(0);
}
//----------------------------------------------------------------------------
const std::vector<std::string> &BeamEmitter::GetAllModuleNames_EA () const
{
	return msModuleNames_EA;
}
//----------------------------------------------------------------------------
const std::vector<std::string> &BeamEmitter::GetAllModuleNames_EO () const
{
	return msModuleNames_EO;
}
//----------------------------------------------------------------------------
void BeamEmitter::GenBuffers ()
{
	Effectable::GenBuffers();

	BeamEmitterController *beCtrl = (BeamEmitterController*)GetEffectableController();
	VertexFormat *vFormat = GetVertexFormat();
	VertexBuffer *vBuffer = GetVertexBuffer();
	IndexBuffer *iBuffer = GetIndexBuffer();

	int numVertex = 0;
	int numIndex = 0;
	vBuffer->SetNumElements(0);
	iBuffer->SetNumElements(0);

	const std::vector<BeamObject> &vec = beCtrl->GetBeamObjects();

	std::vector<BeamObject>::const_iterator it = vec.begin();
	for (; it!=vec.end(); it++)
	{
		if ((int)it->Points.size() > 0)
		{
			numVertex += (int)it->Points.size() * 2;
			numIndex += ((int)it->Points.size()-1) * 6;
		}
	}

	VertexBufferAccessor vba(vFormat, vBuffer);
	assertion(vba.HasPosition(), "Positions must exist\n");

	int curNumVB = 0;
	int curNumIB = 0;
	it = vec.begin();
	for (; it!=vec.end(); it++)
	{
		if ((int)it->Points.size() > 0)
		{
			const BeamObject &beam = *it;

			unsigned short *ib = (unsigned short*)iBuffer->GetData();
			FillBeamVert(vba, ib+curNumIB, curNumVB, beam);

			curNumVB += (int)beam.Points.size() * 2;
			curNumIB += ((int)beam.Points.size() - 1) * 6;
		}
	}

	vBuffer->SetNumElements(curNumVB);
	iBuffer->SetNumElements(numIndex);

	if (!IsFixedBound())
	{
		UpdateModelSpace(Renderable::GU_MODEL_BOUND_ONLY);
	}

	Renderer::UpdateAll(vBuffer);
	Renderer::UpdateAll(iBuffer);
}
//----------------------------------------------------------------------------
void BeamEmitter::FillBeamVert (VertexBufferAccessor &vba, 
	unsigned short *ib, int startVB, const BeamObject &beamObj)
{
	Camera *cam = PX2_GR.GetCurUpdateCamera();
	if (!cam)
		return;

	AVector camD = cam->GetDVector();
	AVector camUp = cam->GetUVector();

	FaceType faceType = GetFaceType();

	AVector dirVec = beamObj.EndPos - beamObj.StartPos;
	if (dirVec == AVector::ZERO)
	{
		dirVec = AVector::UNIT_X;
	}
	AVector dir = dirVec;
	dir.Normalize();

	AVector toMeDir;
	AVector upDir;
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

	upDir = toMeDir.Cross(dir);
	upDir.Normalize();

	float uBegin = beamObj.UV0Begin[0];
	float uEnd =  beamObj.UV0End[0];
	float vBegin =  beamObj.UV0Begin[1];
	float vEnd =  beamObj.UV0End[1];

	int index = 0;
	for (int i=0; i<(int)beamObj.Points.size(); i++)
	{
		const APoint &point = beamObj.Points[i];

		Float2 uv0;
		Float2 uv1;

		GetUV(beamObj.StartRandomIndex, beamObj.Age, uBegin, uEnd, vBegin, vEnd);
		float uRange = uEnd - uBegin;
		
		float baseOver = (float)((int)beamObj.Points.size() - 1);
		if (baseOver <= 0.0f)
			baseOver = 1.0f;
		float u = uBegin + uRange*(float)i/baseOver;
	
		uv0 = Float2(u, vBegin);
		uv1 = Float2(u, vEnd);

		APoint pos0 = point - upDir*beamObj.SizeX;
		vba.Position<Float3>(startVB+2*i) = pos0;
		vba.Color<Float4>(0, startVB+2*i) = Float4(beamObj.Color[0],
			beamObj.Color[1], beamObj.Color[2], beamObj.Alpha);
		vba.TCoord<Float2>(0, startVB+2*i) = uv0;

		APoint pos1 = point + upDir*beamObj.SizeX;
		vba.Position<Float3>(startVB+2*i+1) = pos1;
		vba.Color<Float4>(0, startVB+2*i+1) = Float4(beamObj.Color[0], 
			beamObj.Color[1], beamObj.Color[2], beamObj.Alpha);
		vba.TCoord<Float2>(0, startVB+2*i+1) = uv1;

		if (i >= 1)
		{
			ib[index++] = (unsigned short)(startVB + (i-1) * 2);
			ib[index++] = (unsigned short)(startVB + (i  ) * 2 + 1);
			ib[index++] = (unsigned short)(startVB + (i-1) * 2 + 1);

			ib[index++] = (unsigned short)(startVB + (i-1) * 2);
			ib[index++] = (unsigned short)(startVB + (i  ) * 2);
			ib[index++] = (unsigned short)(startVB + (i  ) * 2 + 1);
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
BeamEmitter::BeamEmitter (LoadConstructor value)
	:
Effectable(value),
	mNumMaxBeams(10),
	mEmitRate(10),
	mIsStartUseLocal(true),
	mIsSimpleLineWave(false),
	mDoAlphaDisAfterStopSpeed(1.0f),
	mWaveTypeUp(WT_RANDOM),
	mWaveTypeExtend(WT_RANDOM),
	mNumLowFrequency(5),
	mNumHighFrequency(3)
{
	mEmitEndPos = APoint(10.0f, 0.0f, 0.0f);
	mLowRangeUp = Float2(-1.0f, 1.0f);
	mLowRangeExtend = Float2::ZERO;
	mHighRangeUp =  Float2(-0.5f, 0.5f);
	mHighRangeExtend = Float2::ZERO;
}
//----------------------------------------------------------------------------
void BeamEmitter::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Effectable::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mNumMaxBeams);
	source.Read(mEmitRate);

	source.ReadAggregate(mEmitStartPos);
	source.ReadAggregate(mEmitEndPos);

	source.ReadBool(mIsSimpleLineWave);

	source.Read(mDoAlphaDisAfterStopSpeed);

	source.ReadEnum(mWaveTypeUp);
	source.ReadEnum(mWaveTypeExtend);

	source.Read(mNumLowFrequency);
	source.Read(mNumHighFrequency);

	source.ReadAggregate(mLowRangeUp);
	source.ReadAggregate(mLowRangeExtend);
	source.ReadAggregate(mHighRangeUp);
	source.ReadAggregate(mHighRangeExtend);

	PX2_END_DEBUG_STREAM_LOAD(BeamEmitter, source);
}
//----------------------------------------------------------------------------
void BeamEmitter::Link (InStream& source)
{
	Effectable::Link(source);
}
//----------------------------------------------------------------------------
void BeamEmitter::PostLink ()
{
	Effectable::PostLink();
}
//----------------------------------------------------------------------------
bool BeamEmitter::Register (OutStream& target) const
{
	if (Effectable::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void BeamEmitter::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Effectable::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mNumMaxBeams);
	target.Write(mEmitRate);

	target.WriteAggregate(mEmitStartPos);
	target.WriteAggregate(mEmitEndPos);

	target.WriteBool(mIsSimpleLineWave);
	target.Write(mDoAlphaDisAfterStopSpeed);

	target.WriteEnum(mWaveTypeUp);
	target.WriteEnum(mWaveTypeExtend);

	target.Write(mNumLowFrequency);
	target.Write(mNumHighFrequency);

	target.WriteAggregate(mLowRangeUp);
	target.WriteAggregate(mLowRangeExtend);
	target.WriteAggregate(mHighRangeUp);
	target.WriteAggregate(mHighRangeExtend);

	PX2_END_DEBUG_STREAM_SAVE(BeamEmitter, target);
}
//----------------------------------------------------------------------------
int BeamEmitter::GetStreamingSize (Stream &stream) const
{
	int size = Effectable::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mNumMaxBeams);
	size += sizeof(mEmitRate);

	size += sizeof(mEmitStartPos);
	size += sizeof(mEmitEndPos);

	size += PX2_BOOLSIZE(mIsSimpleLineWave);

	size += sizeof(mDoAlphaDisAfterStopSpeed);

	size += PX2_ENUMSIZE(mWaveTypeUp);
	size += PX2_ENUMSIZE(mWaveTypeExtend);

	size += sizeof(mNumLowFrequency);
	size += sizeof(mNumHighFrequency);

	size += sizeof(mLowRangeUp);
	size += sizeof(mLowRangeExtend);
	size += sizeof(mHighRangeUp);
	size += sizeof(mHighRangeExtend);

	return size;
}
//----------------------------------------------------------------------------
