// PX2InterpCurveFloat3Controller.cpp

#include "PX2InterpCurveFloat3Controller.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, InterpCurveController, InterpCurveFloat3Controller);
PX2_IMPLEMENT_STREAM(InterpCurveFloat3Controller);
PX2_IMPLEMENT_FACTORY(InterpCurveFloat3Controller);
PX2_IMPLEMENT_DEFAULT_NAMES(InterpCurveController, InterpCurveFloat3Controller);

//----------------------------------------------------------------------------
InterpCurveFloat3Controller::InterpCurveFloat3Controller(const Float3 
	&initValue) :
mInitValue(initValue)
{
	mCurValueRaw = Float3::ZERO;
	mCurValueRelatived = Float3::ZERO;
	mValues.AddPoint(0.0f, mInitValue, Float3::ZERO, Float3::ZERO,
		ICM_LINEAR);
}
//----------------------------------------------------------------------------
InterpCurveFloat3Controller::~InterpCurveFloat3Controller ()
{
}
//----------------------------------------------------------------------------
void InterpCurveFloat3Controller::Clear()
{
	mValues.Reset();
}
//----------------------------------------------------------------------------
void InterpCurveFloat3Controller::AddPoint(float inVal, const Float3 &outVal, 
	InterpCurveMode mode)
{
	mValues.AddPoint(inVal, outVal, Float3::ZERO, Float3::ZERO, mode);
}
//----------------------------------------------------------------------------
void InterpCurveFloat3Controller::AddPoint(float inVal, const AVector &outVal, 
	InterpCurveMode mode)
{
	mValues.AddPoint(inVal, outVal, Float3::ZERO, Float3::ZERO, mode);
}
//----------------------------------------------------------------------------
void InterpCurveFloat3Controller::_Update (double applicationTime,
	double elapsedTime)
{
	PX2_UNUSED(applicationTime);
	PX2_UNUSED(elapsedTime);

	float ctrlTime = (float)GetControlTimeByRangeTime(GetPlayedTimeMinusDelay());
	mCurValueRaw = mValues.Eval(ctrlTime, Float3::ZERO);
	mCurValueRelatived = mCurValueRaw;

	if (mIsRelatively)
		mCurValueRelatived += mInitValue;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void InterpCurveFloat3Controller::RegistProperties ()
{
	InterpCurveController::RegistProperties();

	AddPropertyClass("InterpCurveFloat3Controller");

	int numPoints = mValues.GetNumPoints();
	for (int i=0; i<numPoints; i++)
	{
		std::string name = "["+StringHelp::IntToString(i)+"]";

		InterpCurvePointFloat3 point = mValues.Points[i];
		AddProperty(name, PT_EMFLOAT3, point);
	}
}
//----------------------------------------------------------------------------
void InterpCurveFloat3Controller::OnPropertyChanged (const PropertyObject &obj)
{
	InterpCurveController::OnPropertyChanged(obj);

	size_t begin = obj.Name.find_first_of("[");
	size_t end = obj.Name.find_first_of("]");
	if (begin!=std::string::npos && end!=std::string::npos)
	{	
		size_t end = obj.Name.find_first_of("]");
		std::string valStr = obj.Name.substr(begin+1, end-begin);
		int i = atoi(valStr.c_str());

		mValues.Points[i] = PX2_ANY_AS(obj.Data, InterpCurvePointFloat3);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
InterpCurveFloat3Controller::InterpCurveFloat3Controller (LoadConstructor value)
	:
InterpCurveController(value)
{
	mCurValueRaw = Float3::ZERO;
	mCurValueRelatived = Float3::ZERO;
}
//----------------------------------------------------------------------------
void InterpCurveFloat3Controller::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	InterpCurveController::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadAggregate(mInitValue);
	source.ReadAggregate(mValues);

	PX2_END_DEBUG_STREAM_LOAD(InterpCurveFloat3Controller, source);
}
//----------------------------------------------------------------------------
void InterpCurveFloat3Controller::Link (InStream& source)
{
	InterpCurveController::Link(source);
}
//----------------------------------------------------------------------------
void InterpCurveFloat3Controller::PostLink ()
{
	InterpCurveController::PostLink();
}
//----------------------------------------------------------------------------
bool InterpCurveFloat3Controller::Register (OutStream& target) const
{
	return InterpCurveController::Register(target);
}
//----------------------------------------------------------------------------
void InterpCurveFloat3Controller::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	InterpCurveController::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteAggregate(mInitValue);
	target.WriteAggregate(mValues);

	PX2_END_DEBUG_STREAM_SAVE(InterpCurveFloat3Controller, target);
}
//----------------------------------------------------------------------------
int InterpCurveFloat3Controller::GetStreamingSize (Stream &stream) const
{
	int size = InterpCurveController::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += sizeof(mInitValue);
	size += mValues.GetStreamSize();
	return size;
}
//----------------------------------------------------------------------------