// PX2OpenGLESIndexBuffer.hpp

#ifdef PX2_USE_OPENGLES

#ifndef PX2OPENGLES2INDEXBUFFER_HPP
#define PX2OPENGLES2INDEXBUFFER_HPP

#include "PX2OpenGLESRendererPre.hpp"
#include "PX2IndexBuffer.hpp"

namespace PX2
{

	class Renderer;

	class PdrIndexBuffer
	{
	public:
		PdrIndexBuffer (Renderer* renderer, const IndexBuffer* ibuffer);
		~PdrIndexBuffer ();

		void Enable (Renderer* renderer);
		void Disable (Renderer* renderer);
		void* Lock (Buffer::Locking mode);
		void Unlock ();

	private:
		GLuint mBuffer;
		const IndexBuffer *mIBuffer;
	};

}

#endif

#endif