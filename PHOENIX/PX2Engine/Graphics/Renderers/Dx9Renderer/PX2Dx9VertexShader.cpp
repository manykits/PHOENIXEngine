// PX2Dx9VertexShader.cpp

#ifdef PX2_USE_DX9

#include "PX2Dx9VertexShader.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrVertexShader::PdrVertexShader (Renderer* renderer,
								  const VertexShader* vshader)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;

	// 编译汇编着色器代码
	const char* programText =
		vshader->GetProgram(VertexShader::GetProfile())->c_str();

	int shaderKey = vshader->GetShaderKey();
	assertion(0!=shaderKey, "shaderKey must not be 0.\n");
	std::map<int, Renderer::SharePdrData>::iterator it = renderer->mSharePdrVertexShaders.find(shaderKey);
	if (it != renderer->mSharePdrVertexShaders.end())
	{
		mShader = (IDirect3DVertexShader9*)it->second.DataPtr;
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
			assertion(false, "Failed to assemble vertex shader.\n");
		}
#endif
		PX2_UNUSED(hr);
		assertion(hr == D3D_OK && compiledShader,
			"Failed to assemble vertex shader: %s\n", DXGetErrorString(hr));

		// 创建着色器
		hr = device->CreateVertexShader(
			(DWORD*)(compiledShader->GetBufferPointer()), &mShader);
		assertion(hr == D3D_OK, "Failed to create vertex shader\n");

		// 需要释放的buffers
		if (compiledShader)
		{
			compiledShader->Release();
		}
		if (errors)
		{
			errors->Release();
		}

		renderer->mSharePdrVertexShaders[shaderKey].DataPtr = mShader;
		((IDirect3DVertexShader9*)renderer->mSharePdrVertexShaders[shaderKey].DataPtr)
			->AddRef();
	}
}
//----------------------------------------------------------------------------
PdrVertexShader::~PdrVertexShader ()
{
	mShader->Release();
}
//----------------------------------------------------------------------------
void PdrVertexShader::Enable (Renderer* renderer,
							  const VertexShader* vshader, const ShaderParameters* parameters)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;

	// 激活着色器		
	HRESULT hr = device->SetVertexShader(mShader);
	assertion(hr == D3D_OK, "Failed to enable vertex shader: %s\n",
		DXGetErrorString(hr));

	// 设置着色器常量
	int profile = VertexShader::GetProfile();
	const int numConstants = vshader->GetNumConstants();
	int i;
	for (i = 0; i < numConstants; ++i)
	{
		hr = device->SetVertexShaderConstantF(
			vshader->GetBaseRegister(profile, i),
			parameters->GetConstant(i)->GetData(),
			vshader->GetNumRegistersUsed(i));
		assertion(hr == D3D_OK, "Failed to set shader constant: %s\n",
			DXGetErrorString(hr));
	}

	SetSamplerState(renderer, vshader, profile, parameters,
		renderer->mData->mMaxVShaderImages, D3DVERTEXTEXTURESAMPLER0,
		renderer->mData->mCurrentVSState);
}
//----------------------------------------------------------------------------
void PdrVertexShader::Disable (Renderer* renderer,
							   const VertexShader* vshader, const ShaderParameters* parameters)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;
	HRESULT hr;
	PX2_UNUSED(hr);

#ifdef PX2_PDR_DEBUG
	IDirect3DVertexShader9* activeVShader = 0;
	hr = device->GetVertexShader(&activeVShader);
	assertion(hr == D3D_OK, "Failed to get vertex shader: %s\n",
		DXGetErrorString(hr));
	assertion(activeVShader == mShader, "Mismatched vertex shaders\n");
	activeVShader->Release();
#endif

	int profile = VertexShader::GetProfile();
	DisableTextures(renderer, vshader, profile, parameters,
		renderer->mData->mMaxVShaderImages, D3DVERTEXTEXTURESAMPLER0);

	hr = device->SetVertexShader(0);
	assertion(hr == D3D_OK, "Failed to set vertex shader: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------

#endif