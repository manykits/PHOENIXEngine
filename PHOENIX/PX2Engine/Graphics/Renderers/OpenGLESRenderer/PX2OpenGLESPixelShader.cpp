// PX2OpenGLESPixelShader.cpp

#ifdef PX2_USE_OPENGLES

#include "PX2OpenGLESPixelShader.hpp"
#include "PX2Renderer.hpp"
#include "PX2Log.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrPixelShader::PdrPixelShader (Renderer*, const PixelShader* pshader)
{
	int shaderKey = pshader->GetShaderKey();
	const char* programText =
		pshader->GetProgram(VertexShader::GetProfile())->c_str();
	int programLength = (int)strlen(programText);
    PX2_UNUSED(programLength);

	GLint compiled;

	// Create the mShader object
	mShader = glCreateShader(GL_FRAGMENT_SHADER);

	assertion(mShader != 0, "mShader create failed.");

	// Load the mShader source
	glShaderSource(mShader, 1, &programText, 0);

	// Compile the mShader
	glCompileShader(mShader);

	// Check the compile status
	glGetShaderiv(mShader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) 
	{
		GLint infoLen = 0;

		glGetShaderiv(mShader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = (char *)malloc(sizeof(char) * infoLen);

			glGetShaderInfoLog(mShader, infoLen, NULL, infoLog);
			assertion(false, "Error compiling ps: %d %s \n", shaderKey, infoLog);
			PX2_LOG_INFO("Error compiling ps: %d %s \n", shaderKey, infoLog);

			free(infoLog);
		}

		glDeleteShader(mShader);

		PX2_LOG_INFO("glCompileShader ps failed");
	}
}
//----------------------------------------------------------------------------
PdrPixelShader::~PdrPixelShader ()
{
}
//----------------------------------------------------------------------------
void PdrPixelShader::Enable (Renderer* renderer, const PixelShader* pshader,
							 const ShaderParameters* parameters)
{
	int profile = PixelShader::GetProfile();
	const int numConstants = pshader->GetNumConstants();
	for (int i = 0; i < numConstants; ++i)
	{
		const int numRegisters = pshader->GetNumRegistersUsed(i);
		const float* data = parameters->GetConstant(i)->GetData();
		int baseRegister = pshader->GetBaseRegister(profile, i);

		if (1 == numRegisters)
		{
			glUniform4fv((GLuint)baseRegister, 1, (const GLfloat*)data);
		}
		if (4 == numRegisters)
		{
			glUniformMatrix4fv((GLuint)baseRegister, 1, false, (const GLfloat*)data);
		}
		else
		{
			glUniform4fv((GLuint)baseRegister, numRegisters, (const GLfloat*)data);
		}
	}

	SetSamplerState(renderer, pshader, profile, parameters,
		renderer->mData->mMaxPShaderImages, renderer->mData->mCurrentSS);
}
//----------------------------------------------------------------------------
void PdrPixelShader::Disable (Renderer* renderer, const PixelShader* pshader,
							  const ShaderParameters* parameters)
{
	int profile = VertexShader::GetProfile();
	DisableTextures(renderer, pshader, profile, parameters,
		renderer->mData->mMaxVShaderImages);
}
//----------------------------------------------------------------------------

#endif
