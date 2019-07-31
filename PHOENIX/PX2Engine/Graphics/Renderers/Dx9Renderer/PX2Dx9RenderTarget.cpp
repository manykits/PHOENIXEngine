// PX2Dx9RenderTarget.cpp

#ifdef PX2_USE_DX9

#include "PX2Dx9RenderTarget.hpp"
#include "PX2Dx9Mapping.hpp"
#include "PX2Dx9RendererData.hpp"
#include "PX2Dx9Texture2D.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrRenderTarget::PdrRenderTarget (Renderer* renderer,
								  const RenderTarget* renderTarget) :
mSaveColorSurface(0),
mSaveDepthStencilSurface(0),
mColorTextures(0)
{
	HRESULT hr;
	PX2_UNUSED(hr);

	mNumTargets = renderTarget->GetNumTargets();
	mWidth = renderTarget->GetWidth();
	mHeight = renderTarget->GetHeight();
	mFormat = renderTarget->GetFormat();
	mHasDepthStencil = renderTarget->HasDepthStencil();

	if (mNumTargets > 0)
	{
		mColorTextures = new1<IDirect3DTexture9*>(mNumTargets);
		mColorSurfaces = new1<IDirect3DSurface9*>(mNumTargets);
		for (int i = 0; i < mNumTargets; ++i)
		{
			Texture2D* colorTexture = renderTarget->GetColorTexture(i);
			assertion(!renderer->InTexture2DMap(colorTexture),
				"Texture should not yet exist.\n");

			PdrTexture2D* dxColorTexture = new0 PdrTexture2D(renderer, true,
				colorTexture, renderTarget->HasMipmaps());
			colorTexture->PdrPointer = dxColorTexture;
			renderer->InsertInTexture2DMap(colorTexture, dxColorTexture);
			mColorTextures[i] = dxColorTexture->mTexture;
			mColorTextures[i]->AddRef();

			hr = mColorTextures[i]->GetSurfaceLevel(0, &mColorSurfaces[i]);
			assertion(hr == D3D_OK, "Failed to get rt %d color surface: %s\n",
				i, DXGetErrorString(hr));
		}
	}

	if (mHasDepthStencil)
	{
		Texture2D* depthStencilTexture = renderTarget->GetDepthStencilTexture();
		assertion(!renderer->InTexture2DMap(depthStencilTexture), "Texture should not yet exist.\n");

		PdrTexture2D* dxDepthStencilTexture = new0 PdrTexture2D(renderer, false, depthStencilTexture, false);
		depthStencilTexture->PdrPointer = dxDepthStencilTexture;
		renderer->InsertInTexture2DMap(depthStencilTexture, dxDepthStencilTexture);

		mDepthStencilTexture = dxDepthStencilTexture->mTexture;
		mDepthStencilTexture->AddRef();

		hr = mDepthStencilTexture->GetSurfaceLevel(0, &mDepthStencilSurface);
		assertion(hr == D3D_OK,
			"Failed to get rt depthstencil surface: %s\n",
			DXGetErrorString(hr));
	}
	else
	{
		mDepthStencilTexture = 0;
		mDepthStencilSurface = 0;
	}
}
//----------------------------------------------------------------------------
PdrRenderTarget::~PdrRenderTarget ()
{
	int i;
	for (i = 0; i < mNumTargets; ++i)
	{
		mColorSurfaces[i]->Release();
		mColorTextures[i]->Release();
	}

	if (mColorTextures)
	{
		delete1(mColorTextures);
		delete1(mColorSurfaces);
	}

	if (mHasDepthStencil)
	{
		if (mDepthStencilSurface)
		{
			mDepthStencilSurface->Release();
		}

		if (mDepthStencilTexture)
		{
			mDepthStencilTexture->Release();
		}
	}
}
//----------------------------------------------------------------------------
void PdrRenderTarget::Enable (Renderer* renderer)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;
	HRESULT hr;
	PX2_UNUSED(hr);

	if (mNumTargets > 0)
	{
		hr = device->GetRenderTarget(0, &mSaveColorSurface);
		assertion(hr == D3D_OK, "Failed to get old rt 0 color surface: %s\n",
			DXGetErrorString(hr));

		hr = device->SetRenderTarget(0, mColorSurfaces[0]);
		assertion(hr == D3D_OK, "Failed to set new rt 0 color surface: %s\n",
			DXGetErrorString(hr));

		for (int i = 1; i < mNumTargets; ++i)
		{
			hr = device->SetRenderTarget((DWORD)i, mColorSurfaces[i]);
			assertion(hr == D3D_OK, "Failed to set new rt %d color surface: %s\n",
				i, DXGetErrorString(hr));
		}
	}

	if (mHasDepthStencil)
	{
		hr = device->GetDepthStencilSurface(&mSaveDepthStencilSurface);
		assertion(hr == D3D_OK,
			"Failed to get old rt depthstencil surface: %s\n",
			DXGetErrorString(hr));

		hr = device->SetDepthStencilSurface(mDepthStencilSurface);
		assertion(hr == D3D_OK,
			"Failed to set new rt depthstencil surface: %s\n",
			DXGetErrorString(hr));
	}
}
//----------------------------------------------------------------------------
void PdrRenderTarget::Disable (Renderer* renderer)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;
	HRESULT hr;
	PX2_UNUSED(hr);

	if (mNumTargets > 0)
	{
		if (mSaveColorSurface)
		{
			hr = device->SetRenderTarget(0, mSaveColorSurface);
			assertion(hr == D3D_OK, "Failed to set old rt 0 color surface: %s\n",
				DXGetErrorString(hr));

			mSaveColorSurface->Release();
			mSaveColorSurface = 0;
		}

		for (int i = 1; i < mNumTargets; ++i)
		{
			hr = device->SetRenderTarget((DWORD)i, 0);
			assertion(hr == D3D_OK,
				"Failed to set old rt %d color surface: %s\n", i,
				DXGetErrorString(hr));
		}
	}

	if (mHasDepthStencil)
	{
		if (mSaveDepthStencilSurface)
		{
			hr = device->SetDepthStencilSurface(mSaveDepthStencilSurface);
			assertion(hr == D3D_OK,
				"Failed to set old rt 0 depthstencil surface: %s\n",
				DXGetErrorString(hr));

			mSaveDepthStencilSurface->Release();
			mSaveDepthStencilSurface = 0;
		}
	}
}
//----------------------------------------------------------------------------
void PdrRenderTarget::ReadColor (int i, Renderer* renderer,
								 Texture2D*& texture)
{
	if (i < 0 || i >= mNumTargets)
	{
		assertion(false, "Invalid target index.\n");
		return;
	}

	IDirect3DDevice9* device = renderer->mData->mDevice;
	HRESULT hr;
	PX2_UNUSED(hr);

	// 激活RenderTarget
	if (i == 0)
	{
		hr = device->GetRenderTarget(0, &mSaveColorSurface);
		assertion(hr == D3D_OK, "Failed to get old rt 0 color surface: %s\n",
			DXGetErrorString(hr));
	}

	hr = device->SetRenderTarget((DWORD)i, mColorSurfaces[i]);
	assertion(hr == D3D_OK,
		"Failed to set new rt %d color surface: %s\n", i,
		DXGetErrorString(hr));

	// 建立一个内存的复制
	IDirect3DTexture9* copyTexture = 0;
	hr = D3DXCreateTexture
		(
		device,
		(UINT)mWidth,
		(UINT)mHeight,
		0,
		0,
		gDX9TextureFormat[mFormat],
		D3DPOOL_SYSTEMMEM,
		&copyTexture
		);
	assertion(hr == D3D_OK,
		"Failed to create copy texture: %s\n",
		DXGetErrorString(hr));

	// 获得表面
	IDirect3DSurface9* copySurface = 0;
	hr = copyTexture->GetSurfaceLevel(0, &copySurface);
	assertion(hr == D3D_OK,
		"Failed to get surface level for copy texture: %s\n",
		DXGetErrorString(hr));

	// 拷贝渲染表面
	hr = device->GetRenderTargetData(mColorSurfaces[i], copySurface);
	assertion(hr == D3D_OK,
		"Failed to copy the rt %d surface: %s\n", i,
		DXGetErrorString(hr));

	// 将数据写到磁盘
	D3DLOCKED_RECT rect;
	hr = copySurface->LockRect(&rect, 0, 0);
	assertion(hr == D3D_OK,
		"Failed to lock copy surface: %s\n",
		DXGetErrorString(hr));

	if (texture)
	{
		if (texture->GetFormat() != mFormat ||
			texture->GetWidth() != (int)mWidth ||
			texture->GetHeight() != (int)mHeight)
		{
			assertion(false, "Incompatible texture.\n");
			delete0(texture);
			texture = new0 Texture2D(mFormat, mWidth, mHeight, 1);
		}
	}
	else
	{
		texture = new0 Texture2D(mFormat, mWidth, mHeight, 1);
	}
	memcpy(texture->GetData(0), rect.pBits, texture->GetNumLevelBytes(0));

	hr = copySurface->UnlockRect();
	assertion(hr == D3D_OK,
		"Failed to unlock copy surface: %s\n",
		DXGetErrorString(hr));

	copySurface->Release();
	copyTexture->Release();

	// 置回先前的RenderTarget
	if (i == 0)
	{
		hr = device->SetRenderTarget(0, mSaveColorSurface);
		assertion(hr == D3D_OK,
			"Failed to set old rt 0 color surface: %s\n",
			DXGetErrorString(hr));

		mSaveColorSurface->Release();
	}
}
//----------------------------------------------------------------------------

#endif