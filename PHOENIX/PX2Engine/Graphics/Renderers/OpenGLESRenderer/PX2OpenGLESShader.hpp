// PX2OpenGLESShader.hpp

#ifdef PX2_USE_OPENGLES

#ifndef PX2OPENGLES2SHADER_HPP
#define PX2OPENGLES2SHADER_HPP

#include "PX2OpenGLESRendererPre.hpp"
#include "PX2OpenGLESRendererData.hpp"
#include "PX2Shader.hpp"
#include "PX2ShaderParameters.hpp"

namespace PX2
{

	class Renderer;

	class PdrShader
	{
	public:
		PdrShader ();
		~PdrShader ();

		GLuint GetShader () { return mShader; }

	protected:
		void SetSamplerState (Renderer* renderer, const Shader* shader,
			int profile, const ShaderParameters* parameters, int maxSamplers,
			RendererData::SamplerState* currentSS);

		void DisableTextures (Renderer* renderer, const Shader* shader,
			int profile, const ShaderParameters* parameters, int maxSamplers);

		GLuint mShader;
	};

}

#endif

#endif