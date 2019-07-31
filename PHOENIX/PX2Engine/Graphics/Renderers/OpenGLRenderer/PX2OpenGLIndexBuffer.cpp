// PX2OpenGLIndexBuffer.cpp

#ifdef PX2_USE_OPENGL

#include "PX2OpenGLIndexBuffer.hpp"
#include "PX2OpenGLMapping.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrIndexBuffer::PdrIndexBuffer(Renderer*, const IndexBuffer* ibuffer)
{
	glGenBuffers(1, &mBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibuffer->GetNumBytes(), 0,
		gOGLBufferUsage[ibuffer->GetUsage()]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	void* data = Lock(Buffer::BL_WRITE_ONLY);
	memcpy(data, ibuffer->GetData(), ibuffer->GetNumBytes());
	Unlock();
}
//----------------------------------------------------------------------------
PdrIndexBuffer::~PdrIndexBuffer()
{
	glDeleteBuffers(1, &mBuffer);
}
//----------------------------------------------------------------------------
void PdrIndexBuffer::Enable(Renderer*)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);
}
//----------------------------------------------------------------------------
void PdrIndexBuffer::Disable(Renderer*)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
//----------------------------------------------------------------------------
void* PdrIndexBuffer::Lock(Buffer::Locking mode)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);

	GLvoid* videoMemory = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,
		gOGLBufferLocking[mode]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return videoMemory;
}
//----------------------------------------------------------------------------
void PdrIndexBuffer::Unlock()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBuffer);

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
//----------------------------------------------------------------------------

#endif