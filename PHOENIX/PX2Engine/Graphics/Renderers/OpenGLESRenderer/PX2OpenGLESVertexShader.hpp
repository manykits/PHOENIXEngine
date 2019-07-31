// PX2OpenGLESVertexFormat.hpp

#ifdef PX2_USE_OPENGLES

#ifndef PX2OPENGLES2VERTEXSHADER_HPP
#define PX2OPENGLES2VERTEXSHADER_HPP

#include "PX2OpenGLESRendererPre.hpp"
#include "PX2OpenGLESShader.hpp"
#include "PX2VertexShader.hpp"

namespace PX2
{

	class Renderer;

	class PdrVertexShader : public PdrShader
	{
	public:
		PdrVertexShader (Renderer* renderer, const VertexShader* vshader);
		virtual ~PdrVertexShader ();

		void Enable (Renderer* renderer, const VertexShader* vshader,
			const ShaderParameters* parameters);
		void Disable (Renderer* renderer, const VertexShader* vshader,
			const ShaderParameters* parameters);
	};

}

#endif

#endif