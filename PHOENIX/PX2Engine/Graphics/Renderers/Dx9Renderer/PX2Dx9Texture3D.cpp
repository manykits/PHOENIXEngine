// PX2Dx9Texture3D.cpp

#ifdef PX2_USE_DX9

#include "PX2Dx9Texture3D.hpp"
#include "PX2Dx9Mapping.hpp"
#include "PX2Dx9RendererData.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
PdrTexture3D::PdrTexture3D (Renderer* renderer, const Texture3D* texture)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;

	int numLevels = texture->GetNumLevels();
	HRESULT hr = device->CreateVolumeTexture((UINT)texture->GetWidth(),
		(UINT)texture->GetHeight(), (UINT)texture->GetThickness(),
		(UINT)numLevels, gDX9BufferUsage[texture->GetUsage()],
		gDX9TextureFormat[texture->GetFormat()], D3DPOOL_MANAGED,
		&mTexture, 0);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to create 3D texture: %s\n",
		DXGetErrorString(hr));

	for (int level = 0; level < numLevels; ++level)
	{
		void* data = Lock(level, Buffer::BL_WRITE_ONLY);
		memcpy(data, texture->GetData(level),
			texture->GetNumLevelBytes(level));
		Unlock(level);
	}
}
//----------------------------------------------------------------------------
PdrTexture3D::~PdrTexture3D ()
{
	mTexture->Release();
}
//----------------------------------------------------------------------------
void PdrTexture3D::Enable (Renderer* renderer, int textureUnit)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;

	HRESULT hr = device->SetTexture(textureUnit, mTexture);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to enable 3D texture, unit %d: %s\n",
		textureUnit, DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void PdrTexture3D::Disable (Renderer* renderer, int textureUnit)
{
	IDirect3DDevice9* device = renderer->mData->mDevice;
	HRESULT hr;
	PX2_UNUSED(hr);

#ifdef PX2_PDR_DEBUG
	// 检测需要被取消激活的数据，是否匹配。
	IDirect3DBaseTexture9 *activeTexture = 0;
	hr = device->GetTexture(textureUnit, &activeTexture);
	assertion(hr == D3D_OK, "Failed to get 3D texture, unit %d: %s\n",
		textureUnit, DXGetErrorString(hr));
	assertion(activeTexture == mTexture, "Mismatched textures\n");
	activeTexture->Release();
#endif

	hr = device->SetTexture(textureUnit, 0);
	assertion(hr == D3D_OK, "Failed to disable 3D texture, unit %d: %s\n",
		textureUnit, DXGetErrorString(hr));
}
//----------------------------------------------------------------------------
void* PdrTexture3D::Lock (int level, Buffer::Locking mode)
{
	D3DLOCKED_BOX box;
	HRESULT hr = mTexture->LockBox((UINT)level, &box, 0,
		gDX9BufferLocking[mode]);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to lock level %u of 3D texture: %s\n",
		level, DXGetErrorString(hr));
	return box.pBits;
}
//----------------------------------------------------------------------------
void PdrTexture3D::Unlock (int level)
{
	HRESULT hr = mTexture->UnlockBox((UINT)level);
	PX2_UNUSED(hr);
	assertion(hr == D3D_OK, "Failed to unlock level %u of 3D texture: %s\n",
		level, DXGetErrorString(hr));
}
//----------------------------------------------------------------------------

#endif