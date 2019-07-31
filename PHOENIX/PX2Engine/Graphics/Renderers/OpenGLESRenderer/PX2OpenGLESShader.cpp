// PX2OpenGLESShader.cpp

#ifdef PX2_USE_OPENGLES

#include "PX2OpenGLESShader.hpp"
#include "PX2OpenGLESMapping.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrShader::PdrShader ()
	:
mShader(0)
{
}
//----------------------------------------------------------------------------
PdrShader::~PdrShader ()
{
	glDeleteShader(mShader);
}
//----------------------------------------------------------------------------
void PdrShader::SetSamplerState (Renderer* renderer, const Shader* shader,
								 int profile,
								 const ShaderParameters* parameters, 
								 int maxSamplers,
								 RendererData::SamplerState* currentSS)
{
	int numSamplers = shader->GetNumSamplers();
	if (numSamplers > maxSamplers)
	{
		numSamplers = maxSamplers;
	}

	for (int i = 0; i < numSamplers; ++i)
	{
		Shader::SamplerType type = shader->GetSamplerType(i);
		GLenum target = gOGLTextureTarget[type];
		int textureUnit = shader->GetTextureUnit(profile, i);
		int textureLoc = shader->GetTextureLoc(i);
		const Texture* texture = parameters->GetTexture(i);
		RendererData::SamplerState& current = currentSS[textureUnit];

		switch (type)
		{
		case Shader::ST_1D:
			break;
		case Shader::ST_2D:
			{
				renderer->Enable((const Texture2D*)texture, textureUnit, textureLoc);
				current.GetCurrent(target);

				GLint wrap0 = gOGLWrapMode[shader->GetCoordinate(i, 0)];
				if (wrap0 != current.mWrap[0])
				{
					current.mWrap[0] = wrap0;
					glTexParameteri(target, GL_TEXTURE_WRAP_S, wrap0);
				}

				GLint wrap1 = gOGLWrapMode[shader->GetCoordinate(i, 1)];
				if (wrap1 != current.mWrap[1])
				{
					current.mWrap[1] = wrap1;
					glTexParameteri(target, GL_TEXTURE_WRAP_T, wrap1);
				}
				break;
			}
		case Shader::ST_3D:
			break;
		case Shader::ST_CUBE:
			break;
        default:
            break;
		}

		// Set the magfilter mode.
		Shader::SamplerFilter filter = shader->GetFilter(i);
		if (filter == Shader::SF_NEAREST)
		{
			if (GL_NEAREST != current.mMagFilter)
			{
				current.mMagFilter = GL_NEAREST;
				glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
		}
		else
		{
			if (GL_LINEAR != current.mMagFilter)
			{
				current.mMagFilter = GL_LINEAR;
				glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
		}

		// Set the minfilter mode.
		GLint minFilter = gOGLMinFilter[filter];
		if (minFilter != current.mMinFilter)
		{
			current.mMinFilter = minFilter;
			glTexParameteri(target, GL_TEXTURE_MIN_FILTER, minFilter);
		}
	}
}
//----------------------------------------------------------------------------
void PdrShader::DisableTextures (Renderer* renderer, const Shader* shader,
								 int profile,
								 const ShaderParameters* parameters,
								 int maxSamplers)
{
	int numSamplers = shader->GetNumSamplers();
	if (numSamplers > maxSamplers)
	{
		numSamplers = maxSamplers;
	}

	for (int i = 0; i < numSamplers; ++i)
	{
		Shader::SamplerType type = shader->GetSamplerType(i);
		int textureUnit = shader->GetTextureUnit(profile, i);
		const Texture* texture = parameters->GetTexture(i);

		switch (type)
		{
		case Shader::ST_1D:
			break;
		case Shader::ST_2D:
			{
				renderer->Disable((const Texture2D*)texture, textureUnit);
				break;
			}
		case Shader::ST_CUBE:
			break;
		case Shader::ST_3D:
			break;
		default:
			assertion(false, "Invalid sampler type\n");
			break;
		}
	}
}
//----------------------------------------------------------------------------

#endif

