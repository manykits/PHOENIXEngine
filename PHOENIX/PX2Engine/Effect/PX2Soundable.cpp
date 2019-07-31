// PX2Soundable.cpp

#include "PX2Soundable.hpp"
#include "PX2SoundableController.hpp"
#include "PX2SoundSystem.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Effectable, Soundable);
PX2_IMPLEMENT_STREAM(Soundable);
PX2_IMPLEMENT_FACTORY(Soundable);
PX2_IMPLEMENT_DEFAULT_NAMES(Effectable, Soundable)

//----------------------------------------------------------------------------
std::vector<std::string> Soundable::msModuleNames_EA;
std::vector<std::string> Soundable::msModuleNames_EO;
//----------------------------------------------------------------------------
Soundable::Soundable (const std::string &filename) :
mSourceType(ST_SOUND),
mMusicChannel(1),
mIngoreStopSound(true),
mIs3D(false),
mVolume(1.0f),
mIsLoop(false),
mFilename(filename),
mMinDistance(5.0f),
mMaxDistance(25.0f),
mDistanceUseX(true),
mDistanceUseY(true),
mDistanceUseZ(false)
{
	SetName("Soundable");

	mEffectableCtrl = new0 SoundableController();
	mEffectableCtrl->SetName("SoundableController");
	AttachController(mEffectableCtrl);
}
//----------------------------------------------------------------------------
Soundable::~Soundable ()
{
	Reset ();
}
//----------------------------------------------------------------------------
void Soundable::SetSourceType (SourceType st)
{
	mSourceType = st;
}
//----------------------------------------------------------------------------
void Soundable::Reset ()
{
	Effectable::Reset();

	if (mSound)
	{
		mSound->Stop();
		mSound->MinusNumPlaySameTime();
	}

	mSound = 0;
}
//----------------------------------------------------------------------------
void Soundable::Play ()
{
	Effectable::Play();
}
//----------------------------------------------------------------------------
void Soundable::PlaySound ()
{
	if (mFilename.empty())
		return;

	if (ST_SOUND == mSourceType)
	{
		if (mIs3D)
		{
			SoundCreateInfo3D info;
			info.Position = WorldTransform.GetTranslate();
			info.MinDistance = mMinDistance;
			info.MaxDistance = mMaxDistance;
			info.Volume = mVolume;
			info.Velocity = mVelocity;
			info.IsLoop = mIsLoop;

			Sound *sound = 0;
			SoundSystem::GetSingletonPtr()->PlaySound3DControl(
				mFilename.c_str(), info, sound);
			mSound = sound;
		}
		else
		{
			Sound *sound = 0;
			SoundSystem::GetSingletonPtr()->PlaySound2DControl(
				mFilename.c_str(), mVolume, mIsLoop, sound);
			mSound = sound;
		}
	}
	else
	{
		PX2_SS.PlayMusic(mMusicChannel, mFilename.c_str(), false, 0.0f, mVolume);
	}
}
//----------------------------------------------------------------------------
void Soundable::SetVolume (float volume)
{
	if (mVolume == volume)
		return;

	mVolume = volume;

	if (ST_SOUND == mSourceType)
	{
		if (mSound)
			mSound->SetVolume(volume);
	}
	else
	{
		PX2_SS.SetMusicVolume(1, volume);
	}
}
//----------------------------------------------------------------------------
void Soundable::Set3D (bool is3D)
{
	if (mIs3D == is3D)
		return;

	mIs3D = is3D;

	if (IsPlaying())
	{
		ResetPlay();
	}
}
//----------------------------------------------------------------------------
void Soundable::Pause()
{
	Effectable::Pause();

	if (!mIngoreStopSound)
	{
		if (mSound)
			mSound->Stop();
	}

	if (mSound)
	{
		mSound->MinusNumPlaySameTime();
	}
}
//----------------------------------------------------------------------------
void Soundable::SetVelocity (const AVector &velocity)
{
	mVelocity = velocity;

	if (mSound)
		mSound->SetVelocity(velocity);
}
//----------------------------------------------------------------------------
void Soundable::SetDistance (float minDistance, float maxDistance)
{
	mMinDistance = minDistance;
	mMaxDistance = maxDistance;

	if (mSound)
		mSound->SetDistance(minDistance, maxDistance);
}
//----------------------------------------------------------------------------
void Soundable::SetMinDistance (float minDistance)
{
	mMinDistance = minDistance;

	if (minDistance > mMaxDistance)
		mMaxDistance = minDistance;

	if (mSound)
		mSound->SetDistance(mMinDistance, mMaxDistance);
}
//----------------------------------------------------------------------------
void Soundable::SetMaxDistance (float maxDistance)
{
	if (maxDistance < mMinDistance)
		mMinDistance = maxDistance;

	mMaxDistance = maxDistance;

	if (mSound)
		mSound->SetDistance(mMinDistance, mMaxDistance);
}
//----------------------------------------------------------------------------
void Soundable::SetLoop (bool isLoop)
{
	if (mIsLoop == isLoop)
		return;

	mIsLoop = isLoop;

	if (IsPlaying())
	{
		ResetPlay();
	}
}
//----------------------------------------------------------------------------
void Soundable::SetSoundFilename (const std::string &filename)
{
	if (mFilename == filename)
		return;

	mFilename = filename;

	if (IsPlaying())
	{
		ResetPlay();
	}
}
//----------------------------------------------------------------------------
const std::vector<std::string> &Soundable::GetAllModuleNames_EA () const
{
	return msModuleNames_EA;
}
//----------------------------------------------------------------------------
const std::vector<std::string> &Soundable::GetAllModuleNames_EO () const
{
	return msModuleNames_EO;
}
//----------------------------------------------------------------------------
void Soundable::GenBuffers ()
{
	Effectable::GenBuffers();

	SoundableController *sCtrl = (SoundableController*)
		GetEffectableController();

	if (sCtrl)
	{
		const SoundableObject *sObject = sCtrl->GetSoundableObject();
		if (sObject)
		{
			SetVolume(sObject->Volume);
		}
	}
}
//----------------------------------------------------------------------------
void Soundable::UpdateWorldData(double applicationTime, double elapsedTime)
{
	Movable::UpdateWorldData(applicationTime, elapsedTime);

	if (mSound)
		mSound->SetPosition(WorldTransform.GetTranslate());

	const APoint &listenPos = SoundSystem::GetSingleton().GetListenerPos();

	if (!Is3D())
	{
		AVector dir = WorldTransform.GetTranslate() - listenPos;

		if (!mDistanceUseX)
			dir.X() = 0.0f;

		if (!mDistanceUseY)
			dir.Y() = 0.0f;

		if (!mDistanceUseZ)
			dir.Z() = 0.0f;

		float dist = dir.Length();

		SoundableController *sCtrl = DynamicCast<SoundableController>(GetEffectableController());
		const SoundableObject *sObj = sCtrl->GetSoundableObject();
		if (sObj)
		{
			float curVolume = sObj->Volume;
			float volume = curVolume;

			if (dist < mMinDistance)
			{
				volume = curVolume;
			}
			else if (mMinDistance<=dist && dist<=mMaxDistance)
			{
				float range = mMaxDistance - mMinDistance;
				if (range <= 0.0f)
					range = 0.0f;

				volume = curVolume * (1.0f - (dist - mMinDistance)/range);
			}
			else if (dist > mMaxDistance)
			{
				volume = 0.0f;
			}

			if (mSound)
			{
				mSound->SetVolume(volume);
			}
		}
	}
}
//----------------------------------------------------------------------------
void Soundable::OnGetVisibleSet (Culler&, bool)
{
	/* Nothing to do. */
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void Soundable::RegistProperties ()
{
	Effectable::RegistProperties();

	AddPropertyClass("Soundable");

	std::vector<std::string> sourceTypes;
	sourceTypes.push_back("ST_WAV");
	sourceTypes.push_back("ST_MP3");
	AddPropertyEnum("SourceType", (int)GetSourceType(), sourceTypes);
	AddProperty("MusicChannel", PT_INT, GetMusicChannel());

	AddProperty("IsIngoreStopSound", PT_BOOL, IsIngoreStopSound());
	AddProperty("SoundFilename", PT_STRINGBUTTON, GetSoundFilename());
	AddProperty("Is3D", PT_BOOL, Is3D());
	AddProperty("Volume", PT_FLOAT, GetVolume());
	AddProperty("Velocity", PT_AVECTOR3, GetVelocity());
	AddProperty("MinDistance", PT_FLOAT, GetMinDistance());
	AddProperty("MaxDistance", PT_FLOAT, GetMaxDistance());
	AddProperty("IsLoop", PT_BOOL, IsLoop());
	AddProperty("IsDistanceUseX", PT_BOOL, IsDistanceUseX());
	AddProperty("IsDistanceUseY", PT_BOOL, IsDistanceUseY());
	AddProperty("IsDistanceUseZ", PT_BOOL, IsDistanceUseZ());
}
//----------------------------------------------------------------------------
void Soundable::OnPropertyChanged (const PropertyObject &obj)
{
	Effectable::OnPropertyChanged(obj);

	if ("SourceType" == obj.Name)
	{
		SetSourceType((SourceType)PX2_ANY_AS(obj.Data, int));
	}
	else if ("MusicChannel" == obj.Name)
	{
		int channel = (int)PX2_ANY_AS(obj.Data, int);
		if (channel < 0)
			channel = 0;

		if (channel > 3)
			channel = 3;

		SetMusicChannel(channel);
	}
	else if ("IsIngoreStopSound" == obj.Name)
	{
		SetIngoreStopSound(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("SoundFilename" == obj.Name)
	{
		SetSoundFilename(PX2_ANY_AS(obj.Data, std::string));
	}
	else if ("Is3D" == obj.Name)
	{
		Set3D(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("Volume" == obj.Name)
	{
		SetVolume(PX2_ANY_AS(obj.Data, float));
	}
	else if ("Velocity" == obj.Name)
	{
		SetVelocity(PX2_ANY_AS(obj.Data, AVector));
	}
	else if ("MinDistance" == obj.Name)
	{
		SetMinDistance(PX2_ANY_AS(obj.Data, float));
	}
	else if ("MaxDistance" == obj.Name)
	{
		SetMaxDistance(PX2_ANY_AS(obj.Data, float));
	}
	else if ("IsLoop" == obj.Name)
	{
		SetLoop(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("IsDistanceUseX" == obj.Name)
	{
		SetDistanceUseX(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("IsDistanceUseY" == obj.Name)
	{
		SetDistanceUseY(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("IsDistanceUseZ" == obj.Name)
	{
		SetDistanceUseZ(PX2_ANY_AS(obj.Data, bool));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Soundable::Soundable(LoadConstructor value) :
Effectable(value),
mSourceType(ST_SOUND),
mMusicChannel(1),
mIngoreStopSound(true),
mIs3D(false),
mVolume(1.0f),
mIsLoop(false),
mMinDistance(5.0f),
mMaxDistance(25.0f),
mDistanceUseX(true),
mDistanceUseY(true),
mDistanceUseZ(false)
{
}
//----------------------------------------------------------------------------
void Soundable::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Effectable::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mSourceType);
	source.Read(mMusicChannel);
	source.ReadBool(mIngoreStopSound);
	source.ReadString(mFilename);
	source.ReadBool(mIsLoop);
	source.ReadBool(mIs3D);
	source.Read(mVolume);
	source.Read(mMinDistance);
	source.Read(mMaxDistance);
	source.ReadAggregate(mVelocity);
	source.ReadBool(mDistanceUseX);
	source.ReadBool(mDistanceUseY);
	source.ReadBool(mDistanceUseZ);

	PX2_END_DEBUG_STREAM_LOAD(Soundable, source);
}
//----------------------------------------------------------------------------
void Soundable::Link (InStream& source)
{
	Effectable::Link(source);
}
//----------------------------------------------------------------------------
void Soundable::PostLink ()
{
	Effectable::PostLink();
}
//----------------------------------------------------------------------------
bool Soundable::Register (OutStream& target) const
{
	if (Effectable::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void Soundable::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Effectable::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mSourceType);
	target.Write(mMusicChannel);
	target.WriteBool(mIngoreStopSound);
	target.WriteString(mFilename);
	target.WriteBool(mIsLoop);
	target.WriteBool(mIs3D);
	target.Write(mVolume);
	target.Write(mMinDistance);
	target.Write(mMaxDistance);
	target.WriteAggregate(mVelocity);
	target.WriteBool(mDistanceUseX);
	target.WriteBool(mDistanceUseY);
	target.WriteBool(mDistanceUseZ);

	PX2_END_DEBUG_STREAM_SAVE(Soundable, target);
}
//----------------------------------------------------------------------------
int Soundable::GetStreamingSize (Stream &stream) const
{
	int size = Effectable::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_ENUMSIZE(mSourceType);
	size += sizeof(mMusicChannel);
	size += PX2_BOOLSIZE(mIngoreStopSound);
	size += PX2_STRINGSIZE(mFilename);
	size += PX2_BOOLSIZE(mIsLoop);
	size += PX2_BOOLSIZE(mIs3D);
	size += sizeof(mVolume);
	size += sizeof(mMinDistance);
	size += sizeof(mMaxDistance);
	size += sizeof(mVelocity);
	size += PX2_BOOLSIZE(mDistanceUseX);
	size += PX2_BOOLSIZE(mDistanceUseY);
	size += PX2_BOOLSIZE(mDistanceUseZ);

	return size;
}
//----------------------------------------------------------------------------
