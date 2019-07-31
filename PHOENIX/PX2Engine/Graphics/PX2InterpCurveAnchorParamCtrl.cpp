// PX2InterpCurveAnchorParamCtrl.cpp

#include "PX2InterpCurveAnchorParamCtrl.hpp"
#include "PX2SizeNode.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, InterpCurveFloat3Controller, InterpCurveAnchorParamCtrl);
PX2_IMPLEMENT_STREAM(InterpCurveAnchorParamCtrl);
PX2_IMPLEMENT_FACTORY(InterpCurveAnchorParamCtrl);
PX2_IMPLEMENT_DEFAULT_NAMES(InterpCurveFloat3Controller, InterpCurveAnchorParamCtrl);

//----------------------------------------------------------------------------
InterpCurveAnchorParamCtrl::InterpCurveAnchorParamCtrl(bool isHor) :
InterpCurveFloat3Controller(Float3::ZERO),
mIsHor(isHor)
{
	SetName("ICCtrl_AnchorParam");

	SetPriority(12);
}
//----------------------------------------------------------------------------
InterpCurveAnchorParamCtrl::~InterpCurveAnchorParamCtrl()
{
}
//----------------------------------------------------------------------------
void InterpCurveAnchorParamCtrl::SetHor(bool isHor)
{
	mIsHor = isHor;
}
//----------------------------------------------------------------------------
bool InterpCurveAnchorParamCtrl::IsHor() const
{
	return mIsHor;
}
//----------------------------------------------------------------------------
void InterpCurveAnchorParamCtrl::AddPoint(float inVal, float anchorParam0, 
	float anchorParam1,	InterpCurveMode mode)
{
	InterpCurveFloat3Controller::AddPoint(inVal,
		Float3(anchorParam0, anchorParam1, 0.0f), mode);
}
//----------------------------------------------------------------------------
void InterpCurveAnchorParamCtrl::_Update(double applicationTime,
	double elapsedTime)
{
	InterpCurveFloat3Controller::_Update(applicationTime, elapsedTime);

	const Float3 &curValue = GetCurValueRelatived();

	SizeNode* sn = StaticCast<SizeNode>(mObject);
	if (sn)
	{
		if (IsHor())
			sn->SetAnchorParamHor(Float2(curValue[0], curValue[1]));
		else
			sn->SetAnchorParamVer(Float2(curValue[0], curValue[1]));
	}
}
//----------------------------------------------------------------------------
void InterpCurveAnchorParamCtrl::SetControlledable(Controlledable* object)
{
	if (object)
	{
		if (IsAttachUpdateInit())
		{
			SizeNode* sn = StaticCast<SizeNode>(mObject);
			if (sn)
			{
				if (IsHor())
				{
					Float2 paramHor = sn->GetAnchorParamHor();
					mInitValue[0] = paramHor[0];
					mInitValue[1] = paramHor[1];
				}
				else
				{
					Float2 paramVer = sn->GetAnchorParamVer();
					mInitValue[0] = paramVer[0];
					mInitValue[1] = paramVer[1];
				}
			}
		}
	}
	else
	{
		if (IsDetachResetInit())
		{
			SizeNode* sn = StaticCast<SizeNode>(mObject);
			if (sn)
			{
				if (IsHor())
				{
					Float2 paramHor = Float2(mInitValue[0], mInitValue[1]);
					sn->SetAnchorParamHor(paramHor);
				}
				else
				{
					Float2 paramVer = Float2(mInitValue[0], mInitValue[1]);
					sn->SetAnchorParamVer(paramVer);
				}
			}
		}
	}

	InterpCurveFloat3Controller::SetControlledable(object);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
InterpCurveAnchorParamCtrl::InterpCurveAnchorParamCtrl(LoadConstructor value):
InterpCurveFloat3Controller(value),
mIsHor(false)
{
}
//----------------------------------------------------------------------------
void InterpCurveAnchorParamCtrl::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	InterpCurveFloat3Controller::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadBool(mIsHor);

	PX2_END_DEBUG_STREAM_LOAD(InterpCurveAnchorParamCtrl, source);
}
//----------------------------------------------------------------------------
void InterpCurveAnchorParamCtrl::Link(InStream& source)
{
	InterpCurveFloat3Controller::Link(source);
}
//----------------------------------------------------------------------------
void InterpCurveAnchorParamCtrl::PostLink()
{
	InterpCurveFloat3Controller::PostLink();
}
//----------------------------------------------------------------------------
bool InterpCurveAnchorParamCtrl::Register(OutStream& target) const
{
	return InterpCurveFloat3Controller::Register(target);
}
//----------------------------------------------------------------------------
void InterpCurveAnchorParamCtrl::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	InterpCurveFloat3Controller::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteBool(mIsHor);

	PX2_END_DEBUG_STREAM_SAVE(InterpCurveAnchorParamCtrl, target);
}
//----------------------------------------------------------------------------
int InterpCurveAnchorParamCtrl::GetStreamingSize(Stream &stream) const
{
	int size = InterpCurveFloat3Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += PX2_BOOLSIZE(mIsHor);

	return size;
}
//----------------------------------------------------------------------------