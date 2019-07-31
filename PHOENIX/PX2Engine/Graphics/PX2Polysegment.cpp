// PX2Polysegment.cpp

#include "PX2Polysegment.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Renderable, Polysegment);
PX2_IMPLEMENT_STREAM(Polysegment);
PX2_IMPLEMENT_FACTORY(Polysegment);
PX2_IMPLEMENT_DEFAULT_NAMES(Renderable, Polysegment);

//----------------------------------------------------------------------------
Polysegment::Polysegment (VertexFormat* vformat, VertexBuffer* vbuffer,
						  bool contiguous)
						  :
Renderable(contiguous ? PT_POLYSEGMENTS_CONTIGUOUS : PT_POLYSEGMENTS_DISJOINT,
	   vformat, vbuffer, 0),
	   mContiguous(contiguous)
{
	int numVertices = mVBuffer->GetNumElements();
	assertion(numVertices >= 2,
		"Polysegments must have at least two points.\n");

	if (mContiguous)
	{
		mNumSegments = numVertices - 1;
	}
	else
	{
		assertion((numVertices & 1) == 0,
			"Disconnected segments require an even number of vertices.\n");

		mNumSegments = numVertices/2;
	}
}
//----------------------------------------------------------------------------
Polysegment::~Polysegment ()
{
}
//----------------------------------------------------------------------------
int Polysegment::GetMaxNumSegments () const
{
	int numVertices = mVBuffer->GetNumElements();
	return mContiguous ? numVertices - 1 : numVertices/2;
}
//----------------------------------------------------------------------------
void Polysegment::SetNumSegments (int numSegments)
{
	int numVertices = mVBuffer->GetNumElements();
	if (mContiguous)
	{
		int numVerticesM1 = numVertices - 1;
		if (0 <= numSegments && numSegments <= numVerticesM1)
		{
			mNumSegments = numSegments;
		}
		else
		{
			mNumSegments = numVerticesM1;
		}
	}
	else
	{
		int numVerticesD2 = numVertices/2;
		if (0 <= numSegments && numSegments <= numVerticesD2)
		{
			mNumSegments = numSegments;
		}
		else
		{
			mNumSegments = numVerticesD2;
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
Polysegment::Polysegment (LoadConstructor value)
:
Renderable(value),
mNumSegments(0),
mContiguous(false)
{
}
//----------------------------------------------------------------------------
void Polysegment::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Renderable::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mNumSegments);
	source.ReadBool(mContiguous);

	PX2_END_DEBUG_STREAM_LOAD(Polysegment, source);
}
//----------------------------------------------------------------------------
void Polysegment::Link (InStream& source)
{
	Renderable::Link(source);
}
//----------------------------------------------------------------------------
void Polysegment::PostLink ()
{
	Renderable::PostLink();
}
//----------------------------------------------------------------------------
bool Polysegment::Register (OutStream& target) const
{
	return Renderable::Register(target);
}
//----------------------------------------------------------------------------
void Polysegment::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Renderable::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mNumSegments);
	target.WriteBool(mContiguous);

	PX2_END_DEBUG_STREAM_SAVE(Polysegment, target);
}
//----------------------------------------------------------------------------
int Polysegment::GetStreamingSize (Stream &stream) const
{
	int size = Renderable::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += sizeof(mNumSegments);
	size += PX2_BOOLSIZE(mContiguous);
	return size;
}
//----------------------------------------------------------------------------
