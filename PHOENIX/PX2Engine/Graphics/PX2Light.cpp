// PX2Light.cpp

#include "PX2Light.hpp"
#include "PX2Math.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, Light);
PX2_IMPLEMENT_STREAM(Light);
PX2_IMPLEMENT_FACTORY(Light);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, Light);

//----------------------------------------------------------------------------
Light::Light (Type eType)
:
Ambient(Float4(0.0f, 0.0f, 0.0f, 1.0f)),
Diffuse(Float4(0.0f, 0.0f, 0.0f, 1.0f)),
Specular(Float4(0.0f, 0.0f, 0.0f, 1.0f)),
Range(1.0f),
Constant(1.0f),
Linear(0.0f),
Quadratic(0.0f),
Intensity(1.0f),
Angle(Mathf::PI),
CosAngle(-1.0f),
SinAngle(0.0f),
Exponent(1.0f),
Position(APoint::ORIGIN),
DVector(-AVector::UNIT_Z),
UVector(AVector::UNIT_Y),
RVector(AVector::UNIT_X),
mType(eType)
{
}
//----------------------------------------------------------------------------
Light::~Light ()
{
}
//----------------------------------------------------------------------------
void Light::SetAngle (float angle)
{
	assertion(0.0f < angle && angle <= Mathf::PI,
		"Angle out of range in SetAngle.\n");

	Angle = angle;
	CosAngle = Mathf::Cos(angle);
	SinAngle = Mathf::Sin(angle);
}
//----------------------------------------------------------------------------
void Light::SetDirection (const AVector& direction)
{
	DVector = direction;
	AVector::GenerateOrthonormalBasis(UVector, RVector, DVector);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Light::Light (LoadConstructor value)
:
Object(value),
Ambient(Float4(0.0f, 0.0f, 0.0f, 1.0f)),
Diffuse(Float4(0.0f, 0.0f, 0.0f, 1.0f)),
Specular(Float4(0.0f, 0.0f, 0.0f, 1.0f)),
Range(1.0f),
Constant(0.0f),
Linear(0.0f),
Quadratic(0.0f),
Intensity(0.0f),
Angle(0.0f),
CosAngle(0.0f),
SinAngle(0.0f),
Exponent(0.0f),
Position(APoint::ORIGIN),
DVector(-AVector::UNIT_Z),
UVector(AVector::UNIT_Y),
RVector(AVector::UNIT_X),
mType(LT_QUANTITY)
{
}
//----------------------------------------------------------------------------
void Light::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadEnum(mType);
	source.ReadAggregate(Ambient);
	source.ReadAggregate(Diffuse);
	source.ReadAggregate(Specular);
	source.Read(Range);
	source.Read(Constant);
	source.Read(Linear);
	source.Read(Quadratic);
	source.Read(Intensity);
	source.Read(Angle);
	source.Read(CosAngle);
	source.Read(SinAngle);
	source.Read(Exponent);
	source.ReadAggregate(Position);
	source.ReadAggregate(DVector);
	source.ReadAggregate(UVector);
	source.ReadAggregate(RVector);

	PX2_END_DEBUG_STREAM_LOAD(Light, source);
}
//----------------------------------------------------------------------------
void Light::Link (InStream& source)
{
	Object::Link(source);
}
//----------------------------------------------------------------------------
void Light::PostLink ()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool Light::Register (OutStream& target) const
{
	return Object::Register(target);
}
//----------------------------------------------------------------------------
void Light::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteEnum(mType);
	target.WriteAggregate(Ambient);
	target.WriteAggregate(Diffuse);
	target.WriteAggregate(Specular);
	target.Write(Range);
	target.Write(Constant);
	target.Write(Linear);
	target.Write(Quadratic);
	target.Write(Intensity);
	target.Write(Angle);
	target.Write(CosAngle);
	target.Write(SinAngle);
	target.Write(Exponent);
	target.WriteAggregate(Position);
	target.WriteAggregate(DVector);
	target.WriteAggregate(UVector);
	target.WriteAggregate(RVector);

	PX2_END_DEBUG_STREAM_SAVE(Light, target);
}
//----------------------------------------------------------------------------
int Light::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += PX2_ENUMSIZE(mType);
	size += sizeof(Ambient);
	size += sizeof(Diffuse);
	size += sizeof(Specular);
	size += sizeof(Range);
	size += sizeof(Constant);
	size += sizeof(Linear);
	size += sizeof(Quadratic);
	size += sizeof(Intensity);
	size += sizeof(Angle);
	size += sizeof(CosAngle);
	size += sizeof(SinAngle);
	size += sizeof(Exponent);
	size += sizeof(Position);
	size += sizeof(DVector);
	size += sizeof(UVector);
	size += sizeof(RVector);
	return size;
}
//----------------------------------------------------------------------------
