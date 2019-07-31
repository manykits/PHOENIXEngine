// PX2Dx9PixelShader.cpp

#ifdef PX2_USE_DX9

#include "PX2Dx9PixelShader.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrPixelShader::PdrPixelShader (Renderer* renderer,
								const PixelShader* pshader)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;

	// 编译汇编着色器代码
	const char* programText =
		pshader->GetProgram(PixelShader::GetProfile())->c_str();

	int shaderKey = pshader->GetShaderKey();
	assertion(0!=shaderKey, "shaderKey must not be 0.\n");
	std::map<int, Renderer::SharePdrData>::iterator it = renderer->mSharePdrPixelShaders.find(shaderKey);
	if (it != renderer->mSharePdrPixelShaders.end())
	{
		mShader = (IDirect3DPixelShader9*)(it->second.DataPtr);
		mShader->AddRef();
	}
	else
	{
		int programLength = (int)strlen(programText);
		LPD3DXBUFFER compiledShader = 0;
		LPD3DXBUFFER errors = 0;
		HRESULT hr = D3DXAssembleShader(programText, programLength, 0, 0, 0,
			&compiledShader, &errors);
#ifdef _DEBUG
		if (errors)
		{
			DWORD size = errors->GetBufferSize();
			PX2_UNUSED(size);
			char* data = (char*)errors->GetBufferPointer();
			PX2_UNUSED(data);
			assertion(false, "Failed to assemble pixel shader.\n");
		}
#endif
		PX2_UNUSED(hr);
		assertion(hr == D3D_OK && compiledShader,
			"Failed to assemble pixel shader: %s\n", DXGetErrorString(hr));

		// 创建像素着色器
		hr = device->CreatePixelShader( 
			(DWORD*)(compiledShader->GetBufferPointer()), &mShader);
		assertion(hr == D3D_OK, "Failed to create pixel shader\n");

		// 释放需要释放的数据
		if (compiledShader)
		{
			compiledShader->Release();
		}
		if (errors)
		{
			errors->Release();
		}
				
		renderer->mSharePdrPixelShaders[shaderKey].DataPtr = mShader;
		((IDirect3DPixelShader9*)renderer->mSharePdrPixelShaders[shaderKey].DataPtr)
			->AddRef();
	}
}
//----------------------------------------------------------------------------
PdrPixelShader::~PdrPixelShader ()
{
	mShader->Release();
}
//----------------------------------------------------------------------------
void PdrPixelShader::Enable (Renderer* renderer, const PixelShader* pshader,
							 const ShaderParameters* parameters)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;

	// 激活着色器
	HRESULT hr = device->SetPixelShader(mShader);
	assertion(hr == D3D_OK, "Failed to enable pixel shader: %s\n",
		DXGetErrorString(hr));

	// 设置着色器常量
	int profile = PixelShader::GetProfile();
	const int numConstants = pshader->GetNumConstants();
	int i;
	for (i = 0; i < numConstants; ++i)
	{
		hr = device->SetPixelShaderConstantF(
			pshader->GetBaseRegister(profile, i),
			parameters->GetConstant(i)->GetData(),
			pshader->GetNumRegistersUsed(i));
		assertion(hr == D3D_OK, "Failed to set shader constant: %s\n",
			DXGetErrorString(hr));
	}

	SetSamplerState(renderer, pshader, profile, parameters,
		renderer->mData->mMaxPShaderImages, 0,
		renderer->mData->mCurrentPSState);
}
//----------------------------------------------------------------------------
void PdrPixelShader::Disable (Renderer* renderer, const PixelShader* pshader,
							  const ShaderParameters* parameters)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;
	HRESULT hr;
	PX2_UNUSED(hr);

#ifdef PX2_PDR_DEBUG
	// 检测需要被取消激活的数据，是否匹配。
	IDirect3DPixelShader9* activePShader = 0;
	hr = device->GetPixelShader(&activePShader);
	assertion(hr == D3D_OK, "Failed to get pixel shader: %s\n",
		DXGetErrorString(hr));
	assertion(activePShader == mShader, "Mismatched pixel shaders\n");
	activePShader->Release();
#endif

	int profile = PixelShader::GetProfile();
	DisableTextures(renderer, pshader, profile, parameters,
		renderer->mData->mMaxPShaderImages, 0);

	// 取消激活着色器
	hr = device->SetPixelShader(0);
	assertion(hr == D3D_OK, "Failed to set pixel shader: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------

#endif