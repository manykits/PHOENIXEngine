// PX2Dx9Renderer.cpp

#ifdef PX2_USE_DX9

#include "PX2Renderer.hpp"
#include "PX2Dx9IndexBuffer.hpp"
#include "PX2Dx9PixelShader.hpp"
#include "PX2Dx9RenderTarget.hpp"
#include "PX2Dx9Shader.hpp"
#include "PX2Dx9Texture1D.hpp"
#include "PX2Dx9Texture2D.hpp"
#include "PX2Dx9Texture3D.hpp"
#include "PX2Dx9TextureCube.hpp"
#include "PX2Dx9VertexBuffer.hpp"
#include "PX2Dx9VertexFormat.hpp"
#include "PX2Dx9VertexShader.hpp"
#include "PX2Dx9Mapping.hpp"
#include "PX2Dx9RendererData.hpp"
#include "PX2Dx9RendererInput.hpp"
#include "PX2Polypoint.hpp"
#include "PX2Polysegment.hpp"
#include "PX2TriFan.hpp"
#include "PX2TriStrip.hpp"
#include "PX2TriMesh.hpp"
#include "PX2GraphicsRoot.hpp"

using namespace PX2;

//----------------------------------------------------------------------------
Renderer::Renderer (RendererInput *input, int width, int height,
					Texture::Format colorFormat, Texture::Format depthStencilFormat,
					int numMultisamples)
{
	Initialize(width, height, colorFormat, depthStencilFormat,
		numMultisamples);

	mData = new0 RendererData(input, width, height, colorFormat,
		depthStencilFormat, numMultisamples);

	// 设置缺省渲染状态和采样状态
	mData->SetDefaultProperties(mDefaultAlphaProperty, mDefaultCullProperty,
		mDefaultDepthProperty, mDefaultOffsetProperty, mDefaultStencilProperty,
		mDefaultWireProperty);
}
//----------------------------------------------------------------------------
Renderer::~Renderer ()
{
	delete0(mData);

	Terminate();
}
//----------------------------------------------------------------------------
Renderer *Renderer::CreateRenderer(void *ptData, void *ptDataLinuxWindow,
	void *ptDataLinuxVisual, int width, int height,
	int numMultisamples, RendererInput* &renderInput)
{
	PX2_UNUSED(ptDataLinuxWindow);
	PX2_UNUSED(ptDataLinuxVisual);

	PdrRendererInput *pdrRenderInput = new0 PdrRendererInput();
	renderInput = pdrRenderInput;

	Renderer *renderer = 0;
	Texture::Format colorFormat = Texture::TF_A8R8G8B8;
	Texture::Format depthStencilFormat = Texture::TF_D24S8;

	HWND hWnd = (HWND)ptData;

	pdrRenderInput->mWindowHandle = hWnd;
	pdrRenderInput->mDriver = Direct3DCreate9(D3D_SDK_VERSION);
	assertion(pdrRenderInput->mDriver != 0, "Failed to create Direct3D9\n");
	renderer = new0 Renderer(pdrRenderInput, width, height,
		colorFormat, depthStencilFormat, numMultisamples);

	renderer->SetClearColor(Float4::WHITE);

	return renderer;
}
//----------------------------------------------------------------------------
std::string Renderer::GetRenderTag()
{
	return "Dx9";
}
//----------------------------------------------------------------------------
void Renderer::InitRenderStates ()
{
	mData->SetDefaultProperties(mDefaultAlphaProperty, mDefaultCullProperty,
		mDefaultDepthProperty, mDefaultOffsetProperty, mDefaultStencilProperty,
		mDefaultWireProperty);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 渲染状态管理
//----------------------------------------------------------------------------
void Renderer::SetAlphaProperty (const AlphaProperty* alphaState)
{
	if (!mOverrideAlphaProperty)
	{
		mAlphaProperty = alphaState;
	}
	else
	{
		mAlphaProperty = mOverrideAlphaProperty;
	}

	HRESULT hr;
	if (mAlphaProperty->BlendEnabled)
	{
		if (TRUE != mData->mCurrentRS.mAlphaBlendEnable)
		{
			mData->mCurrentRS.mAlphaBlendEnable = TRUE;
			hr = mData->mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}

		DWORD srcBlend = gDX9AlphaSrcBlend[mAlphaProperty->SrcBlend];
		DWORD srcConstantColor = 0;
		if (srcBlend > 0)
		{
			srcConstantColor = 0;
		}
		else  // CONSTANT format
		{
			switch (mAlphaProperty->SrcBlend)
			{
			case AlphaProperty::SBM_CONSTANT_COLOR:
				srcBlend = D3DBLEND_BLENDFACTOR;
				srcConstantColor = D3DCOLOR_COLORVALUE(
					mAlphaProperty->ConstantColor[0],
					mAlphaProperty->ConstantColor[1],
					mAlphaProperty->ConstantColor[2],
					mAlphaProperty->ConstantColor[3]);
				break;
			case AlphaProperty::SBM_ONE_MINUS_CONSTANT_COLOR:
				srcBlend = D3DBLEND_INVBLENDFACTOR;
				srcConstantColor = D3DCOLOR_COLORVALUE(
					mAlphaProperty->ConstantColor[0],
					mAlphaProperty->ConstantColor[1],
					mAlphaProperty->ConstantColor[2],
					mAlphaProperty->ConstantColor[3]);
				break;
			case AlphaProperty::SBM_CONSTANT_ALPHA:
				srcBlend = D3DBLEND_BLENDFACTOR;
				srcConstantColor = D3DCOLOR_COLORVALUE(
					mAlphaProperty->ConstantColor[3],
					mAlphaProperty->ConstantColor[3],
					mAlphaProperty->ConstantColor[3],
					mAlphaProperty->ConstantColor[3]);
				break;
			case AlphaProperty::SBM_ONE_MINUS_CONSTANT_ALPHA:
				srcBlend = D3DBLEND_INVBLENDFACTOR;
				srcConstantColor = D3DCOLOR_COLORVALUE(
					mAlphaProperty->ConstantColor[3],
					mAlphaProperty->ConstantColor[3],
					mAlphaProperty->ConstantColor[3],
					mAlphaProperty->ConstantColor[3]);
				break;
			}
		}

		DWORD dstBlend = gDX9AlphaDstBlend[mAlphaProperty->DstBlend];
		DWORD dstConstantColor = 0;
		if (dstBlend > 0)
		{
			dstConstantColor = 0;
		}
		else  // CONSTANT format
		{
			switch (mAlphaProperty->DstBlend)
			{
			case AlphaProperty::DBM_CONSTANT_COLOR:
				dstBlend = D3DBLEND_BLENDFACTOR;
				dstConstantColor = D3DCOLOR_COLORVALUE(
					mAlphaProperty->ConstantColor[0],
					mAlphaProperty->ConstantColor[1],
					mAlphaProperty->ConstantColor[2],
					mAlphaProperty->ConstantColor[3]);
				break;
			case AlphaProperty::DBM_ONE_MINUS_CONSTANT_COLOR:
				dstBlend = D3DBLEND_INVBLENDFACTOR;
				dstConstantColor = D3DCOLOR_COLORVALUE(
					mAlphaProperty->ConstantColor[0],
					mAlphaProperty->ConstantColor[1],
					mAlphaProperty->ConstantColor[2],
					mAlphaProperty->ConstantColor[3]);
				break;
			case AlphaProperty::DBM_CONSTANT_ALPHA:
				dstBlend = D3DBLEND_BLENDFACTOR;
				dstConstantColor = D3DCOLOR_COLORVALUE(
					mAlphaProperty->ConstantColor[3],
					mAlphaProperty->ConstantColor[3],
					mAlphaProperty->ConstantColor[3],
					mAlphaProperty->ConstantColor[3]);
				break;
			case AlphaProperty::DBM_ONE_MINUS_CONSTANT_ALPHA:
				dstBlend = D3DBLEND_INVBLENDFACTOR;
				dstConstantColor = D3DCOLOR_COLORVALUE(
					mAlphaProperty->ConstantColor[3],
					mAlphaProperty->ConstantColor[3],
					mAlphaProperty->ConstantColor[3],
					mAlphaProperty->ConstantColor[3]);
				break;
			}
		}

		// DirectX不支持OpenGl的渲染模式GL_CONSTANT_ALPHA和GL_ONE_MINUS_CONSTANT_ALPHA。
		// 所以DirectX的颜色通道被设置为Alpha。
#ifdef _DEBUG
		if (srcBlend == D3DBLEND_BLENDFACTOR
			||  srcBlend == D3DBLEND_INVBLENDFACTOR)
		{
			if (dstBlend == D3DBLEND_BLENDFACTOR
				||  dstBlend == D3DBLEND_INVBLENDFACTOR)
			{
				assertion(srcConstantColor == dstConstantColor,
					"Src and dst colors should be the same\n");
			}
		}
#endif

		if (srcBlend != mData->mCurrentRS.mAlphaSrcBlend)
		{
			mData->mCurrentRS.mAlphaSrcBlend = srcBlend;
			hr = mData->mDevice->SetRenderState(D3DRS_SRCBLEND, srcBlend);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}

		if (dstBlend != mData->mCurrentRS.mAlphaDstBlend)
		{
			mData->mCurrentRS.mAlphaDstBlend = dstBlend;
			hr = mData->mDevice->SetRenderState(D3DRS_DESTBLEND, dstBlend);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}

		if (srcConstantColor != mData->mCurrentRS.mBlendFactor)
		{
			mData->mCurrentRS.mBlendFactor = srcConstantColor;
			hr = mData->mDevice->SetRenderState(D3DRS_BLENDFACTOR,
				srcConstantColor);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}
	else
	{
		if (FALSE != mData->mCurrentRS.mAlphaBlendEnable)
		{
			mData->mCurrentRS.mAlphaBlendEnable = FALSE;
			hr = mData->mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,
				FALSE);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}

	if (mAlphaProperty->CompareEnabled)
	{
		if (TRUE != mData->mCurrentRS.mAlphaTestEnable)
		{
			mData->mCurrentRS.mAlphaTestEnable = TRUE;
			hr = mData->mDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}

		DWORD alphaFunc = gDX9AlphaCompare[mAlphaProperty->Compare];
		if (alphaFunc != mData->mCurrentRS.mAlphaFunc)
		{
			mData->mCurrentRS.mAlphaFunc = alphaFunc;
			hr = mData->mDevice->SetRenderState(D3DRS_ALPHAFUNC, alphaFunc);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}

		DWORD alphaRef = (DWORD)(255.0f*mAlphaProperty->Reference);
		if (alphaRef != mData->mCurrentRS.mAlphaRef)
		{
			mData->mCurrentRS.mAlphaRef = alphaRef;
			hr = mData->mDevice->SetRenderState(D3DRS_ALPHAREF, alphaRef);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}
	else
	{
		if (FALSE != mData->mCurrentRS.mAlphaTestEnable)
		{
			mData->mCurrentRS.mAlphaTestEnable = FALSE;
			hr = mData->mDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}
}
//----------------------------------------------------------------------------
void Renderer::SetCullProperty (const CullProperty* cullState)
{
	if (!mOverrideCullProperty)
	{
		mCullProperty = cullState;
	}
	else
	{
		mCullProperty = mOverrideCullProperty;
	}

	HRESULT hr;
	if (mCullProperty->Enabled)
	{
		int index = (mCullProperty->CCWOrder ? 1 : 0);
		if (mReverseCullOrder)
		{
			index = 1 - index;
		}

		DWORD cullMode = gDX9CullOrder[mCullProperty->CCWOrder ? 1 : 0];
		if (cullMode != mData->mCurrentRS.mCullMode)
		{
			mData->mCurrentRS.mCullMode = cullMode;
			hr = mData->mDevice->SetRenderState(D3DRS_CULLMODE, cullMode);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}
	else
	{
		if (D3DCULL_NONE != mData->mCurrentRS.mCullMode)
		{
			mData->mCurrentRS.mCullMode = D3DCULL_NONE;
			hr = mData->mDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}
}
//----------------------------------------------------------------------------
void Renderer::SetDepthProperty (const DepthProperty* depthState)
{
	if (!mOverrideDepthProperty)
	{
		mDepthProperty = depthState;
	}
	else
	{
		mDepthProperty = mOverrideDepthProperty;
	}

	HRESULT hr;
	if (mDepthProperty->Enabled)
	{
		if (D3DZB_TRUE != mData->mCurrentRS.mZEnable)
		{
			mData->mCurrentRS.mZEnable = D3DZB_TRUE;
			hr = mData->mDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}

		DWORD zFunc = gDX9DepthCompare[mDepthProperty->Compare];
		if (zFunc != mData->mCurrentRS.mZFunc)
		{
			mData->mCurrentRS.mZFunc = zFunc;
			hr = mData->mDevice->SetRenderState(D3DRS_ZFUNC, zFunc);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}
	else
	{
		if (D3DZB_FALSE != mData->mCurrentRS.mZEnable)
		{
			mData->mCurrentRS.mZEnable = D3DZB_FALSE;
			hr = mData->mDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}

	if (mDepthProperty->Writable)
	{
		if (TRUE != mData->mCurrentRS.mZWriteEnable)
		{
			mData->mCurrentRS.mZWriteEnable = TRUE;
			hr = mData->mDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}
	else
	{
		if (FALSE != mData->mCurrentRS.mZWriteEnable)
		{
			mData->mCurrentRS.mZWriteEnable = FALSE;
			hr = mData->mDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}
}
//----------------------------------------------------------------------------
void Renderer::SetOffsetProperty (const OffsetProperty* offsetState)
{
	if (!mOverrideOffsetProperty)
	{
		mOffsetProperty = offsetState;
	}
	else
	{
		mOffsetProperty = mOverrideOffsetProperty;
	}

	SetColorMask(offsetState->AllowGreen, offsetState->AllowBlue,
		offsetState->AllowGreen, offsetState->AllowAlpha);

	// LineEnabled和PointEnabled被Dx9 Render忽略，因为Dx9不支持这两种几何
	// 图形的Offset。

	HRESULT hr;
	if (mOffsetProperty->FillEnabled)
	{
		DWORD slopeScaleDepthBias = *(DWORD*)&mOffsetProperty->Scale;
		if (slopeScaleDepthBias != mData->mCurrentRS.mSlopeScaleDepthBias)
		{
			mData->mCurrentRS.mSlopeScaleDepthBias = slopeScaleDepthBias;
			hr = mData->mDevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS,
				slopeScaleDepthBias);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}

		// TODO. 当前的渲染器总是创建24-bit的深度缓冲区。如果精度改变了，bias的
		// 调整依赖于深度的进度。除数为2^n，n表示深度的精度。
		float bias = mOffsetProperty->Bias/16777216.0f;
		DWORD depthBias = *(DWORD*)&bias;
		if (depthBias != mData->mCurrentRS.mDepthBias)
		{
			mData->mCurrentRS.mDepthBias = depthBias;
			hr = mData->mDevice->SetRenderState(D3DRS_DEPTHBIAS, depthBias);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}
	else
	{
		if (0 != mData->mCurrentRS.mSlopeScaleDepthBias)
		{
			mData->mCurrentRS.mSlopeScaleDepthBias = 0;
			hr = mData->mDevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}

		if (0 != mData->mCurrentRS.mDepthBias)
		{
			mData->mCurrentRS.mDepthBias = 0;
			hr = mData->mDevice->SetRenderState(D3DRS_DEPTHBIAS, 0);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}
}
//----------------------------------------------------------------------------
void Renderer::SetStencilProperty (const StencilProperty* stencilState)
{
	if (!mOverrideStencilProperty)
	{
		mStencilProperty = stencilState;
	}
	else
	{
		mStencilProperty = mOverrideStencilProperty;
	}

	HRESULT hr;
	if (mStencilProperty->Enabled)
	{
		if (TRUE != mData->mCurrentRS.mStencilEnable)
		{
			mData->mCurrentRS.mStencilEnable = TRUE;
			hr = mData->mDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}

		DWORD stencilFunc = gDX9StencilCompare[mStencilProperty->Compare];
		if (stencilFunc != mData->mCurrentRS.mStencilFunc)
		{
			mData->mCurrentRS.mStencilFunc = stencilFunc;
			hr = mData->mDevice->SetRenderState(D3DRS_STENCILFUNC,
				stencilFunc);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}

		DWORD stencilRef = (DWORD)mStencilProperty->Reference;
		if (stencilRef != mData->mCurrentRS.mStencilRef)
		{
			mData->mCurrentRS.mStencilRef = stencilRef;
			hr = mData->mDevice->SetRenderState(D3DRS_STENCILREF, stencilRef);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}

		DWORD stencilMask = (DWORD)mStencilProperty->Mask;
		if (stencilMask != mData->mCurrentRS.mStencilMask)
		{
			mData->mCurrentRS.mStencilMask = stencilMask;
			hr = mData->mDevice->SetRenderState(D3DRS_STENCILMASK,
				stencilMask);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}

		DWORD stencilWriteMask = (DWORD)mStencilProperty->WriteMask;
		if (stencilWriteMask != mData->mCurrentRS.mStencilWriteMask)
		{
			mData->mCurrentRS.mStencilWriteMask = stencilWriteMask;
			hr = mData->mDevice->SetRenderState(D3DRS_STENCILWRITEMASK,
				stencilWriteMask);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}

		DWORD stencilFail = gDX9StencilOperation[mStencilProperty->OnFail];
		if (stencilFail != mData->mCurrentRS.mStencilFail)
		{
			mData->mCurrentRS.mStencilFail = stencilFail;
			hr = mData->mDevice->SetRenderState(D3DRS_STENCILFAIL,
				stencilFail);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}

		DWORD stencilZFail = gDX9StencilOperation[mStencilProperty->OnZFail];
		if (stencilZFail != mData->mCurrentRS.mStencilZFail)
		{
			mData->mCurrentRS.mStencilZFail = stencilZFail;
			hr = mData->mDevice->SetRenderState(D3DRS_STENCILZFAIL,
				stencilZFail);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}

		DWORD stencilPass = gDX9StencilOperation[mStencilProperty->OnZPass];
		if (stencilPass != mData->mCurrentRS.mStencilPass)
		{
			mData->mCurrentRS.mStencilPass = stencilPass;
			hr = mData->mDevice->SetRenderState(D3DRS_STENCILPASS,
				stencilPass);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}
	else
	{
		if (FALSE != mData->mCurrentRS.mStencilEnable)
		{
			mData->mCurrentRS.mStencilEnable = FALSE;
			hr = mData->mDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}
}
//----------------------------------------------------------------------------
void Renderer::SetWireProperty (const WireProperty* wireState)
{
	if (!mOverrideWireProperty)
	{
		mWireProperty = wireState;
	}
	else
	{
		mWireProperty = mOverrideWireProperty;
	}

	HRESULT hr;
	if (mWireProperty->Enabled)
	{
		if (D3DFILL_WIREFRAME != mData->mCurrentRS.mFillMode)
		{
			mData->mCurrentRS.mFillMode = D3DFILL_WIREFRAME;
			hr = mData->mDevice->SetRenderState(D3DRS_FILLMODE,
				D3DFILL_WIREFRAME);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}
	else
	{
		if (D3DFILL_SOLID != mData->mCurrentRS.mFillMode)
		{
			mData->mCurrentRS.mFillMode = D3DFILL_SOLID;
			hr = mData->mDevice->SetRenderState(D3DRS_FILLMODE,
				D3DFILL_SOLID);
			assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
				DXGetErrorString(hr));
		}
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 视口管理
//----------------------------------------------------------------------------
void Renderer::SetViewport (int xPosition, int yPosition, int width,
							int height)
{
	D3DVIEWPORT9 viewport;
	HRESULT hr = mData->mDevice->GetViewport(&viewport);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to get old viewport: %s\n",
		DXGetErrorString(hr));

	// DirectX的ViewPort是左手坐标系的。原点在左上角，y轴向下，x轴向右。我们需
	// 将右手坐标系转换为directx的左手坐标系。
	viewport.X = (DWORD)xPosition;
	viewport.Y = (DWORD)(mHeight - (yPosition + height));
	viewport.Width = (DWORD)width;
	viewport.Height = (DWORD)height;
	hr = mData->mDevice->SetViewport(&viewport);
	assertion(hr == D3D_OK, "Failed to set new viewport: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void Renderer::GetViewport (int& xPosition, int& yPosition,
							int& width, int& height) const
{
	D3DVIEWPORT9 viewport;
	HRESULT hr = mData->mDevice->GetViewport(&viewport);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to get viewport: %s\n",
		DXGetErrorString(hr));

	// DirectX的ViewPort是左手坐标系的。原点在左上角，y轴向下，x轴向右。我们需
	// 要将其变换到右手坐标系，其原点在左下角。
	xPosition = (int)viewport.X;
	yPosition = mHeight - ((int)viewport.Y + (int)viewport.Height);
	width = (int)viewport.Width;
	height = (int)viewport.Height;
}
//----------------------------------------------------------------------------
void Renderer::SetDepthRange (float zMin, float zMax)
{
	D3DVIEWPORT9 viewport;
	HRESULT hr = mData->mDevice->GetViewport(&viewport);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to get old viewport: %s\n",
		DXGetErrorString(hr));

	viewport.MinZ = zMin;
	viewport.MaxZ = zMax;
	hr = mData->mDevice->SetViewport(&viewport);
	assertion(hr == D3D_OK, "Failed to set new viewport: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void Renderer::GetDepthRange (float& zMin, float& zMax) const
{
	D3DVIEWPORT9 viewport;
	HRESULT hr = mData->mDevice->GetViewport(&viewport);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to get viewport: %s\n",
		DXGetErrorString(hr));

	zMin = viewport.MinZ;
	zMax = viewport.MaxZ;
}
//----------------------------------------------------------------------------
void Renderer::Resize (int width, int height)
{
	D3DVIEWPORT9 viewport;
	HRESULT hr = mData->mDevice->GetViewport(&viewport);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to get old viewport: %s\n",
		DXGetErrorString(hr));

	viewport.Width = (DWORD)width;
	viewport.Height = (DWORD)height;
	hr = mData->mDevice->SetViewport(&viewport);
	assertion(hr == D3D_OK, "Failed to set new viewport: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------

template <typename Resource, typename PdrResource>
void DestroyResources (
					   std::map<const Resource*, PdrResource*>& rmap,
					   std::set<const Resource*>& save)
{
	std::map<const Resource*, PdrResource*>::iterator iter = rmap.begin();
	std::map<const Resource*, PdrResource*>::iterator end = rmap.end();
	for (/**/; iter != end; ++iter)
	{
		delete0(iter->second);
		save.insert(iter->first);
	}
	rmap.clear();
}
//----------------------------------------------------------------------------
template <typename Resource>
void RecreateResources (Renderer* renderer,
						const std::set<const Resource*>& save)
{
	std::set<const Resource*>::const_iterator iter = save.begin();
	std::set<const Resource*>::const_iterator end = save.end();
	for (/**/; iter != end; ++iter)
	{
		renderer->Bind(*iter);
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 深度，颜色，模板缓冲区Clear
//----------------------------------------------------------------------------
void Renderer::ResizeWindow (int width, int height)
{
	if (width<=0 || height<=0)
		return;

	OnLostDevice();

	mWidth = width;
	mHeight = height;
	mData->mPresent.BackBufferWidth = mWidth;
	mData->mPresent.BackBufferHeight = mHeight;

	mData->mDeviceLost = true;
	mData->mFont->OnLostDevice();

	mData->DestroyUniqueFont();

	// 重新置回设备
	HRESULT hr = mData->mDevice->Reset(&mData->mPresent);
	if (hr != D3D_OK)
	{
		assertion(false, "Reset failed: %s\n", DXGetErrorString(hr));
		return;
	}

	mData->SetDefaultProperties(mDefaultAlphaProperty, mDefaultCullProperty,
		mDefaultDepthProperty, mDefaultOffsetProperty, mDefaultStencilProperty,
		mDefaultWireProperty);

	// 重新创建资源
	mData->CreateUniqueFont();
}
//----------------------------------------------------------------------------
// 深度，颜色，模板缓冲区Clear
//----------------------------------------------------------------------------
void Renderer::ClearColorBuffer ()
{
	DWORD clearColor = D3DCOLOR_COLORVALUE(mClearColor[0], mClearColor[1],
		mClearColor[2], mClearColor[3]);

	HRESULT hr = mData->mDevice->Clear(0, 0, D3DCLEAR_TARGET, clearColor,
		1.0f, 0);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Clear failed: %s\n", DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void Renderer::ClearDepthBuffer ()
{
	HRESULT hr = mData->mDevice->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, mClearDepth,
		0);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Clear failed: %s\n", DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void Renderer::ClearStencilBuffer ()
{
	HRESULT hr = mData->mDevice->Clear(0, 0, D3DCLEAR_STENCIL, 0, 1.0f,
		(DWORD)mClearStencil);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Clear failed: %s\n", DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void Renderer::ClearBuffers ()
{
	DWORD clearColor = D3DCOLOR_COLORVALUE(mClearColor[0], mClearColor[1],
		mClearColor[2], mClearColor[3]);

	HRESULT hr = mData->mDevice->Clear(0, 0,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		clearColor, mClearDepth, (DWORD)mClearStencil);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Clear failed: %s\n", DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void Renderer::ClearColorBuffer (int x, int y, int w, int h)
{
	D3DRECT rect;
	rect.x1 = (long)x;
	rect.y1 = (long)y;
	rect.x2 = (long)(x + w - 1);
	rect.y2 = (long)(y + h - 1);

	DWORD clearColor = D3DCOLOR_COLORVALUE(mClearColor[0], mClearColor[1],
		mClearColor[2], mClearColor[3]);

	HRESULT hr = mData->mDevice->Clear(1, &rect, D3DCLEAR_TARGET, clearColor,
		1.0f, 0);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Clear failed: %s\n", DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void Renderer::ClearDepthBuffer (int x, int y, int w, int h)
{
	D3DRECT rect;
	rect.x1 = (long)x;
	rect.y1 = (long)y;
	rect.x2 = (long)(x + w - 1);
	rect.y2 = (long)(y + h - 1);

	HRESULT hr = mData->mDevice->Clear(1, &rect, D3DCLEAR_ZBUFFER, 0,
		mClearDepth, 0);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Clear failed: %s\n", DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void Renderer::ClearStencilBuffer (int x, int y, int w, int h)
{
	D3DRECT rect;
	rect.x1 = (long)x;
	rect.y1 = (long)y;
	rect.x2 = (long)(x + w - 1);
	rect.y2 = (long)(y + h - 1);

	HRESULT hr = mData->mDevice->Clear(1, &rect, D3DCLEAR_STENCIL, 0, 1.0f,
		(DWORD)mClearStencil);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Clear failed: %s\n", DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void Renderer::ClearBuffers (int x, int y, int w, int h)
{
	D3DRECT rect;
	rect.x1 = (long)x;
	rect.y1 = (long)y;
	rect.x2 = (long)(x + w - 1);
	rect.y2 = (long)(y + h - 1);

	DWORD clearColor = D3DCOLOR_COLORVALUE(mClearColor[0], mClearColor[1],
		mClearColor[2], mClearColor[3]);

	HRESULT hr = mData->mDevice->Clear(1, &rect,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		clearColor, mClearDepth, (DWORD)mClearStencil);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Clear failed: %s\n", DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void Renderer::DisplayColorBuffer ()
{
	HRESULT hr = mData->mDevice->Present(0, 0, 0, 0);
	if (hr != D3DERR_DEVICELOST)
	{
		assertion(hr == D3D_OK, "Present failed: %s\n",
			DXGetErrorString(hr));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 颜色mask支持
//----------------------------------------------------------------------------
void Renderer::SetColorMask (bool allowRed, bool allowGreen,
							 bool allowBlue, bool allowAlpha)
{
	mAllowRed = allowRed;
	mAllowGreen = allowGreen;
	mAllowBlue = allowBlue;
	mAllowAlpha = allowAlpha;

	DWORD mask = 0;

	if (mAllowRed)
	{
		mask |= D3DCOLORWRITEENABLE_RED;
	}

	if (mAllowGreen)
	{
		mask |= D3DCOLORWRITEENABLE_GREEN;
	}

	if (mAllowBlue)
	{
		mask |= D3DCOLORWRITEENABLE_BLUE;
	}

	if (mAllowAlpha)
	{
		mask |= D3DCOLORWRITEENABLE_ALPHA;
	}

	HRESULT hr = mData->mDevice->SetRenderState(D3DRS_COLORWRITEENABLE, mask);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// predraw 和 postdraw 支持
//----------------------------------------------------------------------------
bool Renderer::PreDraw ()
{
	mNumDrawPrimitivePerFrame = 0;
	//mLastVertexFormat = 0;
	//mLastEnableVertexBuffer = 0;
	//mLastEnableIndexBuffer = 0;
	//mLastVertexShader = 0;
	//mLastPixelShader = 0;
	//mLastMaterialInstance = 0;

	HRESULT hr = mData->mDevice->TestCooperativeLevel();
	if (hr == D3DERR_DEVICELOST)
	{
		if (!mData->mDeviceLost)
		{
			// 这是第一次检测到设备丢失
			mData->mDeviceLost = true;
			mData->mFont->OnLostDevice();
		}
		return false;
	}
	else if (hr == D3DERR_DEVICENOTRESET)
	{
		// 释放所有D3DPOOL_DEFAULT的资源。在Dx9中，着色器不需要重新创建，Dx9纹
		// 理也是managed所有不需要重新创建。在DX10之后，我们就不要做设备丢失检
		// 了。^_^现在我们只是简单的将所有资源都释放，释放完之后重建。

		// 先释放有D3DPOOL_DEFAULT资源。
		OnLostDevice();
		mData->DestroyUniqueFont();

		// 重新置回设备
		hr = mData->mDevice->Reset(&mData->mPresent);
		if (hr != D3D_OK)
		{
			assertion(false, "Reset failed: %s\n", DXGetErrorString(hr));
			return false;
		}

		mData->SetDefaultProperties(mDefaultAlphaProperty, mDefaultCullProperty,
			mDefaultDepthProperty, mDefaultOffsetProperty, mDefaultStencilProperty,
			mDefaultWireProperty);

		// 重新创建资源
		mData->CreateUniqueFont();
	}

	// TODO.  Go-to-fullscreen needs to be supported here.
	hr = mData->mDevice->BeginScene();
	if (hr == D3D_OK)
	{
		return true;
	}

	assertion(false, "BeginScene failed: %s\n", DXGetErrorString(hr));
	return false;
}
//----------------------------------------------------------------------------
void Renderer::PostDraw ()
{
	HRESULT hr = mData->mDevice->EndScene();
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "EndScene failed: %s\n", DXGetErrorString(hr));
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 绘制
//----------------------------------------------------------------------------
void Renderer::Draw (const unsigned char* screenBuffer, bool reflectY)
{
	if (!screenBuffer)
	{
		assertion(false, "Incoming screen buffer is null.\n");
		return;
	}

	IDirect3DSurface9* backBuffer = 0;
	HRESULT hr = mData->mDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO,
		&backBuffer);
	if (hr != D3D_OK || !backBuffer)
	{
		assertion(false, "GetBackBuffer failed: %s\n",
			DXGetErrorString(hr));
		return;
	}

	// TODO:  The backbuffer for now is D3DFMT_A8R8G8B8, so the 4*width is the
	// correct choice.  However, we need a gDX9TextureFormatSize[] array to
	// use here, and we need to infer the "UINT srcPitch" value from it.
	IDirect3DSurface9* surface = 0;
	hr = mData->mDevice->CreateOffscreenPlainSurface(mWidth, mHeight,
		D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &surface, 0);
	if (hr != D3D_OK)
	{
		assertion(false, "Failed to create surface: %s\n",
			DXGetErrorString(hr));
		backBuffer->Release();
		return;
	}

	D3DLOCKED_RECT lockedRect;
	hr = surface->LockRect(&lockedRect, 0, D3DLOCK_DISCARD);
	if (hr != D3D_OK || !lockedRect.pBits)
	{
		assertion(false, "Failed to lock surface: %s\n",
			DXGetErrorString(hr));
		surface->Release();
		backBuffer->Release();
		return;
	}

	if (!reflectY)
	{
		memcpy(lockedRect.pBits, screenBuffer, 4*mWidth*mHeight);
	}
	else
	{
		size_t numRowBytes = 4*mWidth;  // 4 = sizeof(A8R8G8B8)
		const unsigned char* src = screenBuffer;
		unsigned char* trg = (unsigned char*)lockedRect.pBits +
			numRowBytes*(mHeight - 1);
		for (int i = 0; i < mHeight; ++i)
		{
			memcpy(trg, src, numRowBytes);
			src += numRowBytes;
			trg -= numRowBytes;
		}
	}

	hr = surface->UnlockRect();
	if (hr != D3D_OK)
	{
		assertion(false, "Failed to unlock surface: %s\n",
			DXGetErrorString(hr));
		surface->Release();
		backBuffer->Release();
		return;
	}

	hr = mData->mDevice->UpdateSurface(surface, 0, backBuffer, 0);
	if (hr != D3D_OK)
	{
		assertion(false, "Failed to update backbuffer: %s\n",
			DXGetErrorString(hr));
		surface->Release();
		backBuffer->Release();
		return;
	}

	surface->Release();
	backBuffer->Release();
}
//----------------------------------------------------------------------------
void Renderer::Draw (int x, int y, const Float4& color,
					 const std::string& message)
{
	D3DCOLOR dx9color = D3DCOLOR_COLORVALUE(color[0], color[1], color[2],
		color[3]);

	// 计算text包围矩形
	RECT textRect;
	textRect.bottom = y;
	textRect.top = y;
	textRect.left = x;
	textRect.right = x;

	// 设置缺省渲染状态，深度检测必须关闭，因为文字总在最上面。
	SetAlphaProperty(mDefaultAlphaProperty);
	SetCullProperty(mDefaultCullProperty);
	SetOffsetProperty(mDefaultOffsetProperty);
	SetStencilProperty(mDefaultStencilProperty);
	SetWireProperty(mDefaultWireProperty);

	mData->mCurrentRS.mZEnable = D3DZB_FALSE;
	HRESULT hr = mData->mDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));

	mData->mFont->DrawText(
		0,                                  // pSprite
		message.c_str(),                    // pString
		-1,                                 // Count
		&textRect,                          // pRect
		DT_LEFT | DT_BOTTOM | DT_CALCRECT,  // Format
		dx9color);                          // Color

	// 绘制文字
	mData->mFont->DrawText(
		0,
		message.c_str(),
		-1,
		&textRect,
		DT_LEFT | DT_BOTTOM,
		dx9color);

#ifdef PX2_RESET_STATE_AFTER_DRAW
	// 恢复之前的渲染状态。
	mData->mCurrentRS.mZEnable = D3DZB_TRUE;
	hr = mData->mDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
#endif
}
//----------------------------------------------------------------------------
#ifdef PX2_QUERY_PIXEL_COUNT
static IDirect3DQuery9* BeginQuery (IDirect3DDevice9* device)
{
	IDirect3DQuery9* query = 0;
	HRESULT hr = device->CreateQuery(D3DQUERYTYPE_OCCLUSION, &query);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "CreateQuery failed: %s\n",
		DXGetErrorString(hr));
	hr = query->Issue(D3DISSUE_BEGIN);
	assertion(hr == D3D_OK, "Issue (begin) failed: %s\n",
		DXGetErrorString(hr));
	return query;
}

static DWORD EndQuery (IDirect3DQuery9* query)
{
	HRESULT hr = query->Issue(D3DISSUE_END);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Issue (end) failed: %s\n",
		DXGetErrorString(hr));
	DWORD numPixelsDrawn = 0;
	while (S_FALSE == query->GetData(&numPixelsDrawn, sizeof(DWORD),
		D3DGETDATA_FLUSH))
	{
		// Wait until query finishes.
	}
	hr = query->Release();
	assertion(hr == D3D_OK, "Release failed: %s\n",
		DXGetErrorString(hr));
	return numPixelsDrawn;
}

#define PX2_BEGIN_QUERY(device, query) \
	IDirect3DQuery9* query = BeginQuery(device)

#define PX2_END_QUERY(query, numPixelsDrawn) \
	numPixelsDrawn = EndQuery(query)
#else
#define PX2_BEGIN_QUERY(device, query)
#define PX2_END_QUERY(query, numPixelsDrawn)
#endif
//----------------------------------------------------------------------------
void Renderer::DrawPrimitive (const Renderable* renderable)
{
	++mNumDrawPrimitivePerFrame;

	Renderable::PrimitiveType type = renderable->GetPrimitiveType();
	const VertexBuffer* vbuffer = renderable->GetVertexBuffer();
	const IndexBuffer* ibuffer = renderable->GetIndexBuffer();

	int indicesOffset = 0;
	int ibOffset = renderable->GetIBOffset();
	if (0 != ibOffset)
	{
		indicesOffset = ibOffset;
	}
	else
	{
		if (ibuffer)
		{
			indicesOffset = ibuffer->GetOffset();
		}
	}

	HRESULT hr;
	PX2_UNUSED(hr);
	int numPrimitives;
	DWORD numPixelsDrawn;
	PX2_UNUSED(numPixelsDrawn);

	if (type == Renderable::PT_TRIMESH
		||  type == Renderable::PT_TRISTRIP
		||  type == Renderable::PT_TRIFAN)
	{
		int numVertices = vbuffer->GetNumElements();
		numPrimitives = StaticCast<Triangles>(renderable)->GetNumTriangles();
		if (numVertices > 0 && numPrimitives > 0)
		{
			PX2_BEGIN_QUERY(mData->mDevice, query);

			hr = mData->mDevice->DrawIndexedPrimitive(gDX9PrimitiveType[type],
				0, 0, numVertices, indicesOffset, numPrimitives);

			if (hr != D3D_OK)
			{
				std::string errorStr = DXGetErrorString(hr);
				assertion(false, "DrawIndexedPrimitive failed: %s\n",
					errorStr.c_str());
			}

			PX2_END_QUERY(query, numPixelsDrawn);
		}
	}
	else if (type == Renderable::PT_POLYSEGMENTS_CONTIGUOUS)
	{
		numPrimitives = StaticCast<Polysegment>(renderable)->GetNumSegments();
		if (numPrimitives > 0)
		{
			PX2_BEGIN_QUERY(mData->mDevice, query);

			hr = mData->mDevice->DrawPrimitive(D3DPT_LINESTRIP, 0,
				numPrimitives);
			assertion(hr == D3D_OK, "DrawPrimitive failed: %s\n",
				DXGetErrorString(hr));

			PX2_END_QUERY(query, numPixelsDrawn);
		}
	}
	else if (type == Renderable::PT_POLYSEGMENTS_DISJOINT)
	{
		numPrimitives = StaticCast<Polysegment>(renderable)->GetNumSegments();
		if (numPrimitives > 0)
		{
			PX2_BEGIN_QUERY(mData->mDevice, query);

			hr = mData->mDevice->DrawPrimitive(D3DPT_LINELIST, 0,
				numPrimitives);
			assertion(hr == D3D_OK, "DrawPrimitive failed: %s\n",
				DXGetErrorString(hr));

			PX2_END_QUERY(query, numPixelsDrawn);
		}
	}
	else if (type == Renderable::PT_POLYPOINT)
	{
		numPrimitives = StaticCast<Polypoint>(renderable)->GetNumPoints();
		if (numPrimitives > 0)
		{
			PX2_BEGIN_QUERY(mData->mDevice, query);

			hr = mData->mDevice->DrawPrimitive(D3DPT_POINTLIST, 0,
				numPrimitives);
			assertion(hr == D3D_OK, "DrawPrimitive failed: %s\n",
				DXGetErrorString(hr));

			PX2_END_QUERY(query, numPixelsDrawn);
		}
	}
	else
	{
		assertion(false, "Invalid type.\n");
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Pass 管理
//----------------------------------------------------------------------------
void Renderer::Bind (const MaterialPass *pass)
{
	PX2_UNUSED(pass);
}
//----------------------------------------------------------------------------
void Renderer::BindAll (const MaterialPass *pass)
{
	PX2_UNUSED(pass);
}
//----------------------------------------------------------------------------
void Renderer::Unbind (const MaterialPass *pass)
{
	PX2_UNUSED(pass);
}
//----------------------------------------------------------------------------
void Renderer::UnbindAll (const MaterialPass *pass)
{
	PX2_UNUSED(pass);
}
//----------------------------------------------------------------------------
void Renderer::Enable (const Renderable* renderable,
	const MaterialInstance* instance, int index)
{
	const MaterialPass* pass = instance->GetPass(index);
	ShaderParameters* vparams = instance->GetVertexParameters(index);
	ShaderParameters* pparams = instance->GetPixelParameters(index);
	VertexShader* vshader = pass->GetVertexShader();
	PixelShader* pshader = pass->GetPixelShader();

	// 更新着色器常量
	ShaderStruct struc;
	struc.TheRenderable = (Renderable*)renderable;
	struc.TheCamera = (Camera*)mCamera;
	struc.TheEnvirParam = PX2_GR.GetCurEnvirParamController();

	vparams->UpdateConstants(&struc);
	pparams->UpdateConstants(&struc);

	// 设置渲染状态
	SetAlphaProperty(pass->GetAlphaProperty());
	SetCullProperty(pass->GetCullProperty());
	SetDepthProperty(pass->GetDepthProperty());
	SetOffsetProperty(pass->GetOffsetProperty());
	SetStencilProperty(pass->GetStencilProperty());
	SetWireProperty(pass->GetWireProperty());

	// 激活着色器
	Enable(vshader, vparams);
	Enable(pshader, pparams);
}
//----------------------------------------------------------------------------
void Renderer::Disable (const Renderable* renderable,
	const MaterialInstance* instance, int index)
{
	PX2_UNUSED(renderable);

	const MaterialPass* pass = instance->GetPass(index);
	ShaderParameters* vparams = instance->GetVertexParameters(index);
	ShaderParameters* pparams = instance->GetPixelParameters(index);
	VertexShader* vshader = pass->GetVertexShader();
	PixelShader* pshader = pass->GetPixelShader();

	// 取消激活着色器
	Disable(vshader, vparams);
	Disable(pshader, pparams);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// SharePdrData
//----------------------------------------------------------------------------
void Renderer::ClearSharePdrData ()
{
	std::map<int, SharePdrData>::iterator it = mSharePdrVertexShaders.begin();
	for (; it!=mSharePdrVertexShaders.end(); it++)
	{
		IDirect3DVertexShader9 *vs = (IDirect3DVertexShader9*)it->second.DataPtr;
		vs->Release();
	}
	mSharePdrVertexShaders.clear();

	std::map<int, SharePdrData>::iterator it1 = mSharePdrPixelShaders.begin();
	for (; it1!=mSharePdrPixelShaders.end(); it1++)
	{
		IDirect3DPixelShader9 *ps = (IDirect3DPixelShader9*)it1->second.DataPtr;
		ps->Release();
	}
	mSharePdrPixelShaders.clear();
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// RendererData
//----------------------------------------------------------------------------
RendererData::RendererData(RendererInput *input, int width, int height,
							Texture::Format colorFormat, Texture::Format depthStencilFormat,
							int numMultisamples) :
mDeviceLost(false)
{
	PdrRendererInput *pdrRenderInput = (PdrRendererInput*)(input);
	mWindowHandle = pdrRenderInput->mWindowHandle;

	D3DFORMAT dxColorFormat = gDX9TextureFormat[colorFormat];
	D3DFORMAT dxDepthStencilFormat = gDX9TextureFormat[depthStencilFormat];

	mPresent.BackBufferWidth = width;
	mPresent.BackBufferHeight = height;
	mPresent.BackBufferFormat = dxColorFormat;
	mPresent.BackBufferCount = 1;
	mPresent.hDeviceWindow = mWindowHandle;
	mPresent.Windowed = TRUE;
	mPresent.Flags = 0;
	mPresent.FullScreen_RefreshRateInHz = 0;
	mPresent.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// 需要深度和模板缓冲
	mPresent.EnableAutoDepthStencil = TRUE;
	mPresent.AutoDepthStencilFormat = dxDepthStencilFormat;

	// 需要多重采样
	mPresent.SwapEffect = D3DSWAPEFFECT_FLIP; 
	mPresent.MultiSampleType = D3DMULTISAMPLE_NONE;
	mPresent.MultiSampleQuality = 0;
	HRESULT hr;
	if (numMultisamples == 2)
	{
		hr = pdrRenderInput->mDriver->CheckDeviceMultiSampleType(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			dxColorFormat,
			FALSE,
			D3DMULTISAMPLE_2_SAMPLES,
			NULL);

		assertion(hr == D3D_OK, "Two multisamples not supported: %s\n",
			DXGetErrorString(hr));

		if (hr == D3D_OK)
		{
			mPresent.SwapEffect = D3DSWAPEFFECT_DISCARD; 
			mPresent.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES;
		}
	}
	else if (numMultisamples == 4)
	{
		hr = pdrRenderInput->mDriver->CheckDeviceMultiSampleType(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			dxColorFormat,
			FALSE,
			D3DMULTISAMPLE_4_SAMPLES,
			NULL);

		assertion(hr == D3D_OK, "Two multisamples not supported: %s\n",
			DXGetErrorString(hr));

		if (hr == D3D_OK)
		{
			mPresent.SwapEffect = D3DSWAPEFFECT_DISCARD; 
			mPresent.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES;
		}
	}

	hr = pdrRenderInput->mDriver->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		mWindowHandle,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE,
		&mPresent,
		&mDevice);

	assertion(hr == D3D_OK, "Failed to create device: %s\n",
		DXGetErrorString(hr));

	CreateUniqueFont();

	// 设备能力查询
	D3DCAPS9 deviceCaps;
	hr = pdrRenderInput->mDriver->GetDeviceCaps(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		&deviceCaps);

	assertion(hr == D3D_OK, "GetDeviceCaps failed: %s\n",
		DXGetErrorString(hr));

	DWORD vsModel = deviceCaps.VertexShaderVersion;
	if (vsModel < D3DVS_VERSION(1,1))
	{
		assertion(false, "Vertex Shader Model must be at least 1.1.\n");
		vsModel = D3DVS_VERSION(1,1);
	}
	if (vsModel > D3DVS_VERSION(3,0))
	{
		assertion(false, "Vertex Shader Model must be at most 3.0.\n");
		vsModel = D3DVS_VERSION(3,0);
	}
	if (vsModel < D3DVS_VERSION(2,0))
	{
		VertexShader::SetProfile(VertexShader::VP_VS_1_1);
	}
	else if (vsModel < D3DVS_VERSION(3,0))
	{
		VertexShader::SetProfile(VertexShader::VP_VS_2_0);
	}
	else
	{
		VertexShader::SetProfile(VertexShader::VP_VS_3_0);
	}

	DWORD psModel = deviceCaps.PixelShaderVersion;
	if (psModel < D3DPS_VERSION(1,1))
	{
		assertion(false, "Pixel Shader Model must be at least 1.1.\n");
		psModel = D3DPS_VERSION(1,1);
	}
	if (psModel > D3DPS_VERSION(3,0))
	{
		assertion(false, "Pixel Shader Model must be at most 3.0.\n");
		psModel = D3DPS_VERSION(3,0);
	}
	if (psModel < D3DPS_VERSION(2,0))
	{
		PixelShader::SetProfile(PixelShader::PP_PS_1_1);
	}
	else if (psModel < D3DPS_VERSION(3,0))
	{
		PixelShader::SetProfile(PixelShader::PP_PS_2_0);
	}
	else
	{
		PixelShader::SetProfile(PixelShader::PP_PS_3_0);
	}

	mMaxVShaderImages = (vsModel < D3DVS_VERSION(3,0) ? 0 : 4);
	mMaxPShaderImages = 16;
	mMaxTCoords = 8;
}
//----------------------------------------------------------------------------
RendererData::~RendererData ()
{
	DestroyUniqueFont();
	mDevice->Release();
}
//----------------------------------------------------------------------------
void RendererData::CreateUniqueFont ()
{
	HRESULT hr = D3DXCreateFont(
		mDevice,              // pDevice
		15,                   // Height
		0,                    // Width
		FW_REGULAR,           // Weight
		0,                    // MipLevels
		0,                    // Italic
		DEFAULT_CHARSET,      // CharSet
		OUT_DEFAULT_PRECIS,   // OutputPrecision
		ANTIALIASED_QUALITY,  // Quality
		VARIABLE_PITCH,       // PitchAndFamily
		"Times New Roman",	  // pFaceName
		&mFont);              // ppFont

	PX2_UNUSED(hr);

	assertion(hr == D3D_OK, "D3DXCreateFont failed: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void RendererData::DestroyUniqueFont ()
{
	mFont->Release();
}
//----------------------------------------------------------------------------
void RendererData::SetDefaultProperties (const AlphaProperty* aproperty,
									 const CullProperty* cproperty, const DepthProperty* dproperty,
									 const OffsetProperty* oproperty, const StencilProperty* sproperty,
									 const WireProperty* wproperty)
{
	// 设置缺省渲染状态为当前渲染状态
	mCurrentRS.Initialize(mDevice, aproperty, cproperty, dproperty, oproperty, sproperty,
		wproperty);

	// 设置缺省顶点着色器中采样器的状态
	HRESULT hr;
	for (DWORD i = 0; i < MAX_NUM_VSAMPLERS; ++i)
	{
		mCurrentVSState[i] = SamplerState();
		SamplerState& state = mCurrentVSState[i];

		hr = mDevice->SetSamplerState(i, D3DSAMP_MAXANISOTROPY,
			(DWORD)state.mAnisotropy);
		assertion(hr == D3D_OK, "Failed to set anisotropy: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_MIPMAPLODBIAS,
			*(DWORD*)&state.mLodBias);
		assertion(hr == D3D_OK, "Failed to set LOD bias: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_MAGFILTER,
			state.mMagFilter);
		assertion(hr == D3D_OK, "Failed to set magfilter: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_MINFILTER,
			state.mMinFilter);
		assertion(hr == D3D_OK, "Failed to set minfilter: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_MIPFILTER,
			state.mMipFilter);
		assertion(hr == D3D_OK, "Failed to set mipfilter: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_BORDERCOLOR,
			state.mBorderColor);
		assertion(hr == D3D_OK, "Failed to set border color: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_ADDRESSU,
			state.mWrap[0]);
		assertion(hr == D3D_OK, "Failed to set wrap0: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_ADDRESSV,
			state.mWrap[1]);
		assertion(hr == D3D_OK, "Failed to set wrap1: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_ADDRESSW,
			state.mWrap[2]);
		assertion(hr == D3D_OK, "Failed to set wrap2: %s\n",
			DXGetErrorString(hr));
	}

	// 设置缺省像素着色器中采样器的状态
	for (DWORD i = 0; i < MAX_NUM_PSAMPLERS; ++i)
	{
		mCurrentPSState[i] = SamplerState();
		SamplerState& state = mCurrentPSState[i];

		hr = mDevice->SetSamplerState(i, D3DSAMP_MAXANISOTROPY,
			(DWORD)state.mAnisotropy);
		assertion(hr == D3D_OK, "Failed to set anisotropy: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_MIPMAPLODBIAS,
			*(DWORD*)&state.mLodBias);
		assertion(hr == D3D_OK, "Failed to set LOD bias: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_MAGFILTER,
			state.mMagFilter);
		assertion(hr == D3D_OK, "Failed to set magfilter: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_MINFILTER,
			state.mMinFilter);
		assertion(hr == D3D_OK, "Failed to set minfilter: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_MIPFILTER,
			state.mMipFilter);
		assertion(hr == D3D_OK, "Failed to set mipfilter: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_BORDERCOLOR,
			state.mBorderColor);
		assertion(hr == D3D_OK, "Failed to set border color: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_ADDRESSU,
			state.mWrap[0]);
		assertion(hr == D3D_OK, "Failed to set wrap0: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_ADDRESSV,
			state.mWrap[1]);
		assertion(hr == D3D_OK, "Failed to set wrap1: %s\n",
			DXGetErrorString(hr));

		hr = mDevice->SetSamplerState(i, D3DSAMP_ADDRESSW,
			state.mWrap[2]);
		assertion(hr == D3D_OK, "Failed to set wrap2: %s\n",
			DXGetErrorString(hr));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 保存当前的渲染状态，减少渲染状态的转换
//----------------------------------------------------------------------------
RendererData::RenderState::RenderState ()
{
	// 初始化必须在DirectX9设备被创建后，初始化。
}
//----------------------------------------------------------------------------
void RendererData::RenderState::Initialize (IDirect3DDevice9* device,
											const AlphaProperty* aproperty, const CullProperty* cproperty,
											const DepthProperty* dproperty, const OffsetProperty* oproperty,
											const StencilProperty* sproperty, const WireProperty* wproperty)
{
	HRESULT hr;
	PX2_UNUSED(hr);

	// AlphaProperty
	mAlphaBlendEnable = aproperty->BlendEnabled ? TRUE : FALSE;
	mAlphaSrcBlend = gDX9AlphaSrcBlend[aproperty->SrcBlend];
	mAlphaDstBlend = gDX9AlphaDstBlend[aproperty->DstBlend];
	mAlphaTestEnable = aproperty->CompareEnabled ? TRUE : FALSE;
	mAlphaFunc = gDX9AlphaCompare[aproperty->Compare];
	mAlphaRef = (DWORD)(255.0f*aproperty->Reference);
	mBlendFactor = D3DCOLOR_COLORVALUE(
		aproperty->ConstantColor[0],
		aproperty->ConstantColor[1],
		aproperty->ConstantColor[2],
		aproperty->ConstantColor[3]);

	hr = device->SetRenderState(D3DRS_ALPHABLENDENABLE, mAlphaBlendEnable);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_SRCBLEND, mAlphaSrcBlend);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_DESTBLEND, mAlphaDstBlend);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_ALPHATESTENABLE, mAlphaTestEnable);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_ALPHAFUNC, mAlphaFunc);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_ALPHAREF, mAlphaRef);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_BLENDFACTOR, mBlendFactor);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));

	// CullProperty
	mCullMode = cproperty->Enabled ? gDX9CullOrder[cproperty->CCWOrder ? 1 : 0] :
		D3DCULL_NONE;

	hr = device->SetRenderState(D3DRS_CULLMODE, mCullMode);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));

	// DepthProperty
	mZEnable = dproperty->Enabled ? D3DZB_TRUE : D3DZB_FALSE;
	mZFunc = gDX9DepthCompare[dproperty->Compare];
	mZWriteEnable = dproperty->Writable ? TRUE : FALSE;

	hr = device->SetRenderState(D3DRS_ZENABLE, mZEnable);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_ZFUNC, mZFunc);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_ZWRITEENABLE, mZWriteEnable);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));

	// OffsetProperty
	if (oproperty->FillEnabled)
	{
		mSlopeScaleDepthBias = *(DWORD*)&oproperty->Scale;
		float bias = oproperty->Bias/16777216.0f;
		mDepthBias = *(DWORD*)&bias;
	}
	else
	{
		mSlopeScaleDepthBias = 0;
		mDepthBias = 0;
	}

	hr = device->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS,
		mSlopeScaleDepthBias);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_DEPTHBIAS, mDepthBias);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));

	// StencilProperty
	mStencilEnable = sproperty->Enabled ? TRUE : FALSE;
	mStencilFunc = gDX9StencilCompare[sproperty->Compare];
	mStencilRef = (DWORD)sproperty->Reference;
	mStencilMask = (DWORD)sproperty->Mask;
	mStencilWriteMask = (DWORD)sproperty->WriteMask;
	mStencilFail = gDX9StencilOperation[sproperty->OnFail];
	mStencilZFail = gDX9StencilOperation[sproperty->OnZFail];
	mStencilPass = gDX9StencilOperation[sproperty->OnZPass];

	hr = device->SetRenderState(D3DRS_STENCILENABLE, mStencilEnable);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_STENCILFUNC, mStencilFunc);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_STENCILREF, mStencilRef);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_STENCILMASK, mStencilMask);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_STENCILWRITEMASK, mStencilWriteMask);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_STENCILFAIL, mStencilFail);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_STENCILZFAIL, mStencilZFail);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
	hr = device->SetRenderState(D3DRS_STENCILPASS, mStencilPass);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));

	// WireProperty
	mFillMode = wproperty->Enabled ? D3DFILL_WIREFRAME : D3DFILL_SOLID;

	hr = device->SetRenderState(D3DRS_FILLMODE, mFillMode);
	assertion(hr == D3D_OK, "SetRenderState failed: %s\n",
		DXGetErrorString(hr));
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 保存当前的采样状态，减少状态的转换
//----------------------------------------------------------------------------
RendererData::SamplerState::SamplerState ()
{
	mAnisotropy = 1.0f;
	mLodBias = 0.0f;
	mMagFilter = D3DTEXF_POINT;
	mMinFilter = D3DTEXF_POINT;
	mMipFilter = D3DTEXF_NONE;
	mBorderColor = 0;
	mWrap[0] = D3DTADDRESS_WRAP;
	mWrap[1] = D3DTADDRESS_WRAP;
	mWrap[2] = D3DTADDRESS_WRAP;
}
//----------------------------------------------------------------------------

#endif