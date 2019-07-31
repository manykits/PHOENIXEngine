// PX2OpenGLPixelShader.cpp

#ifdef PX2_USE_OPENGL

#include "PX2OpenGLPixelShader.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrPixelShader::PdrPixelShader(Renderer* renderer, const PixelShader* pshader)
{
	int shaderKey = pshader->GetShaderKey();
	assertion(0 != shaderKey, "shaderKey must not be 0.\n");

	std::map<int, Renderer::SharePdrData>::iterator it = renderer->mSharePdrPixelShaders.find(shaderKey);
	if (it != renderer->mSharePdrPixelShaders.end())
	{
		mShader = it->second.DataInt;
	}
	else
	{
		const char* programText =
			pshader->GetProgram(PixelShader::GetProfile())->c_str();
		int programLength = (int)strlen(programText);

		glEnable(GL_FRAGMENT_PROGRAM_ARB);
		glGenProgramsARB(1, &mShader);
		glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, mShader);
		glProgramStringARB(GL_FRAGMENT_PROGRAM_ARB, GL_PROGRAM_FORMAT_ASCII_ARB,
			programLength, programText);
		glDisable(GL_FRAGMENT_PROGRAM_ARB);

		Renderer::SharePdrData data;
		data.DataInt = mShader;
		renderer->mSharePdrPixelShaders[shaderKey] = data;
	}
}
//----------------------------------------------------------------------------
PdrPixelShader::~PdrPixelShader()
{
	// 现在不再这个地方释放，在Render析构时释放
	//glDeleteProgramsARB(1, &mShader);
}
//----------------------------------------------------------------------------
void PdrPixelShader::Enable(Renderer* renderer, const PixelShader* pshader,
	const ShaderParameters* parameters)
{
	// Enable the buffer by setting the state.
	glEnable(GL_FRAGMENT_PROGRAM_ARB);
	glBindProgramARB(GL_FRAGMENT_PROGRAM_ARB, mShader);

	// Set the shader constants.
	int profile = PixelShader::GetProfile();
	const int numConstants = pshader->GetNumConstants();
	for (int i = 0; i < numConstants; ++i)
	{
		const int numRegisters = pshader->GetNumRegistersUsed(i);
		const float* data = parameters->GetConstant(i)->GetData();
		int baseRegister = pshader->GetBaseRegister(profile, i);

		for (int j = 0; j < numRegisters; ++j)
		{
			glProgramLocalParameter4fvARB(GL_FRAGMENT_PROGRAM_ARB,
				(GLuint)baseRegister, data);
			baseRegister++;
			data += 4;
		}
	}

	SetSamplerState(renderer, pshader, profile, parameters,
		renderer->mData->mMaxPShaderImages, renderer->mData->mCurrentSS);
}
//----------------------------------------------------------------------------
void PdrPixelShader::Disable(Renderer* renderer, const PixelShader* pshader,
	const ShaderParameters* parameters)
{
	glDisable(GL_FRAGMENT_PROGRAM_ARB);
	int profile = PixelShader::GetProfile();
	DisableTextures(renderer, pshader, profile, parameters,
		renderer->mData->mMaxPShaderImages);
}
//----------------------------------------------------------------------------

#endif
