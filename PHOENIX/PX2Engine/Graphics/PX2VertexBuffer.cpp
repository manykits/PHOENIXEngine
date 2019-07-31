// PX2VertexBuffer.cpp

#include "PX2VertexBuffer.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Buffer, VertexBuffer);
PX2_IMPLEMENT_STREAM(VertexBuffer);
PX2_IMPLEMENT_FACTORY(VertexBuffer);
PX2_IMPLEMENT_DEFAULT_NAMES(Buffer, VertexBuffer);
PX2_IMPLEMENT_DEFAULT_STREAM(Buffer, VertexBuffer);

//----------------------------------------------------------------------------
VertexBuffer::VertexBuffer (int numVertices, int vertexSize, Usage usage)
    :
    Buffer(numVertices, vertexSize, usage)
{
}
//----------------------------------------------------------------------------
VertexBuffer::~VertexBuffer ()
{
    Renderer::UnbindAll(this);
}
//----------------------------------------------------------------------------
