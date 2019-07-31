// PX2Float2Array.cpp

#include "PX2Float2Array.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, Float2Array);
PX2_IMPLEMENT_STREAM(Float2Array);
PX2_IMPLEMENT_FACTORY(Float2Array);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, Float2Array);

//----------------------------------------------------------------------------
Float2Array::Float2Array (int numElements, Float2* elements)
:
mNumElements(numElements),
mElements(elements)
{
}
//----------------------------------------------------------------------------
Float2Array::~Float2Array ()
{
	delete1(mElements);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
Float2Array::Float2Array (LoadConstructor value)
:
Object(value),
mNumElements(0),
mElements(0)
{
}
//----------------------------------------------------------------------------
void Float2Array::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadAggregateRR(mNumElements, mElements);

	PX2_END_DEBUG_STREAM_LOAD(Float2Array, source);
}
//----------------------------------------------------------------------------
void Float2Array::Link (InStream& source)
{
	Object::Link(source);
}
//----------------------------------------------------------------------------
void Float2Array::PostLink ()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool Float2Array::Register (OutStream& target) const
{
	return Object::Register(target);
}
//----------------------------------------------------------------------------
void Float2Array::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteAggregateW(mNumElements, mElements);

	PX2_END_DEBUG_STREAM_SAVE(Float2Array, target);
}
//----------------------------------------------------------------------------
int Float2Array::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += sizeof(mNumElements);
	size += mNumElements*sizeof(mElements[0]);
	return size;
}
//----------------------------------------------------------------------------
