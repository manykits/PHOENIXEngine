// PX2OpenGLShader.hpp

#ifdef PX2_USE_OPENGL

#ifndef PX2OPENGLSHADER_HPP
#define PX2OPENGLSHADER_HPP

#include "PX2OpenGLRendererPre.hpp"
#include "PX2OpenGLRendererData.hpp"
#include "PX2Shader.hpp"
#include "PX2ShaderParameters.hpp"

namespace PX2
{

	class Renderer;

	class PdrShader
	{
	public:
		PdrShader();
		~PdrShader();

	protected:
		void SetSamplerState(Renderer* renderer, const Shader* shader,
			int profile, const ShaderParameters* parameters, int maxSamplers,
			RendererData::SamplerState* currentSS);

		void DisableTextures(Renderer* renderer, const Shader* shader,
			int profile, const ShaderParameters* parameters, int maxSamplers);
	};

}

#endif

#endif