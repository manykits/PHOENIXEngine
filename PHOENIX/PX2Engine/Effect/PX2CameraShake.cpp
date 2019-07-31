// PX2CameraShake.cpp

#include "PX2CameraShake.hpp"
#include "PX2CameraShakeController.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Effectable, CameraShake);
PX2_IMPLEMENT_STREAM(CameraShake);
PX2_IMPLEMENT_FACTORY(CameraShake);
PX2_IMPLEMENT_DEFAULT_NAMES(Effectable, CameraShake);

//----------------------------------------------------------------------------
CameraShakeChannel::CameraShakeChannel () :
UseX(true),
UseY(true),
UseZ(true),
Strength(-1.0f),
MaxDistance(100.0)
{
}
//----------------------------------------------------------------------------
CameraShakeChannel::~CameraShakeChannel ()
{
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
CameraShakeChannel CameraShake::ShakeChannels[NUMSHAKEMAXCHANNELS];
std::vector<std::string> CameraShake::msModuleNames_EA;
std::vector<std::string> CameraShake::msModuleNames_EO;
//----------------------------------------------------------------------------
CameraShake::CameraShake ()
	:
mIsUseX(true),
mIsUseY(true),
mIsUseZ(true),
mMaxDistance(10.0f),
mEmitStrength(0.1f),
mShakeChannel(-1)
{
	SetName("CameraShake");

	mEffectableCtrl = new0 CameraShakeController();
	mEffectableCtrl->SetName("CameraShakeController");
	AttachController(mEffectableCtrl);
}
//----------------------------------------------------------------------------
CameraShake::~CameraShake ()
{
	if (mShakeChannel >= 0)
	{
		FreeChannel(mShakeChannel);
	}
}
//----------------------------------------------------------------------------
void CameraShake::Play ()
{
	Effectable::Play();

	if (mShakeChannel >= 0)
	{
		FreeChannel(mShakeChannel);
	}

	mShakeChannel = CameraShake::AllocChannel(mMaxDistance, mIsUseX, mIsUseY, 
		mIsUseZ);
}
//----------------------------------------------------------------------------
void CameraShake::Pause()
{
	Effectable::Pause();

	if (mShakeChannel >= 0)
	{
		FreeChannel(mShakeChannel);
		mShakeChannel = -1;
	}
}
//----------------------------------------------------------------------------
void CameraShake::SetMaxDistance (float dist)
{
	mMaxDistance = dist;
}
//----------------------------------------------------------------------------
void CameraShake::SetEmitStrength (float strength)
{
	mEmitStrength = strength;
}
//----------------------------------------------------------------------------
int CameraShake::AllocChannel (float maxDist,  bool useX, bool useY, bool useZ)
{
	for(int i=0; i<NUMSHAKEMAXCHANNELS; i++)
	{
		if (ShakeChannels[i].Strength < 0.0f)
		{
			ShakeChannels[i].UseX = useX;
			ShakeChannels[i].UseY = useY;
			ShakeChannels[i].UseZ = useZ;
			ShakeChannels[i].Strength = 0.0f;
			ShakeChannels[i].MaxDistance = maxDist;

			return i;
		}
	}
	return -1;
}
//----------------------------------------------------------------------------
void CameraShake::FreeChannel (int channel)
{
	if (channel>=0 && channel<NUMSHAKEMAXCHANNELS)
	{
		ShakeChannels[channel].Strength = -1.0f;
	}
	else
	{
		assertion(false, "channel should be in right range.\n");
	}
}
//----------------------------------------------------------------------------
void CameraShake::SetChannelShake (int channel, float strength, 
	const APoint &shakeObjPos)
{
	if (channel>=0 && channel<NUMSHAKEMAXCHANNELS)
	{
		ShakeChannels[channel].Strength = strength;
		ShakeChannels[channel].Center = shakeObjPos;
	}
	else
	{
		assertion(false, "channel should be in right range.\n");
	}
}
//----------------------------------------------------------------------------
float CameraShake::ShakeCamera (const APoint &recvCenter)
{
	float strengthMax = 0.0f;

	for (int i=0; i<CameraShake::NUMSHAKEMAXCHANNELS; i++)
	{
		CameraShakeChannel &channel = CameraShake::ShakeChannels[i];
		if (channel.Strength < 0.0f)
			continue;

		AVector dir = recvCenter - channel.Center;

		float allLengthSquare = 0.0f;

		if (channel.UseX)
			allLengthSquare += dir.X() * dir.X();

		if (channel.UseY)
			allLengthSquare += dir.Y() * dir.Y();

		if (channel.UseZ)
			allLengthSquare += dir.Z() * dir.Z();

		float allLength = Mathf::Sqrt(allLengthSquare);

		float s = (channel.MaxDistance-allLength)/channel.MaxDistance 
			* channel.Strength;

		if (s > strengthMax)
			strengthMax = s;
	}

	return strengthMax;
}
//----------------------------------------------------------------------------
const std::vector<std::string> &CameraShake::GetAllModuleNames_EA () const
{
	return msModuleNames_EA;
}
//----------------------------------------------------------------------------
const std::vector<std::string> &CameraShake::GetAllModuleNames_EO () const
{
	return msModuleNames_EO;
}
//----------------------------------------------------------------------------
void CameraShake::GenBuffers ()
{
	Effectable::GenBuffers();

	CameraShakeController *csCtrl = (CameraShakeController*)
		GetEffectableController();

	const APoint &screenPos = WorldTransform.GetTranslate();

	if (csCtrl && mShakeChannel>=0)
	{
		const CameraShakeObject *csObject = csCtrl->GetCameraShakeObject();
		if (csObject)
		{
			SetChannelShake(mShakeChannel, csObject->Strength, screenPos);
		}
	}
}
//----------------------------------------------------------------------------
 void CameraShake::OnGetVisibleSet (Culler&, bool)
 {
	 /* Nothing to do. */
 }
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void CameraShake::RegistProperties ()
{
	Effectable::RegistProperties();

	AddPropertyClass("CameraShake");

	AddProperty("IsUseX", PT_BOOL, IsUseX());
	AddProperty("IsUseY", PT_BOOL, IsUseY());
	AddProperty("IsUseZ", PT_BOOL, IsUseZ());

	AddProperty("MaxDistance", PT_FLOAT, GetMaxDistance());
	AddProperty("EmitStrength", PT_FLOAT, GetEmitStrength());

	AddProperty("ShakeChannel", PT_INT, GetShakeChannel(), false);
}
//----------------------------------------------------------------------------
void CameraShake::OnPropertyChanged (const PropertyObject &obj)
{
	Effectable::OnPropertyChanged(obj);

	if ("IsUseX" == obj.Name)
	{
		SetUseX(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("IsUseY" == obj.Name)
	{
		SetUseY(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("IsUseZ" == obj.Name)
	{
		SetUseZ(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("MaxDistance" == obj.Name)
	{
		SetMaxDistance(PX2_ANY_AS(obj.Data, float));
	}
	else if ("EmitStrength" == obj.Name)
	{
		SetEmitStrength(PX2_ANY_AS(obj.Data, float));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
CameraShake::CameraShake (LoadConstructor value)
	:
Effectable(value),
mIsUseX(true),
mIsUseY(true),
mIsUseZ(true),
mMaxDistance(10.0f),
mEmitStrength(0.1f),
mShakeChannel(-1)
{
}
//----------------------------------------------------------------------------
void CameraShake::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Effectable::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadBool(mIsUseX);
	source.ReadBool(mIsUseY);
	source.ReadBool(mIsUseZ);
	source.Read(mMaxDistance);
	source.Read(mEmitStrength);

	PX2_END_DEBUG_STREAM_LOAD(CameraShake, source);
}
//----------------------------------------------------------------------------
void CameraShake::Link (InStream& source)
{
	Effectable::Link(source);
}
//----------------------------------------------------------------------------
void CameraShake::PostLink ()
{
	Effectable::PostLink();
}
//----------------------------------------------------------------------------
bool CameraShake::Register (OutStream& target) const
{
	if (Effectable::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void CameraShake::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Effectable::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteBool(mIsUseX);
	target.WriteBool(mIsUseY);
	target.WriteBool(mIsUseZ);
	target.Write(mMaxDistance);
	target.Write(mEmitStrength);

	PX2_END_DEBUG_STREAM_SAVE(CameraShake, target);
}
//----------------------------------------------------------------------------
int CameraShake::GetStreamingSize (Stream &stream) const
{
	int size = Effectable::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_BOOLSIZE(mIsUseX);
	size += PX2_BOOLSIZE(mIsUseY);
	size += PX2_BOOLSIZE(mIsUseZ);
	size += sizeof(mMaxDistance);
	size += sizeof(mEmitStrength);

	return size;
}
//----------------------------------------------------------------------------