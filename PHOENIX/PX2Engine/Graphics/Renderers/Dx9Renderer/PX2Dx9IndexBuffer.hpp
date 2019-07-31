// PX2Dx9IndexBuffer.hpp

#ifdef PX2_USE_DX9

#ifndef PX2DX9INDEXBUFFER_HPP
#define PX2DX9INDEXBUFFER_HPP

#include "PX2Dx9RendererPre.hpp"
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
		IDirect3DIndexBuffer9* mBuffer;
	};

}
#endif

#endif