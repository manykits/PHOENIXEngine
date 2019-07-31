// PX2OpenGLESVertexShader.cpp

#ifdef PX2_USE_OPENGLES

#include "PX2OpenGLESVertexShader.hpp"
#include "PX2Renderer.hpp"
#include "PX2Log.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrVertexShader::PdrVertexShader (Renderer*, const VertexShader* vshader)
{
	int shaderKey = vshader->GetShaderKey();
	const char* programText =
		vshader->GetProgram(VertexShader::GetProfile())->c_str();
	int programLength = (int)strlen(programText);
    PX2_UNUSED(programLength);

	GLint compiled;

	// Create the mShader object
	mShader = glCreateShader(GL_VERTEX_SHADER);

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
			assertion(false, "Error compiling vs: %d %s \n", shaderKey, infoLog);      
			PX2_LOG_ERROR("Error compiling vs: %d %s \n", shaderKey, infoLog);

			free(infoLog);
		}

		glDeleteShader(mShader);

		PX2_LOG_ERROR("glCompileShader vs failed.");
	}
}
//----------------------------------------------------------------------------
PdrVertexShader::~PdrVertexShader ()
{
}
//----------------------------------------------------------------------------
void PdrVertexShader::Enable (Renderer* renderer, const VertexShader* vshader, 
	const ShaderParameters* parameters)
{
	int profile = VertexShader::GetProfile();
	const int numConstants = vshader->GetNumConstants();
	for (int i=0; i<numConstants; ++i)
	{
		const int numRegisters = vshader->GetNumRegistersUsed(i);
		float *data = parameters->GetConstant(i)->GetData();
		int baseRegister = vshader->GetBaseRegister(profile, i);

		if (1 == numRegisters)
		{
			glUniform4fv((GLuint)baseRegister, 1, (const GLfloat*)data);
		}
		else if (4 == numRegisters)
		{
			glUniformMatrix4fv((GLuint)baseRegister, 1, false, (const GLfloat*)data);
		}
		else
		{
			glUniform4fv((GLuint)baseRegister, numRegisters, (const GLfloat*)data);
		}
	}

	SetSamplerState(renderer, vshader, profile, parameters,
		renderer->mData->mMaxVShaderImages, renderer->mData->mCurrentSS);
}
//----------------------------------------------------------------------------
void PdrVertexShader::Disable (Renderer* renderer, const VertexShader* vshader,
	const ShaderParameters* parameters)
{
	int profile = VertexShader::GetProfile();
	DisableTextures(renderer, vshader, profile, parameters,
		renderer->mData->mMaxVShaderImages);
}
//----------------------------------------------------------------------------

#endif