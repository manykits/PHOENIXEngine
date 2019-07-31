// PX2Dx9PixelShader.hpp

#ifdef PX2_USE_DX9

#ifndef PX2DX9PIXELSHADER_HPP
#define PX2DX9PIXELSHADER_HPP

#include "PX2Dx9RendererPre.hpp"
#include "PX2Dx9Shader.hpp"
#include "PX2PixelShader.hpp"

namespace PX2
{

	class Renderer;

	class PdrPixelShader : public PdrShader
	{
	public:
		PdrPixelShader (Renderer* renderer, const PixelShader* pshader);
		~PdrPixelShader ();

		void Enable (Renderer* renderer, const PixelShader* pshader,
			const ShaderParameters* parameters);
		void Disable (Renderer* renderer, const PixelShader* pshader,
			const ShaderParameters* parameters);

	private:
		IDirect3DPixelShader9* mShader;
	};

}

#endif

#endif