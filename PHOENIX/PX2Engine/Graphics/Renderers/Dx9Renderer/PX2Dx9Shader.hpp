// PX2Dx9Shader.hpp

#ifdef PX2_USE_DX9

#ifndef PX2DX9SHADER_HPP
#define PX2DX9SHADER_HPP

#include "PX2Dx9RendererPre.hpp"
#include "PX2Dx9RendererData.hpp"
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

	protected:
		void SetSamplerState (Renderer* renderer, const Shader* shader,
			int profile, const ShaderParameters* parameters, int maxSamplers,
			DWORD base, RendererData::SamplerState* currentSS);

		void DisableTextures (Renderer* renderer, const Shader* shader,
			int profile, const ShaderParameters* parameters, int maxSamplers,
			DWORD base);
	};

}

#endif

#endif