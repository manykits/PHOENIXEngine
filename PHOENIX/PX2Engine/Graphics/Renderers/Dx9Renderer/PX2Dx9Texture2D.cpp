// PX2Dx9Texture2D.cpp

#ifdef PX2_USE_DX9

#include "PX2Dx9Texture2D.hpp"
#include "PX2Dx9Mapping.hpp"
#include "PX2Dx9RendererData.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrTexture2D::PdrTexture2D (Renderer* renderer, const Texture2D* texture)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;
	Texture::Format tdFormat = texture->GetFormat();
	int numBytes = texture->GetNumTotalBytes();
	int numLevels = texture->GetNumLevels();
	char *srcData = texture->GetData(0);

	int numElement = 0;
	unsigned char* newSrc = 0;
	int newNumBytes = 0;
	D3DPOOL pool = D3DPOOL_MANAGED;
	int SrcBase = 0, newSrcBase = 0;	
	D3DFORMAT format;

	if (texture->GetUsage() == Buffer::BU_TEXTURE)
	{
		pool = D3DPOOL_MANAGED;
	}
	else
	{
		pool = D3DPOOL_DEFAULT;
	}

	int charMax = 255;

	switch (tdFormat)
	{
	case Texture::TF_R8G8B8:
		numElement = numBytes/3;
		newNumBytes = numBytes + numElement;
		newSrc = new1<unsigned char>(newNumBytes);
		assertion(newSrc!=0, "new failed.");	
		for (int i = 0; i < numElement; i++)
		{
			newSrc[newSrcBase    ] = srcData[SrcBase + 2];  // B
			newSrc[newSrcBase + 1] = srcData[SrcBase + 1];	// G
			newSrc[newSrcBase + 2] = srcData[SrcBase + 0];	// R
			newSrc[newSrcBase + 3] = (unsigned char)charMax;// A

			newSrcBase += 4;
			SrcBase += 3;
		}
		format = D3DFMT_X8R8G8B8;
		break;
	default:
		format = gDX9TextureFormat[texture->GetFormat()];
		break;
	}

	UINT texWidth = (UINT)texture->GetWidth();
	UINT texHeight = (UINT)texture->GetHeight();
	HRESULT hr = device->CreateTexture(texWidth, texHeight, (UINT)numLevels,
		gDX9BufferUsage[texture->GetUsage()],
		format, pool, &mTexture, 0);
	if (hr != D3D_OK)
	{
		assertion(false, "Failed to create 2D texture: %s\n", DXGetErrorString(hr));
	}

	if (pool == D3DPOOL_MANAGED)
	{
		if (tdFormat == Texture::TF_R8G8B8)
		{
			unsigned char *src1 = newSrc;
			int levelByte = 0;

			for (int level = 0; level < numLevels; ++level)
			{
				int curWidth = texture->GetDimension(0, level);
				int curHegiht = texture->GetDimension(1, level);

				levelByte = 4*curWidth*curHegiht;

				char* data = (char*)Lock(level, Buffer::BL_WRITE_ONLY);
				memcpy(data, src1, levelByte);
				Unlock(level);

				src1 += levelByte;
			}

			delete1(newSrc);
		}
		else
		{
			for (int level = 0; level < numLevels; ++level)
			{
				void* data = Lock(level, Buffer::BL_WRITE_ONLY);
				memcpy(data, texture->GetData(level),
					texture->GetNumLevelBytes(level));
				Unlock(level);
			}
		}
	}
}
//----------------------------------------------------------------------------
PdrTexture2D::PdrTexture2D (Renderer* renderer, bool isColorTexture,
							const Texture2D* texture, bool autoGenMipMap)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;
	HRESULT hr;
	PX2_UNUSED(hr);

	if (isColorTexture)
	{
		UINT levels = 1;
		DWORD usage = gDX9BufferUsage[texture->GetUsage()];
		if (autoGenMipMap)
		{
			levels = 0;
			usage |= D3DUSAGE_AUTOGENMIPMAP;
		}

		hr = device->CreateTexture((UINT)texture->GetWidth(),
			(UINT)texture->GetHeight(), levels, usage,
			gDX9TextureFormat[texture->GetFormat()], D3DPOOL_DEFAULT,
			&mTexture, 0);
		assertion(hr == D3D_OK,
			"Failed to create render target color texture: %s\n",
			DXGetErrorString(hr));
	}
	else
	{
		hr = device->CreateTexture((UINT)texture->GetWidth(),
			(UINT)texture->GetHeight(), 1,
			gDX9BufferUsage[texture->GetUsage()],
			gDX9TextureFormat[texture->GetFormat()],
			D3DPOOL_DEFAULT, &mTexture, 0);
		assertion(hr == D3D_OK,
			"Failed to create render target depthstencil texture: %s\n",
			DXGetErrorString(hr));
	}
}
//----------------------------------------------------------------------------
PdrTexture2D::~PdrTexture2D ()
{
	mTexture->Release();
}
//----------------------------------------------------------------------------
void PdrTexture2D::Enable (Renderer* renderer, int textureUnit, int loc)
{
	PX2_UNUSED(renderer);
	PX2_UNUSED(textureUnit);
	PX2_UNUSED(loc);
	assertion(false, "Dx9Renderer do not need this.\n");
}
//----------------------------------------------------------------------------
void PdrTexture2D::Enable (Renderer* renderer, int textureUnit)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;

	HRESULT hr = device->SetTexture(textureUnit, mTexture);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to enable 2D texture, unit %d: %s\n",
		textureUnit, DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void PdrTexture2D::Disable (Renderer* renderer, int textureUnit)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;
	HRESULT hr;
	PX2_UNUSED(hr);

#ifdef PX2_PDR_DEBUG
	// 检测需要被取消激活的数据，是否匹配。
	IDirect3DBaseTexture9 *activeTexture = 0;
	hr = device->GetTexture(textureUnit, &activeTexture);
	assertion(hr == D3D_OK, "Failed to get 2D texture, unit %d: %s\n",
		textureUnit, DXGetErrorString(hr));
	assertion(activeTexture == mTexture, "Mismatched textures\n");
	activeTexture->Release();
#endif

	hr = device->SetTexture(textureUnit, 0);
	assertion(hr == D3D_OK, "Failed to disable 2D texture, unit %d: %s\n",
		textureUnit, DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void* PdrTexture2D::Lock (int level, Buffer::Locking mode)
{
	D3DLOCKED_RECT rect;
	HRESULT hr = mTexture->LockRect((UINT)level, &rect, 0,
		gDX9BufferLocking[mode]);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to lock level %u of 2D texture: %s\n",
		level, DXGetErrorString(hr));
	return rect.pBits;
}
//----------------------------------------------------------------------------
void PdrTexture2D::Unlock (int level)
{
	HRESULT hr = mTexture->UnlockRect((UINT)level);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to unlock level %u of 2D texture: %s\n",
		level, DXGetErrorString(hr));
}
//----------------------------------------------------------------------------

#endif