// PX2ScriptController.cpp

#include "PX2ScriptController.hpp"
#include "PX2Controlledable.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, ScriptController);
PX2_IMPLEMENT_STREAM(ScriptController);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, ScriptController);
PX2_IMPLEMENT_FACTORY(ScriptController);

//---------------------------------------------------------------------------
ScriptController::ScriptController():
mIsInitUpdated(false),
mNumFixUpdatePerSeconds(30),
mFiexUpdateTiming(0),
mOneFrameSeconds(0.0f)
{
	mOneFrameSeconds = 1.0f / (float)mNumFixUpdatePerSeconds;
}
//---------------------------------------------------------------------------
ScriptController::~ScriptController()
{
}
//---------------------------------------------------------------------------
void ScriptController::SetFileClass(const std::string &filename,
	const std::string &className)
{
	mFilename = filename;
	mClassName = className;
}
//---------------------------------------------------------------------------
void ScriptController::SetStringClass(const std::string &str,
	const std::string &className)
{
	mString = str;
	mClassName = className;
}
//---------------------------------------------------------------------------
const std::string &ScriptController::GetFilename() const
{
	return mFilename;
}
//---------------------------------------------------------------------------
const std::string &ScriptController::GetClassName() const
{
	return mClassName;
}
//---------------------------------------------------------------------------
int ScriptController::SelfP() const
{
	return 0;
}
//---------------------------------------------------------------------------
void ScriptController::SetControlledable(Controlledable* object)
{
	if (!mObject)
	{
		ScriptOnDetach();
	}

	Controller::SetControlledable(object);

	if (object)
	{
		ScriptOnAttached();
	}
}
//---------------------------------------------------------------------------
bool ScriptController::Update(double applicationTime, double elapsedTime)
{
	if (!mIsInitUpdated)
	{
		ScriptOnInitUpdate();

		mIsInitUpdated = true;
	}

	ScriptOnUpdate();

	mFiexUpdateTiming += (float)elapsedTime;
	int numGenFrams = (int)(mFiexUpdateTiming*mNumFixUpdatePerSeconds);
	if (numGenFrams > 0)
	{
		for (int i = 0; i< numGenFrams; i++)
		{
			FixUpdate();
		}
		mFiexUpdateTiming -= (float)(numGenFrams* mOneFrameSeconds);
	}

	return Controller::Update(applicationTime, elapsedTime);
}
//---------------------------------------------------------------------------
void ScriptController::FixUpdate()
{
	ScriptOnFixUpdate();
}
//---------------------------------------------------------------------------
void ScriptController::Reset()
{
	ScriptOnReset();

	Controller::Reset();
}
//---------------------------------------------------------------------------
void ScriptController::Play()
{
	ScriptOnPlay();

	Controller::Play();
}
//---------------------------------------------------------------------------
void ScriptController::Pause()
{
	ScriptOnPause();

	Controller::Pause();
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnAttached()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnDetach()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnInitUpdate()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnUpdate()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnFixUpdate()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnReset()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnPlay()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnPause()
{
}
//---------------------------------------------------------------------------
void ScriptController::ScriptOnPlayUpdate()
{
}
//---------------------------------------------------------------------------
void ScriptController::_Update(double applicationTime, double elapsedTime)
{
	Controller::_Update(applicationTime, elapsedTime);

	ScriptOnPlayUpdate();
}
//---------------------------------------------------------------------------
void ScriptController::SetNumFixUpdatePerSeconds(int num)
{
	mNumFixUpdatePerSeconds = num;
	mOneFrameSeconds = 1.0f / mNumFixUpdatePerSeconds;
}
//---------------------------------------------------------------------------
int ScriptController::GetNumFixUpdatePerSeconds() const
{
	return mNumFixUpdatePerSeconds;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ScriptController
//---------------------------------------------------------------------------
void ScriptController::RegistProperties()
{
	Controller::RegistProperties();

	AddPropertyClass("ScriptController");
	AddProperty("Filename", PT_STRING, GetFilename(), false);
	AddProperty("ClassName", PT_STRING, GetClassName(), false);
}
//---------------------------------------------------------------------------
void ScriptController::OnPropertyChanged(const PropertyObject &obj)
{
	Controller::OnPropertyChanged(obj);
}
//---------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
ScriptController::ScriptController(LoadConstructor value) :
Controller(value),
mIsInitUpdated(false),
mNumFixUpdatePerSeconds(30),
mFiexUpdateTiming(0),
mOneFrameSeconds(0.0f)
{
}
//----------------------------------------------------------------------------
void ScriptController::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mNumFixUpdatePerSeconds);
	mOneFrameSeconds = 1.0f / (float)mNumFixUpdatePerSeconds;

	source.ReadString(mFilename);
	source.ReadString(mString);
	source.ReadString(mClassName);

	PX2_END_DEBUG_STREAM_LOAD(ScriptController, source);
}
//----------------------------------------------------------------------------
void ScriptController::Link(InStream& source)
{
	Controller::Link(source);
}
//----------------------------------------------------------------------------
void ScriptController::PostLink()
{
	Controller::PostLink();
}
//----------------------------------------------------------------------------
bool ScriptController::Register(OutStream& target) const
{
	if (Controller::Register(target))
	{
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void ScriptController::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mNumFixUpdatePerSeconds);

	target.WriteString(mFilename);
	target.WriteString(mString);
	target.WriteString(mClassName);

	PX2_END_DEBUG_STREAM_SAVE(ScriptController, target);
}
//----------------------------------------------------------------------------
int ScriptController::GetStreamingSize(Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mNumFixUpdatePerSeconds);

	size += PX2_STRINGSIZE(mFilename);
	size += PX2_STRINGSIZE(mString);
	size += PX2_STRINGSIZE(mClassName);

	return size;
}
//----------------------------------------------------------------------------