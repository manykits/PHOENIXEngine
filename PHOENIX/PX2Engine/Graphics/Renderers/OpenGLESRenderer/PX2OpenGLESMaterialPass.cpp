// PX2OpenGLESMaterialPass.cpp

#ifdef PX2_USE_OPENGLES

#include "PX2OpenGLESMaterialPass.hpp"
#include "PX2Renderer.hpp"
#include "PX2OpenGLESVertexShader.hpp"
#include "PX2OpenGLESPixelShader.hpp"
#include "PX2Log.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrMaterialPass::PdrMaterialPass (Renderer *renderer, const MaterialPass *pass)
{
	mPass = pass;

	VertexShader *vshader = pass->GetVertexShader();
	PixelShader *pshader = pass->GetPixelShader();

	int shaderKey = vshader->GetShaderKey();
	std::map<int, Renderer::SharePdrData>::iterator it = 
		renderer->mSharePdrMaterialPasses.find(shaderKey);
	if (it != renderer->mSharePdrMaterialPasses.end())
	{
		mProgramObject = it->second.DataInt;

		int vProfile = VertexShader::GetProfile();

		int vNumConstants = vshader->GetNumConstants();
		for (int i=0; i<vNumConstants; ++i)
		{
			vshader->SetBaseRegister(vProfile, i, it->second.ES_V_CLoc[i]);
		}
		int numVTexs = vshader->GetNumSamplers();
		for (int i=0; i<numVTexs; i++)
		{
			vshader->SetTextureLoc(i, it->second.ES_V_TLoc[i]);
		}

		int pProfile = PixelShader::GetProfile();
		int pNumConstants = pshader->GetNumConstants();
		for (int i=0; i<pNumConstants; ++i)
		{
			pshader->SetBaseRegister(pProfile, i, it->second.ES_P_CLoc[i]);
		}
		int numPTexs = pshader->GetNumSamplers();
		for (int i=0; i<numPTexs; i++)
		{
			pshader->SetTextureLoc(i, it->second.ES_P_TLoc[i]);
		}
	}
	else
	{
		renderer->Bind(vshader);
		renderer->Bind(pshader);

		PdrVertexShader *pdrVS = renderer->GetResource(pass->GetVertexShader());
		PdrPixelShader *pdrPS = renderer->GetResource(pass->GetPixelShader());

		mProgramObject = glCreateProgram();
		assertion(mProgramObject != 0, "glCreateProgram failed.");

		GLint vShader = pdrVS->GetShader();
		GLint pShader = pdrPS->GetShader();
		PX2_GL_CHECK(glAttachShader (mProgramObject, vShader));
		PX2_GL_CHECK(glAttachShader (mProgramObject, pShader));

		glBindAttribLocation(mProgramObject, ALP_POSITION, "modelPosition");
		glBindAttribLocation(mProgramObject, ALP_NORMAL, "modelNormal");
		glBindAttribLocation(mProgramObject, ALP_COLOR0, "modelColor0");
		glBindAttribLocation(mProgramObject, ALP_TEXCOORD0, "modelTCoord0");
		glBindAttribLocation(mProgramObject, ALP_TEXCOORD1, "modelTCoord1");
		glBindAttribLocation(mProgramObject, ALP_TEXCOORD2, "modelTCoord2");
		glBindAttribLocation(mProgramObject, ALP_TEXCOORD3, "modelTCoord3");
		glBindAttribLocation(mProgramObject, ALP_TEXCOORD4, "modelTCoord4");

		// Link the program
		glLinkProgram (mProgramObject);

		// Check the link status
		GLint linked;
		glGetProgramiv(mProgramObject, GL_LINK_STATUS, &linked);

		if (!linked) 
		{
			GLint infoLen = 0;

			glGetProgramiv(mProgramObject, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen > 1)
			{
				char* infoLog = (char*)malloc(sizeof(char) * infoLen);

				glGetProgramInfoLog(mProgramObject, infoLen, NULL, infoLog);

				assertion(false, "Error linking program:\n%s\n", infoLog);
				PX2_LOG_ERROR("Error linking program:\n%s\n", infoLog);

				free(infoLog);
			}

			glDeleteProgram(mProgramObject);

			//PX2_LOG_ERROR("glLinkProgram failed.");
		}
		else
		{
			Renderer::SharePdrData data;

			data.DataInt = mProgramObject;

			int vProfile = VertexShader::GetProfile();
			int vNumConstants = vshader->GetNumConstants();
			for (int i=0; i<vNumConstants; ++i)
			{
				const std::string &name = vshader->GetConstantName(i);
				int location = glGetUniformLocation(mProgramObject, name.c_str());
				vshader->SetBaseRegister(vProfile, i, location);
				data.ES_V_CLoc[i] = (char)location;
			}
			int numVTexs = vshader->GetNumSamplers();
			for (int i=0; i<numVTexs; i++)
			{
				std::string sampleName = vshader->GetSamplerName(i);
				int location = glGetUniformLocation(mProgramObject, sampleName.c_str());
				vshader->SetTextureLoc(i, location);
				data.ES_V_TLoc[i] = (char)location;
			}

			int pProfile = PixelShader::GetProfile();
			int pNumConstants = pshader->GetNumConstants();
			for (int i=0; i<pNumConstants; ++i)
			{
				const std::string &name = pshader->GetConstantName(i);
				int location = glGetUniformLocation(mProgramObject, name.c_str());
				pshader->SetBaseRegister(pProfile, i, location);
				data.ES_P_CLoc[i] = (char)location;
			}
			int numPTexs = pshader->GetNumSamplers();
			for (int i=0; i<numPTexs; i++)
			{
				std::string sampleName = pshader->GetSamplerName(i);
				int location = glGetUniformLocation(mProgramObject, sampleName.c_str());
				pshader->SetTextureLoc(i, location);
				data.ES_P_TLoc[i] = (char)location;
			}

			renderer->mSharePdrMaterialPasses[shaderKey] = data;
		}
	}
}
//----------------------------------------------------------------------------
PdrMaterialPass::~PdrMaterialPass ()
{
	// 现在不再这个地方释放，在Render析构时释放
	//glDeleteProgram(mProgramObject);
}
//----------------------------------------------------------------------------
void PdrMaterialPass::Enable (Renderer* renderer)
{
	PX2_UNUSED(renderer);
	glUseProgram(mProgramObject);
}
//----------------------------------------------------------------------------
void PdrMaterialPass::Disable (Renderer* renderer)
{
	PX2_UNUSED(renderer);
}
//----------------------------------------------------------------------------

#endif
