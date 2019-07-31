// PX2Dx9VertexBuffer.hpp

#ifdef PX2_USE_DX9

#ifndef PX2DX9VERTEXBUFFER_HPP
#define PX2DX9VERTEXBUFFER_HPP

#include "PX2Dx9RendererPre.hpp"
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
		IDirect3DVertexBuffer9* mBuffer;
	};

}

#endif

#endif