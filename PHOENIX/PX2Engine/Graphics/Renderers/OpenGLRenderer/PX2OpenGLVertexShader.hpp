// PX2OpenGLVertexShader.hpp

#ifdef PX2_USE_OPENGL

#ifndef PX2OPENGLVERTEXSHADER_H
#define PX2OPENGLVERTEXSHADER_H

#include "PX2OpenGLRendererPre.hpp"
#include "PX2OpenGLShader.hpp"
#include "PX2VertexShader.hpp"

namespace PX2
{

	class Renderer;

	class PdrVertexShader : public PdrShader
	{
	public:
		PdrVertexShader(Renderer* renderer, const VertexShader* vshader);
		~PdrVertexShader();

		void Enable(Renderer* renderer, const VertexShader* vshader,
			const ShaderParameters* parameters);
		void Disable(Renderer* renderer, const VertexShader* vshader,
			const ShaderParameters* parameters);

	private:
		GLuint mShader;
	};

}

#endif

#endif