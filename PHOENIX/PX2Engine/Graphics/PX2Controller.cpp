// PX2Controller.cpp

#include "PX2Controller.hpp"
#include "PX2Controlledable.hpp"
#include "PX2Math.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, Controller);
PX2_IMPLEMENT_STREAM(Controller);
PX2_IMPLEMENT_ABSTRACT_FACTORY(Controller);

//----------------------------------------------------------------------------
Controller::Controller () :
Repeat(RT_NONE),
MinTime(0.0),
MaxTime(10.0),
Phase(0.0),
Frequency(1.0),
Active(true),
mObject(0),
mPriority(0),
mIsPlaySelfCtrl(false),
mMaxPlayTime(-1.0f),
mDelayPlayTime(0.0f),
mIsPlaying(false),
mPlayedTime(0.0f),
mIsPlayedDoDetach(false),
mPlayedCallback(0),
mIsEverPlayed(false)
{
	SetName("Controller");
}
//----------------------------------------------------------------------------
Controller::~Controller ()
{
}
//----------------------------------------------------------------------------
void Controller::SetMaxPlayTime (float time)
{
	mMaxPlayTime = time;
}
//----------------------------------------------------------------------------
void Controller::SetDelayPlayTime (float time)
{
	mDelayPlayTime = time;
}
//----------------------------------------------------------------------------
float Controller::GetPlayedTimeMinusDelay () const
{
	float val = mPlayedTime - mDelayPlayTime;

	if (val < 0.0f)
		val = -0.0001f;

	return val;
}
//----------------------------------------------------------------------------
void Controller::SetPlaySelfCtrl(bool playSelf)
{
	mIsPlaySelfCtrl = playSelf;
}
//----------------------------------------------------------------------------
void Controller::ResetPlay ()
{
	Reset();
	Play();
}
//----------------------------------------------------------------------------
void Controller::Play ()
{
	mIsPlaying = true;
}
//----------------------------------------------------------------------------
bool Controller::IsPlaying () const
{
	return mIsPlaying;
}
//----------------------------------------------------------------------------
void Controller::Pause()
{
	mIsPlaying = false;
}
//----------------------------------------------------------------------------
void Controller::Reset ()
{
	Pause();

	mIsEverPlayed = false;
	mPlayedTime = 0.0f;
}
//----------------------------------------------------------------------------
void Controller::SetPriority (int priority)
{
	mPriority = priority;
}
//----------------------------------------------------------------------------
bool Controller::Update(double applicationTime, double elapsedTime1)
{
	if (Active)
	{
		float elapsedTime = (float)elapsedTime1;
		if (elapsedTime > 0.2f)
			elapsedTime = 0.2f;

		float maxPlayTime = GetMaxPlayTime();

		if (IsPlaying())
		{
			mPlayedTime += elapsedTime;

			float playedTimeMinusDelay = GetPlayedTimeMinusDelay();

			if (maxPlayTime>0.0f && playedTimeMinusDelay>=maxPlayTime)
			{
				Pause();
			}

			if (!mIsEverPlayed)
			{
				mIsEverPlayed = true;
				_InitUpdate(applicationTime, elapsedTime);
			}

			_Update(applicationTime, elapsedTime);

			if (!IsPlaying())
			{
				if (mPlayedCallback)
					mPlayedCallback(this);

				Controlledable *ctrlAble = GetControlledable();
				if (ctrlAble && IsPlayedDoDetach())
				{
					ctrlAble->DetachController(this);
					return true;
				}
			}
		}

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Controller::SetControlledable (Controlledable* object)
{
	mObject = object;
}
//----------------------------------------------------------------------------
bool Controller::LessThan (const Controller *ctrl0, const Controller *ctrl1)
{
	int priority0 = ctrl0->GetPriority();
	int priority1 = ctrl1->GetPriority();

	if (priority0 == priority1)
		return ctrl0 < ctrl1;

	return priority0 > priority1;
}
//----------------------------------------------------------------------------
double Controller::GetControlTimeByRangeTime (double rangeTime)
{
	double controlTime = Frequency*rangeTime + Phase;

	if (Repeat == RT_NONE)
	{
		return controlTime;
	}
	if (Repeat == RT_CLAMP)
	{
		// 将时间夹取到区间[min,max]
		if (controlTime < MinTime)
		{
			return MinTime;
		}
		if (controlTime > MaxTime)
		{
			return MaxTime;
		}
		return controlTime;
	}

	double timeRange = MaxTime - MinTime;
	if (timeRange > 0.0)
	{
		double multiples = (controlTime - MinTime)/timeRange;
		double integerTime = Mathd::Floor(multiples);
		double fractionTime = multiples - integerTime;
		if (Repeat == RT_WRAP)
		{
			return MinTime + fractionTime*timeRange;
		}

		// Repeat == RT_CYCLE
		if (((int)integerTime) & 1)
		{
			// 后退.
			return MaxTime - fractionTime*timeRange;
		}
		else
		{
			// 前进
			return MinTime + fractionTime*timeRange;
		}
	}

	// minimum和maximum相同，返回minimum
	return MinTime;
}
//----------------------------------------------------------------------------
void Controller::_InitUpdate(double applicationTime, double elapsedTime)
{
	PX2_UNUSED(applicationTime);
	PX2_UNUSED(elapsedTime);
}
//----------------------------------------------------------------------------
void Controller::_Update(double applicationTime, double elapsedTime)
{
	PX2_UNUSED(applicationTime);
	PX2_UNUSED(elapsedTime);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void Controller::RegistProperties ()
{
	Object::RegistProperties();

	AddPropertyClass("Controller");

	AddProperty("IsActive", PT_BOOL, Active);

	AddProperty("Priority", PT_INT, GetPriority());
	AddProperty("IsPlaySelfCtrl", PT_BOOL, IsPlaySelfCtrl());

	AddProperty("MaxPlayTime", PT_FLOAT, GetMaxPlayTime());
	AddProperty("DelayPlayTime", PT_FLOAT, GetDelayPlayTime());
	AddProperty("IsPlayedDoDetach", PT_BOOL, IsPlayedDoDetach());
	
	std::vector<std::string> repeatTypes;
	repeatTypes.push_back("PT_NONE");
	repeatTypes.push_back("RT_CLAMP");
	repeatTypes.push_back("RT_WRAP");
	repeatTypes.push_back("RT_CYCLE");
	AddPropertyEnum("RepeatType", (int)Repeat, repeatTypes);

	AddProperty("MinTime", PT_FLOAT, (float)MinTime);
	AddProperty("MaxTime", PT_FLOAT, (float)MaxTime);
	AddProperty("Phase", PT_FLOAT, (float)Phase);
	AddProperty("Frequency", PT_FLOAT, (float)Frequency);
}
//----------------------------------------------------------------------------
void Controller::OnPropertyChanged (const PropertyObject &obj)
{
	Object::OnPropertyChanged(obj);

	if ("IsActive" == obj.Name)
	{
		Active = PX2_ANY_AS(obj.Data, bool);
	}
	else if ("Priority" == obj.Name)
	{
		SetPriority(PX2_ANY_AS(obj.Data, int));
	}
	else if ("IsPlaySelfCtrl" == obj.Name)
	{
		SetPlaySelfCtrl(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("MaxPlayTime" == obj.Name)
	{
		SetMaxPlayTime(PX2_ANY_AS(obj.Data, float));
	}
	else if ("DelayPlayTime" == obj.Name)
	{
		SetDelayPlayTime(PX2_ANY_AS(obj.Data, float));
	}
	else if ("IsPlayedDoDetach" == obj.Name)
	{
		SetPlayedDoDetach(PX2_ANY_AS(obj.Data, bool));
	}
	else if ("RepeatType" == obj.Name)
	{
		Repeat = (RepeatType)PX2_ANY_AS(obj.Data, int);
	}
	else if ("MinTime" == obj.Name)
	{
		MinTime = PX2_ANY_AS(obj.Data, float);
	}
	else if ("MaxTime" == obj.Name)
	{
		MaxTime = PX2_ANY_AS(obj.Data, float);
	}
	else if ("Phase" == obj.Name)
	{
		Phase = PX2_ANY_AS(obj.Data, float);
	}
	else if ("Frequency" == obj.Name)
	{
		Frequency = PX2_ANY_AS(obj.Data, float);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 名称支持
//----------------------------------------------------------------------------
Object* Controller::GetObjectByName (const std::string& name)
{
	// mObject 不用查询
	return Object::GetObjectByName(name);
}
//----------------------------------------------------------------------------
void Controller::GetAllObjectsByName (const std::string& name,
									  std::vector<Object*>& objects)
{
	// mObject 不用查询
	Object::GetAllObjectsByName(name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Controller::Controller (LoadConstructor value)
:
Object(value),
Repeat(RT_NONE),
MinTime(0.0),
MaxTime(10.0),
Phase(0.0),
Frequency(1.0),
Active(true),
mObject(0),
mPriority(0),
mIsPlaySelfCtrl(false),
mMaxPlayTime(-1.0f),
mDelayPlayTime(0.0f),
mIsPlaying(false),
mPlayedTime(0.0f),
mIsPlayedDoDetach(false),
mPlayedCallback(0),
mIsEverPlayed(false)
{
}
//----------------------------------------------------------------------------
void Controller::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(Repeat);
	source.Read(MinTime);
	source.Read(MaxTime);
	source.Read(Phase);
	source.Read(Frequency);
	source.ReadBool(Active);
	source.ReadPointer(mObject);
	source.Read(mPriority);
	source.ReadBool(mIsPlaySelfCtrl);
	source.Read(mMaxPlayTime);
	source.Read(mDelayPlayTime);
	source.ReadBool(mIsPlayedDoDetach);

	PX2_END_DEBUG_STREAM_LOAD(Controller, source);
}
//----------------------------------------------------------------------------
void Controller::Link (InStream& source)
{
	Object::Link(source);

	source.ResolveLink(mObject);
}
//----------------------------------------------------------------------------
void Controller::PostLink ()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool Controller::Register (OutStream& target) const
{
	if (Object::Register(target))
	{
		target.Register(mObject);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void Controller::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(Repeat);
	target.Write(MinTime);
	target.Write(MaxTime);
	target.Write(Phase);
	target.Write(Frequency);
	target.WriteBool(Active);
	target.WritePointer(mObject);
	target.Write(mPriority);
	target.WriteBool(mIsPlaySelfCtrl);
	target.Write(mMaxPlayTime);
	target.Write(mDelayPlayTime);
	target.WriteBool(mIsPlayedDoDetach);

	PX2_END_DEBUG_STREAM_SAVE(Controller, target);
}
//----------------------------------------------------------------------------
int Controller::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += PX2_ENUMSIZE(Repeat);
	size += sizeof(MinTime);
	size += sizeof(MaxTime);
	size += sizeof(Phase);
	size += sizeof(Frequency);
	size += PX2_BOOLSIZE(Active);
	size += PX2_POINTERSIZE(mObject);
	size += sizeof(mPriority);
	size += PX2_BOOLSIZE(mIsPlaySelfCtrl);
	size += sizeof(mMaxPlayTime);
	size += sizeof(mDelayPlayTime);
	size += PX2_BOOLSIZE(mIsPlayedDoDetach);

	return size;
}
//----------------------------------------------------------------------------