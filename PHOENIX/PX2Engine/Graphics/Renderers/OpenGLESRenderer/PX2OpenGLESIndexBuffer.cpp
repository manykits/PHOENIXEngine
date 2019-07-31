// PX2OpenGLESIndexBuffer.cpp

#ifdef PX2_USE_OPENGLES

#include "PX2OpenGLESIndexBuffer.hpp"
#include "PX2OpenGLESMapping.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrIndexBuffer::PdrIndexBuffer (Renderer*, const IndexBuffer* ibuffer)
	:
mBuffer(0),
mIBuffer(ibuffer)
{
	PX2_GL_CHECK(glGenBuffers(1, &mBuffer));

	Lock(Buffer::BL_WRITE_ONLY);
}
//----------------------------------------------------------------------------
PdrIndexBuffer::~PdrIndexBuffer ()
{
	PX2_GL_CHECK(glDeleteBuffers(1, &mBuffer));
	mBuffer = 0;
	mIBuffer = 0;
}
//----------------------------------------------------------------------------
void PdrIndexBuffer::Enable (Renderer*)
{
	PX2_GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer));
}
//----------------------------------------------------------------------------
void PdrIndexBuffer::Disable (Renderer*)
{
	PX2_GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
//----------------------------------------------------------------------------
void* PdrIndexBuffer::Lock (Buffer::Locking mode)
{
	PX2_UNUSED(mode);

	PX2_GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer));

	PX2_GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIBuffer->GetNumBytes(), 
		mIBuffer->GetData(), gOGLBufferUsage[mIBuffer->GetUsage()]));

	PX2_GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	return 0;
}
//----------------------------------------------------------------------------
void PdrIndexBuffer::Unlock ()
{
}
//----------------------------------------------------------------------------

#endif
