// PX2Polypoint.cpp

#include "PX2Polypoint.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Renderable, Polypoint);
PX2_IMPLEMENT_STREAM(Polypoint);
PX2_IMPLEMENT_FACTORY(Polypoint);
PX2_IMPLEMENT_DEFAULT_NAMES(Renderable, Polypoint);

//----------------------------------------------------------------------------
Polypoint::Polypoint (VertexFormat* vformat, VertexBuffer* vbuffer)
:
Renderable(PT_POLYPOINT, vformat, vbuffer, 0)
{
	mNumPoints = mVBuffer->GetNumElements();
}
//----------------------------------------------------------------------------
Polypoint::~Polypoint ()
{
}
//----------------------------------------------------------------------------
int Polypoint::GetMaxNumPoints () const
{
	return mVBuffer->GetNumElements();
}
//----------------------------------------------------------------------------
void Polypoint::SetNumPoints (int numPoints)
{
	int numVertices = mVBuffer->GetNumElements();
	if (0 <= numPoints && numPoints <= numVertices)
	{
		mNumPoints = numPoints;
	}
	else
	{
		mNumPoints = numVertices;
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ³Ö¾Ã»¯
//----------------------------------------------------------------------------
Polypoint::Polypoint (LoadConstructor value)
:
Renderable(value),
mNumPoints(0)
{
}
//----------------------------------------------------------------------------
void Polypoint::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Renderable::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mNumPoints);

	PX2_END_DEBUG_STREAM_LOAD(Polypoint, source);
}
//----------------------------------------------------------------------------
void Polypoint::Link (InStream& source)
{
	Renderable::Link(source);
}
//----------------------------------------------------------------------------
void Polypoint::PostLink ()
{
	Renderable::PostLink();
}
//----------------------------------------------------------------------------
bool Polypoint::Register (OutStream& target) const
{
	return Renderable::Register(target);
}
//----------------------------------------------------------------------------
void Polypoint::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Renderable::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mNumPoints);

	PX2_END_DEBUG_STREAM_SAVE(Polypoint, target);
}
//----------------------------------------------------------------------------
int Polypoint::GetStreamingSize (Stream &stream) const
{
	int size = Renderable::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += sizeof(mNumPoints);
	return size;
}
//----------------------------------------------------------------------------