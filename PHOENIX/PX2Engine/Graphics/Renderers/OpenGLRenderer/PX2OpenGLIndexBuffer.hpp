// PX2OpenGLIndexBuffer.hpp

#ifdef PX2_USE_OPENGL

#ifndef PX2OPENGLINDEXBUFFER_HPP
#define PX2OPENGLINDEXBUFFER_HPP

#include "PX2OpenGLRendererPre.hpp"
#include "PX2IndexBuffer.hpp"

namespace PX2
{

	class Renderer;

	class PdrIndexBuffer
	{
	public:
		PdrIndexBuffer(Renderer* renderer, const IndexBuffer* ibuffer);
		~PdrIndexBuffer();

		void Enable(Renderer* renderer);
		void Disable(Renderer* renderer);
		void* Lock(Buffer::Locking mode);
		void Unlock();

	private:
		GLuint mBuffer;
	};

}

#endif

#endif
