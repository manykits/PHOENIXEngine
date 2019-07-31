// PX2CurveSegment.cpp

#include "PX2CurveSegment.hpp"
#include "PX2Math.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, CurveSegment);
PX2_IMPLEMENT_STREAM(CurveSegment);
PX2_IMPLEMENT_ABSTRACT_FACTORY(CurveSegment);

//----------------------------------------------------------------------------
CurveSegment::CurveSegment (float umin, float umax)
:
mUMin(umin),
mUMax(umax)
{
}
//----------------------------------------------------------------------------
CurveSegment::~CurveSegment ()
{
}
//----------------------------------------------------------------------------
float CurveSegment::GetUMin () const
{
	return mUMin;
}
//----------------------------------------------------------------------------
float CurveSegment::GetUMax () const
{
	return mUMax;
}
//----------------------------------------------------------------------------
AVector CurveSegment::Tangent (float u) const
{
	AVector velocity = PU(u);
	velocity.Normalize();
	return velocity;
}
//----------------------------------------------------------------------------
AVector CurveSegment::Normal (float u) const
{
	AVector velocity = PU(u);
	AVector acceleration = PUU(u);
	float VDotV = velocity.Dot(velocity);
	float VDotA = velocity.Dot(acceleration);
	AVector normal = VDotV*acceleration - VDotA*velocity;
	normal.Normalize();
	return normal;
}
//----------------------------------------------------------------------------
AVector CurveSegment::Binormal (float u) const
{
	AVector velocity = PU(u);
	AVector acceleration = PUU(u);
	float VDotV = velocity.Dot(velocity);
	float VDotA = velocity.Dot(acceleration);
	AVector normal = VDotV*acceleration - VDotA*velocity;
	normal.Normalize();
	velocity.Normalize();
	AVector binormal = velocity.Cross(normal);
	return binormal;
}
//----------------------------------------------------------------------------
void CurveSegment::GetFrame (float u, APoint& position, AVector& tangent,
							 AVector& normal, AVector& binormal) const
{
	position = P(u);
	AVector velocity = PU(u);
	AVector acceleration = PUU(u);
	float VDotV = velocity.Dot(velocity);
	float VDotA = velocity.Dot(acceleration);
	normal = VDotV*acceleration - VDotA*velocity;
	normal.Normalize();
	tangent = velocity;
	tangent.Normalize();
	binormal = tangent.Cross(normal);
}
//----------------------------------------------------------------------------
float CurveSegment::Curvature (float u) const
{
	AVector velocity = PU(u);
	float speedSqr = velocity.SquaredLength();

	if (speedSqr >= Mathf::ZERO_TOLERANCE)
	{
		AVector acceleration = PUU(u);
		AVector cross = velocity.Cross(acceleration);
		float numer = cross.Length();
		float denom = Mathf::Pow(speedSqr, 1.5f);
		return numer/denom;
	}
	else
	{
		// Curvature is indeterminate, just return 0.
		return 0.0f;
	}
}
//----------------------------------------------------------------------------
float CurveSegment::Torsion (float u) const
{
	AVector velocity = PU(u);
	AVector acceleration = PUU(u);
	AVector cross = velocity.Cross(acceleration);
	float denom = cross.SquaredLength();

	if (denom >= Mathf::ZERO_TOLERANCE)
	{
		AVector jerk = PUUU(u);
		float numer = cross.Dot(jerk);
		return numer/denom;
	}
	else
	{
		// Torsion is indeterminate, just return 0.
		return 0.0f;
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Ãû³Æ
//----------------------------------------------------------------------------
Object* CurveSegment::GetObjectByName (const std::string& name)
{
	return Object::GetObjectByName(name);
}
//----------------------------------------------------------------------------
void CurveSegment::GetAllObjectsByName (const std::string& name,
										std::vector<Object*>& objects)
{
	Object::GetAllObjectsByName(name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
CurveSegment::CurveSegment (LoadConstructor value)
:
Object(value),
mUMin(0.0f),
mUMax(0.0f)
{
}
//----------------------------------------------------------------------------
void CurveSegment::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mUMin);
	source.Read(mUMax);

	PX2_END_DEBUG_STREAM_LOAD(CurveSegment, source);
}
//----------------------------------------------------------------------------
void CurveSegment::Link (InStream& source)
{
	Object::Link(source);
}
//----------------------------------------------------------------------------
void CurveSegment::PostLink ()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool CurveSegment::Register (OutStream& target) const
{
	return Object::Register(target);
}
//----------------------------------------------------------------------------
void CurveSegment::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mUMin);
	target.Write(mUMax);

	PX2_END_DEBUG_STREAM_SAVE(CurveSegment, target);
}
//----------------------------------------------------------------------------
int CurveSegment::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += sizeof(mUMin);
	size += sizeof(mUMax);
	return size;
}
//----------------------------------------------------------------------------