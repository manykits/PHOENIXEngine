// PX2Dx9Shader.cpp

#ifdef PX2_USE_DX9

#include "PX2Dx9Shader.hpp"
#include "PX2Dx9Mapping.hpp"
#include "PX2Dx9RendererData.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrShader::PdrShader ()
{
}
//----------------------------------------------------------------------------
PdrShader::~PdrShader ()
{
}
//----------------------------------------------------------------------------
void PdrShader::SetSamplerState (Renderer* renderer, const Shader* shader,
								 int profile, 
								 const ShaderParameters* parameters, 
								 int maxSamplers,
								 DWORD base,
								 RendererData::SamplerState* currentSS)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;
	HRESULT hr;

	int numSamplers = shader->GetNumSamplers();
	if (numSamplers > maxSamplers)
	{
		numSamplers = maxSamplers;
	}

	for (int i = 0; i < numSamplers; ++i)
	{
		Shader::SamplerType type = shader->GetSamplerType(i);
		DWORD textureUnit = (DWORD)shader->GetTextureUnit(profile, i) + base;
		const Texture* texture = parameters->GetTexture(i);
		RendererData::SamplerState& current = currentSS[textureUnit];

		switch (type)
		{
		case Shader::ST_1D:
			{
				renderer->Enable((const Texture1D*)texture, textureUnit);

				DWORD wrap0 = gDX9WrapMode[shader->GetCoordinate(i, 0)];
				if (wrap0 != current.mWrap[0])
				{
					current.mWrap[0] = wrap0;
					hr = device->SetSamplerState(textureUnit, D3DSAMP_ADDRESSU,
						wrap0);
					assertion(hr == D3D_OK, "Failed to set wrap0: %s\n",
						DXGetErrorString(hr));
				}

				DWORD wrap1 = wrap0;
				if (wrap1 != current.mWrap[1])
				{
					current.mWrap[1] = wrap1;
					hr = device->SetSamplerState(textureUnit, D3DSAMP_ADDRESSV,
						wrap1);
					assertion(hr == D3D_OK, "Failed to set wrap1: %s\n",
						DXGetErrorString(hr));
				}
				break;
			}
		case Shader::ST_2D:
			{
				renderer->Enable((const Texture2D*)texture, textureUnit);

				DWORD wrap0 = gDX9WrapMode[shader->GetCoordinate(i, 0)];
				if (wrap0 != current.mWrap[0])
				{
					current.mWrap[0] = wrap0;
					hr = device->SetSamplerState(textureUnit, D3DSAMP_ADDRESSU,
						wrap0);
					assertion(hr == D3D_OK, "Failed to set wrap0: %s\n",
						DXGetErrorString(hr));
				}

				DWORD wrap1 = gDX9WrapMode[shader->GetCoordinate(i, 1)];
				if (wrap1 != current.mWrap[1])
				{
					current.mWrap[1] = wrap1;
					hr = device->SetSamplerState(textureUnit, D3DSAMP_ADDRESSV,
						wrap1);
					assertion(hr == D3D_OK, "Failed to set wrap1: %s\n",
						DXGetErrorString(hr));
				}
				break;
			}
		case Shader::ST_3D:
			{
				renderer->Enable((const Texture3D*)texture, textureUnit);

				DWORD wrap0 = gDX9WrapMode[shader->GetCoordinate(i, 0)];
				if (wrap0 != current.mWrap[0])
				{
					current.mWrap[0] = wrap0;
					hr = device->SetSamplerState(textureUnit, D3DSAMP_ADDRESSU,
						wrap0);
					assertion(hr == D3D_OK, "Failed to set wrap0: %s\n",
						DXGetErrorString(hr));
				}

				DWORD wrap1 = gDX9WrapMode[shader->GetCoordinate(i, 1)];
				if (wrap1 != current.mWrap[1])
				{
					current.mWrap[1] = wrap1;
					hr = device->SetSamplerState(textureUnit, D3DSAMP_ADDRESSV,
						wrap1);
					assertion(hr == D3D_OK, "Failed to set wrap1: %s\n",
						DXGetErrorString(hr));
				}

				DWORD wrap2 = gDX9WrapMode[shader->GetCoordinate(i, 2)];
				if (wrap2 != current.mWrap[2])
				{
					current.mWrap[2] = wrap2;
					hr = device->SetSamplerState(textureUnit, D3DSAMP_ADDRESSW,
						wrap2);
					assertion(hr == D3D_OK, "Failed to set wrap2: %s\n",
						DXGetErrorString(hr));
				}
				break;
			}
		case Shader::ST_CUBE:
			{
				renderer->Enable((const TextureCube*)texture, textureUnit);

				DWORD wrap0 = gDX9WrapMode[shader->GetCoordinate(i, 0)];
				if (wrap0 != current.mWrap[0])
				{
					current.mWrap[0] = wrap0;
					hr = device->SetSamplerState(textureUnit, D3DSAMP_ADDRESSU,
						wrap0);
					assertion(hr == D3D_OK, "Failed to set wrap0: %s\n",
						DXGetErrorString(hr));
				}

				DWORD wrap1 = gDX9WrapMode[shader->GetCoordinate(i, 1)];
				if (wrap1 != current.mWrap[1])
				{
					current.mWrap[1] = wrap1;
					hr = device->SetSamplerState(textureUnit, D3DSAMP_ADDRESSV,
						wrap1);
					assertion(hr == D3D_OK, "Failed to set wrap1: %s\n",
						DXGetErrorString(hr));
				}
				break;
			}
		default:
			assertion(false, "Invalid sampler type\n");
			break;
		}

		// 设置各向异性值
		const float maxAnisotropy = (float)Shader::MAX_ANISOTROPY;
		float anisotropy = shader->GetAnisotropy(i);
		if (anisotropy < 1.0f || anisotropy > maxAnisotropy)
		{
			anisotropy = 1.0f;
		}
		if (anisotropy != current.mAnisotropy)
		{
			current.mAnisotropy = anisotropy;
			hr = device->SetSamplerState(textureUnit, D3DSAMP_MAXANISOTROPY,
				(DWORD)anisotropy);
			assertion(hr == D3D_OK, "Failed to set anisotropy: %s\n",
				DXGetErrorString(hr));
		}

		// 设置LOD偏移
		float lodBias = shader->GetLodBias(i);
		if (lodBias != current.mLodBias)
		{
			current.mLodBias = lodBias;
			hr = device->SetSamplerState(textureUnit,
				D3DSAMP_MIPMAPLODBIAS, *(DWORD*)&lodBias);
			assertion(hr == D3D_OK, "Failed to set LOD bias: %s\n",
				DXGetErrorString(hr));
		}

		// 设置纹理过滤模式
		Shader::SamplerFilter filter = shader->GetFilter(i);
		if (filter == Shader::SF_NEAREST)
		{
			if (D3DTEXF_POINT != current.mMagFilter)
			{
				current.mMagFilter = D3DTEXF_POINT;
				hr = device->SetSamplerState(textureUnit,
					D3DSAMP_MAGFILTER, D3DTEXF_POINT);
				assertion(hr == D3D_OK, "Failed to set magfilter: %s\n",
					DXGetErrorString(hr));
			}
		}
		else
		{
			if (D3DTEXF_LINEAR != current.mMagFilter)
			{
				current.mMagFilter = D3DTEXF_LINEAR;
				hr = device->SetSamplerState(textureUnit,
					D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
				assertion(hr == D3D_OK, "Failed to set magfilter: %s\n",
					DXGetErrorString(hr));
			}
		}

		// 设置mipmap
		if (1.0f < anisotropy && anisotropy <= maxAnisotropy)
		{
			if (D3DTEXF_ANISOTROPIC != current.mMinFilter)
			{
				current.mMinFilter = D3DTEXF_ANISOTROPIC;
				hr = device->SetSamplerState(textureUnit, D3DSAMP_MINFILTER,
					D3DTEXF_ANISOTROPIC);
				assertion(hr == D3D_OK, "Failed to set minfilter: %s\n",
					DXGetErrorString(hr));
			}

			DWORD mipFilter = gDX9MipFilter[filter];
			if (mipFilter != current.mMipFilter)
			{
				current.mMipFilter = mipFilter;
				hr = device->SetSamplerState(textureUnit, D3DSAMP_MIPFILTER,
					mipFilter);
				assertion(hr == D3D_OK, "Failed to set mipfilter: %s\n",
					DXGetErrorString(hr));
			}
		}
		else
		{
			DWORD minFilter = gDX9MinFilter[filter];
			if (minFilter != current.mMinFilter)
			{
				current.mMinFilter = minFilter;
				hr = device->SetSamplerState(textureUnit, D3DSAMP_MINFILTER,
					minFilter);
				assertion(hr == D3D_OK, "Failed to set minfilter: %s\n",
					DXGetErrorString(hr));
			}

			DWORD mipFilter = gDX9MipFilter[filter];
			if (mipFilter != current.mMipFilter)
			{
				current.mMipFilter = mipFilter;
				hr = device->SetSamplerState(textureUnit, D3DSAMP_MIPFILTER,
					mipFilter);
				assertion(hr == D3D_OK, "Failed to set mipfilter: %s\n",
					DXGetErrorString(hr));
			}
		}

		// 设置夹取模式颜色
		Float4 color = shader->GetBorderColor(i);
		D3DCOLOR borderColor = D3DCOLOR_COLORVALUE(color[0], color[1],
			color[2], color[3]);
		if (borderColor != current.mBorderColor)
		{
			current.mBorderColor = borderColor;
			hr = device->SetSamplerState(textureUnit, D3DSAMP_BORDERCOLOR,
				borderColor);
			assertion(hr == D3D_OK, "Failed to set border color: %s\n",
				DXGetErrorString(hr));
		}
	}
}
//----------------------------------------------------------------------------
void PdrShader::DisableTextures (Renderer* renderer, const Shader* shader,
								 int profile, const ShaderParameters* parameters, int maxSamplers,
								 DWORD base)
{
	int numSamplers = shader->GetNumSamplers();
	if (numSamplers > maxSamplers)
	{
		numSamplers = maxSamplers;
	}

	for (int i = 0; i < numSamplers; ++i)
	{
		Shader::SamplerType type = shader->GetSamplerType(i);
		int textureUnit = shader->GetTextureUnit(profile, i) + base;
		const Texture* texture = parameters->GetTexture(i);

		switch (type)
		{
		case Shader::ST_1D:
			{
				renderer->Disable((const Texture1D*)texture, textureUnit);
				break;
			}
		case Shader::ST_2D:
			{
				renderer->Disable((const Texture2D*)texture, textureUnit);
				break;
			}
		case Shader::ST_CUBE:
			{
				renderer->Disable((const TextureCube*)texture, textureUnit);
				break;
			}
		case Shader::ST_3D:
			{
				renderer->Disable((const Texture3D*)texture, textureUnit);
				break;
			}
		default:
			assertion(false, "Invalid sampler type\n");
			break;
		}
	}
}
//----------------------------------------------------------------------------

#endif