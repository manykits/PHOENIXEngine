// PX2UIScoreTextCtrl.cpp

#include "PX2UIScoreTextCtrl.hpp"
#include "PX2UIScoreText.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, InterpCurveFloatController, UIScoreStaticTextCtrl);
PX2_IMPLEMENT_STREAM(UIScoreStaticTextCtrl);
PX2_IMPLEMENT_FACTORY(UIScoreStaticTextCtrl);
PX2_IMPLEMENT_DEFAULT_NAMES(InterpCurveFloatController, UIScoreStaticTextCtrl);

//----------------------------------------------------------------------------
UIScoreStaticTextCtrl::UIScoreStaticTextCtrl() :
mSpeed(10.0f),
mNum(0)
{
	SetName("UIScoreStaticTextCtrl");

	MaxTime = 0.0f;

	SetAttachUpdateInit(true);
	SetRelatively(false);
	SetDetachResetInit(false);
}
//----------------------------------------------------------------------------
UIScoreStaticTextCtrl::~UIScoreStaticTextCtrl()
{
}
//----------------------------------------------------------------------------
void UIScoreStaticTextCtrl::SetSpeed(float speed)
{
	mSpeed = speed;
}
//----------------------------------------------------------------------------
void UIScoreStaticTextCtrl::SetNumInit(int num)
{
	if ((int)mValues.Points.size() > 0)
	{
		mValues.Points[0].InVal = 0.0f;
		mValues.Points[0].OutVal = (float)num;
		mValues.Points[0].InterpMode = ICM_LINEAR;
	}

	MaxTime = 0.0f;
	mNum = num;

	Reset();
	Pause();
}
//----------------------------------------------------------------------------
void UIScoreStaticTextCtrl::SetNum(int num)
{
	int dst = num - mNum;
	float fDist = (float)dst;

	float time = Mathf::FAbs(fDist / mSpeed);

	float insertTime = 0.0f;
	int numPoints = mValues.GetNumPoints();
	if (numPoints > 0)
	{
		insertTime = mValues.Points[numPoints - 1].InVal;
	}
	insertTime += time;

	float fNum = (float)num;
	mValues.AddPoint(insertTime, fNum, 0.0f, 0.0f, ICM_LINEAR);
	MaxTime = insertTime;

	mNum = num;

	Play();
}
//----------------------------------------------------------------------------
bool UIScoreStaticTextCtrl::Update(double applicationTime, double elapsedTime)
{
	return InterpCurveFloatController::Update(applicationTime, elapsedTime);
}
//----------------------------------------------------------------------------
void UIScoreStaticTextCtrl::_Update(double applicationTime,
	double elapsedTime)
{
	InterpCurveFloatController::_Update(applicationTime, elapsedTime);

	float fCurValue = GetCurValueRelatived();
	int curValue = (int)fCurValue;

	UIScoreText *sst = StaticCast<UIScoreText>(mObject);
	if (sst)
	{
		sst->SetNum(curValue);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
UIScoreStaticTextCtrl::UIScoreStaticTextCtrl(LoadConstructor value)
:
InterpCurveFloatController(value),
mSpeed(10.0f),
mNum(0)
{
}
//----------------------------------------------------------------------------
void UIScoreStaticTextCtrl::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	InterpCurveFloatController::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mSpeed);

	PX2_END_DEBUG_STREAM_LOAD(UIScoreStaticTextCtrl, source);
}
//----------------------------------------------------------------------------
void UIScoreStaticTextCtrl::Link(InStream& source)
{
	InterpCurveFloatController::Link(source);
}
//----------------------------------------------------------------------------
void UIScoreStaticTextCtrl::PostLink()
{
	InterpCurveFloatController::PostLink();
}
//----------------------------------------------------------------------------
bool UIScoreStaticTextCtrl::Register(OutStream& target) const
{
	return InterpCurveFloatController::Register(target);
}
//----------------------------------------------------------------------------
void UIScoreStaticTextCtrl::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	InterpCurveFloatController::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mSpeed);

	PX2_END_DEBUG_STREAM_SAVE(UIScoreStaticTextCtrl, target);
}
//----------------------------------------------------------------------------
int UIScoreStaticTextCtrl::GetStreamingSize(Stream &stream) const
{
	int size = InterpCurveFloatController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mSpeed);

	return size;
}
//----------------------------------------------------------------------------