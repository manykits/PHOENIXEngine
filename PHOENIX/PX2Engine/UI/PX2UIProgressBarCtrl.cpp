// PX2UIProgressBarCtrl.cpp 

#include "PX2UIProgressBarCtrl.hpp"
#include "PX2UIProgressBar.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, InterpCurveFloatController, UIProgressBarCtrl);
PX2_IMPLEMENT_STREAM(UIProgressBarCtrl);
PX2_IMPLEMENT_FACTORY(UIProgressBarCtrl);
PX2_IMPLEMENT_DEFAULT_NAMES(InterpCurveFloatController, UIProgressBarCtrl);

//----------------------------------------------------------------------------
UIProgressBarCtrl::UIProgressBarCtrl() :
InterpCurveFloatController(0.0f),
mSpeed(0.1f),
mProgress(0.0f)
{
	SetName("UIProgressBarCtrl");

	MaxTime = 0.0f;

	SetAttachUpdateInit(true);
	SetRelatively(false);
	SetDetachResetInit(false);
}
//----------------------------------------------------------------------------
UIProgressBarCtrl::~UIProgressBarCtrl()
{
}
//----------------------------------------------------------------------------
void UIProgressBarCtrl::SetProgressInit(float progress)
{
	mValues.Reset();
	mValues.AddPoint(0.0f, progress, 0.0f, 0.0f, ICM_LINEAR);

	MaxTime = 0.0f;
	mProgress = progress;

	Reset();
	Pause();
}
//----------------------------------------------------------------------------
void UIProgressBarCtrl::SetProgress(float progress)
{
	float dst = progress - mProgress;

	float time = Mathf::FAbs(dst / mSpeed);

	float insertTime = 0.0f;
	int numPoints = mValues.GetNumPoints();
	if (numPoints > 0)
	{
		insertTime = mValues.Points[numPoints - 1].InVal;
	}
	insertTime += time;

	mValues.AddPoint(insertTime, progress, 0.0f, 0.0f, ICM_LINEAR);
	MaxTime = insertTime;

	mProgress = progress;

	Play();
}
//----------------------------------------------------------------------------
void UIProgressBarCtrl::_Update(double applicationTime, double elapsedTime)
{
	PX2_UNUSED(applicationTime);
	PX2_UNUSED(elapsedTime);

	float lastPointTime = 0.0f;
	int numPoints = mValues.GetNumPoints();
	if (numPoints > 0)
	{
		lastPointTime = mValues.Points[numPoints - 1].InVal;
	}

	float useTime = GetPlayedTimeMinusDelay();
	if (useTime > lastPointTime)
	{
		Pause();
		mPlayedTime = lastPointTime + mDelayPlayTime;
		useTime = lastPointTime;
	}

	float ctrlTime = (float)GetControlTimeByRangeTime(useTime);
	mCurValueRaw = mValues.Eval(ctrlTime, 0.0f);
	mCurValueRelatived = mCurValueRaw;

	if (mIsRelatively)
		mCurValueRelatived += mInitValue;

	UIProgressBar *pb = StaticCast<UIProgressBar>(mObject);
	if (pb)
	{
		pb->SetProgress(mCurValueRelatived);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
UIProgressBarCtrl::UIProgressBarCtrl(LoadConstructor value)
:
InterpCurveFloatController(value),
mSpeed(0.1f),
mProgress(0.0f)
{
	MaxTime = 0.0f;
}
//----------------------------------------------------------------------------
void UIProgressBarCtrl::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	InterpCurveFloatController::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mProgress);

	PX2_END_DEBUG_STREAM_LOAD(UIProgressBarCtrl, source);
}
//----------------------------------------------------------------------------
void UIProgressBarCtrl::Link(InStream& source)
{
	InterpCurveFloatController::Link(source);
}
//----------------------------------------------------------------------------
void UIProgressBarCtrl::PostLink()
{
	InterpCurveFloatController::PostLink();
}
//----------------------------------------------------------------------------
bool UIProgressBarCtrl::Register(OutStream& target) const
{
	return InterpCurveFloatController::Register(target);
}
//----------------------------------------------------------------------------
void UIProgressBarCtrl::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	InterpCurveFloatController::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mProgress);

	PX2_END_DEBUG_STREAM_SAVE(UIProgressBarCtrl, target);
}
//----------------------------------------------------------------------------
int UIProgressBarCtrl::GetStreamingSize(Stream &stream) const
{
	int size = InterpCurveFloatController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mProgress);

	return size;
}
//----------------------------------------------------------------------------