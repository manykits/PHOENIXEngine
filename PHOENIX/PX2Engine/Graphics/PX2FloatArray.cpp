// PX2FloatArray.cpp

#include "PX2FloatArray.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Object, FloatArray);
PX2_IMPLEMENT_STREAM(FloatArray);
PX2_IMPLEMENT_FACTORY(FloatArray);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, FloatArray);

//----------------------------------------------------------------------------
FloatArray::FloatArray (int numElements, float* elements)
:
mNumElements(numElements),
mElements(elements)
{
}
//----------------------------------------------------------------------------
FloatArray::~FloatArray ()
{
	delete1(mElements);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
FloatArray::FloatArray (LoadConstructor value)
:
Object(value),
mNumElements(0),
mElements(0)
{
}
//----------------------------------------------------------------------------
void FloatArray::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.ReadRR(mNumElements, mElements);

	PX2_END_DEBUG_STREAM_LOAD(FloatArray, source);
}
//----------------------------------------------------------------------------
void FloatArray::Link (InStream& source)
{
	Object::Link(source);
}
//----------------------------------------------------------------------------
void FloatArray::PostLink ()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool FloatArray::Register (OutStream& target) const
{
	return Object::Register(target);
}
//----------------------------------------------------------------------------
void FloatArray::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.WriteW(mNumElements, mElements);

	PX2_END_DEBUG_STREAM_SAVE(FloatArray, target);
}
//----------------------------------------------------------------------------
int FloatArray::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += sizeof(mNumElements);
	size += mNumElements*sizeof(mElements[0]);
	return size;
}
//----------------------------------------------------------------------------
