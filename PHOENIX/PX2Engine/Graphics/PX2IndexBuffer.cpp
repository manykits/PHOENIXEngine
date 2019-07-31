// PX2IndexBuffer.cpp

#include "PX2IndexBuffer.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Buffer, IndexBuffer);
PX2_IMPLEMENT_STREAM(IndexBuffer);
PX2_IMPLEMENT_FACTORY(IndexBuffer);
PX2_IMPLEMENT_DEFAULT_NAMES(Buffer, IndexBuffer);

//----------------------------------------------------------------------------
IndexBuffer::IndexBuffer (int numIndices, int indexSize, Usage usage)
:
Buffer(numIndices, indexSize, usage),
mOffset(0)
{
}
//----------------------------------------------------------------------------
IndexBuffer::~IndexBuffer ()
{
	Renderer::UnbindAll(this);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
IndexBuffer::IndexBuffer (LoadConstructor value)
:
Buffer(value),
mOffset(0)
{
}
//----------------------------------------------------------------------------
void IndexBuffer::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Buffer::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mOffset);

	PX2_END_DEBUG_STREAM_LOAD(IndexBuffer, source);
}
//----------------------------------------------------------------------------
void IndexBuffer::Link (InStream& source)
{
	Buffer::Link(source);
}
//----------------------------------------------------------------------------
void IndexBuffer::PostLink ()
{
	Buffer::PostLink();
}
//----------------------------------------------------------------------------
bool IndexBuffer::Register (OutStream& target) const
{
	return Buffer::Register(target);
}
//----------------------------------------------------------------------------
void IndexBuffer::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Buffer::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mOffset);

	PX2_END_DEBUG_STREAM_SAVE(IndexBuffer, target);
}
//----------------------------------------------------------------------------
int IndexBuffer::GetStreamingSize (Stream &stream) const
{
	int size = Buffer::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += sizeof(mOffset);
	return size;
}
//----------------------------------------------------------------------------
