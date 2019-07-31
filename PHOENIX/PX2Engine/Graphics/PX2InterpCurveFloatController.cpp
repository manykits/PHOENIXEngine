// PX2InterpCurveFloatController.cpp

#include "PX2InterpCurveFloatController.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, InterpCurveController, InterpCurveFloatController);
PX2_IMPLEMENT_STREAM(InterpCurveFloatController);
PX2_IMPLEMENT_FACTORY(InterpCurveFloatController);
PX2_IMPLEMENT_DEFAULT_NAMES(InterpCurveController, InterpCurveFloatController);

//----------------------------------------------------------------------------
InterpCurveFloatController::InterpCurveFloatController (float initValue) :
mInitValue(initValue),
mCurValueRaw(0.0f),
mCurValueRelatived(0.0f)
{
	mValues.AddPoint(0.0f, mInitValue, 0.0f, 0.0f, ICM_LINEAR);
}
//----------------------------------------------------------------------------
InterpCurveFloatController::~InterpCurveFloatController ()
{
}
//----------------------------------------------------------------------------
void InterpCurveFloatController::Clear()
{
	mValues.Reset();
}
//----------------------------------------------------------------------------
void InterpCurveFloatController::AddPoint(float inVal, float outVal, InterpCurveMode mode)
{
	mValues.AddPoint(inVal, outVal, 0.0, 0.0, mode);
}
//----------------------------------------------------------------------------
void InterpCurveFloatController::_Update(double applicationTime,
	double elapsedTime)
{
	PX2_UNUSED(applicationTime);
	PX2_UNUSED(elapsedTime);

	float playedTimeMinusDelay = GetPlayedTimeMinusDelay();
	float ctrlTime = (float)GetControlTimeByRangeTime(playedTimeMinusDelay);
	mCurValueRaw = mValues.Eval(ctrlTime, 0.0f);
	mCurValueRelatived = mCurValueRaw;

	if (mIsRelatively)
		mCurValueRelatived += mInitValue;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// InterpCurveFloatController
//----------------------------------------------------------------------------
void InterpCurveFloatController::RegistProperties ()
{
	InterpCurveController::RegistProperties();

	AddPropertyClass("InterpCurveFloatController");

	int numPoints = mValues.GetNumPoints();
	for (int i=0; i<numPoints; i++)
	{
		std::string name = "["+StringHelp::IntToString(i)+"]";
		
		InterpCurvePointFloat point = mValues.Points[i];
		AddProperty(name, PT_EMFLOAT, point);
	}
}
//----------------------------------------------------------------------------
void InterpCurveFloatController::OnPropertyChanged (const PropertyObject &obj)
{
	InterpCurveController::OnPropertyChanged(obj);

	size_t begin = obj.Name.find_first_of("[");
	size_t end = obj.Name.find_first_of("]");
	if (begin!=std::string::npos && end!=std::string::npos)
	{	
		size_t end = obj.Name.find_first_of("]");
		std::string valStr = obj.Name.substr(begin+1, end-begin);
		int i = atoi(valStr.c_str());

		mValues.Points[i] = PX2_ANY_AS(obj.Data, InterpCurvePointFloat);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
InterpCurveFloatController::InterpCurveFloatController (LoadConstructor value)
	:
InterpCurveController(value),
mCurValueRaw(0.0f),
mCurValueRelatived(0.0f)
{
}
//----------------------------------------------------------------------------
void InterpCurveFloatController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	InterpCurveController::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mInitValue);
	source.ReadAggregate(mValues);

	PX2_END_DEBUG_STREAM_LOAD(InterpCurveFloatController, source);
}
//----------------------------------------------------------------------------
void InterpCurveFloatController::Link (InStream& source)
{
	InterpCurveController::Link(source);
}
//----------------------------------------------------------------------------
void InterpCurveFloatController::PostLink ()
{
	InterpCurveController::PostLink();
}
//----------------------------------------------------------------------------
bool InterpCurveFloatController::Register (OutStream& target) const
{
	return InterpCurveController::Register(target);
}
//----------------------------------------------------------------------------
void InterpCurveFloatController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	InterpCurveController::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mInitValue);
	target.WriteAggregate(mValues);

	PX2_END_DEBUG_STREAM_SAVE(InterpCurveFloatController, target);
}
//----------------------------------------------------------------------------
int InterpCurveFloatController::GetStreamingSize (Stream &stream) const
{
	int size = InterpCurveController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += sizeof(mInitValue);
	size += mValues.GetStreamSize();
	return size;
}
//----------------------------------------------------------------------------