// PX2Dx9VertexShader.hpp

#ifdef PX2_USE_DX9

#ifndef PX2DX9VERTEXSHADER_HPP
#define PX2DX9VERTEXSHADER_HPP

#include "PX2Dx9RendererPre.hpp"
#include "PX2Dx9Shader.hpp"
#include "PX2VertexShader.hpp"

namespace PX2
{

	class Renderer;

	class PdrVertexShader : public PdrShader
	{
	public:
		PdrVertexShader (Renderer* renderer, const VertexShader* vshader);
		~PdrVertexShader ();

		void Enable (Renderer* renderer, const VertexShader* vshader,
			const ShaderParameters* parameters);
		void Disable (Renderer* renderer, const VertexShader* vshader,
			const ShaderParameters* parameters);

	private:
		IDirect3DVertexShader9* mShader;
	};

}

#endif

#endif