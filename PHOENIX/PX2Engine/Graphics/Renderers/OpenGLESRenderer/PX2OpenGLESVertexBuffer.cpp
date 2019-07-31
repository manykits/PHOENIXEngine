// PX2OpenGLESVertexBuffer.cpp

#ifdef PX2_USE_OPENGLES

#include "PX2OpenGLESVertexBuffer.hpp"
#include "PX2OpenGLESMapping.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrVertexBuffer::PdrVertexBuffer (Renderer*, const VertexBuffer* vbuffer)
	:
mBuffer(0),
mVBuffer(vbuffer)
{
	PX2_GL_CHECK(glGenBuffers(1, &mBuffer));

	Lock(Buffer::BL_WRITE_ONLY);
}
//----------------------------------------------------------------------------
PdrVertexBuffer::~PdrVertexBuffer ()
{
	PX2_GL_CHECK(glDeleteBuffers(1, &mBuffer));
}
//----------------------------------------------------------------------------
void PdrVertexBuffer::Enable (Renderer*, unsigned int, unsigned int,
							  unsigned int)
{
	PX2_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mBuffer));
}
//----------------------------------------------------------------------------
void PdrVertexBuffer::Disable (Renderer*, unsigned int)
{
	PX2_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
//----------------------------------------------------------------------------
void* PdrVertexBuffer::Lock (Buffer::Locking mode)
{
	PX2_UNUSED(mode);

	PX2_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mBuffer));

	PX2_GL_CHECK(glBufferData(GL_ARRAY_BUFFER, mVBuffer->GetNumBytes(), mVBuffer->GetData(),
		gOGLBufferUsage[mVBuffer->GetUsage()]));

	PX2_GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

	return 0;
}
//----------------------------------------------------------------------------
void PdrVertexBuffer::Unlock ()
{
}
//----------------------------------------------------------------------------

#endif
