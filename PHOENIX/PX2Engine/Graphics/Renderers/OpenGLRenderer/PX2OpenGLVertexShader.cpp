// PX2OpenGLVertexShader.cpp

#ifdef PX2_USE_OPENGL

#include "PX2OpenGLVertexShader.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrVertexShader::PdrVertexShader(Renderer*renderer, const VertexShader* vshader)
{
	int shaderKey = vshader->GetShaderKey();
	assertion(0 != shaderKey, "shaderKey must not be 0.\n");

	std::map<int, Renderer::SharePdrData>::iterator it = renderer->mSharePdrVertexShaders.find(shaderKey);
	if (it != renderer->mSharePdrVertexShaders.end())
	{
		mShader = it->second.DataInt;
	}
	else
	{
		const char* programText =
			vshader->GetProgram(VertexShader::GetProfile())->c_str();
		int programLength = (int)strlen(programText);

		glEnable(GL_VERTEX_PROGRAM_ARB);
		glGenProgramsARB(1, &mShader);
		glBindProgramARB(GL_VERTEX_PROGRAM_ARB, mShader);
		glProgramStringARB(GL_VERTEX_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
			programLength, programText);
		glDisable(GL_VERTEX_PROGRAM_ARB);

		Renderer::SharePdrData data;
		data.DataInt = mShader;
		renderer->mSharePdrVertexShaders[shaderKey] = data;
	}
}
//----------------------------------------------------------------------------
PdrVertexShader::~PdrVertexShader()
{
	// 现在不再这个地方释放，在Render析构时释放
	//glDeleteProgramsARB(1, &mShader);
}
//----------------------------------------------------------------------------
void PdrVertexShader::Enable(Renderer* renderer,
	const VertexShader* vshader, const ShaderParameters* parameters)
{
	// Enable the buffer by setting the state.
	glEnable(GL_VERTEX_PROGRAM_ARB);
	glBindProgramARB(GL_VERTEX_PROGRAM_ARB, mShader);

	// Set the shader constants.
	int profile = VertexShader::GetProfile();
	const int numConstants = vshader->GetNumConstants();
	for (int i = 0; i < numConstants; ++i)
	{
		const int numRegisters = vshader->GetNumRegistersUsed(i);
		const float* data = parameters->GetConstant(i)->GetData();
		int baseRegister = vshader->GetBaseRegister(profile, i);
		for (int j = 0; j < numRegisters; ++j)
		{
			glProgramLocalParameter4fvARB(GL_VERTEX_PROGRAM_ARB,
				(GLuint)baseRegister, data);
			baseRegister++;
			data += 4;
		}
	}

	SetSamplerState(renderer, vshader, profile, parameters,
		renderer->mData->mMaxVShaderImages, renderer->mData->mCurrentSS);
}
//----------------------------------------------------------------------------
void PdrVertexShader::Disable(Renderer* renderer,
	const VertexShader* vshader, const ShaderParameters* parameters)
{
	glDisable(GL_VERTEX_PROGRAM_ARB);
	int profile = VertexShader::GetProfile();
	DisableTextures(renderer, vshader, profile, parameters,
		renderer->mData->mMaxVShaderImages);
}
//----------------------------------------------------------------------------

#endif
