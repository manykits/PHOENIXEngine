// PX2OpenGLESVertexBuffer.hpp

#ifdef PX2_USE_OPENGLES

#ifndef PX2OPENGLES2VERTEXBUFFER_HPP
#define PX2OPENGLES2VERTEXBUFFER_HPP

#include "PX2OpenGLESRendererPre.hpp"
#include "PX2VertexBuffer.hpp"

namespace PX2
{

	class Renderer;

	class PdrVertexBuffer
	{
	public:
		PdrVertexBuffer (Renderer* renderer, const VertexBuffer* vbuffer);
		~PdrVertexBuffer ();

		void Enable (Renderer* renderer, unsigned int vertexSize,
			unsigned int streamIndex, unsigned int offset);
		void Disable (Renderer* renderer, unsigned int streamIndex);
		void* Lock (Buffer::Locking mode);
		void Unlock ();

	private:
		GLuint mBuffer;
		const VertexBuffer *mVBuffer;
	};

}

#endif

#endif