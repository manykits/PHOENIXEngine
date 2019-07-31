// PX2Buffer.cpp

#include "PX2Buffer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI_V(PX2, Object, Buffer, 1);
PX2_IMPLEMENT_STREAM(Buffer);
PX2_IMPLEMENT_ABSTRACT_FACTORY(Buffer);
PX2_IMPLEMENT_DEFAULT_NAMES(Object, Buffer);

//----------------------------------------------------------------------------
Buffer::Buffer ()
:
mNumElements(0),
mNumMaxElements(0),
mElementSize(0),
mUsage(Buffer::BU_QUANTITY),
mNumBytes(0),
mData(0),
PdrPointer(0)
{
}
//----------------------------------------------------------------------------
Buffer::Buffer (int numElements, int elementSize, Usage usage)
:
mNumElements(numElements),
mNumMaxElements(numElements),
mElementSize(elementSize),
mUsage(usage),
mNumBytes(numElements*elementSize),
PdrPointer(0)
{
	assertion(mNumElements > 0, "Number of elements must be positive\n");
	assertion(mElementSize > 0, "Element size must be positive\n");

	mData = new1<char>(mNumBytes);
	memset(mData, 0, mNumBytes);
}
//----------------------------------------------------------------------------
Buffer::~Buffer ()
{
	delete1(mData);
}
//----------------------------------------------------------------------------
void Buffer::ClearData ()
{
	delete1(mData);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
Buffer::Buffer (LoadConstructor value)
:
Object(value),
mNumElements(0),
mNumMaxElements(0),
mElementSize(0),
mUsage(Buffer::BU_QUANTITY),
mNumBytes(0),
mData(0),
PdrPointer(0)
{
}
//----------------------------------------------------------------------------
void Buffer::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Object::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mNumElements);

	int readedVersion = GetReadedVersion();
	if (1 <= readedVersion)
	{
		source.Read(mNumMaxElements);
	}	
	
	source.Read(mElementSize);
	source.ReadEnum(mUsage);
	source.ReadRR(mNumBytes, mData);

	PX2_END_DEBUG_STREAM_LOAD(Buffer, source);
}
//----------------------------------------------------------------------------
void Buffer::Link (InStream& source)
{
	Object::Link(source);
}
//----------------------------------------------------------------------------
void Buffer::PostLink ()
{
	Object::PostLink();
}
//----------------------------------------------------------------------------
bool Buffer::Register (OutStream& target) const
{
	return Object::Register(target);
}
//----------------------------------------------------------------------------
void Buffer::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Object::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mNumElements);
	target.Write(mNumMaxElements);
	target.Write(mElementSize);
	target.WriteEnum(mUsage);
	target.WriteW(mNumBytes, mData);

	PX2_END_DEBUG_STREAM_SAVE(Buffer, target);
}
//----------------------------------------------------------------------------
int Buffer::GetStreamingSize (Stream &stream) const
{
	int size = Object::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += sizeof(mNumElements);

	if (stream.IsIn())
	{
		int readedVersion = GetReadedVersion();
		if (1 <= readedVersion)
		{
			size += sizeof(mNumMaxElements);
		}
	}
	else
	{
		size += sizeof(mNumMaxElements);
	}

	size += sizeof(mElementSize);
	size += PX2_ENUMSIZE(mUsage);
	size += sizeof(mNumBytes);
	size += mNumBytes*sizeof(mData[0]);
	return size;
}
//------------------------------------------------------------------------