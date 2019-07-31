// PX2OpenGLPixelShader.hpp

#ifndef PX2OPENGLPIXELSHADER_HPP
#define PX2OPENGLPIXELSHADER_HPP

#ifdef PX2_USE_OPENGL

#include "PX2OpenGLRendererPre.hpp"
#include "PX2OpenGLShader.hpp"
#include "PX2PixelShader.hpp"

namespace PX2
{

	class Renderer;

	class PdrPixelShader : public PdrShader
	{
	public:
		PdrPixelShader(Renderer* renderer, const PixelShader* pshader);
		~PdrPixelShader();

		// Pixel shader operations.
		void Enable(Renderer* renderer, const PixelShader* pshader,
			const ShaderParameters* parameters);
		void Disable(Renderer* renderer, const PixelShader* pshader,
			const ShaderParameters* parameters);

	private:
		GLuint mShader;
	};

}

#endif

#endif
