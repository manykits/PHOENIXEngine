// PX2Dx9VertexFormat.hpp

#ifdef PX2_USE_DX9

#ifndef PX2DX9VERTEXFORMAT_HPP
#define PX2DX9VERTEXFORMAT_HPP

#include "PX2Dx9RendererPre.hpp"
#include "PX2VertexFormat.hpp"

namespace PX2
{

	class Renderer;

	class PdrVertexFormat
	{
	public:
		PdrVertexFormat (Renderer* renderer, const VertexFormat* vformat);
		~PdrVertexFormat ();

		void Enable (Renderer* renderer);
		void Disable (Renderer* renderer);

	private:
		D3DVERTEXELEMENT9 mElements[VertexFormat::AM_MAX_ATTRIBUTES + 1];
		IDirect3DVertexDeclaration9* mDeclaration;
	};

}

#endif

#endif